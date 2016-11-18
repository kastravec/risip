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

#include "risipaccount.h"
#include "risipendpoint.h"
#include "risipmessage.h"
#include "risipendpoint.h"
#include "risipaccountconfiguration.h"
#include "risipbuddy.h"
#include "risipcall.h"
#include "risipaccountprofile.h"
#include "risipcontactmanager.h"
#include "risipmodels.h"

#include <QDebug>

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
    if(!m_risipAccount)
        return;

    m_risipAccount->setIncomingPjsipCall(new PjsipCall(*m_risipAccount->pjsipAccount(), prm.callId));
    m_risipAccount->incomingCall();
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
 * @class RisipAccount
 * @brief RisipAccount represents the Account class of Pjsip C++ API. It creates instances of Account classes (PjsipAccount)
 * and manages these instances.
 */
RisipAccount::RisipAccount(QObject *parent)
    :QObject(parent)
    ,m_pjsipAccount(NULL)
    ,m_profile(NULL)
    ,m_configuration(new RisipAccountConfiguration(this))
    ,m_sipEndpoint(NULL)
    ,m_autoSignIn(true)
    ,m_status(NotCreated)
    ,m_incomingPjsipCall(NULL)
    ,m_allBuddies()
    ,m_error()

{
}

RisipAccount::~RisipAccount()
{
    while (!m_allBuddies.isEmpty())
        m_allBuddies.takeFirst()->releaseFromAccount();

    delete m_pjsipAccount;
    m_pjsipAccount = NULL;

    delete m_profile;
    m_profile = NULL;
}

RisipAccountProfile *RisipAccount::profile()
{
    return m_profile;
}

void RisipAccount::setProfile(RisipAccountProfile *profile)
{
    if(m_profile != profile) {
        delete m_profile;
        m_profile = NULL;

        m_profile = profile;
        if(m_profile)
            m_profile->setParent(this);

        emit profileChanged(m_profile);
    }
}

RisipAccountConfiguration *RisipAccount::configuration() const
{
    return m_configuration;
}

/**
 * @brief RisipAccount::setConfiguration
 * @param config setting it as the configuration object for this account.
 *
 * This function also changes ownership of the config object, it is passed to the account.
 */
void RisipAccount::setConfiguration(RisipAccountConfiguration *config)
{
    if(m_configuration != config) {

        delete m_configuration;
        m_configuration = NULL;
        m_configuration = config;

        if(m_configuration != NULL) {
            m_configuration->setParent(this);
            setStatus(NotCreated);
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

/**
 * @brief RisipAccount::buddies
 * @return a list of all buddies
 *
 */
QQmlListProperty<RisipBuddy> RisipAccount::buddies()
{
    if(m_pjsipAccount) {
        BuddyVector buddies = m_pjsipAccount->enumBuddies();
        RisipBuddy *buddy;
        for(int i=0; i<m_allBuddies.count(); ++i) {
            buddy = new RisipBuddy(this);
            buddy->setAccount(this);
            buddy->setPjsipBuddy(static_cast<PjsipBuddy *>(buddies[i]));
            m_allBuddies.append(buddy);
        }
    }

    return QQmlListProperty<RisipBuddy>(this, m_allBuddies);
}

int RisipAccount::errorCode() const
{
    return m_error.status;
}

QString RisipAccount::errorMessage() const
{
    return QString::fromStdString(m_error.reason);
}

QString RisipAccount::errorInfo() const
{
    return QString::fromStdString(m_error.info(true));
}

/**
 * @brief RisipAccount::incomingPjsipCall
 * @return
 *
 * Internal API.
 *
 * Returns pointer to newly incoming call.
 */
PjsipCall *RisipAccount::incomingPjsipCall()
{
    return m_incomingPjsipCall;
}

/**
 * @brief RisipAccount::setIncomingPjsipCall
 * @param call
 *
 * Internal API.
 * Do not use this function, unless you are SURE of what you are doing.
 *
 */
void RisipAccount::setIncomingPjsipCall(PjsipCall *call)
{
    if(m_incomingPjsipCall != call)
        m_incomingPjsipCall = call;
}

/**
 * @brief RisipAccount::findBuddy
 * @param uri buddy uri used as search parameter
 * @return the RisipBuddy pointer is return if buddy found, otherwise just NULL
 */
RisipBuddy *RisipAccount::findBuddy(const QString &uri)
{
    try {
        PjsipBuddy *pjsipBuddy = static_cast<PjsipBuddy *>(m_pjsipAccount->findBuddy(uri.toStdString()));
        if(pjsipBuddy) {
            return pjsipBuddy->risipInterface();
        }
    } catch (Error &err) {
        setError(err);
    }

    return NULL;
}

/**
 * @brief RisipAccount::addBuddy
 * @param buddyUri uri of the new buddy to be added
 *
 * Use this function to add a new buddy to this account
 */
void RisipAccount::addBuddy(const QString &buddyUri)
{
    if(!m_pjsipAccount)
        return;

    if(findBuddy(buddyUri))
        return;

    RisipBuddy *buddy = new RisipBuddy(this);
    buddy->setUri(buddyUri);
    addRisipBuddy(buddy);
}

/**
 * @brief RisipAccount::addRisipBuddy
 * @param buddy buddy to be added to this account
 *
 * Use this function to add this buddy to this account.
 * NOTE: uri of the buddy should be valid before setting it otherwise it will fail
 */
void RisipAccount::addRisipBuddy(RisipBuddy *buddy)
{
    if(!buddy)
        return;

    if(findBuddy(buddy->uri()))
        return;

    buddy->setAccount(this);
    buddy->create();

    //FIXME check if no model found
    RisipBuddiesModel *model = qobject_cast<RisipBuddiesModel*>(RisipContactManager::instance()->buddyModelForAccount(m_configuration->uri()));
    model->addBuddy(buddy);
}

/**
 * @brief RisipAccount::pjsipAccount
 * @return the pjsip object pointer
 *
 * Internal API.
 */
PjsipAccount *RisipAccount::pjsipAccount() const
{
    return m_pjsipAccount;
}

/**
 * @brief RisipAccount::setPjsipAccountInterface
 * @param acc
 *
 * Internal API.
 */
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

    qDebug()<<"SETTING PRESENCE: " <<m_presence.status;
    try {
        m_pjsipAccount->setOnlineStatus(m_presence);
    } catch (Error &err) {
        setError(err);
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
            setError(err);
        }
    }
}

bool RisipAccount::autoSignIn() const
{
    return m_autoSignIn;
}

void RisipAccount::setAutoSignIn(bool signin)
{
    if(m_autoSignIn != signin) {
        m_autoSignIn = signin;
        emit autoSignInChanged(m_autoSignIn);
    }
}

/**
 * @brief RisipAccount::login
 * Creates a SIP Account and registers it with the SIP server. It updates the Account::status property accordinly.
 *
 * Use this function to login with this account.
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
            || m_status == AccountError
            || m_status == NotConfigured) {

        //create transport if none exists and return if not success
        if(m_sipEndpoint->activeTransportId() == -1) {
            if(!m_sipEndpoint->createTransportNetwork(m_configuration)) {
                setStatus(NotConfigured);
                return;
            }
        }

        m_configuration->setTransportId(m_sipEndpoint->activeTransportId());

        // Create the account
        m_pjsipAccount = new PjsipAccount;
        m_pjsipAccount->setRisipInterface(this);
        try {
            m_pjsipAccount->create(m_configuration->pjsipAccountConfig());
        } catch (Error& err) {
            setStatus(AccountError);
            setError(err);
            return;
        }
        //updating status
        setStatus(Registering);
        setPresence(RisipBuddy::Online);
    }
}

/**
 * @brief RisipAccount::logout
 *
 * Use this function to logout with this account.
 */
void RisipAccount::logout()
{
    if(m_status == SignedIn) {
        try {
            m_pjsipAccount->setRegistration(false);
        } catch(Error& err) {
            setStatus(AccountError);
            setError(err);
        }
    }
}

/**
 * @brief RisipAccount::setStatus
 * @param status
 *
 * Internal API.
 * You never need to use this function. Unless you know what ou are doing.
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
        case SignedIn:
            setPresence(RisipBuddy::Online);
            break;
        case SignedOut:
        case NotCreated:
        case AccountError:
        {
            //destroying list of risip buddy objects
            while (!m_allBuddies.isEmpty())
                m_allBuddies.takeFirst()->releaseFromAccount();

            //deleting the internal pjsipAccount object
            delete m_pjsipAccount;
            m_pjsipAccount = NULL;
            break;
        }
        }

        emit statusChanged(m_status);
        emit statusTextChanged(statusText());
    }
}

void RisipAccount::setError(const Error &error)
{
    qDebug()<<"ERROR: " <<"code: "<<error.status <<" info: " << QString::fromStdString(error.info(true));

    if(m_error.status != error.status) {

        m_error.status = error.status;
        m_error.reason = error.reason;
        m_error.srcFile = error.srcFile;
        m_error.srcLine = error.srcLine;
        m_error.title = error.title;

        emit errorCodeChanged(m_error.status);
        emit errorMessageChanged(QString::fromStdString(m_error.reason));
        emit errorInfoChanged(QString::fromStdString(m_error.info(true)));
    }
}
