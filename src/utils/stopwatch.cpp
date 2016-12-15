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
#include "stopwatch.h"

#include <QElapsedTimer>

class StopWatch::Private
{
public:
    int status;
    QElapsedTimer elapsedTimer;
    qint64 elapsedTime;
};

StopWatch::StopWatch(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->elapsedTime = 0.0;
}

StopWatch::~StopWatch()
{
    delete m_data;
    m_data = NULL;
}

int StopWatch::status() const
{
    return m_data->status;
}

qint64 StopWatch::elapsedTime() const
{
    return m_data->elapsedTime;
}

void StopWatch::start()
{
    m_data->elapsedTime = 0.0;
    m_data->status = Running;
    m_data->elapsedTimer.start();
    emit statusChanged(Running);
}

void StopWatch::stop()
{
    m_data->elapsedTime = m_data->elapsedTimer.elapsed();
    m_data->status = Idle;
    emit statusChanged(Idle);
}

void StopWatch::reset()
{
    m_data->elapsedTime = 0.0;
}
