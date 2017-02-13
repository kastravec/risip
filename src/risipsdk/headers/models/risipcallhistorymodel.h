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
#ifndef RISIPCALLHISTORYMODEL_H
#define RISIPCALLHISTORYMODEL_H

#include "risipsdkglobal.h"
#include <QAbstractListModel>

namespace risip {

class RisipAccount;
class RisipCall;

class RISIP_VOIPSDK_EXPORT RisipCallHistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CallDataRole {
        CallContactRole = Qt::UserRole + 1, //callContact
        CallDirectionRole, // callDirection
        CallDurationRole, // callDuration
        CallTimestampRole // callTimestamp
    };

    Q_PROPERTY(RisipAccount * account READ account WRITE setAccount NOTIFY accountChanged)

    explicit RisipCallHistoryModel(QObject *parent = 0);
    ~RisipCallHistoryModel();

    RisipAccount *account();
    void setAccount(RisipAccount *account);

    // Basic functionality:
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void addCallRecord(RisipCall *call);
    void removeCallRecord(RisipCall *call);

Q_SIGNALS:
    void accountChanged(RisipAccount *account);

private:
    RisipAccount *m_account;
    QList<RisipCall *> m_calls;
};

} //end of risip namespace

#endif // RISIPCALLHISTORYMODEL_H
