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
#include "risipmodels.h"
#include "risipglobals.h"
#include "risipphonenumber.h"

#include <QDebug>

class RisipPhoneContact::Private
{
public:
    int m_id;
    QString m_fullName;
    QString m_email;
    QHash<QString, RisipPhoneNumber *> m_phoneNumbers;
    RisipPhoneNumbersModel *m_phoneNumbersModel;
    QByteArray m_contactImageData;
};

RisipPhoneContact::RisipPhoneContact(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->m_id = -1;
    m_data->m_phoneNumbersModel = new RisipPhoneNumbersModel(this);
    m_data->m_phoneNumbersModel->setPhoneContact(this);
}

RisipPhoneContact::~RisipPhoneContact()
{
    delete m_data;
    m_data = NULL;
}

int RisipPhoneContact::contactId() const
{
    return m_data->m_id;
}

void RisipPhoneContact::setContactId(int id)
{
    if(m_data->m_id != id) {
        m_data->m_id = id;
        emit contactIdChanged(m_data->m_id);
    }
}

QString RisipPhoneContact::fullName() const
{
    return m_data->m_fullName;
}

void RisipPhoneContact::setFullName(QString name)
{
    if(m_data->m_fullName != name) {
        m_data->m_fullName = name;
        emit fullNameChanged(m_data->m_fullName);
    }
}

QString RisipPhoneContact::email() const
{
    return m_data->m_email;
}

void RisipPhoneContact::setEmail(const QString &email)
{
    if(m_data->m_email != email) {
        m_data->m_email = email;
        emit emailChanged(email);
    }
}

RisipPhoneNumbersModel *RisipPhoneContact::phoneNumbersModel() const
{
    return m_data->m_phoneNumbersModel;
}

QByteArray RisipPhoneContact::contactImageData() const
{
    return m_data->m_contactImageData;
}

void RisipPhoneContact::setContactImageData(QByteArray imageData)
{
    if(m_data->m_contactImageData != imageData) {
        m_data->m_contactImageData = imageData;
        emit contactImageDataChanged(m_data->m_contactImageData);
    }
}

QQmlListProperty<RisipPhoneNumber> RisipPhoneContact::phoneNumbers()
{
    QList<RisipPhoneNumber *> numbers = m_data->m_phoneNumbers.values();
    return QQmlListProperty<RisipPhoneNumber>(this, numbers);
}

QList<RisipPhoneNumber *> RisipPhoneContact::phoneNumberList() const
{
    return m_data->m_phoneNumbers.values();
}

RisipPhoneNumber *RisipPhoneContact::phoneNumberForNumber(const QString &number)
{
    if(m_data->m_phoneNumbers.contains(number))
        return m_data->m_phoneNumbers[number];

    return NULL;
}

void RisipPhoneContact::addPhoneNumber(const QString &number, const QString &label)
{
    RisipPhoneNumber *phoneNumber = new RisipPhoneNumber(number, this);
    phoneNumber->setLabel(label);
    if(!m_data->m_phoneNumbers.contains(phoneNumber->fullNumber()))
        m_data->m_phoneNumbers[phoneNumber->fullNumber()] = phoneNumber;
}

void RisipPhoneContact::addPhoneNumber(RisipPhoneNumber *number)
{
    if(number) {
        if(!m_data->m_phoneNumbers.contains(number->fullNumber()))
            m_data->m_phoneNumbers[number->fullNumber()] = number;
    }
}

void RisipPhoneContact::removePhoneNumber(RisipPhoneNumber *number)
{
    if(number) {
        if(m_data->m_phoneNumbers.contains(number->fullNumber()))
            m_data->m_phoneNumbers.take(number->fullNumber())->deleteLater();
    }
}

