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

class OpenCageDataAPI::Private
{
public:
    QString key;
    QString version;
    QString format;
    int responsecode;
    int longitude;
    int latitude;
    QString countrycode;
    QString city;
};


OpenCageDataAPI::OpenCageDataAPI(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{

}

OpenCageDataAPI::~OpenCageDataAPI()
{
    delete m_data;
    m_data  = NULL;
}

QString OpenCageDataAPI::key() const
{
    m_data->key;
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

double OpenCageDataAPI::longitude() const
{
    return m_data->longitude;
}

void OpenCageDataAPI::setLongitude(double val)
{

}

double OpenCageDataAPI::latitude() const
{
    return m_data->latitude;
}

void OpenCageDataAPI::setLatitude(double val)
{

}

QString OpenCageDataAPI::countryCode() const
{
    return m_data->countrycode;
}

void OpenCageDataAPI::setCountryCode(const QString &code)
{
    if(m_data->countrycode != code) {
        m_data->countrycode = code;
        emit countryCodeChanged(m_data->countrycode);
    }
}

QString OpenCageDataAPI::city() const
{
    return m_data->city;
}

void OpenCageDataAPI::networkResponseHandler(const QByteArray &data)
{

}
