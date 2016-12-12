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

#include <QGeoPositionInfo>

class OpenCageData;

class OpenCageDataAPI : public QObject
{
    Q_OBJECT
public:
    enum Status {
        Idle = 1,
        Running,
        Error = 0
    };

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

    Q_ENUM(Status)
    Q_ENUM(Format)
    Q_ENUM(ResponseCode)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QString apiVersion READ apiVersion WRITE setApiVersion NOTIFY apiVersionChanged)
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(int responseCode READ responseCode NOTIFY responseCodeChanged)
    Q_PROPERTY(QString baseUrl READ baseUrl CONSTANT)

    explicit OpenCageDataAPI(QObject *parent = 0);
    ~OpenCageDataAPI();

    int status() const;

    QString key() const;
    void setKey(const QString &key);

    QString apiVersion() const;
    void setApiVersion(const QString &version);

    QString format() const;
    void setFormat(const QString &format);

    int responseCode() const;
    QString baseUrl() const;

public Q_SLOTS:
    void reverseGeoPosition(const QGeoCoordinate &coordinates);

Q_SIGNALS:
    void statusChanged(int status);
    void keyChanged(const QString &key);
    void apiVersionChanged(const QString &version);
    void formatChanged(const QString &format);
    void responseCodeChanged(int code);
    void openCageDataReady(OpenCageData *data);

private Q_SLOTS:
    void reverseResponseHandler(const QByteArray &data);
    void reverseResponseErrorHandler(int errorCode, const QByteArray &data);

private:
    class Private;
    Private *m_data;
};

#endif // OPENCAGEDATAAPI_H
