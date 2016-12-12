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

#include "opencagedata.h"

class OpenCageData::Private
{
public:
    double longitude;
    double latitude;
    QString countrycode;
    QString city;
    QByteArray rawData;
};

OpenCageData::OpenCageData(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{

}

OpenCageData::~OpenCageData()
{
    delete m_data;
    m_data = NULL;
}

double OpenCageData::longitude() const
{
    return m_data->longitude;
}

void OpenCageData::setLongitude(double val)
{

}

double OpenCageData::latitude() const
{
    return m_data->latitude;
}

void OpenCageData::setLatitude(double val)
{

}

QString OpenCageData::countryCode() const
{
    return m_data->countrycode;
}

void OpenCageData::setCountryCode(const QString &code)
{
    if(m_data->countrycode != code) {
        m_data->countrycode = code;
        emit countryCodeChanged(m_data->countrycode);
    }
}

QString OpenCageData::city() const
{
    return m_data->city;
}

QByteArray OpenCageData::rawData() const
{
    return m_data->rawData;
}

void OpenCageData::setRawData(const QByteArray &rawdata)
{
    if(m_data->rawData != rawdata) {
        m_data->rawData = rawdata;
        emit rawDataChanged(m_data->rawData);
    }
}
