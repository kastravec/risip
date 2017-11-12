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

#ifndef RISIPACCOUNTLISTMODEL_H
#define RISIPACCOUNTLISTMODEL_H

#include "risipsdkglobal.h"
#include <QAbstractListModel>

namespace risip {

class RisipAccount;

class RISIP_VOIPSDK_EXPORT RisipAccountListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum RisipAccountListDataRole {
        AccountURI = Qt::UserRole + 1,
        UserName,
        Password
    };

    Q_ENUM(RisipAccountListDataRole)
    Q_PROPERTY(QString defaultAccountUri READ defaultAccountUri WRITE setDefaultAccountUri NOTIFY defaultAccountUriChanged)
    Q_PROPERTY(RisipAccount * defaultAccount READ defaultAccount WRITE setDefaultAccount NOTIFY defaultAccountChanged)

    explicit RisipAccountListModel(QObject *parent = 0);
    ~RisipAccountListModel();

    QString defaultAccountUri() const;
    void setDefaultAccountUri(const QString &uri);

    RisipAccount *defaultAccount() const;
    void setDefaultAccount(RisipAccount *account);

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    RisipAccount *account(const QString &uri);
    RisipAccount *account(int index);
    bool exists(const QString &uri);
    bool clear();

Q_SIGNALS:
    void defaultAccountUriChanged(const QString &uri);
    void defaultAccountChanged(RisipAccount *account);

private Q_SLOTS:
    void addSipAccount(RisipAccount *account);
    void removeSipAccount(RisipAccount *account);
    void removeSipAccount(const QString &uri);

private:
    friend class Risip;
    class Private;
    Private *m_data;
};

} //end of namespace

#endif // RISIPACCOUNTLISTMODEL_H
