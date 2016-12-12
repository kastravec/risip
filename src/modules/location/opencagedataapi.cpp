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
#include "opencagedataapi.h"

#include "utils/httpnetworkrequest.h"
#include "opencagedata.h"

#include <QDebug>

class OpenCageDataAPI::Private
{
public:
    int status;
    QString key;
    QString version;
    QString format;
    int responsecode;
    QString baseUrl;
    QGeoCoordinate currentCoordinates;
};


OpenCageDataAPI::OpenCageDataAPI(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->status = Idle;
    m_data->key = QString("7cbcb6b229c48b29122cc10dfb9fd29d");
    m_data->baseUrl = QString("https://api.opencagedata.com/geocode/v1/json?");
    m_data->format = Json;
    m_data->version = QString("v1");
}

OpenCageDataAPI::~OpenCageDataAPI()
{
    delete m_data;
    m_data  = NULL;
}

int OpenCageDataAPI::status() const
{
    return m_data->status;
}

QString OpenCageDataAPI::key() const
{
    return m_data->key;
}

void OpenCageDataAPI::setKey(const QString &key)
{
    if(m_data->key != key) {
        m_data->key = key;
        emit keyChanged(m_data->key);
    }
}

QString OpenCageDataAPI::apiVersion() const
{
    return m_data->version;
}

void OpenCageDataAPI::setApiVersion(const QString &version)
{
    if(m_data->version != version) {
        m_data->version = version;
        emit apiVersionChanged(m_data->version);
    }
}

QString OpenCageDataAPI::format() const
{
    return m_data->format;
}

void OpenCageDataAPI::setFormat(const QString &format)
{
    if(m_data->format != format) {
        m_data->format = format;
        emit formatChanged(m_data->format);
    }
}

int OpenCageDataAPI::responseCode() const
{
    return m_data->responsecode;
}

QString OpenCageDataAPI::baseUrl() const
{
    return m_data->baseUrl;
}

void OpenCageDataAPI::reverseGeoPosition(const QGeoCoordinate &coordinates)
{
    if(m_data->currentCoordinates.latitude() != coordinates.latitude()
            || m_data->currentCoordinates.longitude() != coordinates.longitude()) {
        m_data->currentCoordinates = coordinates;
        HttpNetworkRequest *httpRequest = new HttpNetworkRequest(this);
        httpRequest->setBaseUrl(m_data->baseUrl);

        QJsonObject headers;
        headers.insert(QString("Content-type"), QJsonValue(QString("application/json")));
        headers.insert(QString("Accept"), QJsonValue(QString("application/json")));
        httpRequest->setHttpHeaders(headers);

        QString coordinatesString = QString::number(coordinates.latitude())
                + QString(",")
                + QString::number(coordinates.longitude());

        QJsonObject parameters;
        parameters.insert(QString("key"), QJsonValue(m_data->key));
        parameters.insert(QString("q"), QJsonValue(coordinatesString));
        httpRequest->setUrlParameters(parameters);

        connect(httpRequest, &HttpNetworkRequest::replyReady,
                this, &OpenCageDataAPI::reverseResponseHandler);
        connect(httpRequest, &HttpNetworkRequest::replyError,
                this, &OpenCageDataAPI::reverseResponseErrorHandler);

        httpRequest->get();
    }
}

void OpenCageDataAPI::reverseResponseHandler(const QByteArray &data)
{
    qDebug()<<"LOCATION IS: " << data;

    OpenCageData *opencagedata = new OpenCageData;
    opencagedata->setRawData(data);
    emit openCageDataReady(opencagedata);

    sender()->deleteLater();
}

void OpenCageDataAPI::reverseResponseErrorHandler(int errorCode, const QByteArray &data)
{
    qDebug()<<"LOCATION NETWORK ERROR: " << errorCode << data;
    sender()->deleteLater();
}
