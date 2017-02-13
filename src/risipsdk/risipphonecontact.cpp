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

#include "models/risipphonenumbersmodel.h"

#include <QDebug>

namespace risip {

class RisipPhoneContact::Private
{
public:
    int id;
    QString fullName;
    QString email;
    QHash<QString, RisipPhoneNumber *> phoneNumbers;
    RisipPhoneNumbersModel *phoneNumbersModel;
    QByteArray contactImageData;
};

RisipPhoneContact::RisipPhoneContact(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->id = -1;
    m_data->phoneNumbersModel = new RisipPhoneNumbersModel(this);
    m_data->phoneNumbersModel->setPhoneContact(this);
}

RisipPhoneContact::~RisipPhoneContact()
{
    delete m_data;
    m_data = NULL;
}

int RisipPhoneContact::contactId() const
{
    return m_data->id;
}

void RisipPhoneContact::setContactId(int id)
{
    if(m_data->id != id) {
        m_data->id = id;
        emit contactIdChanged(m_data->id);
    }
}

QString RisipPhoneContact::fullName() const
{
    return m_data->fullName;
}

void RisipPhoneContact::setFullName(QString name)
{
    if(m_data->fullName != name) {
        m_data->fullName = name;
        emit fullNameChanged(m_data->fullName);
        emit initialsChanged(initials());
    }
}

QString RisipPhoneContact::email() const
{
    return m_data->email;
}

void RisipPhoneContact::setEmail(const QString &email)
{
    if(m_data->email != email) {
        m_data->email = email;
        emit emailChanged(email);
    }
}

QString RisipPhoneContact::initials() const
{
    QStringList firstLast = m_data->fullName.split(" ");
    if(firstLast.count() == 0)
        return QString(".");
    if(firstLast.count() == 1)
        return firstLast.at(0).left(1).toUpper();
    return firstLast.at(0).left(1) + QString(".") + firstLast.at(1).left(1);
}

RisipPhoneNumbersModel *RisipPhoneContact::phoneNumbersModel() const
{
    return m_data->phoneNumbersModel;
}

QByteArray RisipPhoneContact::contactImageData() const
{
    return m_data->contactImageData;
}

void RisipPhoneContact::setContactImageData(QByteArray imageData)
{
    if(m_data->contactImageData != imageData) {
        m_data->contactImageData = imageData;
        emit contactImageDataChanged(m_data->contactImageData);
    }
}

QQmlListProperty<RisipPhoneNumber> RisipPhoneContact::phoneNumbers()
{
    QList<RisipPhoneNumber *> numbers = m_data->phoneNumbers.values();
    return QQmlListProperty<RisipPhoneNumber>(this, numbers);
}

QList<RisipPhoneNumber *> RisipPhoneContact::phoneNumberList() const
{
    return m_data->phoneNumbers.values();
}

RisipPhoneNumber *RisipPhoneContact::phoneNumberForNumber(const QString &number)
{
    if(m_data->phoneNumbers.contains(number))
        return m_data->phoneNumbers[number];

    return NULL;
}

void RisipPhoneContact::addPhoneNumber(const QString &number, const QString &label)
{
    RisipPhoneNumber *phoneNumber = new RisipPhoneNumber(number, this);
    phoneNumber->setLabel(label);
    if(!m_data->phoneNumbers.contains(phoneNumber->fullNumber()))
        m_data->phoneNumbers[phoneNumber->fullNumber()] = phoneNumber;
}

void RisipPhoneContact::addPhoneNumber(RisipPhoneNumber *number)
{
    if(number) {
        if(!m_data->phoneNumbers.contains(number->fullNumber()))
            m_data->phoneNumbers[number->fullNumber()] = number;
    }
}

void RisipPhoneContact::removePhoneNumber(RisipPhoneNumber *number)
{
    if(number) {
        if(m_data->phoneNumbers.contains(number->fullNumber()))
            m_data->phoneNumbers.take(number->fullNumber())->deleteLater();
    }
}

} //end of risip namespace

