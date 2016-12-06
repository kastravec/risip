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
#include <QDebug>

RisipGeoPositionProvider *RisipGeoPositionProvider::m_instance = NULL;
RisipGeoPositionProvider::RisipGeoPositionProvider(QObject *parent)
    :QObject(parent)
    ,m_geoPositionInfoSource(NULL)
    ,m_geoCodeReply(NULL)
    ,m_status(Idle)
    ,m_updateInterval(5000)
    ,m_countryCode()
    ,m_country()
    ,m_city()
{
}

RisipGeoPositionProvider *RisipGeoPositionProvider::instance()
{
    if(m_instance == NULL)
        m_instance = new RisipGeoPositionProvider;

    return m_instance;
}

RisipGeoPositionProvider::~RisipGeoPositionProvider()
{
    delete m_geoPositionInfoSource;
    m_geoPositionInfoSource = NULL;
}

int RisipGeoPositionProvider::status() const
{
    return m_status;
}

int RisipGeoPositionProvider::updateInterval() const
{
    return m_updateInterval;
}

void RisipGeoPositionProvider::setUpdateInterval(int interval)
{
    if(m_updateInterval != interval) {
        m_updateInterval = interval;
        emit updateIntervalChanged(m_updateInterval);
    }
}

QGeoCoordinate RisipGeoPositionProvider::currentCoordinates() const
{
    return m_geoInfo.coordinate();
}

QString RisipGeoPositionProvider::countryCode() const
{
    return m_countryCode;
}

QString RisipGeoPositionProvider::country() const
{
    return m_country;
}

QString RisipGeoPositionProvider::city() const
{
    return m_city;
}

void RisipGeoPositionProvider::start()
{
    if(m_geoPositionInfoSource) {
        delete m_geoPositionInfoSource;
        m_geoPositionInfoSource = NULL;
    }

    m_geoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
    if(m_geoPositionInfoSource) {

        m_geoPositionInfoSource->setUpdateInterval(m_updateInterval);

        connect(m_geoPositionInfoSource, &QGeoPositionInfoSource::positionUpdated,
                this, &RisipGeoPositionProvider::geoPositionUpdateHandler, Qt::DirectConnection);
        connect(m_geoPositionInfoSource, SIGNAL(error(QGeoPositionInfoSource::Error)),
                this, SLOT(errorHandler(QGeoPositionInfoSource::Error)), Qt::DirectConnection);

        m_geoPositionInfoSource->startUpdates();
        setStatus(Running);
    } else {
        setStatus(ProviderError);
    }
}

void RisipGeoPositionProvider::stop()
{
    if(m_geoPositionInfoSource) {
        m_geoPositionInfoSource->stopUpdates();
        delete m_geoPositionInfoSource;
        m_geoPositionInfoSource = NULL;
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
        if(m_geoInfo != info) {
            m_geoInfo = info;

//TODO geo reversing is not implemented
//            m_geoCodeReply = m_geoCodeManager.reverseGeocode(m_geoInfo.coordinate());
//            if(m_geoCodeReply) {
//                connect(m_geoCodeReply, &QGeoCodeReply::finished,
//                        this, &RisipGeoPositionProvider::geoLocationReverseHandler, Qt::DirectConnection);
//                connect(m_geoCodeReply, &QGeoCodeReply::error,
//                        this, &RisipGeoPositionProvider::geoLocationReverseErrorHandler, Qt::DirectConnection);
//            }

            emit currentCoordinatesChanged(m_geoInfo.coordinate());
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

void RisipGeoPositionProvider::geoLocationReverseHandler()
{
    QList<QGeoLocation> alllocations = m_geoCodeReply->locations();

    qDebug()<<"ALL LOCATIONS: " <<alllocations.count()
           << "LOCATION 1: " <<alllocations.first().address().countryCode();

    //            emit countryCodeChanged(m_countryCode);
    //            emit countryChanged(m_country);
    //            emit cityChanged(m_city);
    m_geoCodeReply->deleteLater();
}

/**
 * @brief RisipGeoPositionProvider::geoLocationReverseErrorHandler
 * @param error
 * @param errorString
 *
 * Internal API.
 *
 * Geo reversing is not implemented.
 */
void RisipGeoPositionProvider::geoLocationReverseErrorHandler(int error, const QString &errorString)
{
    qDebug()<<"LOCATION REVERSE ERROR: " << error << errorString;
}

void RisipGeoPositionProvider::setStatus(int status)
{
    if(m_status != status) {
        m_status = status;
        emit statusChanged(m_status);
    }
}
