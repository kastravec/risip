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

#include "risipsdkglobal.h"
#include "risipaccountconfiguration.h"

#include <QQmlListProperty>

#include <pjsua2.hpp>
using namespace pj;

namespace risip {

class RisipBuddy;
class RisipMessage;
class RisipEndpoint;
class RisipCall;
class RisipCallHistoryModel;
class PjsipCall;
class PjsipAccount;

class RISIP_VOIPSDK_EXPORT RisipAccount: public QObject
{
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
    Q_PROPERTY(RisipAccountConfiguration * configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    Q_PROPERTY(RisipEndpoint * sipEndPoint READ sipEndPoint WRITE setSipEndPoint NOTIFY sipEndPointChanged)
    Q_PROPERTY(int presence READ presence WRITE setPresence NOTIFY presenceChanged)
    Q_PROPERTY(QString presenceNote READ presenceNote WRITE setPresenceNote NOTIFY presenceNoteChanged)
    Q_PROPERTY(bool autoSignIn READ autoSignIn WRITE setAutoSignIn NOTIFY autoSignInChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(QQmlListProperty<RisipBuddy> buddies READ buddies NOTIFY buddiesChanged)
    Q_PROPERTY(int errorCode READ errorCode NOTIFY errorCodeChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString errorInfo READ errorInfo NOTIFY errorInfoChanged)
    Q_PROPERTY(int lastResponseCode READ lastResponseCode NOTIFY lastResponseCodeChanged)

    RisipAccount(QObject *parent = 0);
    ~RisipAccount();

    RisipAccountConfiguration *configuration() const;
    void setConfiguration(RisipAccountConfiguration *config);

    RisipEndpoint *sipEndPoint() const;
    void setSipEndPoint(RisipEndpoint *endpoint);

    int presence() const;
    void setPresence(int new_presence);

    QString presenceNote() const;
    void setPresenceNote(const QString &note);

    bool autoSignIn() const;
    void setAutoSignIn(bool signin);

    int status() const;
    QString statusText() const;

    QQmlListProperty<RisipBuddy> buddies();
    int buddiesCount(QQmlListProperty<RisipBuddy>*list);

    int errorCode() const;
    QString errorMessage() const;
    QString errorInfo() const;
    int lastResponseCode() const;

    Q_INVOKABLE RisipBuddy *findBuddy(const QString &uri);
    Q_INVOKABLE void addBuddy(const QString &buddyUri);
    Q_INVOKABLE void addRisipBuddy(risip::RisipBuddy *buddy);

Q_SIGNALS:
    void configurationChanged(RisipAccountConfiguration *config);
    void sipEndPointChanged(RisipEndpoint *sipendpoint);
    void presenceChanged(int presence);
    void presenceNoteChanged(const QString &note);
    void statusChanged(int status);
    void autoSignInChanged(bool signin);
    void statusTextChanged(const QString &statusText);
    void buddiesChanged(QQmlListProperty<RisipBuddy> buddies);
    void incomingCall();
    void incomingMessage(RisipMessage *message);
    void errorCodeChanged(int code);
    void errorMessageChanged(const QString &message);
    void errorInfoChanged(const QString &info);
    void lastResponseCodeChanged(int response);

public Q_SLOTS:
    void login();
    void logout();

private:
    PjsipAccount *pjsipAccount() const;
    void setPjsipAccountInterface(PjsipAccount *acc);
    PjsipCall *incomingPjsipCall();
    void setIncomingPjsipCall(PjsipCall *call);
    void setStatus(int status);
    void setError(const Error &error);
    void setLastResponseCode(int response);

    friend class RisipBuddy;
    friend class RisipCall;
    friend class PjsipAccount;

    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIPACCOUNT_H
