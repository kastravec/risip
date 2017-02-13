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
#include "risipphonenumbersmodel.h"

#include "risipphonecontact.h"
#include "risipphonenumber.h"

#include <QDebug>

namespace risip {

RisipPhoneNumbersModel::RisipPhoneNumbersModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_phoneContact(NULL)
{
}

RisipPhoneNumbersModel::~RisipPhoneNumbersModel()
{
}

RisipPhoneContact *RisipPhoneNumbersModel::phoneContact() const
{
    return m_phoneContact;
}

void RisipPhoneNumbersModel::setPhoneContact(RisipPhoneContact *contact)
{
    if(m_phoneContact != contact) {
//        beginResetModel();
        m_phoneContact = contact;
//        endResetModel();
        emit phoneContactChanged(m_phoneContact);
    }
}

QHash<int, QByteArray> RisipPhoneNumbersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FullNumber] = "fullNumber";
    roles[CountryPrefix] = "countryPrefix";
    roles[CountryCode] = "countryCode";
    roles[RegionPrefix] = "regionPrefix";
    roles[Number] = "number";
    roles[RawNumber] = "rawNumber";
    return roles;
}

int RisipPhoneNumbersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_phoneContact)
        return 0;

    return m_phoneContact->phoneNumberList().count();
}

QVariant RisipPhoneNumbersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_phoneContact
            || index.row() >= m_phoneContact->phoneNumberList().count())
        return QVariant();

    switch (role) {
    case FullNumber:
        return m_phoneContact->phoneNumberList()[index.row()]->fullNumber();
    case CountryPrefix:
        return m_phoneContact->phoneNumberList()[index.row()]->countryPrefix();
    case CountryCode:
        return m_phoneContact->phoneNumberList()[index.row()]->countryCode();
    case RegionPrefix:
        return QVariant();
    case Number:
        return m_phoneContact->phoneNumberList()[index.row()]->number();
    case RawNumber:
        return m_phoneContact->phoneNumberList()[index.row()]->rawNumber();
    default:
        return QVariant();
    }

    return QVariant();
}

} //end of risip namespace
