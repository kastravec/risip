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
#ifndef OPENCAGEDATA_H
#define OPENCAGEDATA_H

#include <QObject>

class OpenCageData : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(QString countryCode READ countryCode WRITE setCountryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(QByteArray rawData READ rawData NOTIFY rawDataChanged)

    explicit OpenCageData(QObject *parent = 0);
    ~OpenCageData();

    double longitude() const;
    void setLongitude(double val);

    double latitude() const;
    void setLatitude(double val);

    QString countryCode() const;
    void setCountryCode(const QString &code);

    QString city() const;

    QByteArray rawData() const;
    void setRawData(const QByteArray &rawdata);

Q_SIGNALS:
    void longitudeChanged(double val);
    void latitudeChanged(double val);
    void countryCodeChanged(const QString &code);
    void cityChanged(const QString &city);
    void rawDataChanged(const QByteArray &data);

private:
    class Private;
    Private *m_data;
};

#endif // OPENCAGEDATA_H
