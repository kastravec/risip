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
#include "risipmorapi.h"

#include "utils/httpnetworkrequest.h"
#include "risipmoruser.h"
#include "risipuserprofile.h"
#include "risipmordevice.h"

class RisipMorApi::Private
{
public:
    int lcrId;
    QString uniqueHash;
    QString secretKey;
    QString host;
};

RisipMorApi::RisipMorApi(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    //default values
    m_data->secretKey = QString("masakerfare");
    m_data->uniqueHash = QString("db1c570a60");
    m_data->host = QString("148.251.48.171");
    m_data->lcrId = 3;
}

RisipMorApi::~RisipMorApi()
{
    delete m_data;
    m_data = NULL;
}

int RisipMorApi::lcrId() const
{
    return m_data->lcrId;
}

QString RisipMorApi::uniqueHash() const
{
    return m_data->uniqueHash;
}

void RisipMorApi::setUniqueHash(const QString &hash)
{
    if(m_data->uniqueHash != hash) {
        m_data->uniqueHash = hash;
        emit uniqueHashChanged(m_data->uniqueHash);
    }
}

QString RisipMorApi::secretKey() const
{
    return m_data->secretKey;
}

void RisipMorApi::setSecretKey(const QString &key)
{
    if(m_data->secretKey != key) {
        m_data->secretKey = key;
        emit secretKeyChanged(m_data->secretKey);
    }
}

QString RisipMorApi::host() const
{
    return m_data->host;
}

void RisipMorApi::setHost(const QString &host)
{
    if(m_data->host != host) {
        m_data->host = host;
        emit hostChanged(m_data->host);
    }
}


