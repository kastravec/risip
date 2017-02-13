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
#include "pjsipaccount.h"

#include "risipaccount.h"
#include "risipbuddy.h"
#include "risipmessage.h"

#include "pjsipcall.h"

#include <QDebug>

namespace risip {

/**
 * @class PjsipAccount class
 * @brief The PjsipAccount class
 * This is an internal class. It inherits the Account class from Pjsip and implements the respective
 * callbacks, see Pjsip Account C++ API reference.
 *
 * Instances of this class are created from RisipAccount class, which is the Qt wrapper representative.
 * PjsipAccount contains the pointer to the RisipAccount which created it, in order to
 * in order to send messages/callbacks to the wrapper class.
 *
 */
PjsipAccount::PjsipAccount()
    :Account()
    ,m_risipAccount(NULL)
{}

PjsipAccount::~PjsipAccount()
{}

void PjsipAccount::onRegState(OnRegStateParam &prm)
{
    if(m_risipAccount != NULL) {
        m_risipAccount->setLastResponseCode(prm.code);
        AccountInfo accountInfo = getInfo();
        switch (accountInfo.regStatus) {
        case PJSIP_SC_OK:
            if(accountInfo.regIsActive)
                m_risipAccount->setStatus(RisipAccount::SignedIn);
            else
                m_risipAccount->setStatus(RisipAccount::SignedOut);
            break;
        case PJSIP_SC_TRYING:
            if(accountInfo.regIsActive)
                m_risipAccount->setStatus(RisipAccount::UnRegistering);
            else
                m_risipAccount->setStatus(RisipAccount::Registering);
            break;
        default:
            m_risipAccount->setStatus(RisipAccount::AccountError);
            break;
        }
    }
}

void PjsipAccount::onRegStarted(OnRegStartedParam &prm)
{
    if(prm.renew)
        m_risipAccount->setStatus(RisipAccount::Registering);
    else
        m_risipAccount->setStatus(RisipAccount::UnRegistering);
}

/**
 * @brief PjsipAccount::onIncomingCall
 * @param prm
 *
 * Callback from the PJSIP library, an incoming call.
 */
void PjsipAccount::onIncomingCall(OnIncomingCallParam &prm)
{
    if(!m_risipAccount)
        return;

    m_risipAccount->setIncomingPjsipCall(new PjsipCall(*m_risipAccount->pjsipAccount(), prm.callId));
}

void PjsipAccount::onIncomingSubscribe(OnIncomingSubscribeParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipAccount::onInstantMessage(OnInstantMessageParam &prm)
{
    qDebug()<< "Incoming IM : " << QString::fromStdString(prm.fromUri) << QString::fromStdString(prm.msgBody);

    //creating the account if needed or simply reuse it
    RisipBuddy *buddy = m_risipAccount->findBuddy(QString::fromStdString(prm.fromUri));
    if(!buddy) {
        buddy = new RisipBuddy;
        buddy->setAccount(m_risipAccount);
        buddy->setUri(QString::fromStdString(prm.fromUri));
    }

    //constructing a risip message to be passed around
    RisipMessage *message = new RisipMessage;
    message->setBuddy(buddy);
    message->setDirection(RisipMessage::Incoming);
    QString msgBody = QString::fromStdString(prm.msgBody);
    QString contenttype = QString::fromStdString(prm.contentType);
    message->setMessageBody(msgBody);
    message->setContentType(contenttype);
    m_risipAccount->incomingMessage(message);
}

void PjsipAccount::onInstantMessageStatus(OnInstantMessageStatusParam &prm)
{
    RisipMessage *message = static_cast<RisipMessage *>(prm.userData);
    switch (prm.code) {
    case PJSIP_SC_OK:
    case PJSIP_SC_ACCEPTED:
        message->setStatus(RisipMessage::Sent);
        qDebug()<<"Message delivered: " <<message->messageBody();
        break;
    default:
        message->setStatus(RisipMessage::Failed);
        break;
    }
}

void PjsipAccount::onTypingIndication(OnTypingIndicationParam &prm)
{
    Q_UNUSED(prm)
}

//buddy subscribe / notify callbacks
void PjsipAccount::onMwiInfo(OnMwiInfoParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipAccount::setRisipInterface(RisipAccount *acc)
{
    m_risipAccount = acc;
}

} //end of risip namespace
