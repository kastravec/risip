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
            m_risipAccount->setAccountStatus(RisipAccount::SignedIn);
        else
            m_risipAccount->setAccountStatus(RisipAccount::SignedOut);
        break;
    case PJSIP_SC_TRYING:
        if(accountInfo.regIsActive)
            m_risipAccount->setAccountStatus(RisipAccount::UnRegistering);
        else
            m_risipAccount->setAccountStatus(RisipAccount::Registering);
        break;
    default:
        m_risipAccount->setAccountStatus(RisipAccount::AccountError);
        break;
    }
}

void PjsipAccount::onRegStarted(OnRegStartedParam &prm)
{
    if(prm.renew)
        m_risipAccount->setAccountStatus(RisipAccount::Registering);
    else
        m_risipAccount->setAccountStatus(RisipAccount::UnRegistering);
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
    RisipMessage *message = new RisipMessage;

    qDebug()<< QString::fromStdString(prm.rdata.wholeMsg);

    message->createMessage(QString::fromStdString(prm.msgBody),
                           QString::fromStdString(prm.contactUri),
                           QString::fromStdString(prm.fromUri),
                           QString::fromStdString(prm.contentType));

    m_risipAccount->incomingMessage(message);
}

void PjsipAccount::onInstantMessageStatus(OnInstantMessageStatusParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipAccount::onTypingIndication(OnTypingIndicationParam &prm)
{
    Q_UNUSED(prm)
}

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
            setAccountStatus(NotCreated);
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

PjsipAccount *RisipAccount::pjsipAccount() const
{
    return m_pjsipAccount;
}

void RisipAccount::setPjsipAccountInterface(PjsipAccount *acc)
{
    m_pjsipAccount = acc;
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
        try {
            m_pjsipAccount = new PjsipAccount;
            m_pjsipAccount->setRisipInterface(this);
            m_pjsipAccount->create(m_configuration->pjsipAccountConfig());
            setAccountStatus(Registering);
        } catch(Error& err) {
            setAccountStatus(AccountError);
            qDebug() << "Account creation error: " << err.info().c_str() << endl;
        }
    }
}

void RisipAccount::logout()
{
    if(m_status == SignedIn) {
        try {
            m_pjsipAccount->setRegistration(false);
        } catch(Error& err) {
            setAccountStatus(AccountError);
            qDebug() << "Unregister error: " << err.info().c_str() << endl;
        }
    }
}

/**
 * @brief RisipAccount::setAccountStatus
 * @param status
 *
 * If the status is NotCreated, SignedOut and AccountError then this function
 * deletes the internal pjsipAccount object and active network transport.
 */
void RisipAccount::setAccountStatus(int status)
{
    if(m_status != status) {
        m_status = status;
        emit statusChanged(m_status);
        emit statusTextChanged(statusText());

        //delete internal pjsipaccount object when not needed.
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
    }
}


