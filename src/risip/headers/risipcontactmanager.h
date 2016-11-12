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
#ifndef RISIPCONTACTMANAGER_H
#define RISIPCONTACTMANAGER_H

#include <QObject>
#include <QAbstractItemModel>
#include <QQmlListProperty>

class RisipAccount;
class RisipBuddiesModel;
class RisipContactHistoryModel;
class RisipiOSContactAcessManager;
class RisipPhoneContactsModel;
class RisipPhoneContact;

class RisipContactManager : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(RisipAccount * activeAccount READ activeAccount WRITE setActiveAccount NOTIFY activeAccountChanged)
    Q_PROPERTY(QAbstractItemModel * activeBuddiesModel READ activeBuddiesModel WRITE setActiveBuddiesModel NOTIFY activeBuddiesModelChanged)
    Q_PROPERTY(QAbstractItemModel * activeContactHistory READ activeContactHistory WRITE setActiveContactHistory NOTIFY activeContactHistoryChanged)
    Q_PROPERTY(QAbstractItemModel * phoneContactsModel READ phoneContactsModel NOTIFY phoneContactsModelChanged)
    Q_PROPERTY(QQmlListProperty<QAbstractItemModel> buddyModels READ buddyModels NOTIFY buddyModelsChanged)
    Q_PROPERTY(QQmlListProperty<QAbstractItemModel> contactHistoryModels READ contactHistoryModels NOTIFY contactHistoryModelsChanged)

    static RisipContactManager *instance();
    ~RisipContactManager();

    RisipAccount *activeAccount() const;
    void setActiveAccount(RisipAccount *activeAccount);

    QAbstractItemModel *activeBuddiesModel() const;
    QAbstractItemModel *activeContactHistory() const;
    QAbstractItemModel *phoneContactsModel() const;

    QQmlListProperty<QAbstractItemModel> buddyModels();
    QQmlListProperty<QAbstractItemModel> contactHistoryModels();

    void createModelsForAccount(RisipAccount *account);
    void removeModelsForAccount(const RisipAccount *account);

    QAbstractItemModel *buddyModelForAccount(const QString &account) const;
    QAbstractItemModel *contactHistoryModelForAccount(const QString &account) const;

    Q_INVOKABLE void fetchPhoneContacts();

Q_SIGNALS:
    void activeAccountChanged(RisipAccount *activeAccount);
    void activeBuddiesModelChanged(QAbstractItemModel *model);
    void activeContactHistoryChanged(QAbstractItemModel *model);
    void buddyModelsChanged();
    void contactHistoryModelsChanged();
    void phoneContactsModelChanged(QAbstractItemModel *model);

private:
    explicit RisipContactManager(QObject *parent = 0);
    static RisipContactManager *m_instance;
    void setActiveBuddiesModel(QAbstractItemModel *model);
    void setActiveContactHistory(QAbstractItemModel *history);

    RisipAccount *m_activeAccount;
    QHash<QString, QAbstractItemModel *> m_accountBuddyModels;
    QHash<QString, QAbstractItemModel *> m_accountContactHistoryModels;
    QAbstractItemModel *m_activeBuddiesModel;
    QAbstractItemModel *m_activeContactHistoryModel;
    RisipPhoneContactsModel *m_phoneContactsModel;

    //responsible for fetching contacts from the ios device.
#ifdef Q_OS_IOS
    RisipiOSContactAcessManager *m_iosContacts = NULL;
#endif

};

#endif // RISIPCONTACTMANAGER_H
