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

#include "risipcallhistorymodel.h"
#include "risipcall.h"

#include <QDebug>

RisipCallHistoryModel::RisipCallHistoryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

RisipCallHistoryModel::~RisipCallHistoryModel()
{
}

RisipAccount *RisipCallHistoryModel::account()
{
    return m_account;
}

void RisipCallHistoryModel::setAccount(RisipAccount *account)
{
    if(m_account != account) {
        m_account = account;
        emit accountChanged(m_account);
    }
}

QHash<int, QByteArray> RisipCallHistoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CallContactRole] = "callContact";
    roles[CallDirectionRole] = "callDirection";
    roles[CallDurationRole] = "callDuration";
    roles[CallTimestampRole] = "callTimestamp";
    return roles;
}

int RisipCallHistoryModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_calls.count();
}

QVariant RisipCallHistoryModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    if (!index.isValid())
        return QVariant();

    RisipCall *call = m_calls.at(index.row());
    if(!call) {
        qDebug()<<"No call data found in the history!";
        return QVariant();
    }

    switch (role) {
    case CallDirectionRole:
        return call->callDirection();
    case CallDurationRole:
        return call->callDuration();
    case CallTimestampRole:
        return call->timestamp();
    }

    return QVariant();
}

void RisipCallHistoryModel::addCallRecord(RisipCall *call)
{
    if(call) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_calls.append(call);
        endInsertRows();
    }
}

void RisipCallHistoryModel::removeCallRecord(RisipCall *call)
{
    if(call) {
        beginRemoveRows(QModelIndex(), m_calls.indexOf(call), 1);
        m_calls.removeAll(call);
        endRemoveRows();
    }
}
