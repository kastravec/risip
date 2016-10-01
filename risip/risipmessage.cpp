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

#include "risipmessage.h"

RisipMessage::RisipMessage(QObject *parent)
    :QObject(parent)
{
}

RisipMessage::~RisipMessage()
{
}

QString RisipMessage::messageBody() const
{
    return m_messageBody;
}

QString RisipMessage::contactUri() const
{
    return m_contactUri;
}

QString RisipMessage::senderUri() const
{
    return m_senderUri;
}

QString RisipMessage::contentType() const
{
    return m_contentType;
}

void RisipMessage::createMessage(const QString &messBody, const QString &contactUri, const QString senderUri, const QString &contentType)
{
    setMessageBody(messBody);
    setContactUri(contactUri);
    setSenderUri(senderUri);
    setContentType(contentType);
}

void RisipMessage::setMessageBody(const QString &messageBody)
{
    if(m_messageBody != messageBody) {
        m_messageBody = messageBody;
        emit messageBodyChanged(m_messageBody);
    }
}

void RisipMessage::setContactUri(const QString &contact)
{
    if(m_contactUri != contact) {
        m_contactUri = contact;
        emit contactUriChanged(m_contactUri);
    }
}

void RisipMessage::setSenderUri(const QString &sender)
{
    if(m_senderUri != sender) {
        m_senderUri = sender;
        emit senderUriChanged(m_senderUri);
    }
}

void RisipMessage::setContentType(const QString &type)
{
    if(m_contentType != type) {
        m_contentType = type;
        emit contentTypeChanged(m_contentType);
    }
}
