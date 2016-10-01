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

#include "risipcall.h"
#include "risipendpoint.h"
#include "risipaccountconfiguration.h"
#include "risipmedia.h"

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

void PjsipCall::onCallState(OnCallStateParam &prm)
{
    Q_UNUSED(prm)
    if(m_risipCall == NULL) {
        qDebug()<<"risip call wrapper not set!!";
        return;
    }

    CallInfo callInfo = getInfo();
    switch (callInfo.state) {
    case PJSIP_INV_STATE_CALLING:
        m_risipCall->setStatus(RisipCall::Calling);
        break;
    case PJSIP_INV_STATE_CONNECTING:
        m_risipCall->setStatus(RisipCall::Connecting);
        break;
    case PJSIP_INV_STATE_CONFIRMED:
        m_risipCall->setStatus(RisipCall::Confirmed);
        break;
    case PJSIP_INV_STATE_DISCONNECTED:
        m_risipCall->setStatus(RisipCall::Disconnected);
        break;
    case PJSIP_INV_STATE_EARLY:
        m_risipCall->setStatus(RisipCall::Early);
        break;
    case PJSIP_INV_STATE_INCOMING:
        m_risipCall->setStatus(RisipCall::Incoming);
        break;
    case PJSIP_INV_STATE_NULL:
        m_risipCall->setStatus(RisipCall::Null);
        break;
    }
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
 * See more details on @RisipCall::initalizeMediaHandler
 */
void PjsipCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    Q_UNUSED(prm)
    if(m_risipCall)
        m_risipCall->initializeMediaHandler();
}

void PjsipCall::onCallSdpCreated(OnCallSdpCreatedParam &prm)
{
    Q_UNUSED(prm)
}
void PjsipCall::onStreamCreated(OnStreamCreatedParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onStreamDestroyed(OnStreamDestroyedParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onDtmfDigit(OnDtmfDigitParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onCallTransferRequest(OnCallTransferRequestParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onCallTransferStatus(OnCallTransferStatusParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onCallReplaceRequest(OnCallReplaceRequestParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onCallReplaced(OnCallReplacedParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onCallRxOffer(OnCallRxOfferParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onCallTxOffer(OnCallTxOfferParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onInstantMessage(OnInstantMessageParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onInstantMessageStatus(OnInstantMessageStatusParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onTypingIndication(OnTypingIndicationParam &prm)
{
    Q_UNUSED(prm)
}

pjsip_redirect_op PjsipCall::onCallRedirected(OnCallRedirectedParam &prm)
{
    Q_UNUSED(prm)
    return PJSIP_REDIRECT_ACCEPT;
}

void PjsipCall::onCallMediaTransportState(OnCallMediaTransportStateParam &prm)
{
    Q_UNUSED(prm)
}

void PjsipCall::onCallMediaEvent(OnCallMediaEventParam &prm)
{
    Q_UNUSED(prm)
}

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
    ,m_risipMedia(NULL)
{
    setPjsipCall(NULL);
    setCallType(RisipCall::Voip);
}

RisipCall::~RisipCall()
{
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

        if(m_account)
            connect(m_account, SIGNAL(incomingCall(int)),
                    this, SLOT(incomingCall(int)), Qt::DirectConnection);
    }
}

RisipMedia *RisipCall::media() const
{
    return m_risipMedia;
}

void RisipCall::setMedia(RisipMedia *med)
{
    if(m_risipMedia != med) {
        m_risipMedia->deleteLater();
        m_risipMedia = med;

        if(m_risipMedia) {
            m_risipMedia->setActiveCall(this);
        }

        emit mediaChanged(m_risipMedia);
    }
}

int RisipCall::callId() const
{
    return m_callId;
}

void RisipCall::setCallId(int id)
{
    if(m_callId != id) {
        m_callId = id;
        emit callIdChanged(m_callId);
    }
}

void RisipCall::setPjsipCall(PjsipCall *call)
{
    m_pjsipCall = call;
    if(m_pjsipCall != NULL) {
        m_pjsipCall->setRisipCall(this);
        setCallId(m_pjsipCall->getId());
    }
}

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

    CallInfo callInfo = m_pjsipCall->getInfo();
    switch (callInfo.state) {
    case PJSIP_INV_STATE_CALLING:
        return RisipCall::Calling;
    case PJSIP_INV_STATE_CONNECTING:
        return RisipCall::Connecting;
    case PJSIP_INV_STATE_CONFIRMED:
        return RisipCall::Confirmed;
    case PJSIP_INV_STATE_DISCONNECTED:
        return RisipCall::Disconnected;
    case PJSIP_INV_STATE_EARLY:
        return RisipCall::Early;
    case PJSIP_INV_STATE_INCOMING:
        return RisipCall::Incoming;
    case PJSIP_INV_STATE_NULL:
        return RisipCall::Null;
    }

    return RisipCall::Null;
}

void RisipCall::setStatus(int status)
{
    if(m_status != status) {
        m_status = status;

        //deleting the pjsip object if the call is disconnected or null
        if(m_status == Disconnected
                || m_status == Null) {
            delete m_pjsipCall;
            setMedia(NULL);
            m_pjsipCall = NULL;
        }

        emit statusChanged(m_status);
    }
}

void RisipCall::initializeMediaHandler()
{
    if(!m_risipMedia)
        setMedia(new RisipMedia);

    m_risipMedia->startCallMedia();
}

void RisipCall::answer()
{
    if(!m_account)
        return;

    if(m_pjsipCall != NULL) { //check if call object is set
        CallOpParam prm;
        prm.statusCode = PJSIP_SC_OK;
        try {
            m_pjsipCall->answer(prm);
        } catch (Error &err) {
            qDebug()<<"error answering the call... " <<QString::fromStdString(err.reason) <<err.status;
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

    CallOpParam prm;
    try {
        m_pjsipCall->hangup(prm);
    } catch (Error &err) {
        qDebug()<<"error hanging up the call... " <<QString::fromStdString(err.reason) <<err.status;
    }
}

void RisipCall::makeCall(const QString &contact)
{
    if(contact.isEmpty()
            || contact.isNull()
            || m_callType == Undefined
            || !m_account)
        return;

    contact.trimmed();
    QString contactUri;
    if(m_callType == Voip) {
        contactUri.prepend(QString("sip:"));
        contactUri.append(contact + QString("@") + m_account->configuration()->serverAddress());
    } else {
        contactUri = contact;
    }

    //delete first internal pjsip call object
    if(m_pjsipCall) {
        delete m_pjsipCall;
        m_pjsipCall = NULL;
    }

    setPjsipCall(new PjsipCall(*m_account->pjsipAccount()));

    CallOpParam prm(true);
    try {
        m_pjsipCall->makeCall(contactUri.toStdString(), prm);
    } catch (Error err) {
        qDebug()<<"making call failed: " <<QString::fromStdString(err.info());
    }
}

void RisipCall::incomingCall(int callId)
{
    if(!m_account)
        return;

    //delete first internal pjsip call object
    if(m_pjsipCall) {
        delete m_pjsipCall;
        m_pjsipCall = NULL;
    }

    //creating the internal pjsip call object for managing the incoming call.
    // and emiting the incomingCall signal
    setPjsipCall(new PjsipCall(*m_account->pjsipAccount(), callId));
    emit incomingCall();
}
