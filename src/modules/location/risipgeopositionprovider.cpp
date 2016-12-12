/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
**    http://risip.io
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You have received a copy of the GNU General Public License
**    along with this program. See LICENSE.GPLv3
**    A copy of the license can be found also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/
#include "risipgeopositionprovider.h"

#include <QGeoLocation>
#include <QGeoAddress>
#include <QGeoCodeReply>
#include <QTimer>
#include <QDebug>

class RisipGeoPositionProvider::Private
{
public:
    QGeoPositionInfoSource *geoPositionInfoSource;
    QGeoPositionInfo geoInfo;
    int status;
    int updateInterval;
};

RisipGeoPositionProvider *RisipGeoPositionProvider::m_instance = NULL;
RisipGeoPositionProvider::RisipGeoPositionProvider(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->geoPositionInfoSource = NULL;
    m_data->geoInfo = QGeoPositionInfo();
    m_data->status = Idle;
    m_data->updateInterval = 7000;
}

RisipGeoPositionProvider *RisipGeoPositionProvider::instance()
{
    if(m_instance == NULL)
        m_instance = new RisipGeoPositionProvider;

    return m_instance;
}

RisipGeoPositionProvider::~RisipGeoPositionProvider()
{
    delete m_data->geoPositionInfoSource;
    m_data->geoPositionInfoSource = NULL;

    delete m_data;
    m_data = NULL;
}

int RisipGeoPositionProvider::status() const
{
    return m_data->status;
}

int RisipGeoPositionProvider::updateInterval() const
{
    return m_data->updateInterval;
}

void RisipGeoPositionProvider::setUpdateInterval(int interval)
{
    if(m_data->updateInterval != interval) {
        m_data->updateInterval = interval;
        emit updateIntervalChanged(m_data->updateInterval);
    }
}

QGeoCoordinate RisipGeoPositionProvider::currentCoordinates() const
{
    return m_data->geoInfo.coordinate();
}

void RisipGeoPositionProvider::start()
{
    if(m_data->geoPositionInfoSource) {
        delete m_data->geoPositionInfoSource;
        m_data->geoPositionInfoSource = NULL;
    }

    m_data->geoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
    if(m_data->geoPositionInfoSource) {
        m_data->geoPositionInfoSource->setUpdateInterval(m_data->updateInterval);

        connect(m_data->geoPositionInfoSource, &QGeoPositionInfoSource::positionUpdated,
                this, &RisipGeoPositionProvider::geoPositionUpdateHandler, Qt::DirectConnection);
        connect(m_data->geoPositionInfoSource, SIGNAL(error(QGeoPositionInfoSource::Error)),
                this, SLOT(errorHandler(QGeoPositionInfoSource::Error)), Qt::DirectConnection);

        m_data->geoPositionInfoSource->startUpdates();
        QTimer::singleShot(4000, this, &RisipGeoPositionProvider::stop);
        setStatus(Running);
    } else {
        setStatus(ProviderError);
    }
}

void RisipGeoPositionProvider::stop()
{
    if(m_data->geoPositionInfoSource) {
        m_data->geoPositionInfoSource->stopUpdates();
        delete m_data->geoPositionInfoSource;
        m_data->geoPositionInfoSource = NULL;
    }

    setStatus(Idle);
}

/**
 * @brief RisipGeoPositionProvider::geoPositionUpdateHandler
 * @param info
 *
 * Internal API.
 *
 * Handling the geo position/location update
 */
void RisipGeoPositionProvider::geoPositionUpdateHandler(const QGeoPositionInfo &info)
{
    if(info.isValid()) {
        if(m_data->geoInfo != info) {
            m_data->geoInfo = info;

            qDebug()<<"COORDINATES: " << m_data->geoInfo.coordinate();
            emit currentCoordinatesChanged(m_data->geoInfo.coordinate());
            setStatus(Idle);
        }
    }
}

/**
 * @brief RisipGeoPositionProvider::errorHandler
 * @param positioningError
 *
 * Internal API.
 *
 * Handler for geo position source provider errors.
 */
void RisipGeoPositionProvider::errorHandler(QGeoPositionInfoSource::Error positioningError)
{
    qDebug()<<"ERROR: " <<positioningError;
}

void RisipGeoPositionProvider::setStatus(int status)
{
    if(m_data->status != status) {
        m_data->status = status;
        emit statusChanged(m_data->status);
    }
}
