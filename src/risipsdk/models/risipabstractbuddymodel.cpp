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
#include "risipabstractbuddymodel.h"
#include "risipbuddy.h"
#include "risip.h"

#include <QDebug>

namespace risip {

RisipAbstractBuddyModel::RisipAbstractBuddyModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_account(Risip::instance()->defaultAccount())
{
}

RisipAbstractBuddyModel::~RisipAbstractBuddyModel()
{
}

RisipAccount *RisipAbstractBuddyModel::account() const
{
    return m_account;
}

void RisipAbstractBuddyModel::setAccount(RisipAccount *account)
{
    if(m_account != account) {
        m_account = account;
        emit accountChanged(m_account);
    }
}

QHash<int, QByteArray> RisipAbstractBuddyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UriRole] = "uri";
    roles[ContactRole] = "contact";
    roles[TypeRole] = "type";

    return roles;
}

int RisipAbstractBuddyModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_buddies.count();
}

QVariant RisipAbstractBuddyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    RisipBuddy *buddy = m_buddies.at(index.row());
    if(!buddy) {
        qDebug()<<"no buddy found!";
        return QVariant();
    }

    switch (role) {
    case UriRole:
        return buddy->uri();
        break;
    case ContactRole:
        return buddy->contact();
        break;
    case TypeRole:
        return buddy->type();
        break;
    default:
        return QVariant();
    }

    return QVariant();
}

void RisipAbstractBuddyModel::addBuddy(RisipBuddy *buddy)
{
    if(buddy) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_buddies.append(buddy);
        endInsertRows();
    }
}

void RisipAbstractBuddyModel::removeBuddy(RisipBuddy *buddy)
{
    if(buddy) {
        beginRemoveRows(QModelIndex(), m_buddies.indexOf(buddy), 1);
        m_buddies.removeAll(buddy);
        buddy->releaseFromAccount();
        endRemoveRows();
    }
}

} //end of risip namespace
