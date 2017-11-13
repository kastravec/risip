/***********************************************************************************
**    Copyright (C) 2017  Petref Saraci
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

#include "risipaccountlistmodel.h"

#include "risip.h"
#include "risipaccount.h"
#include <QDebug>

namespace risip {

class RisipAccountListModel::Private {

public:
    QHash<QString, RisipAccount *> accounts;
    QString defaultAccountUri;
    RisipAccount *defaultAccount;
};

RisipAccountListModel::RisipAccountListModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_data(new Private)
{
    m_data->defaultAccountUri = QString();
    m_data->defaultAccount = new RisipAccount(this);
}

RisipAccountListModel::~RisipAccountListModel()
{
    delete m_data;
    m_data = nullptr;
}

QString RisipAccountListModel::defaultAccountUri() const
{
    return m_data->defaultAccountUri;
}

void RisipAccountListModel::setDefaultAccountUri(const QString &uri)
{
    if(!uri.isEmpty()
            && exists(uri)
            && m_data->defaultAccountUri != uri) {
        m_data->defaultAccountUri = uri;
        setDefaultAccount(account(m_data->defaultAccountUri));
        emit defaultAccountUriChanged(m_data->defaultAccountUri);
    }
}

RisipAccount *RisipAccountListModel::defaultAccount() const
{
    return m_data->defaultAccount;
}

void RisipAccountListModel::setDefaultAccount(RisipAccount *account)
{
    if(account && m_data->defaultAccount != account) {
        if(m_data->defaultAccount
                && m_data->defaultAccount->configuration()->uri() != account->configuration()->uri()) {
            m_data->defaultAccount = account;
            m_data->defaultAccount->setParent(this);
            emit defaultAccountChanged(m_data->defaultAccount);
        }
    }
}

QHash<int, QByteArray> RisipAccountListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AccountURI] = "accountUri";
    roles[UserName] = "userName";
    roles[Password] = "password";
    return roles;
}

int RisipAccountListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_data->accounts.count();
}

QVariant RisipAccountListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()
            || index.row() >= m_data->accounts.count())
        return QVariant();

    RisipAccount *account = m_data->accounts.value(m_data->accounts.keys().at(index.row()));

    switch (role) {
    case AccountURI:
        return account->configuration()->uri();
    case UserName:
        return account->configuration()->userName();
    case Password:
        return account->configuration()->password();
    default:
        return QVariant();
    }
}

RisipAccount *RisipAccountListModel::account(const QString &uri)
{
    return m_data->accounts.value(uri, new RisipAccount(this));
}

RisipAccount *RisipAccountListModel::account(int index)
{
    if( -1 < index <= rowCount())
        return m_data->accounts.value(m_data->accounts.keys().at(index), new RisipAccount(this));

    return new RisipAccount(this);
}

bool RisipAccountListModel::exists(const QString &uri)
{
    if(!uri.isEmpty())
        return m_data->accounts.contains(uri);

    return false;
}

bool RisipAccountListModel::clear()
{
    for(int i = 0; i < m_data->accounts.count(); ++i)
        removeSipAccount(m_data->accounts.keys()[i]);

    return true;
}

void RisipAccountListModel::addSipAccount(RisipAccount *account)
{
    if(account) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        emit layoutAboutToBeChanged();

        m_data->accounts.insert(account->configuration()->uri(), account);
        account->setParent(this);

        endInsertColumns();
        emit layoutChanged();
    }
}

void RisipAccountListModel::removeSipAccount(RisipAccount *account)
{
    if(account) {

        QString accountName = account->configuration()->uri();
        int indx = m_data->accounts.keys().indexOf(accountName);

        if(indx != -1) {
            beginRemoveRows(QModelIndex(), indx, 1);
            m_data->accounts[accountName]->deleteLater();
            m_data->accounts.remove(accountName);
            account->deleteLater();
            endRemoveRows();
        }
    }
}

void RisipAccountListModel::removeSipAccount(const QString &uri)
{
    if(!uri.isEmpty()) {
        int indx = m_data->accounts.keys().indexOf(uri);

        if(indx != -1) {
            beginRemoveRows(QModelIndex(), indx, 1);
            m_data->accounts[uri]->deleteLater();
            m_data->accounts.remove(uri);
            endRemoveRows();
        }
    }
}

} //end of namespace
