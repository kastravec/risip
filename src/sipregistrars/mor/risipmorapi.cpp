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

#include "risipaccountprofile.h"
#include "risipaccount.h"
#include "utils/httpnetworkrequest.h"
#include "risipmorapi.h"

RisipMorApi *RisipMorApi::m_instance = NULL;
RisipMorApi::RisipMorApi(QObject *parent)
    :QObject(parent)
{
}

RisipMorApi *RisipMorApi::instance()
{
    if(!m_instance)
        m_instance = new RisipMorApi;
    return m_instance;
}

RisipMorApi::~RisipMorApi()
{
}

int RisipMorApi::status() const
{
    return m_status;
}

RisipAccountProfile *RisipMorApi::getUserProfile(const QString &username)
{
    return 0;
}

void RisipMorApi::registerAccount(RisipAccountProfile *profile)
{
    if(profile && profile->valid()) {
        HttpNetworkRequest *httpRequest = new HttpNetworkRequest(this);
        httpRequest->setRisipData(profile);
        connect(httpRequest, &HttpNetworkRequest::replyReady,
                this, &RisipMorApi::accountRegstrationHandler);

        profile->setStatus(RisipAccountProfile::Registering);
        httpRequest->get();
    }
}

void RisipMorApi::getUserBalance(const QString &username)
{

}

void RisipMorApi::getUserBalance(RisipAccountProfile *profile)
{

}

void RisipMorApi::accountRegstrationHandler(const QByteArray &data)
{
    HttpNetworkRequest *httpReply = qobject_cast<HttpNetworkRequest *>(sender());
    RisipAccountProfile *profile = static_cast<RisipAccountProfile *>(httpReply->risipData());

    qDebug()<<"HTTP reply " << data << httpReply->status() << " profile: " << profile->username();

    httpReply->deleteLater();
}

void RisipMorApi::accountRegistrationError(int errorCode)
{
    qDebug()<<"HTTP account registration error: " << errorCode;
}

void RisipMorApi::userBalanceUpdateHandler(const QByteArray &data)
{

}

void RisipMorApi::userBalanceUpdateError(int errorCode)
{

}
