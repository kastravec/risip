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

#include "risipmodels.h"
#include "risip.h"
#include "risipaccount.h"
#include "risipbuddy.h"
#include "risipcall.h"

#include <QSortFilterProxyModel>
#include <QDebug>

RisipAbstractBuddyModel::RisipAbstractBuddyModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_account(Risip::instance()->defaultAccount())
    ,m_proxy(new QSortFilterProxyModel(this))
{
    m_proxy->setSourceModel(this);
}

RisipAbstractBuddyModel::~RisipAbstractBuddyModel()
{
    delete m_proxy;
    m_proxy = NULL;
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

QSortFilterProxyModel *RisipAbstractBuddyModel::proxy() const
{
    return m_proxy;
}

void RisipAbstractBuddyModel::setProxy(QSortFilterProxyModel *proxy)
{
    if(m_proxy != proxy) {
        if(m_proxy) {
            delete m_proxy;
            m_proxy = NULL;
        }

        m_proxy = proxy;
        emit proxyChanged(m_proxy);
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

RisipBuddiesModel::RisipBuddiesModel(QObject *parent)
    :RisipAbstractBuddyModel(parent)
{}

RisipBuddiesModel::~RisipBuddiesModel()
{}

RisipContactHistoryModel::RisipContactHistoryModel(QObject *parent)
    :RisipAbstractBuddyModel(parent)
{}

RisipContactHistoryModel::~RisipContactHistoryModel()
{}

RisipCallHistoryModel::RisipCallHistoryModel(QObject *parent)
    : QAbstractListModel(parent)
    ,m_account(Risip::instance()->defaultAccount())
    ,m_proxy(new QSortFilterProxyModel(this))
{
    m_proxy->setSourceModel(this);
}

RisipCallHistoryModel::~RisipCallHistoryModel()
{
    delete m_proxy;
    m_proxy = NULL;
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

QSortFilterProxyModel *RisipCallHistoryModel::proxy() const
{
    return m_proxy;
}

void RisipCallHistoryModel::setProxy(QSortFilterProxyModel *proxy)
{
    if(m_proxy != proxy) {
        m_proxy = proxy;
        emit proxyChanged(m_proxy);
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
