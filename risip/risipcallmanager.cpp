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
#include "risipcallmanager.h"

#include "globals.h"
#include "risip.h"
#include "risipcall.h"
#include "risipaccount.h"
#include "risipbuddy.h"
#include "risipaccountconfiguration.h"

#include <QDebug>

RisipCallManager *RisipCallManager::m_callManagerInstance = NULL;
RisipCallManager *RisipCallManager::instance()
{
    if(m_callManagerInstance == NULL)
        m_callManagerInstance = new RisipCallManager;

    return m_callManagerInstance;
}

RisipCallManager::RisipCallManager(QObject *parent)
    :QObject(parent)
{
    connect(Risip::instance(), &Risip::defaultAccountChanged,
            this, &RisipCallManager::setDefaultAccount, Qt::UniqueConnection);
}

/**
 * @brief RisipCallManager::setActiveCall
 * @param call
 *
 * Internal API . No need to use this anywhere.
 */
void RisipCallManager::setActiveCall(RisipCall *call)
{
    if(m_activeCall != call) {
        m_activeCall = call;
        emit activeCallChanged(m_activeCall);
    }
}

RisipCallManager::~RisipCallManager()
{
    m_callManagerInstance = NULL;
}

RisipAccount *RisipCallManager::defaultAccount()
{
    return m_defaultAccount;
}

void RisipCallManager::setDefaultAccount(RisipAccount *account)
{
    if(m_defaultAccount !=account) {
        disconnect(m_defaultAccount);
        m_defaultAccount = account;

        if(m_defaultAccount)
            connect(m_defaultAccount, &RisipAccount::incomingCall,
                    this, &RisipCallManager::accountIncomingCall, Qt::UniqueConnection);

        emit defaultAccountChanged(m_defaultAccount);
    }
}

RisipCall *RisipCallManager::activeCall()
{
    return m_activeCall;
}

void RisipCallManager::callContact(const QString contact)
{
    RisipBuddy *buddy = m_defaultAccount->findBuddy(
                RisipGlobals::formatToSip(contact, m_defaultAccount->configuration()->uri()));
    if(buddy == NULL) {
        buddy = new RisipBuddy;
        buddy->setAccount(defaultAccount());
        buddy->setContact(contact);
    }

    callBuddy(buddy);
}

void RisipCallManager::callBuddy(RisipBuddy *buddy)
{
    RisipCall *call = new RisipCall(this);
    call->setBuddy(buddy);
    call->setAccount(m_defaultAccount);
    m_callHistory.append(call);
    call->call();

    qDebug()<<"calling contact: " << call->buddy()->uri();

    emit outgoingCall(call);
}

void RisipCallManager::callPhone(const QString &number)
{
}

void RisipCallManager::accountIncomingCall()
{
    RisipCall *call = new RisipCall(this);
    call->setAccount(m_defaultAccount);
    call->initiateIncomingCall();
    m_callHistory.append(call);

    QString remoteUri = QString::fromStdString(call->pjsipCall()->getInfo().remoteUri);
    RisipBuddy *buddy = m_defaultAccount->findBuddy(remoteUri);
    if(buddy == NULL) {
        buddy = new RisipBuddy;
        buddy->setUri(remoteUri);
        buddy->setAccount(defaultAccount());
    }

    call->setBuddy(buddy);
    qDebug()<<"incoming call from contact : " << call->buddy()->uri();
    emit incomingCall(call);
}

void RisipCallManager::updateDefaultAccount()
{
    setDefaultAccount(Risip::instance()->defaultAccount());
}
