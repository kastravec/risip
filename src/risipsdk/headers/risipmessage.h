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
**    A copy of the license can be found also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/

#ifndef RISIPMESSAGE_H
#define RISIPMESSAGE_H

#include "risipsdkglobal.h"
#include <QObject>
#include <pjsua2.hpp>
using namespace pj;

namespace risip {

class RisipBuddy;
class RisipAccount;

class RISIP_VOIPSDK_EXPORT RisipMessage : public QObject
{
    Q_OBJECT

public:
    enum Status {
        Sent = 1,
        Pending,
        Failed,
        Null = 1
    };

    enum Direction {
        Incoming = 1,
        Outgoing,
        Unknown = 1
    };

    Q_ENUM(Status)
    Q_ENUM(Direction)
    Q_PROPERTY(QString messageBody READ messageBody WRITE setMessageBody NOTIFY messageBodyChanged)
    Q_PROPERTY(QString contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(RisipBuddy * buddy READ buddy NOTIFY buddyChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(int direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(int messageId READ messageId NOTIFY messageIdChanged)

    RisipMessage(QObject *parent = 0);
    ~RisipMessage();

    QString messageBody() const;
    void setMessageBody(QString &messageBody);

    QString contentType() const;
    void setContentType(QString &type);

    RisipBuddy *buddy() const;
    void setBuddy(RisipBuddy *buddy);

    int status() const;
    void setStatus(int st);

    int direction() const;
    void setDirection(int dir);

    int messageId() const;
    void setMessageId(int msg_id);

    void setIncomingMessageParam(OnInstantMessageParam prm);
    void setInstantMessageStatusParam(OnInstantMessageStatusParam);
    SendInstantMessageParam messageParamForSend();

Q_SIGNALS:
    void messageBodyChanged(QString &message);
    void contentTypeChanged(QString &contentType);
    void buddyChanged(RisipBuddy *buddy);
    void statusChanged(int status);
    void directionChanged(int direction);
    void messageIdChanged(int id);

private:
    RisipBuddy *m_risipBuddy;
    int m_status;
    int m_direction;
    int m_messageId;

    SendInstantMessageParam m_instantMessageParam;
    OnInstantMessageParam m_incomingMessageParam;
    OnInstantMessageStatusParam m_instantMessageStatusParam;
};

} //end of risip namespace

#endif // RISIPMESSAGE_H
