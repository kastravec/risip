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
#ifndef RISIPGEOPOSITIONPROVIDER_H
#define RISIPGEOPOSITIONPROVIDER_H

#include "risipsdkglobal.h"
#include <QObject>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>

class QGeoCodeReply;
class QGeoCodingManager;

class RISIP_SDK_EXPORT RisipGeoPositionProvider : public QObject
{
    Q_OBJECT
public:

    enum Status {
        Running = 111,
        Idle,
        ProviderError = 222
    };

    Q_ENUM(Status)
    Q_ENUM(QGeoPositionInfoSource::Error)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

    static RisipGeoPositionProvider *instance();
    ~RisipGeoPositionProvider();

    int status() const;
    int updateInterval() const;
    void setUpdateInterval(int interval);
    QGeoCoordinate currentCoordinates() const;

public Q_SLOTS:
    void start();
    void stop();

Q_SIGNALS:
    void statusChanged(int status);
    void updateIntervalChanged(int interval);
    void currentCoordinatesChanged(const QGeoCoordinate &coordinate);

private Q_SLOTS:
    void geoPositionUpdateHandler(const QGeoPositionInfo &info);
    void errorHandler(QGeoPositionInfoSource::Error positioningError);

private:
    explicit RisipGeoPositionProvider(QObject *parent = 0);
    void setStatus(int status);

    static RisipGeoPositionProvider *m_instance;
    class Private;
    Private *m_data;
};

#endif // RISIPGEOPOSITIONPROVIDER_H
