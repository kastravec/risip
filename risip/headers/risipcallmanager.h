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
#ifndef RISIPCALLMANAGER_H
#define RISIPCALLMANAGER_H

#include <QObject>

class RisipAccount;
class RisipBuddy;
class RisipCall;

class RisipCallManager : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(RisipAccount * defaultAccount READ defaultAccount WRITE setDefaultAccount NOTIFY defaultAccountChanged)
    Q_PROPERTY(RisipCall * activeCall READ activeCall NOTIFY activeCallChanged)

    static RisipCallManager *instance();
    ~RisipCallManager();

    RisipAccount *defaultAccount();
    void setDefaultAccount(RisipAccount *defaultAccount);

    RisipCall *activeCall();
    void setActiveCall(RisipCall *call);

    Q_INVOKABLE void callContact(const QString uri);
    Q_INVOKABLE void callBuddy(RisipBuddy *buddy);
    Q_INVOKABLE void callPhone(const QString &number);

Q_SIGNALS:
    void defaultAccountChanged(RisipAccount *account);
    void activeCallChanged(RisipCall *call);
    void incomingCall(RisipCall *call);
    void outgoingCall(RisipCall *call);

private Q_SLOTS:
    void accountIncomingCall();
    void updateDefaultAccount();

private:
    explicit RisipCallManager(QObject *parent = 0);
    static RisipCallManager *m_callManagerInstance;

    RisipAccount *m_defaultAccount;
    RisipCall *m_activeCall;
    QList<RisipCall *> m_callHistory;
};

#endif // RISIPCALLMANAGER_H
