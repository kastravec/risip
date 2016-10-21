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

#ifndef RISIPACCOUNT_H
#define RISIPACCOUNT_H

#include <QObject>
#include <QQmlListProperty>

#include <pjsua2.hpp>
using namespace pj;

class RisipAccount;
class RisipMessage;
class RisipEndpoint;
class RisipAccountConfiguration;
class RisipBuddy;
class RisipCall;
class PjsipCall;
class RisipCallHistoryModel;
class RisipAccountProfile;

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

class RisipAccount: public QObject
{
    friend PjsipAccount;
    Q_OBJECT

public:
    enum Status {
        NotConfigured = 0,
        NotCreated,
        Registering,
        UnRegistering,
        SignedIn,
        SignedOut,
        AccountError = -1
    };

    Q_ENUM(Status)
    Q_PROPERTY(RisipAccountProfile * profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(RisipAccountConfiguration * configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    Q_PROPERTY(RisipEndpoint * sipEndPoint READ sipEndPoint WRITE setSipEndPoint NOTIFY sipEndPointChanged)
    Q_PROPERTY(int presence READ presence WRITE setPresence NOTIFY presenceChanged)
    Q_PROPERTY(QString presenceNote READ presenceNote WRITE setPresenceNote NOTIFY presenceNoteChanged)
    Q_PROPERTY(bool autoSignIn READ autoSignIn WRITE setAutoSignIn NOTIFY autoSignInChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(QQmlListProperty<RisipBuddy> buddies READ buddies NOTIFY buddiesChanged)

    RisipAccount(QObject *parent = 0);
    ~RisipAccount();

    RisipAccountProfile *profile();
    void setProfile(RisipAccountProfile *profile);

    RisipAccountConfiguration *configuration() const;
    void setConfiguration(RisipAccountConfiguration *config);

    RisipEndpoint *sipEndPoint() const;
    void setSipEndPoint(RisipEndpoint *endpoint);

    PjsipAccount *pjsipAccount() const;
    void setPjsipAccountInterface(PjsipAccount *acc);

    int presence() const;
    void setPresence(int prs);

    QString presenceNote() const;
    void setPresenceNote(const QString &note);

    bool autoSignIn() const;
    void setAutoSignIn(bool signin);

    int status() const;
    QString statusText() const;

    QQmlListProperty<RisipBuddy> buddies();
    int buddiesCount(QQmlListProperty<RisipBuddy>*list);

    void addBuddy(RisipBuddy *buddy);
    void removeBuddy(RisipBuddy *buddy);

    PjsipCall *incomingPjsipCall();
    void setIncomingPjsipCall(PjsipCall *call);

    Q_INVOKABLE RisipBuddy *findBuddy(const QString &uri);

Q_SIGNALS:
    void profileChanged(RisipAccountProfile *profile);
    void configurationChanged(RisipAccountConfiguration *config);
    void sipEndPointChanged(RisipEndpoint *sipendpoint);
    void presenceChanged(int presence);
    void presenceNoteChanged(QString note);
    void statusChanged(int status);
    void autoSignInChanged(bool signin);
    void statusTextChanged(QString);
    void buddiesChanged(QQmlListProperty<RisipBuddy> buddies);
    void incomingCall();
    void incomingMessage(RisipMessage *message);

public Q_SLOTS:
    void login();
    void logout();
    void refreshBuddyList();

private:
    void setStatus(int status);

    PjsipAccount *m_pjsipAccount;
    RisipAccountProfile *m_profile;
    RisipAccountConfiguration *m_configuration;
    RisipEndpoint *m_sipEndpoint;
    PresenceStatus m_presence;
    bool m_autoSignIn;
    int m_status;
    QHash<QString, RisipBuddy *> m_buddies;
    PjsipCall *m_incomingPjsipCall;
};

#endif // RISIPACCOUNT_H
