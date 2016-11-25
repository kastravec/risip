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

#include "risipcall.h"
#include "risipendpoint.h"
#include "risipaccountconfiguration.h"
#include "risipmedia.h"
#include "risipbuddy.h"
#include "risipmodels.h"
#include "risipcallmanager.h"
#include "risipphonecontact.h"
#include "risipphonenumber.h"
#include "risipglobals.h"

#include "pjsipwrapper/pjsipaccount.h"
#include "pjsipwrapper/pjsipcall.h"

#include <QCoreApplication>

#include <QDebug>

class RisipCall::Private
{
public:
    RisipAccount *account;
    RisipBuddy *buddy;
    RisipPhoneNumber *phoneNumber;
    RisipMedia *risipMedia;
    PjsipCall *pjsipCall;
    int callType;
    QDateTime timestamp;
    int callDirection;
    Error error;
};

RisipCall::RisipCall(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->account = NULL;
    m_data->buddy = NULL;
    m_data->phoneNumber = NULL;
    m_data->risipMedia = NULL;
    m_data->pjsipCall = NULL;
    m_data->callType = RisipCall::Sip;
    m_data->callDirection = Unknown;
}

RisipCall::~RisipCall()
{
    setPjsipCall(NULL);
    delete m_data;
    m_data = NULL;
}

RisipAccount *RisipCall::account() const
{
    return m_data->account;
}

void RisipCall::setAccount(RisipAccount *acc)
{
    if(m_data->account != acc ) {
        m_data->account = acc;
        emit accountChanged(m_data->account);
    }
}

RisipBuddy *RisipCall::buddy() const
{
    return m_data->buddy;
}

void RisipCall::setBuddy(RisipBuddy *buddy)
{
    if(m_data->buddy != buddy) {
        m_data->buddy = buddy;
        emit buddyChanged(m_data->buddy);
    }

    setCallType(Sip);
}

RisipPhoneNumber *RisipCall::phoneNumber() const
{
    return m_data->phoneNumber;
}

void RisipCall::setPhoneNumber(RisipPhoneNumber *number)
{
    if(m_data->phoneNumber != number) {
        m_data->phoneNumber = number;
        emit phoneNumberChanged(m_data->phoneNumber);
    }

    setCallType(Pstn);
}

RisipMedia *RisipCall::media() const
{
    return m_data->risipMedia;
}

void RisipCall::setMedia(RisipMedia *med)
{
    if(m_data->risipMedia != med) {
        if(m_data->risipMedia) {
            delete m_data->risipMedia;
            m_data->risipMedia = NULL;
        }

        m_data->risipMedia = med;
        if(m_data->risipMedia) {
            m_data->risipMedia->setActiveCall(this);
        }

        emit mediaChanged(m_data->risipMedia);
    }
}

int RisipCall::callId() const
{
    if(m_data->pjsipCall)
        m_data->pjsipCall->getId();

    return -1;
}

/**
 * @brief RisipCall::setPjsipCall
 * @param call
 *
 * Internal API. Initiates the call
 */
void RisipCall::setPjsipCall(PjsipCall *call)
{
    delete m_data->pjsipCall;
    m_data->pjsipCall = NULL;

    m_data->pjsipCall = call;
    setMedia(NULL);

    if(m_data->pjsipCall != NULL) {
        m_data->pjsipCall->setRisipCall(this);
    }

    emit statusChanged(status());
}

/**
 * @brief RisipCall::pjsipCall
 * @return pjsip object
 *
 * Internal API.
 */
PjsipCall *RisipCall::pjsipCall() const
{
    return m_data->pjsipCall;
}

int RisipCall::callType() const
{
    return m_data->callType;
}

void RisipCall::setCallType(int type)
{
    if(m_data->callType != type) {
        m_data->callType = type;
        emit callTypeChanged(m_data->callType);
    }
}

int RisipCall::status() const
{
    if(m_data->pjsipCall == NULL)
        return Null;

    if(m_data->pjsipCall->isActive()) {
        CallInfo callInfo = m_data->pjsipCall->getInfo();
        switch (callInfo.state) {
        case PJSIP_INV_STATE_CALLING:
            return RisipCall::OutgoingCallStarted;
        case PJSIP_INV_STATE_CONNECTING:
            return RisipCall::ConnectingToCall;
        case PJSIP_INV_STATE_CONFIRMED:
            return RisipCall::CallConfirmed;
        case PJSIP_INV_STATE_DISCONNECTED:
            return RisipCall::CallDisconnected;
        case PJSIP_INV_STATE_EARLY:
            return RisipCall::CallEarly;
        case PJSIP_INV_STATE_INCOMING:
            return RisipCall::IncomingCallStarted;
        case PJSIP_INV_STATE_NULL:
            return RisipCall::Null;
        }
    }

    return RisipCall::Null;
}

QDateTime RisipCall::timestamp() const
{
    return m_data->timestamp;
}

/**
 * @brief RisipCall::createTimestamp
 *
 * Internal API.
 */
void RisipCall::createTimestamp()
{
    m_data->timestamp = QDateTime::currentDateTime();
    emit timestampChanged(m_data->timestamp);
}

int RisipCall::callDirection() const
{
    return m_data->callDirection;
}

/**
 * @brief RisipCall::setCallDirection
 * @param direction
 *
 * Internal API.
 */
void RisipCall::setCallDirection(int direction)
{
    if(m_data->callDirection != direction) {
        m_data->callDirection = direction;
        emit callDirectionChanged(m_data->callDirection);
    }
}

/**
 * @brief RisipCall::callDuration
 * @return call duration in msec
 *
 * Retuns the duration of the call in milliseconds.
 */
int RisipCall::callDuration() const
{
    if(!m_data->pjsipCall)
        return 0;

    return (int)m_data->pjsipCall->getInfo().connectDuration.msec;
}

int RisipCall::errorCode() const
{
    return m_data->error.status;
}

QString RisipCall::errorMessage()
{
    return QString::fromStdString(m_data->error.reason);
}

QString RisipCall::errorInfo() const
{
    return QString::fromStdString(m_data->error.info(true));
}

/**
 * @brief RisipCall::initializeMediaHandler
 *
 * Internal API.
 *
 * Initializes media objects for an active call that has been established and answered.
 */
void RisipCall::initializeMediaHandler()
{
    if(!m_data->risipMedia)
        setMedia(new RisipMedia);

    m_data->risipMedia->startCallMedia();
}

/**
 * @brief RisipCall::answer
 *
 * Answers an incoming call.
 *
 * @see RisipCallManager how incoming are handled.
 */
void RisipCall::answer()
{
    if(!m_data->account)
        return;

    if(m_data->pjsipCall != NULL) { //check if call object is set
        RisipCallManager::instance()->setActiveCall(this);
        CallOpParam prm;
        prm.statusCode = PJSIP_SC_OK;
        try {
            m_data->pjsipCall->answer(prm);
        } catch (Error &err) {
            setError(err);
        }
    } else {
        qDebug()<<"no account set or call id!";
    }
}

void RisipCall::hangup()
{
    if(m_data->pjsipCall == NULL
            || !m_data->pjsipCall->isActive()) {
        qDebug()<<"no call exists/active";

        emit statusChanged(status());
        return;
    }

    CallOpParam prm;
    try {
        m_data->pjsipCall->hangup(prm);
    } catch (Error &err) {
        setError(err);
    }

    emit statusChanged(status());
    RisipCallManager::instance()->setActiveCall(NULL);
}

/**
 * @brief RisipCall::call
 *
 * Internal API.
 * Use may use it with caution. @see RisipCallManager
 */
void RisipCall::call()
{
    if(m_data->callType == Undefined
            || !m_data->account
            || (!m_data->buddy && !m_data->phoneNumber))
        return;

    if(m_data->account->status() != RisipAccount::SignedIn)
        return;

    setCallDirection(RisipCall::Outgoing);
    createTimestamp();
    setPjsipCall(new PjsipCall(*m_data->account->pjsipAccount()));
    CallOpParam prm(true);

    QString callee;
    if(m_data->callType == Pstn && m_data->phoneNumber) {
        callee = RisipGlobals::formatToSip(m_data->phoneNumber->fullNumber(),
                                           m_data->account->configuration()->serverAddress());
    } else if(m_data->callType == Sip && m_data->buddy) {
        callee = m_data->buddy->uri();
    }
    if(!callee.isNull()) {
        try {
            m_data->pjsipCall->makeCall(callee.toStdString(), prm);
        } catch (Error err) {
            setError(err);
        }
    }
}

/**
 * @brief RisipCall::hold
 * @param hold
 *
 * Use this function to hold/unhold an active call.
 */
void RisipCall::hold(bool hold)
{
    if(m_data->pjsipCall == NULL
            || !m_data->pjsipCall->isActive()) {
        qDebug()<<"no call exists nor is active";
        return;
    }

    if(hold) {
        CallOpParam prm;
        prm.options = PJSUA_CALL_UPDATE_CONTACT;
        try {
            m_data->pjsipCall->setHold(prm);
        } catch (Error &err) {
            setError(err);
        }
    } else {
        CallOpParam prm;
        prm.opt.flag = PJSUA_CALL_UNHOLD;
        try {
            m_data->pjsipCall->reinvite(prm);
        } catch (Error &err) {
            setError(err);
        }
    }
}

/**
 * @brief RisipCall::initiateIncomingCall
 *
 * Internal API.
 *
 * Used for initiating/handling an incoming call from the account.
 *
 * @see RisipCallManager how it is used.
 */
void RisipCall::initiateIncomingCall()
{
    if(!m_data->account)
        return;

    if(m_data->account->status() == RisipAccount::SignedIn) {
        setPjsipCall(m_data->account->incomingPjsipCall());
        createTimestamp();
        setCallDirection(RisipCall::Incoming);
    }
}

void RisipCall::setError(const Error &error)
{
    if(m_data->error.status != error.status) {
        qWarning()<<" ERROR: " <<"code: "<<error.status <<" info: " << QString::fromStdString(error.info(true));

        m_data->error.status = error.status;
        m_data->error.reason = error.reason;
        m_data->error.srcFile = error.srcFile;
        m_data->error.srcLine = error.srcLine;
        m_data->error.title = error.title;

        emit errorCodeChanged(m_data->error.status);
        emit errorMessageChanged(QString::fromStdString(m_data->error.reason));
        emit errorInfoChanged(QString::fromStdString(m_data->error.info(true)));
        QCoreApplication::processEvents();
    }
}
