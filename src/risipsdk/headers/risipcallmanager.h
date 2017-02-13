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
#ifndef RISIPCALLMANAGER_H
#define RISIPCALLMANAGER_H

#include "risipsdkglobal.h"
#include "risipaccount.h"
#include "risipcallhistorymodel.h"
#include "risipcall.h"

#include <QObject>
#include <QAbstractItemModel>
#include <QQmlListProperty>

namespace risip {

class RisipBuddy;
class RisipPhoneNumber;

class RISIP_VOIPSDK_EXPORT RisipCallManager : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(RisipAccount * activeAccount READ activeAccount WRITE setActiveAccount NOTIFY activeAccountChanged)
    Q_PROPERTY(RisipCall * activeCall READ activeCall NOTIFY activeCallChanged)
    Q_PROPERTY(QAbstractItemModel * activeCallHistoryModel READ activeCallHistoryModel WRITE setActiveCallHistoryModel NOTIFY activeCallHistoryModelChanged)
    Q_PROPERTY(QQmlListProperty<QAbstractItemModel> callHistoryModels READ callHistoryModels NOTIFY callHistoryModelsChanged)

    static RisipCallManager *instance();
    ~RisipCallManager();

    RisipAccount *activeAccount() const;
    void setActiveAccount(risip::RisipAccount *activeAccount);

    RisipCall *activeCall();
    void setActiveCall(RisipCall *call);

    //history related methods
    QAbstractItemModel *activeCallHistoryModel() const;
    void setActiveCallHistoryModel(QAbstractItemModel *model);
    QQmlListProperty<QAbstractItemModel> callHistoryModels();
    Q_INVOKABLE QAbstractItemModel *historyCallModelForAccount(const QString &account) const;


    //call related methods
    //FIXME using namespace for the return paramter so that MOC gets it properly
    Q_INVOKABLE risip::RisipCall *callSIPContact(const QString &contact);
    Q_INVOKABLE risip::RisipCall *callBuddy(RisipBuddy *buddy);
    Q_INVOKABLE risip::RisipCall *callPhone(const QString &number);
    Q_INVOKABLE risip::RisipCall *callExternalSIP(const QString &uri);

    void createModelsForAccount(RisipAccount *account);
    void removeModelsForAccount(const RisipAccount *account);

Q_SIGNALS:
    void activeAccountChanged(RisipAccount *activeAccount);
    void activeCallChanged(RisipCall *call);
    void activeCallHistoryModelChanged(QAbstractItemModel *model);
    void callHistoryModelsChanged(QQmlListProperty<QAbstractItemModel> models);
    void incomingCall(risip::RisipCall *call);
    void outgoingCall(risip::RisipCall *call);

private Q_SLOTS:
    void accountIncomingCall();

private:
    explicit RisipCallManager(QObject *parent = 0);
    static RisipCallManager *m_callManagerInstance;

    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIPCALLMANAGER_H
