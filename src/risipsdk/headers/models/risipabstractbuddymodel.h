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
#ifndef RISIPABSTRACTBUDDYMODEL_H
#define RISIPABSTRACTBUDDYMODEL_H

#include "risipsdkglobal.h"
#include "risipaccount.h"

#include <QAbstractListModel>

namespace risip {

class RisipBuddy;

class RISIP_VOIPSDK_EXPORT RisipAbstractBuddyModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum BuddyRole {
        UriRole = Qt::UserRole + 1,
        ContactRole,
        TypeRole
    };

    explicit RisipAbstractBuddyModel(QObject *parent = NULL);
    virtual ~RisipAbstractBuddyModel();

    Q_ENUM(BuddyRole)
    Q_PROPERTY(RisipAccount * account READ account WRITE setAccount NOTIFY accountChanged)

    RisipAccount *account() const;
    void setAccount(RisipAccount *account);

    // Basic functionality:
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual void addBuddy(RisipBuddy *buddy);
    virtual void removeBuddy(RisipBuddy *buddy);

Q_SIGNALS:
    void accountChanged(RisipAccount *account);

private:
    RisipAccount *m_account;
    QList<RisipBuddy *> m_buddies;
};

} //end of risip namespace

#endif // RISIPABSTRACTBUDDYMODEL_H
