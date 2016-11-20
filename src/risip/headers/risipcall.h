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
************************************************************************************/

#ifndef RISIPCALL_H
#define RISIPCALL_H

#include <QDateTime>

#include "risipaccount.h"
using namespace pj;

class RisipMedia;
class RisipCall;
class RisipBuddy;
class RisipPhoneNumber;

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
    Q_OBJECT

public:

    enum CallType {
        Pstn = 1,
        Sip,
        Undefined = -1
    };

    enum CallDirection {
        Incoming = 1,
        Outgoing,
        Unknown = -1
    };

    enum Status {
        OutgoingCallStarted = 1,
        IncomingCallStarted,
        ConnectingToCall,
        CallConfirmed,
        CallDisconnected,
        CallEarly,
        Null
    };

    Q_ENUM(CallType)
    Q_ENUM(CallDirection)
    Q_ENUM(Status)
    Q_PROPERTY(int callType READ callType WRITE setCallType NOTIFY callTypeChanged)
    Q_PROPERTY(RisipAccount * account READ account WRITE setAccount NOTIFY accountChanged)
    Q_PROPERTY(RisipBuddy * buddy READ buddy WRITE setBuddy NOTIFY buddyChanged)
    Q_PROPERTY(RisipPhoneNumber * phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(RisipMedia * media READ media NOTIFY mediaChanged)
    Q_PROPERTY(int callId READ callId NOTIFY callIdChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged)
    Q_PROPERTY(int callDirection READ callDirection NOTIFY callDirectionChanged)
    Q_PROPERTY(int callDuration READ callDuration NOTIFY callDurationChanged)
    Q_PROPERTY(int errorCode READ errorCode NOTIFY errorCodeChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString errorInfo READ errorInfo NOTIFY errorInfoChanged)

    RisipCall(QObject *parent = 0);
    ~RisipCall();

    RisipAccount *account() const;
    void setAccount(RisipAccount *acc);

    RisipBuddy *buddy() const;
    void setBuddy(RisipBuddy *buddy);

    RisipPhoneNumber *phoneNumber() const;
    void setPhoneNumber(RisipPhoneNumber *number);

    int callType() const;
    void setCallType(int type);

    RisipMedia *media() const;
    int callId() const;
    int status() const;
    QDateTime timestamp() const;
    int callDirection() const;
    int callDuration() const;
    int errorCode() const;
    QString errorMessage();
    QString errorInfo() const;

public Q_SLOTS:
    void answer();
    void hangup();
    void call();
    void hold(bool hold);

Q_SIGNALS:
    void accountChanged(RisipAccount *account);
    void buddyChanged(RisipBuddy *buddy);
    void phoneNumberChanged(RisipPhoneNumber *number);
    void mediaChanged(RisipMedia *media);
    void callIdChanged(int callId);
    void callTypeChanged(int type);
    void statusChanged(int status);
    void timestampChanged(QDateTime timestamp);
    void callDirectionChanged(int direction);
    void callDurationChanged(int duration);
    void errorCodeChanged(int code);
    void errorMessageChanged(const QString &message);
    void errorInfoChanged(const QString &info);

private:
    PjsipCall *pjsipCall() const;
    void setPjsipCall(PjsipCall *call);
    void createTimestamp();
    void setCallDirection(int direction);
    void initiateIncomingCall();
    void initializeMediaHandler();
    void setMedia(RisipMedia *med);
    void setError(const Error &error);

    RisipAccount *m_account;
    RisipBuddy *m_buddy;
    RisipPhoneNumber *m_phoneNumber;
    RisipMedia *m_risipMedia;
    PjsipCall *m_pjsipCall;
    int m_callType;
    QDateTime m_timestamp;
    int m_callDirection;
    Error m_error;

    friend class RisipCallManager;
    friend class RisipMedia;
    friend class PjsipCall;
};

#endif // RISIPCALL_H
