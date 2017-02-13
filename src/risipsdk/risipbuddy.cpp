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

#include "risipbuddy.h"
#include "risipmessage.h"
#include "risipcall.h"
#include "risipaccountconfiguration.h"
#include "risipmodels.h"

#include "pjsipwrapper/pjsipaccount.h"
#include "pjsipwrapper/pjsipbuddy.h"

#include <QDebug>

namespace risip {

class RisipBuddy::Private
{
public:
    Error error;
    PjsipBuddy *pjsipBuddy;
    RisipAccount *account;
    BuddyConfig buddyConfig;
    QString contact;
    int type;
};

RisipBuddy::RisipBuddy(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->pjsipBuddy = NULL;
    m_data->account = NULL;
    m_data->type = Internal;

    //always subscribe to the buddy presence
    m_data->buddyConfig.subscribe = true;
}

RisipBuddy::~RisipBuddy()
{
    delete m_data;
    m_data = NULL;
}

RisipAccount *RisipBuddy::account() const
{
    return m_data->account;
}

void RisipBuddy::setAccount(RisipAccount *acc)
{
    if(m_data->account != acc) {
        m_data->account = acc;

        if(!m_data->contact.isEmpty() && m_data->account)
            setUri(QString("<sip:")
                   + m_data->contact
                   + QString("@")
                   + m_data->account->configuration()->serverAddress()
                   + QString(">"));

        emit accountChanged(m_data->account);
    }
}

QString RisipBuddy::uri() const
{
    return QString::fromStdString(m_data->buddyConfig.uri);
}

//TODO what about the contact?
void RisipBuddy::setUri(QString contactUri)
{
    //FIXME too many conversions of strings here..
    if(QString::fromStdString(m_data->buddyConfig.uri) != contactUri) {
        m_data->buddyConfig.uri = contactUri.toStdString();
        emit uriChanged(contactUri);
        emit contactChanged(contact());
    }
}

QString RisipBuddy::contact()
{
    if(m_data->account) {
        m_data->contact = uri().remove(QString("<sip:"));
        m_data->contact = m_data->contact.remove(QString("@") + m_data->account->configuration()->serverAddress() + QString(">"));
    }

    return m_data->contact;
}

void RisipBuddy::setContact(const QString contact)
{
    if(m_data->contact != contact) {
        m_data->contact = contact;

        if(type() == Internal) {
            if(!m_data->contact.isEmpty() && m_data->account)
                setUri(QString("<sip:")
                       + m_data->contact
                       + QString("@")
                       + m_data->account->configuration()->serverAddress()
                       + QString(">"));
        }

        emit contactChanged(m_data->contact);
    }
}

int RisipBuddy::type() const
{
    return m_data->type;
}

void RisipBuddy::setType(int type)
{
    if(m_data->type != type) {
        m_data->type = type;
        emit typeChanged(m_data->type);
    }
}

PjsipBuddy *RisipBuddy::pjsipBuddy() const
{
    return m_data->pjsipBuddy;
}

/**
 * @brief RisipBuddy::setPjsipBuddy
 * @param buddy internal pjsip buddy object
 *
 * Internal API. Do NOT use.
 */
void RisipBuddy::setPjsipBuddy(PjsipBuddy *buddy)
{
    if(m_data->pjsipBuddy)
        delete m_data->pjsipBuddy;

    m_data->pjsipBuddy = buddy;
    m_data->pjsipBuddy->setRisipInterface(this);
    m_data->buddyConfig.uri = buddy->getInfo().uri;
}

bool RisipBuddy::valid() const
{
    if(uri().isEmpty())
        return false;

    return true;
}

void RisipBuddy::setContactNumber(const QString &number)
{
    setContact(number);
    setType(Pstn);
}

void RisipBuddy::create()
{
    //check if uri and account are set - buddy cannot be created without those properties
    if(uri().isEmpty()
            || m_data->account == NULL)
        return;

    if(m_data->pjsipBuddy != NULL) {
        if(m_data->account->findBuddy(uri()))
            return;
    } else {
        delete m_data->pjsipBuddy;
        m_data->pjsipBuddy = NULL;
        m_data->pjsipBuddy = new PjsipBuddy;
        m_data->pjsipBuddy->setRisipInterface(this);
        try {
            m_data->pjsipBuddy->create(*m_data->account->pjsipAccount(), m_data->buddyConfig);
        } catch (Error &err) {
            setError(err);
        }
    }
}

/**
 * @brief RisipBuddy::release
 *
 * Removes this buddy from the list and destroys it. All pending and failed messages are deleted.
 * Must call this function in order to remove a contact and delete any other application reference to it.
 */
void RisipBuddy::releaseFromAccount()
{
    delete m_data->pjsipBuddy;
    m_data->pjsipBuddy = NULL;
}

RisipMessage *RisipBuddy::sendInstantMessage(QString message)
{
    if(m_data->pjsipBuddy == NULL
            || m_data->account == NULL)
        return NULL;

    if(m_data->account->status() == RisipAccount::SignedIn
            && m_data->pjsipBuddy->isValid()) {
        RisipMessage *risipMessage = new RisipMessage;
        risipMessage->setMessageBody(message);
        sendInstantMessage(risipMessage);
        return risipMessage;
    }

    return NULL;
}

void RisipBuddy::sendInstantMessage(RisipMessage *message)
{
    if(m_data->pjsipBuddy == NULL
            || m_data->account == NULL
            || message == NULL)
        return;

    if(m_data->account->status() == RisipAccount::SignedIn
            && m_data->pjsipBuddy->isValid()) {
        message->setBuddy(this);
        message->setStatus(RisipMessage::Pending);
        message->setDirection(RisipMessage::Outgoing);

        qDebug()<<"SENDING INSTANT MESSAGE: " << uri() << message->messageBody();

        try {
            m_data->pjsipBuddy->sendInstantMessage(message->messageParamForSend());
        } catch (Error &err) {
            setError(err);
        }
    }
}

void RisipBuddy::setError(const Error &error)
{
    qDebug()<<"ERROR: " <<"code: "<<error.status <<" info: " << QString::fromStdString(error.info(true));

    if(m_data->error.status != error.status) {

        m_data->error.status = error.status;
        m_data->error.reason = error.reason;
        m_data->error.srcFile = error.srcFile;
        m_data->error.srcLine = error.srcLine;
        m_data->error.title = error.title;

//        emit errorCodeChanged(m_error.status);
//        emit errorMessageChanged(QString::fromStdString(m_error.reason));
//        emit errorInfoChanged(QString::fromStdString(m_error.info(true)));
    }
}

int RisipBuddy::presence() const
{
    if(m_data->pjsipBuddy == NULL
            || m_data->account == NULL)
        return Null;

    switch (m_data->pjsipBuddy->getInfo().presStatus.status) {
    case PJSUA_BUDDY_STATUS_ONLINE:
        return Online;
    case PJSUA_BUDDY_STATUS_OFFLINE:
        return Offline;
    case PJSUA_BUDDY_STATUS_UNKNOWN:
        return Unknown;
    }

    return Unknown;
}

} //end of risip namespace
