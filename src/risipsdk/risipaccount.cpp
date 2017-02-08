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
#include "risipcontactmanager.h"
#include "risipmodels.h"
#include "risip.h"

#include "pjsipwrapper/pjsipaccount.h"
#include "pjsipwrapper/pjsipbuddy.h"

#include <QDebug>

class RisipAccount::Private
{
public:
    PjsipAccount *pjsipAccount;
    RisipAccountConfiguration *configuration;
    RisipEndpoint *sipEndpoint;
    PresenceStatus presence_pjsip;
    int presence_risip;
    bool autoSignIn;
    int status;
    PjsipCall *incomingPjsipCall;
    QList<RisipBuddy *> allBuddies;
    Error error;
    int lastSipResponseCode;
};

/**
 * @class RisipAccount
 * @brief RisipAccount represents the Account class of Pjsip C++ API. It creates instances of Account classes (PjsipAccount)
 * and manages these instances.
 */
RisipAccount::RisipAccount(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->pjsipAccount = NULL;
    m_data->configuration = new RisipAccountConfiguration(this);
    m_data->sipEndpoint = NULL;
    m_data->autoSignIn = true;
    m_data->status = NotCreated;
    m_data->incomingPjsipCall = NULL;
    m_data->lastSipResponseCode = Risip::PJSIP_SC_OK;
    m_data->presence_risip = RisipBuddy::Online;
}

RisipAccount::~RisipAccount()
{
    while (!m_data->allBuddies.isEmpty())
        m_data->allBuddies.takeFirst()->releaseFromAccount();

    delete m_data->pjsipAccount;
    m_data->pjsipAccount = NULL;

    delete m_data;
    m_data = NULL;
}

RisipAccountConfiguration *RisipAccount::configuration() const
{
    return m_data->configuration;
}

/**
 * @brief RisipAccount::setConfiguration
 * @param config setting it as the configuration object for this account.
 *
 * This function also changes ownership of the config object, it is passed to the account.
 */
void RisipAccount::setConfiguration(RisipAccountConfiguration *config)
{
    if(m_data->configuration != config) {

        delete m_data->configuration;
        m_data->configuration = NULL;
        m_data->configuration = config;

        if(m_data->configuration != NULL) {
            m_data->configuration->setParent(this);
            setStatus(NotCreated);
        }

        emit configurationChanged(m_data->configuration);
    }
}

RisipEndpoint *RisipAccount::sipEndPoint() const
{
    return m_data->sipEndpoint;
}

void RisipAccount::setSipEndPoint(RisipEndpoint *endpoint)
{
    if(m_data->sipEndpoint != endpoint) {
        m_data->sipEndpoint = endpoint;
        emit sipEndPointChanged(m_data->sipEndpoint);
    }
}

int RisipAccount::status() const
{
    return m_data->status;
}

QString RisipAccount::statusText() const
{
    switch (m_data->status) {
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
    if(m_data->pjsipAccount) {
        BuddyVector buddies = m_data->pjsipAccount->enumBuddies();
        RisipBuddy *buddy;
        for(int i=0; i<m_data->allBuddies.count(); ++i) {
            buddy = new RisipBuddy(this);
            buddy->setAccount(this);
            buddy->setPjsipBuddy(static_cast<PjsipBuddy *>(buddies[i]));
            m_data->allBuddies.append(buddy);
        }
    }

    return QQmlListProperty<RisipBuddy>(this, m_data->allBuddies);
}

int RisipAccount::errorCode() const
{
    return m_data->error.status;
}

QString RisipAccount::errorMessage() const
{
    return QString::fromStdString(m_data->error.reason);
}

QString RisipAccount::errorInfo() const
{
    return QString::fromStdString(m_data->error.info(true));
}

/**
 * @brief RisipAccount::lastResponseCode
 * @return SIP response code
 *
 * This property holds the last SIP response code received.
 * Use this to properly handle sessions, i.e registration
 */
int RisipAccount::lastResponseCode() const
{
    return m_data->lastSipResponseCode;
}

/**
 * @brief RisipAccount::setLastResponseCode
 * @param response SIP response code received
 *
 * Internal API
 *
 * Internal callback function for updating the response code property
 */
void RisipAccount::setLastResponseCode(int response)
{
    if(m_data->lastSipResponseCode != response) {
        m_data->lastSipResponseCode = response;
        emit lastResponseCodeChanged(response);
    }
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
    return m_data->incomingPjsipCall;
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
    m_data->incomingPjsipCall = call;
    emit incomingCall();
}

/**
 * @brief RisipAccount::findBuddy
 * @param uri buddy uri used as search parameter
 * @return the RisipBuddy pointer is return if buddy found, otherwise just NULL
 */
RisipBuddy *RisipAccount::findBuddy(const QString &uri)
{
    if(!uri.isEmpty()) {
        try {
            PjsipBuddy *pjsipBuddy = static_cast<PjsipBuddy *>(m_data->pjsipAccount->findBuddy(uri.toStdString()));
            if(pjsipBuddy) {
                return pjsipBuddy->risipInterface();
            }
        } catch (Error &err) {
            setError(err);
        }
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
    if(!m_data->pjsipAccount)
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

    buddy->setAccount(this);
    if(findBuddy(buddy->uri()))
        return;

    buddy->create();

    //FIXME check if no model found
    RisipBuddiesModel *model = qobject_cast<RisipBuddiesModel*>(RisipContactManager::instance()->buddyModelForAccount(m_data->configuration->uri()));
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
    return m_data->pjsipAccount;
}

/**
 * @brief RisipAccount::setPjsipAccountInterface
 * @param acc
 *
 * Internal API.
 */
void RisipAccount::setPjsipAccountInterface(PjsipAccount *acc)
{
    m_data->pjsipAccount = acc;
}

int RisipAccount::presence() const
{
    if(m_data->pjsipAccount != NULL
            && (status() == SignedIn || status() == SignedOut)) {

        switch (m_data->presence_pjsip.activity) {
        case PJRPID_ACTIVITY_AWAY:
            return RisipBuddy::Away;
        case PJRPID_ACTIVITY_BUSY:
            return RisipBuddy::Busy;
        }

        if(m_data->pjsipAccount->getInfo().onlineStatus)
            return RisipBuddy::Online;
        else
            return RisipBuddy::Offline;
    }

    return RisipBuddy::Unknown;
}

void RisipAccount::setPresence(int new_presence)
{
    if(m_data->pjsipAccount == NULL)
        return;

    switch (new_presence) {
    case RisipBuddy::Online:
        m_data->presence_pjsip.status = PJSUA_BUDDY_STATUS_ONLINE;
        break;
    case RisipBuddy::Offline:
        m_data->presence_pjsip.status = PJSUA_BUDDY_STATUS_OFFLINE;
        break;
    case RisipBuddy::Away:
        m_data->presence_pjsip.activity = PJRPID_ACTIVITY_AWAY;
        break;
    case RisipBuddy::Busy:
        m_data->presence_pjsip.activity = PJRPID_ACTIVITY_BUSY;
        break;
    }

    if(m_data->presence_risip != new_presence) {
        qDebug()<<"SETTING PRESENCE: " <<m_data->presence_pjsip.status;
        try {
            m_data->pjsipAccount->setOnlineStatus(m_data->presence_pjsip);
        } catch (Error &err) {
            setError(err);
        }

        emit presenceChanged(new_presence);
    }
}

QString RisipAccount::presenceNote() const
{
    if(m_data->pjsipAccount != NULL)
        return QString::fromStdString(m_data->presence_pjsip.note);

    return QString();
}

void RisipAccount::setPresenceNote(const QString &note)
{
    if(QString::fromStdString(m_data->presence_pjsip.note) != note) {
        m_data->presence_pjsip.note = note.toStdString();
        emit presenceNoteChanged(note);

        try {
            m_data->pjsipAccount->setOnlineStatus(m_data->presence_pjsip);
        } catch (Error &err) {
            setError(err);
        }
    }
}

bool RisipAccount::autoSignIn() const
{
    return m_data->autoSignIn;
}

void RisipAccount::setAutoSignIn(bool signin)
{
    if(m_data->autoSignIn != signin) {
        m_data->autoSignIn = signin;
        emit autoSignInChanged(m_data->autoSignIn);
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
    if(!m_data->sipEndpoint)
        return;

    /** if account is not created then initialize it for the first time with
    * username , password and registration.
    */
    if(m_data->status == NotCreated
            || m_data->status == SignedOut
            || m_data->status == AccountError
            || m_data->status == NotConfigured) {

        //create transport if none exists and return if not success
        if(m_data->sipEndpoint->activeTransportId() == -1) {
            if(!m_data->sipEndpoint->createTransportNetwork(m_data->configuration)) {
                setStatus(NotConfigured);
                return;
            }
        }

        m_data->configuration->setTransportId(m_data->sipEndpoint->activeTransportId());

        // Create the account
        m_data->pjsipAccount = new PjsipAccount;
        m_data->pjsipAccount->setRisipInterface(this);
        try {
            m_data->pjsipAccount->create(m_data->configuration->pjsipAccountConfig());
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
    if(m_data->status == SignedIn) {
        try {
            m_data->pjsipAccount->setRegistration(false);
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
    if(m_data->status != status) {
        m_data->status = status;

        //delete internal pjsipaccount object when not needed.
        //update buddy list when account is ready
        switch (m_data->status) {
        case SignedIn:
            setPresence(RisipBuddy::Online);
            break;
        case SignedOut:
        case NotCreated:
        case AccountError:
        {
            //destroying list of risip buddy objects
            while (!m_data->allBuddies.isEmpty())
                m_data->allBuddies.takeFirst()->releaseFromAccount();

            //deleting the internal pjsipAccount object
            delete m_data->pjsipAccount;
            m_data->pjsipAccount = NULL;
            break;
        }
        }

        emit statusChanged(m_data->status);
        emit statusTextChanged(statusText());
    }
}

void RisipAccount::setError(const Error &error)
{
    qDebug()<<"ERROR: " <<"code: "<<error.status <<" info: " << QString::fromStdString(error.info(true));

    if(m_data->error.status != error.status) {

        m_data->error.status = error.status;
        m_data->error.reason = error.reason;
        m_data->error.srcFile = error.srcFile;
        m_data->error.srcLine = error.srcLine;
        m_data->error.title = error.title;

        emit errorCodeChanged(m_data->error.status);
        emit errorMessageChanged(QString::fromStdString(m_data->error.reason));
        emit errorInfoChanged(QString::fromStdString(m_data->error.info(true)));
    }
}
