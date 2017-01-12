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
#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "risipsdkglobal.h"
#include <QObject>

class RISIP_VOIPSDK_EXPORT StopWatch : public QObject
{
    Q_OBJECT
public:

    enum Status {
        Running = 1,
        Idle = 0
    };

    Q_ENUM(Status)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(qint64 elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)

    explicit StopWatch(QObject *parent = 0);
    ~StopWatch();

    int status() const;
    qint64 elapsedTime() const;

public Q_SLOTS:
    void start();
    void stop();
    void reset();

Q_SIGNALS:
    void statusChanged(int status);
    void elapsedTimeChanged(qint64 time);

private:
    class Private;
    Private *m_data;
};

#endif // STOPWATCH_H
