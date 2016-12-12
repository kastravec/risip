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
#include "risiplocation.h"

#include "opencagedataapi.h"
#include "opencagedata.h"
#include "risipgeopositionprovider.h"

#include <QDebug>

class RisipLocation::Private
{
public:
    OpenCageDataAPI *openCageDataApi;
    RisipGeoPositionProvider *geoPositionProvider;
};

RisipLocation *RisipLocation::m_instance = NULL;
RisipLocation::RisipLocation(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->openCageDataApi = new OpenCageDataAPI(this);
    m_data->geoPositionProvider = RisipGeoPositionProvider::instance();

    connect(m_data->geoPositionProvider, &RisipGeoPositionProvider::currentCoordinatesChanged,
            m_data->openCageDataApi, &OpenCageDataAPI::reverseGeoPosition, Qt::DirectConnection);

    connect(m_data->openCageDataApi, &OpenCageDataAPI::openCageDataReady,
            this, &RisipLocation::openCageDataReplyHandler);
}

RisipLocation::~RisipLocation()
{
    delete m_data;
    m_data = NULL;
}

QGeoCoordinate RisipLocation::currentCoordinates() const
{
    return QGeoCoordinate();
}

RisipLocation *RisipLocation::instance()
{
    if(m_instance == NULL)
        m_instance = new RisipLocation;

    return m_instance;
}

void RisipLocation::updateCurrentLocation()
{
    m_data->geoPositionProvider->start();
}

void RisipLocation::openCageDataReplyHandler(OpenCageData *data)
{
    if(!data)
        return;

    qDebug()<<"REPLY OPEN CAGE" << data->rawData();
    data->deleteLater();
    m_data->geoPositionProvider->stop();
}
