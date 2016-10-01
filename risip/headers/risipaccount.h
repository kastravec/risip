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

#ifndef RISIPACCOUNT_H
#define RISIPACCOUNT_H

#include <QObject>

#include <pjsua2.hpp>
using namespace pj;

class RisipAccount;
class RisipMessage;
class RisipEndpoint;
class RisipAccountConfiguration;

/**
 * @brief The PjsipAccount class
 *
 * This is an internal class. It inherits the Account class from Pjsip and implements the respective
 * callbacks, see Pjsip Account C++ API reference.
 *
 * Instances of this class are created from RisipAccount class, which is the Qt wrapper representative.
 * PjsipAccount contains the pointer to the RisipAccount which created it, in order to
 * in order to send messages/callbacks to the wrapper class.
 *
 */
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

/**
 * @brief The RisipAccount class
 *
 * RisipAccount represents the Account class of Pjsip C++ API. It creates instances of Account classes (PjsipAccount)
 * and manages these instances.
 */
class RisipAccount: public QObject
{
    friend PjsipAccount;
    Q_OBJECT

public:
    enum AccountStatus {
        NotConfigured = 0,
        NotCreated,
        Registering,
        UnRegistering,
        SignedIn,
        SignedOut,
        AccountError = -1
    };

    Q_ENUMS(AccountStatus)
    Q_PROPERTY(RisipAccountConfiguration * configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    Q_PROPERTY(RisipEndpoint * sipEndPoint READ sipEndPoint WRITE setSipEndPoint NOTIFY sipEndPointChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)

    RisipAccount(QObject *parent = 0);
    ~RisipAccount();

    RisipAccountConfiguration *configuration() const;
    void setConfiguration(RisipAccountConfiguration *config);

    RisipEndpoint *sipEndPoint() const;
    void setSipEndPoint(RisipEndpoint *endpoint);

    PjsipAccount *pjsipAccount() const;
    void setPjsipAccountInterface(PjsipAccount *acc);

    int status() const;
    QString statusText() const;

Q_SIGNALS:
    void configurationChanged(RisipAccountConfiguration *config);
    void sipEndPointChanged(RisipEndpoint *sipendpoint);
    void statusChanged(int status);
    void statusTextChanged(QString);
    void incomingCall(int callId);
    void incomingMessage(RisipMessage *message);

public Q_SLOTS:
    void login();
    void logout();


private:
    void setAccountStatus(int status);

    PjsipAccount *m_pjsipAccount;
    RisipAccountConfiguration *m_configuration;
    RisipEndpoint *m_sipEndpoint;
    int m_status;
};

#endif // RISIPACCOUNT_H
