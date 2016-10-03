/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
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
**    A copy of the license is also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/

#include "risipaccount.h"
#include "risipendpoint.h"
#include "risipmessage.h"
#include "risipendpoint.h"
#include "risipaccountconfiguration.h"
#include "risipbuddy.h"

#include <QDebug>

PjsipAccount::PjsipAccount()
    :Account()
    ,m_risipAccount(NULL)
{}

PjsipAccount::~PjsipAccount()
{}

void PjsipAccount::onRegState(OnRegStateParam &prm)
{
    Q_UNUSED(prm);
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
    m_risipAccount->incomingCall(prm.callId);
}

void PjsipAccount::onIncomingSubscribe(OnIncomingSubscribeParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipAccount::onInstantMessage(OnInstantMessageParam &prm)
{
    //creating the account if needed or simply reuse it
    RisipBuddy *buddy = m_risipAccount->findBuddy(QString::fromStdString(prm.fromUri));
    if(!buddy) {
        buddy = new RisipBuddy;
        buddy->setAccount(m_risipAccount);
        buddy->setUri(QString::fromStdString(prm.fromUri));
        m_risipAccount->addBuddy(buddy); //add new buddy to pjsip always
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

/**
 * @brief RisipAccount::RisipAccount
 * @param parent
 *
 * RisipAccount is a wrapper on on the PJSUA2 Account class. It provides a Qt based API ready to be used in QML
 */
RisipAccount::RisipAccount(QObject *parent)
    :QObject(parent)
    ,m_pjsipAccount(NULL)
    ,m_configuration(new RisipAccountConfiguration)
    ,m_sipEndpoint(NULL)
    ,m_status(NotConfigured)
{
}

RisipAccount::~RisipAccount()
{
    if(m_pjsipAccount) {
        delete m_pjsipAccount;
    }
}

RisipAccountConfiguration *RisipAccount::configuration() const
{
    return m_configuration;
}

void RisipAccount::setConfiguration(RisipAccountConfiguration *config)
{
    m_configuration->deleteLater();

    if(m_configuration != config) {
        m_configuration = config;
        if(m_configuration != NULL) {
            setStatus(NotCreated);
            config->setParent(this);
        }
        emit configurationChanged(m_configuration);
    }
}

RisipEndpoint *RisipAccount::sipEndPoint() const
{
    return m_sipEndpoint;
}

void RisipAccount::setSipEndPoint(RisipEndpoint *endpoint)
{
    if(m_sipEndpoint != endpoint) {
        m_sipEndpoint = endpoint;
        emit sipEndPointChanged(m_sipEndpoint);
    }
}

int RisipAccount::status() const
{
    return m_status;
}

QString RisipAccount::statusText() const
{
    switch (m_status) {
    case SignedIn:
        return QString("Signed in.");
    case SignedOut:
        return QString("Signed out.");
    case Registering:
        return QString("Registering.");
    case UnRegistering:
        return QString("Unregistering.");
    case NotConfigured:
        return QString("Not configured.");
    case NotCreated:
        return QString("Not Created.");
    case AccountError:
        return QString("Account internal error!.");
    }
    return QString();
}

QQmlListProperty<RisipBuddy> RisipAccount::buddies()
{
    QList<RisipBuddy *> allBuddies = m_buddies.values();
    return QQmlListProperty<RisipBuddy>(this, allBuddies);
}

void RisipAccount::addBuddy(RisipBuddy *buddy)
{
    if(buddy) {
        if(!m_buddies.contains(buddy->uri())) {
            m_buddies[buddy->uri()] = buddy;
            emit buddiesChanged(buddies());
        }
    }
}

void RisipAccount::removeBuddy(RisipBuddy *buddy)
{
    if(buddy) {
        if(m_buddies.contains(buddy->uri())) {
            m_buddies.remove(buddy->uri());
            buddy->deleteLater();
            emit buddiesChanged(buddies());
        }
    }
}

RisipBuddy *RisipAccount::findBuddy(const QString &uri)
{
    if(m_buddies.contains(uri))
        return m_buddies[uri];

    return 0;
}

PjsipAccount *RisipAccount::pjsipAccount() const
{
    return m_pjsipAccount;
}

void RisipAccount::setPjsipAccountInterface(PjsipAccount *acc)
{
    m_pjsipAccount = acc;
}

int RisipAccount::presence() const
{
    if(m_pjsipAccount != NULL
            && (status() == SignedIn || status() == SignedOut)) {

        switch (m_presence.activity) {
        case PJRPID_ACTIVITY_AWAY:
            return RisipBuddy::Away;
        case PJRPID_ACTIVITY_BUSY:
            return RisipBuddy::Busy;
        }

        if(m_pjsipAccount->getInfo().onlineStatus)
            return RisipBuddy::Online;
        else
            return RisipBuddy::Offline;
    }

    return RisipBuddy::Unknown;
}

void RisipAccount::setPresence(int prs)
{
    if(m_pjsipAccount == NULL)
        return;

    switch (prs) {
    case RisipBuddy::Online:
        if(m_presence.status != PJSUA_BUDDY_STATUS_ONLINE) {
            m_presence.status = PJSUA_BUDDY_STATUS_ONLINE;
            emit presenceChanged(presence());
        }
        break;
    case RisipBuddy::Offline:
        if(m_presence.status != PJSUA_BUDDY_STATUS_OFFLINE) {
            m_presence.status = PJSUA_BUDDY_STATUS_OFFLINE;
            emit presenceChanged(presence());
        }
        break;
    case RisipBuddy::Away:
        if(m_presence.activity != PJRPID_ACTIVITY_AWAY) {
            m_presence.activity = PJRPID_ACTIVITY_AWAY;
            emit presenceChanged(presence());
        }
        break;
    case RisipBuddy::Busy:
        if(m_presence.activity != PJRPID_ACTIVITY_BUSY) {
            m_presence.activity = PJRPID_ACTIVITY_BUSY;
            emit presenceChanged(presence());
        }
        break;
    }

    try {
        m_pjsipAccount->setOnlineStatus(m_presence);
    } catch (Error &err) {
        qDebug()<<"Error setting the presence for the account!" << QString::fromStdString( err.info(true) );
    }
}

QString RisipAccount::presenceNote() const
{
    if(m_pjsipAccount != NULL)
        return QString::fromStdString(m_presence.note);

    return QString();
}

void RisipAccount::setPresenceNote(const QString &note)
{
    if(QString::fromStdString(m_presence.note) != note) {
        m_presence.note = note.toStdString();
        emit presenceNoteChanged(note);

        try {
            m_pjsipAccount->setOnlineStatus(m_presence);
        } catch (Error &err) {
            qDebug()<<"Error setting presence notes for the account!" << QString::fromStdString( err.info(true) );
        }
    }
}

/**
 * @brief RisipAccount::login
 * Creates a SIP Account and registers it with the SIP server. It updates the Account::status property accordinly.
 */
void RisipAccount::login()
{
    if(!m_sipEndpoint)
        return;

    /** if account is not created then initialize it for the first time with
    * username , password and registration.
    */
    if(m_status == NotCreated
            || m_status == SignedOut
            || m_status == AccountError) {


        //create transport and return if not success
       if(!m_sipEndpoint->createTransportNetwork(m_configuration))
           return;

       m_configuration->setTransportId(m_sipEndpoint->activeTransportId());

       // Create the account
       m_pjsipAccount = new PjsipAccount;
       m_pjsipAccount->setRisipInterface(this);
       try {
            m_pjsipAccount->create(m_configuration->pjsipAccountConfig());
        } catch(Error& err) {
            setStatus(AccountError);
            qDebug() << "Account creation error: " << err.info().c_str() << endl;
        }
       //updating status
       setStatus(Registering);
    }
}

void RisipAccount::logout()
{
    if(m_status == SignedIn) {
        try {
            m_pjsipAccount->setRegistration(false);
        } catch(Error& err) {
            setStatus(AccountError);
            qDebug() << "Unregister error: " << err.info().c_str() << endl;
        }
    }
}

/**
 * @brief RisipAccount::setStatus
 * @param status
 *
 * If the status is NotCreated, SignedOut and AccountError then this function
 * deletes the internal pjsipAccount object and the active network transport object.
 *
 */
void RisipAccount::setStatus(int status)
{
    if(m_status != status) {
        m_status = status;

        //delete internal pjsipaccount object when not needed.
        //update buddy list when account is ready
        switch (m_status) {
        case SignedOut:
        case NotCreated:
        case AccountError:
        {
            //deleting the internal pjsipAccount object
            if(m_pjsipAccount) {
                delete m_pjsipAccount;
                m_pjsipAccount = NULL;
            }

            //destroying the active transport network protocol, not needed.
            if(m_sipEndpoint)
                m_sipEndpoint->destroyActiveTransport();

            break;
        }
        default:
            break;
        }

        emit statusChanged(m_status);
        emit statusTextChanged(statusText());
    }
}

void RisipAccount::refreshBuddyList()
{
    if(m_pjsipAccount == NULL)
        return;

    for(int i=0; i<m_buddies.keys().count(); ++i)
        m_buddies.take(m_buddies.keys()[i])->deleteLater();

    BuddyVector buddiesVector;
    try {
        buddiesVector = m_pjsipAccount->enumBuddies();
    } catch (Error &err) {
        qDebug()<<"Buddies cannot be retrieved " << QString::fromStdString(err.info(true));
    }

    RisipBuddy *risipBuddy;
    for(int i=0; i<buddiesVector.size(); ++i) {
        risipBuddy = new RisipBuddy;
        risipBuddy->setPjsipBuddy( static_cast<PjsipBuddy*>(buddiesVector[i]) );
        m_buddies[risipBuddy->uri()] = risipBuddy;
    }

    emit buddiesChanged(buddies());
}


