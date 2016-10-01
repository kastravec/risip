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

#ifndef RISIPCALL_H
#define RISIPCALL_H

#include "risipaccount.h"
using namespace pj;

class RisipMedia;
class RisipCall;

class PjsipCall: public Call
{
public:
    PjsipCall(PjsipAccount &account, int callId = PJSUA_INVALID_ID);
    ~PjsipCall();

    void onCallState(OnCallStateParam &prm);
    void onCallTsxState(OnCallTsxStateParam &prm);
    void onCallMediaState(OnCallMediaStateParam &prm);
    void onCallSdpCreated(OnCallSdpCreatedParam &prm);
    void onStreamCreated(OnStreamCreatedParam &prm);
    void onStreamDestroyed(OnStreamDestroyedParam &prm);
    void onDtmfDigit(OnDtmfDigitParam &prm);
    void onCallTransferRequest(OnCallTransferRequestParam &prm);
    void onCallTransferStatus(OnCallTransferStatusParam &prm);
    void onCallReplaceRequest(OnCallReplaceRequestParam &prm);
    void onCallReplaced(OnCallReplacedParam &prm);
    void onCallRxOffer(OnCallRxOfferParam &prm);
    void onCallTxOffer(OnCallTxOfferParam &prm);
    void onInstantMessage(OnInstantMessageParam &prm);
    void onInstantMessageStatus(OnInstantMessageStatusParam &prm);
    void onTypingIndication(OnTypingIndicationParam &prm);
    pjsip_redirect_op onCallRedirected(OnCallRedirectedParam &prm);
    void onCallMediaTransportState(OnCallMediaTransportStateParam &prm);
    void onCallMediaEvent(OnCallMediaEventParam &prm);
    void onCreateMediaTransport(OnCreateMediaTransportParam &prm);

    void setRisipCall(RisipCall *risipcall);

private:
    RisipCall *m_risipCall;
};

class RisipCall : public QObject
{
    friend PjsipCall;

    Q_OBJECT
public:

    enum CallType {
        Gsm = 1,
        Voip,
        Undefined = -1
    };

    enum Status {
        Calling = 1,
        Connecting,
        Confirmed,
        Disconnected,
        Early,
        Incoming,
        Null
    };

    Q_ENUM(CallType)
    Q_ENUM(Status)
    Q_PROPERTY(RisipAccount * account READ account WRITE setAccount NOTIFY accountChanged)
    Q_PROPERTY(RisipMedia * media READ media NOTIFY mediaChanged)
    Q_PROPERTY(int callType READ callType WRITE setCallType NOTIFY callTypeChanged)
    Q_PROPERTY(int callId READ callId WRITE setCallId NOTIFY callIdChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)

    RisipCall(QObject *parent = 0);
    ~RisipCall();

    RisipAccount *account() const;
    void setAccount(RisipAccount *acc);

    RisipMedia *media() const;

    int callType() const;
    void setCallType(int type);

    int callId() const;
    void setCallId(int id);

    int status() const;

    void setPjsipCall(PjsipCall *call);
    PjsipCall *pjsipCall() const;

public Q_SLOTS:
    void answer();
    void hangup();
    void makeCall(const QString &contact);

private Q_SLOTS:
    void incomingCall(int callId);

Q_SIGNALS:
    void accountChanged(RisipAccount *account);
    void mediaChanged(RisipMedia *media);
    void callIdChanged(int callId);
    void callTypeChanged(int type);
    void statusChanged(int status);
    void incomingCall();

private:
    void setStatus(int status);
    void initializeMediaHandler();
    void setMedia(RisipMedia *med);

    RisipAccount *m_account;
    RisipMedia *m_risipMedia;
    PjsipCall *m_pjsipCall;
    int m_callType;
    int m_status;
    int m_callId;
};

#endif // RISIPCALL_H
