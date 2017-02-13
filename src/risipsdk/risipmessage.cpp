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

#include "risipmessage.h"

namespace risip {

RisipMessage::RisipMessage(QObject *parent)
    :QObject(parent)
{
    m_status = Null;
    m_messageId = -1;
    m_direction = Unknown;
}

RisipMessage::~RisipMessage()
{
}

QString RisipMessage::messageBody() const
{
    return QString::fromStdString(m_instantMessageParam.content);
}

void RisipMessage::setMessageBody(QString &messageBody)
{
    //TODO comparing two std:strings or qstring ??
    if(m_instantMessageParam.content != messageBody.toStdString()) {
        m_instantMessageParam.content = messageBody.toStdString();
        emit messageBodyChanged(messageBody);
    }
}

QString RisipMessage::contentType() const
{
    return QString::fromStdString(m_instantMessageParam.contentType);
}

void RisipMessage::setContentType(QString &type)
{
    //TODO std::string comparison ??
    if(m_instantMessageParam.contentType != type.toStdString()) {
        m_instantMessageParam.contentType = type.toStdString();
        emit contentTypeChanged(type);
    }
}

RisipBuddy *RisipMessage::buddy() const
{
    return m_risipBuddy;
}

void RisipMessage::setBuddy(RisipBuddy *buddy)
{
    if(m_risipBuddy != buddy) {
        m_risipBuddy = buddy;
        emit buddyChanged(m_risipBuddy);

    }
}

int RisipMessage::status() const
{
    return m_status;
}

void RisipMessage::setStatus(int st)
{
    if(m_status != st) {
        m_status = st;
        emit statusChanged(m_status);
    }
}

int RisipMessage::direction() const
{
    return m_direction;
}

void RisipMessage::setDirection(int dir)
{
    if(m_direction != dir) {
        m_direction = dir;
        emit directionChanged(m_direction);
    }
}

int RisipMessage::messageId() const
{
    return m_messageId;
}

void RisipMessage::setMessageId(int msg_id)
{
    if(m_messageId != msg_id) {
        m_messageId = msg_id;
        emit messageIdChanged(m_messageId);
    }
}

void RisipMessage::setIncomingMessageParam(OnInstantMessageParam prm)
{
}

void RisipMessage::setInstantMessageStatusParam(OnInstantMessageStatusParam)
{

}

SendInstantMessageParam RisipMessage::messageParamForSend()
{
    m_instantMessageParam.userData = this;
    return m_instantMessageParam;
}

} //end of risip namespace
