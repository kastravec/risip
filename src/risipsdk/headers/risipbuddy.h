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

#ifndef RISIPBUDDY_H
#define RISIPBUDDY_H

#include "risipsdkglobal.h"
#include "risipaccount.h"

#include <QObject>
#include <QQmlListProperty>

#include <pjsua2.hpp>
using namespace pj;

namespace risip {

class RisipMessage;
class PjsipBuddy;

class RISIP_VOIPSDK_EXPORT RisipBuddy : public QObject
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

    enum Type {
        Internal = 1,
        ExternalSIP,
        Pstn,
        Undefined = -1
    };

    Q_ENUM(Presence)
    Q_ENUM(Type)
    Q_PROPERTY(int presence READ presence NOTIFY presenceChanged)
    Q_PROPERTY(RisipAccount * account READ account WRITE setAccount NOTIFY accountChanged)
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString contact READ contact WRITE setContact NOTIFY contactChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

    RisipBuddy(QObject *parent = 0);
    ~RisipBuddy();

    int presence() const;

    RisipAccount *account() const;
    void setAccount(risip::RisipAccount *acc);

    QString uri() const;
    void setUri(QString contactUri);

    QString contact();
    void setContact(const QString contact);

    int type() const;
    void setType(int type);

    bool valid() const;
    Q_INVOKABLE void setContactNumber(const QString &number);
    Q_INVOKABLE risip::RisipMessage *sendInstantMessage(QString message);

public Q_SLOTS:
    void create();
    void releaseFromAccount();
    void sendInstantMessage(RisipMessage *message);

Q_SIGNALS:
    void presenceChanged(int presence);
    void accountChanged(RisipAccount *account);
    void uriChanged(QString &uri);
    void contactChanged(const QString &contact);
    void typeChanged(int type);
    void validChanged(bool valid);

private:
    void setError(const Error &error);
    PjsipBuddy *pjsipBuddy() const;
    void setPjsipBuddy(PjsipBuddy *buddy);

    friend class RisipAccount;

    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIPBUDDY_H
