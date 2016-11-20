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
#include <QQmlListProperty>

class RisipAccount;
class RisipBuddiesModel;
class RisipContactHistoryModel;
class RisipiOSContactAcessManager;
class RisipPhoneContactsModel;
class RisipPhoneContact;
class RisipPhoneNumber;

class RisipContactManager : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(RisipAccount * activeAccount READ activeAccount WRITE setActiveAccount NOTIFY activeAccountChanged)
    Q_PROPERTY(RisipBuddiesModel * activeBuddiesModel READ activeBuddiesModel WRITE setActiveBuddiesModel NOTIFY activeBuddiesModelChanged)
    Q_PROPERTY(RisipContactHistoryModel * activeContactHistory READ activeContactHistory WRITE setActiveContactHistory NOTIFY activeContactHistoryChanged)
    Q_PROPERTY(RisipPhoneContactsModel * phoneContactsModel READ phoneContactsModel NOTIFY phoneContactsModelChanged)
    Q_PROPERTY(QQmlListProperty<RisipBuddiesModel> buddyModels READ buddyModels NOTIFY buddyModelsChanged)
    Q_PROPERTY(QQmlListProperty<RisipContactHistoryModel> contactHistoryModels READ contactHistoryModels NOTIFY contactHistoryModelsChanged)
    Q_PROPERTY(QQmlListProperty<RisipPhoneContact> phoneContacts READ phoneContacts)

    static RisipContactManager *instance();
    ~RisipContactManager();

    RisipAccount *activeAccount() const;
    void setActiveAccount(RisipAccount *activeAccount);

    RisipBuddiesModel *activeBuddiesModel() const;
    RisipContactHistoryModel *activeContactHistory() const;
    RisipPhoneContactsModel *phoneContactsModel() const;

    QQmlListProperty<RisipBuddiesModel> buddyModels();
    QQmlListProperty<RisipContactHistoryModel> contactHistoryModels();

    QQmlListProperty<RisipPhoneContact> phoneContacts();
    QList<RisipPhoneContact *> phoneContactList() const;

    void createModelsForAccount(RisipAccount *account);
    void removeModelsForAccount(const RisipAccount *account);

    RisipBuddiesModel *buddyModelForAccount(const QString &account) const;
    RisipContactHistoryModel *contactHistoryModelForAccount(const QString &account) const;

    Q_INVOKABLE void fetchPhoneContacts();
    Q_INVOKABLE RisipPhoneContact *contactForName(const QString &name);
    Q_INVOKABLE RisipPhoneNumber *phoneNumberForNumber(const QString &number);

Q_SIGNALS:
    void activeAccountChanged(RisipAccount *activeAccount);
    void activeBuddiesModelChanged(RisipBuddiesModel *model);
    void activeContactHistoryChanged(RisipContactHistoryModel *model);
    void buddyModelsChanged();
    void contactHistoryModelsChanged();
    void phoneContactsModelChanged(RisipPhoneContactsModel *model);

private Q_SLOTS:
    void phoneContactDiscovered(RisipPhoneContact *contact);

private:
    explicit RisipContactManager(QObject *parent = 0);
    static RisipContactManager *m_instance;
    void setActiveBuddiesModel(RisipBuddiesModel *model);
    void setActiveContactHistory(RisipContactHistoryModel *history);

    RisipAccount *m_activeAccount;
    QHash<QString, RisipBuddiesModel *> m_accountBuddyModels;
    QHash<QString, RisipContactHistoryModel *> m_accountContactHistoryModels;
    RisipBuddiesModel *m_activeBuddiesModel;
    RisipContactHistoryModel *m_activeContactHistoryModel;
    RisipPhoneContactsModel *m_phoneContactsModel;

    QHash<QString, RisipPhoneContact *> m_phoneContacts;
    QHash<QString, RisipPhoneNumber *> m_phoneNumbers;

    //responsible for fetching contacts from the ios device.
#ifdef Q_OS_IOS
    RisipiOSContactAcessManager *m_iosContacts = NULL;
#endif

};

#endif // RISIPCONTACTMANAGER_H
