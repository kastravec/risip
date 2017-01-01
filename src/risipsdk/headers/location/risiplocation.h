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
#ifndef RISIPLOCATION_H
#define RISIPLOCATION_H

#include "risipsdkglobal.h"
#include <QObject>
#include <QGeoCoordinate>

class OpenCageData;
class RISIP_SDK_EXPORT RisipLocation : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QGeoCoordinate currentCoordinates READ currentCoordinates NOTIFY currentCoordinatesChanged)

    static RisipLocation *instance();
    ~RisipLocation();

    QGeoCoordinate currentCoordinates() const;

public Q_SLOTS:
    void updateCurrentLocation();

Q_SIGNALS:
    void currentCoordinatesChanged(const QGeoCoordinate &coordinates);

private Q_SLOTS:
    void openCageDataReplyHandler(OpenCageData *data);

private:
    explicit RisipLocation(QObject *parent = 0);
    static RisipLocation *m_instance;

    class Private;
    Private *m_data;
};

#endif // RISIPLOCATION_H
