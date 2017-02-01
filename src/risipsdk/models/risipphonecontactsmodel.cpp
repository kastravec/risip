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
#include "risipphonecontactsmodel.h"
#include "risipphonecontact.h"

#include <QDebug>

RisipPhoneContactsModel::RisipPhoneContactsModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_phoneContacts()
{
}

RisipPhoneContactsModel::~RisipPhoneContactsModel()
{
}

QHash<int, QByteArray> RisipPhoneContactsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ContactId] = "contactId";
    roles[FullName] = "fullName";
    roles[Initials] = "initials";
    roles[PhoneNumberList] = "phoneNumberList";
    return roles;
}

int RisipPhoneContactsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_phoneContacts.count();
}

QVariant RisipPhoneContactsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()
            || index.row() >= m_phoneContacts.count())
        return QVariant();

    RisipPhoneContact *contact = m_phoneContacts.at(index.row());
    if(!contact) {
        qDebug()<<"No contact found in model..!";
        return QVariant();
    }

    switch (role) {
    case ContactId:
        return contact->contactId();
    case FullName:
        if(contact)
            return contact->fullName();
        return QVariant();
    case Initials: {
        return contact->initials();
    }
    case PhoneNumberList:
        return QVariant::fromValue(contact->phoneNumberList());
    }

    return QVariant();
}

RisipPhoneContact *RisipPhoneContactsModel::contactForIndex(int index)
{
    if(m_phoneContacts.count() >= index >= 0)
        return m_phoneContacts[index];

    return NULL;
}

void RisipPhoneContactsModel::addContact(RisipPhoneContact *contact)
{
    if(!contact)
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_phoneContacts.append(contact);
    endInsertRows();
}

void RisipPhoneContactsModel::removeContact(RisipPhoneContact *contact)
{
    if(!contact && !m_phoneContacts.contains(contact))
        return;

    beginRemoveRows(QModelIndex(), m_phoneContacts.indexOf(contact), 1);
    m_phoneContacts.removeAll(contact);
    contact->deleteLater();
    endRemoveRows();
}
