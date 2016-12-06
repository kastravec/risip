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
#include "risipmordevice.h"
#include "risipmorapi.h"

class RisipMorDevice::Private
{
public:
    int status;
    int type;
    QString username;
    QString password;
    QString extension;
    QString callerId;
    RisipMorApi *morApi;
};

RisipMorDevice::RisipMorDevice(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->morApi = new RisipMorApi(this);
    m_data->callerId = QString("");
}

RisipMorDevice::~RisipMorDevice()
{
    delete m_data;
    m_data = NULL;
}

int RisipMorDevice::status() const
{
    return m_data->status;
}

int RisipMorDevice::type() const
{
    return m_data->type;
}

void RisipMorDevice::setType(int type)
{
    if(m_data->type != type) {
        m_data->type = type;
        emit typeChanged(m_data->type);
    }
}

QString RisipMorDevice::username() const
{
    return m_data->username;
}

void RisipMorDevice::setUsername(const QString &name)
{
    if(m_data->username != name) {
        m_data->username = name;
        emit usernameChanged(m_data->username);
    }
}

QString RisipMorDevice::password() const
{
    return m_data->password;
}

void RisipMorDevice::setPassword(const QString &pass)
{
    if(m_data->password != pass) {
        m_data->password = pass;
        emit passwordChanged(m_data->password);
    }
}

QString RisipMorDevice::extension() const
{
    return m_data->extension;
}

void RisipMorDevice::setExtension(const QString &ext)
{
    if(m_data->extension != ext) {
        m_data->extension = ext;
        emit extensionChanged(m_data->extension);
    }
}

QString RisipMorDevice::callerId() const
{
    return m_data->callerId;
}

void RisipMorDevice::setCallerId(const QString &id)
{
    if(m_data->callerId != id) {
        m_data->callerId = id;
        emit callerIdChanged(m_data->callerId);
    }
}

void RisipMorDevice::create()
{

}

void RisipMorDevice::update()
{

}

void RisipMorDevice::creationHandler()
{
    setStatus(Ready);
}

void RisipMorDevice::updateHandler()
{
    setStatus(Ready);
}

void RisipMorDevice::setStatus(int status)
{
    if(m_data->status != status) {
        m_data->status = status;
        emit statusChanged(m_data->status);
    }
}

