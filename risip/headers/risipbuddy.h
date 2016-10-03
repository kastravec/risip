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

#ifndef RISIPBUDDY_H
#define RISIPBUDDY_H

#include <QObject>
#include <QQmlListProperty>

#include <pjsua2.hpp>
using namespace pj;

class RisipBuddy;
class RisipAccount;
class RisipMessage;

class PjsipBuddy: public Buddy
{
public:
    PjsipBuddy();
    ~PjsipBuddy();

    void onBuddyState();
    void setRisipBuddyInterface(RisipBuddy *risipBuddy);

private:
    RisipBuddy *m_risipBuddyInterface;
};

class RisipBuddy : public QObject
{
    Q_OBJECT
public:
    enum Presence {
        Online = 1,
        Offline,
        Away,
        Busy,
        Unknown,
        Null = -1
    };

    Q_ENUM(Presence)
    Q_PROPERTY(int presence READ presence NOTIFY presenceChanged)
    Q_PROPERTY(RisipAccount * account READ account WRITE setAccount NOTIFY accountChanged)
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)

    RisipBuddy(QObject *parent = 0);
    ~RisipBuddy();

    int presence() const;

    RisipAccount *account() const;
    void setAccount(RisipAccount *acc);

    QString uri() const;
    void setUri(QString contactUri);

    PjsipBuddy *pjsipBuddy() const;
    void setPjsipBuddy(PjsipBuddy *buddy);

public Q_SLOTS:
    void addToList();
    void release();
    RisipMessage *sendInstantMessage(QString message);
    void sendInstantMessage(RisipMessage *message);

Q_SIGNALS:
    void presenceChanged(int presence);
    void accountChanged(RisipAccount *account);
    void uriChanged(QString &uri);

private:
    PjsipBuddy *m_pjsipBuddy;
    RisipAccount *m_account;
    BuddyConfig m_buddyConfig;
};

#endif // RISIPBUDDY_H
