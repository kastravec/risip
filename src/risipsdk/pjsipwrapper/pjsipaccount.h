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
#ifndef PJSIPACCOUNT_H
#define PJSIPACCOUNT_H

#include <pjsua2.hpp>
using namespace pj;

namespace risip {

class RisipAccount;

class PjsipAccount: public Account
{
public:
    PjsipAccount();
    ~PjsipAccount();

    void onRegState(OnRegStateParam &prm);
    void onRegStarted(OnRegStartedParam &prm);
    void onIncomingCall(OnIncomingCallParam &prm);
    void onIncomingSubscribe(OnIncomingSubscribeParam &prm);
    void onInstantMessage(OnInstantMessageParam &prm);
    void onInstantMessageStatus(OnInstantMessageStatusParam &prm);
    void onTypingIndication(OnTypingIndicationParam &prm);
    void onMwiInfo(OnMwiInfoParam &prm);

    void setRisipInterface(RisipAccount *acc);

private:
    RisipAccount *m_risipAccount;
};

} //end of risip namespace

#endif // PJSIPACCOUNT_H
