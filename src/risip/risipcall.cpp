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

#include <QObject>
#include <QDebug>

PjsipCall::PjsipCall(PjsipAccount &account, int callId)
    :Call(account, callId)
{
    setRisipCall(NULL);
}

PjsipCall::~PjsipCall()
{
}

/**
 * @brief PjsipCall::onCallState
 * @param prm
 *
 * Pjsip callback each time the state of the call is updated.
 * A call is active when a session has been initiated.
 */
void PjsipCall::onCallState(OnCallStateParam &prm)
{
    Q_UNUSED(prm)
    if(m_risipCall == NULL) {
        qDebug()<<"risip call wrapper not set!!";
        return;
    }

    //if call is no longer active then set to NULL
    // emit a status change signal otherwise
    if(!isActive())
        m_risipCall->setPjsipCall(NULL);
    else
        m_risipCall->statusChanged(m_risipCall->status());
}

void PjsipCall::onCallTsxState(OnCallTsxStateParam &prm)
{
    Q_UNUSED(prm)
}

/**
 * @brief PjsipCall::onCallMediaState
 * @param prm
 *
 * This callback from Pjsip is triggered each time Pjsip intializes the media, in practice this means when the
 * call is being answered and Audio/Video is ready to be transmitted.
 *
 * See more details on @see RisipCall::initalizeMediaHandler
 */
void PjsipCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    Q_UNUSED(prm)
    if(m_risipCall)
        m_risipCall->initializeMediaHandler();
}

//logging purpose
void PjsipCall::onCallSdpCreated(OnCallSdpCreatedParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onStreamCreated(OnStreamCreatedParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onStreamDestroyed(OnStreamDestroyedParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onDtmfDigit(OnDtmfDigitParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCallTransferRequest(OnCallTransferRequestParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCallTransferStatus(OnCallTransferStatusParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCallReplaceRequest(OnCallReplaceRequestParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCallReplaced(OnCallReplacedParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCallRxOffer(OnCallRxOfferParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCallTxOffer(OnCallTxOfferParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onInstantMessage(OnInstantMessageParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onInstantMessageStatus(OnInstantMessageStatusParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onTypingIndication(OnTypingIndicationParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
pjsip_redirect_op PjsipCall::onCallRedirected(OnCallRedirectedParam &prm)
{
    Q_UNUSED(prm)
    return PJSIP_REDIRECT_ACCEPT;
}

//logging purpose
void PjsipCall::onCallMediaTransportState(OnCallMediaTransportStateParam &prm)
{
    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCallMediaEvent(OnCallMediaEventParam &prm)
{

    Q_UNUSED(prm)
}

//logging purpose
void PjsipCall::onCreateMediaTransport(OnCreateMediaTransportParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::setRisipCall(RisipCall *risipcall)
{
    m_risipCall = risipcall;
}

RisipCall::RisipCall(QObject *parent)
    :QObject(parent)
    ,m_account(NULL)
    ,m_buddy(NULL)
    ,m_risipMedia(NULL)
    ,m_pjsipCall(NULL)
    ,m_callType(RisipCall::Voip)
    ,m_callDirection(Unknown)
    ,m_error()
{
}

RisipCall::~RisipCall()
{
   setPjsipCall(NULL);
}

RisipAccount *RisipCall::account() const
{
    return m_account;
}

void RisipCall::setAccount(RisipAccount *acc)
{
    if(m_account != acc ) {
        m_account = acc;
        emit accountChanged(m_account);
    }
}

RisipBuddy *RisipCall::buddy() const
{
    return m_buddy;
}

void RisipCall::setBuddy(RisipBuddy *buddy)
{
    if(m_buddy != buddy) {
        m_buddy = buddy;
        emit buddyChanged(m_buddy);
    }
}

RisipMedia *RisipCall::media() const
{
    return m_risipMedia;
}

void RisipCall::setMedia(RisipMedia *med)
{
    if(m_risipMedia != med) {
        if(m_risipMedia) {
            delete m_risipMedia;
            m_risipMedia = NULL;
        }

        m_risipMedia = med;
        if(m_risipMedia) {
            m_risipMedia->setActiveCall(this);
        }

        emit mediaChanged(m_risipMedia);
    }
}

void RisipCall::setError(const Error &error)
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

int RisipCall::callId() const
{
    if(m_pjsipCall)
        m_pjsipCall->getId();

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
    delete m_pjsipCall;
    m_pjsipCall = NULL;

    m_pjsipCall = call;
    setMedia(NULL);

    if(m_pjsipCall != NULL) {
        m_pjsipCall->setRisipCall(this);
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
    return m_pjsipCall;
}

int RisipCall::callType() const
{
    return m_callType;
}

void RisipCall::setCallType(int type)
{
    if(m_callType != type) {
        m_callType = type;
        emit callTypeChanged(m_callType);
    }
}

int RisipCall::status() const
{
    if(m_pjsipCall == NULL)
        return Null;

    if(m_pjsipCall->isActive()) {
        CallInfo callInfo = m_pjsipCall->getInfo();
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
    return m_timestamp;
}

/**
 * @brief RisipCall::createTimestamp
 *
 * Internal API.
 */
void RisipCall::createTimestamp()
{
    m_timestamp = QDateTime::currentDateTime();
    emit timestampChanged(m_timestamp);
}

int RisipCall::callDirection() const
{
    return m_callDirection;
}

/**
 * @brief RisipCall::setCallDirection
 * @param direction
 *
 * Internal API.
 */
void RisipCall::setCallDirection(int direction)
{
    if(m_callDirection != direction) {
        m_callDirection = direction;
        emit callDirectionChanged(m_callDirection);
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
    if(!m_pjsipCall)
        return 0;

    return (int)m_pjsipCall->getInfo().connectDuration.msec;
}

int RisipCall::errorCode() const
{
    return m_error.status;
}

QString RisipCall::errorMessage()
{
    return QString::fromStdString(m_error.reason);
}

QString RisipCall::errorInfo() const
{
    return QString::fromStdString(m_error.info(true));
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
    if(!m_risipMedia)
        setMedia(new RisipMedia);

    m_risipMedia->startCallMedia();
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
    if(!m_account)
        return;

    if(m_pjsipCall != NULL) { //check if call object is set
        RisipCallManager::instance()->setActiveCall(this);
        CallOpParam prm;
        prm.statusCode = PJSIP_SC_OK;
        try {
            m_pjsipCall->answer(prm);
        } catch (Error &err) {
            setError(err);
        }
    } else {
        qDebug()<<"no account set or call id!";
    }
}

void RisipCall::hangup()
{
    if(m_pjsipCall == NULL) {
        qDebug()<<"no call exists!";
        return;
    }

    if(!m_pjsipCall->isActive())
        return;

    CallOpParam prm;
    try {
        m_pjsipCall->hangup(prm);
    } catch (Error &err) {
        setError(err);
    }

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
    if(m_callType == Undefined
            || !m_account
            || !m_buddy)
        return;

    if(m_account->status() != RisipAccount::SignedIn)
        return;

    setCallDirection(RisipCall::Outgoing);
    createTimestamp();
    setPjsipCall(new PjsipCall(*m_account->pjsipAccount()));
    CallOpParam prm(true);
    try {
        m_pjsipCall->makeCall(m_buddy->uri().toStdString(), prm);
    } catch (Error err) {
        setError(err);
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
    if(!m_account)
        return;

    if(m_account->status() == RisipAccount::SignedIn) {
        setPjsipCall(m_account->incomingPjsipCall());
        createTimestamp();
        setCallDirection(RisipCall::Incoming);
    }
}
