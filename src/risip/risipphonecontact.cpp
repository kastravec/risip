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
#include "risipphonecontact.h"

RisipPhoneContact::RisipPhoneContact(QObject *parent)
    :QObject(parent)
    ,m_id(-1)
    ,m_fullName()
    ,m_phoneNumbers()
{

}

RisipPhoneContact::~RisipPhoneContact()
{
}

int RisipPhoneContact::contactId() const
{
    return m_id;
}

void RisipPhoneContact::setContactId(int id)
{
    if(m_id != id) {
        m_id = id;
        emit contactIdChanged(m_id);
    }
}

QString RisipPhoneContact::fullName() const
{
    return m_fullName;
}

void RisipPhoneContact::setFullName(const QString &name)
{
    if(m_fullName != name) {
        m_fullName = name;
        emit fullNameChanged(m_fullName);
    }
}

QStringList RisipPhoneContact::phoneNumbers() const
{
    return m_phoneNumbers;
}

void RisipPhoneContact::setPhoneNumbers(const QStringList &numbers)
{
    if(m_phoneNumbers != numbers ) {
        m_phoneNumbers = numbers;
        emit phoneNumbersChanged(m_phoneNumbers);
    }
}
