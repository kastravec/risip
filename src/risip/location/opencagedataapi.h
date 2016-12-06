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
#ifndef OPENCAGEDATAAPI_H
#define OPENCAGEDATAAPI_H

#include <QObject>

class OpenCageDataAPI : public QObject
{
    Q_OBJECT
public:
    enum Format {
        Json = 1,
        GeoJson,
        Xml,
        Map
    };

    enum ResponseCode {
        OK = 200,
        InvalidRequest = 400,
        PaymentRequired = 402,
        MissingApiKey = 403,
        InvalidApiEndpoint = 404,
        ReqestTimeOut = 408,
        RequestTooLong = 410,
        InternalServerError = 503
    };

    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QString apiVersion READ apiVersion WRITE setApiVersion NOTIFY apiVersionChanged)
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(int responseCode READ responseCode NOTIFY responseCodeChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(QString countryCode READ countryCode WRITE setCountryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)

    explicit OpenCageDataAPI(QObject *parent = 0);
    ~OpenCageDataAPI();

    QString key() const;
    void setKey(const QString &key);

    QString apiVersion() const;
    void setApiVersion(const QString &version);

    QString format() const;
    void setFormat(const QString &format);

    int responseCode() const;

    double longitude() const;
    void setLongitude(double val);

    double latitude() const;
    void setLatitude(double val);

    QString countryCode() const;
    void setCountryCode(const QString &code);

    QString city() const;

Q_SIGNALS:
    void keyChanged(const QString &key);
    void apiVersionChanged(const QString &version);
    void formatChanged(const QString &format);
    void responseCodeChanged(int code);
    void longitudeChanged(double val);
    void latitudeChanged(double val);
    void countryCodeChanged(const QString &code);
    void cityChanged(const QString &city);

private Q_SLOTS:
    void networkResponseHandler(const QByteArray &data);

private:
    class Private;
    Private *m_data;
};

#endif // OPENCAGEDATAAPI_H
