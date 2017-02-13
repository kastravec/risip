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
#ifndef RISIPPHONECONTACTSMODEL_H
#define RISIPPHONECONTACTSMODEL_H

#include "risipsdkglobal.h"
#include <QAbstractListModel>

namespace risip {

class RisipPhoneContact;

class RISIP_VOIPSDK_EXPORT RisipPhoneContactsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RisipPhoneContactDataRole {
        ContactId = Qt::UserRole + 1,
        FullName,
        Initials,
        PhoneNumberList
    };

    explicit RisipPhoneContactsModel(QObject *parent = NULL);
    ~RisipPhoneContactsModel();

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE RisipPhoneContact *contactForIndex(int index);

public Q_SLOTS:
    void addContact(RisipPhoneContact *contact);
    void removeContact(RisipPhoneContact *contact);

private:
    QList<RisipPhoneContact *> m_phoneContacts;
};

} //end of risip namespace

#endif // RISIPPHONECONTACTSMODEL_H
