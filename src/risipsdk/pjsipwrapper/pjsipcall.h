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

#ifndef PJSIPCALL_H
#define PJSIPCALL_H

#include "pjsipaccount.h"

namespace risip {

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

} //end of risip namespace

#endif // PJSIPCALL_H
