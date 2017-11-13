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

#include "risipcontactmanager.h"
#include "risip.h"
#include "risipaccount.h"
#include "risipaccountconfiguration.h"
#include "risipbuddy.h"
#include "android/risipandroidcontactaccessmanager.h"
#include <QCoreApplication>

#include <QDebug>

namespace risip {

class RisipContactManager::Private
{
public:
    RisipAccount *m_activeAccount;
    QHash<QString, RisipBuddiesModel *> m_accountBuddyModels;
    QHash<QString, RisipContactHistoryModel *> m_accountContactHistoryModels;
    RisipBuddiesModel *m_activeBuddiesModel;
    RisipContactHistoryModel *m_activeContactHistoryModel;
    RisipPhoneContactsModel *m_phoneContactsModel;

    QHash<QString, RisipPhoneContact *> m_phoneContacts;
    QHash<QString, RisipPhoneNumber *> m_phoneNumbers;
    RisipPhoneContact *m_activePhoneContact;

    //responsible for fetching contacts from the ios device.
#ifdef Q_OS_IOS
    RisipiOSContactAcessManager *m_iosContacts = NULL;
#endif

#ifdef Q_OS_ANDROID
    RisipAndroidContactAccessManager *androidContactAccessManager = NULL;
#endif

};

RisipContactManager *RisipContactManager::m_instance = NULL;
RisipContactManager *RisipContactManager::instance()
{
    if(!m_instance)
        m_instance = new RisipContactManager;

    return m_instance;
}

RisipContactManager::RisipContactManager(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->m_activeAccount = Risip::instance()->defaultAccount();
    m_data->m_activeBuddiesModel = NULL;
    m_data->m_activeContactHistoryModel = NULL;
    m_data->m_phoneContactsModel = new RisipPhoneContactsModel(this);
    m_data->m_activePhoneContact = NULL;
}

RisipContactManager::~RisipContactManager()
{
    delete m_data;
    m_data = NULL;
}

/**
 * @brief RisipContactManager::activeAccount
 * @return the current active SIP account
 *
 * This property holds the current active account.
 */
RisipAccount *RisipContactManager::activeAccount() const
{
    return m_data->m_activeAccount;
}

/**
 * @brief RisipContactManager::setActiveAccount
 * @param account the new active account
 *
 * Use with caution. This sets active account for the RisipContactManager.
 * By default all operations and other "active" type of properties will be refreshed
 * and will be based on this account.
 * see @RisipContactManager::activeBuddiesModel see @RisipContactManager::activeContactHistoryModel
 */
void RisipContactManager::setActiveAccount(RisipAccount *account)
{
    if(m_data->m_activeAccount != account) {
        m_data->m_activeAccount = account;
        if(!m_data->m_activeAccount)
            m_data->m_activeAccount = Risip::instance()->defaultAccount();

        if(m_data->m_activeAccount) {
            if(buddyModelForAccount(m_data->m_activeAccount->configuration()->uri())) {
                setActiveBuddiesModel(buddyModelForAccount(m_data->m_activeAccount->configuration()->uri()));
            } else {
                //TODO create model for this new active account
            }

            if(contactHistoryModelForAccount(m_data->m_activeAccount->configuration()->uri())) {
                setActiveContactHistory(contactHistoryModelForAccount(m_data->m_activeAccount->configuration()->uri()));
            } else {
                //TODO create model for this new active account
            }
        }

        emit activeAccountChanged(m_data->m_activeAccount);
    }
}

RisipPhoneContact *RisipContactManager::activePhoneContact() const
{
    return m_data->m_activePhoneContact;
}

void RisipContactManager::setActivePhoneContact(const QString &contactName)
{
    if(!contactName.isEmpty() && m_data->m_phoneContacts.contains(contactName))
        setActivePhoneContact(m_data->m_phoneContacts[contactName]);
}

void RisipContactManager::setActivePhoneContact(RisipPhoneContact *phoneContact)
{
    if(m_data->m_activePhoneContact != phoneContact) {
        m_data->m_activePhoneContact = phoneContact;
        emit activePhoneContactChanged(m_data->m_activePhoneContact);
    }
}

RisipBuddiesModel *RisipContactManager::activeBuddiesModel() const
{
    return m_data->m_activeBuddiesModel;
}

void RisipContactManager::setActiveBuddiesModel(RisipBuddiesModel *model)
{
    if(m_data->m_activeBuddiesModel != model) {
        m_data->m_activeBuddiesModel = model;
        emit activeBuddiesModelChanged(m_data->m_activeBuddiesModel);
    }
}

/**
 * @brief RisipContactManager::activeContactHistory
 * @return contact history model
 *
 * This model has the contact history for the active account.
 *
 * @see RisipContactManger::activeAccount
 */
RisipContactHistoryModel *RisipContactManager::activeContactHistory() const
{
    return m_data->m_activeContactHistoryModel;
}

void RisipContactManager::setActiveContactHistory(RisipContactHistoryModel *history)
{
    if(m_data->m_activeContactHistoryModel != history) {
        m_data->m_activeContactHistoryModel = history;
        emit activeContactHistoryChanged(m_data->m_activeContactHistoryModel);
    }
}

QQmlListProperty<RisipBuddiesModel> RisipContactManager::buddyModels()
{
    QList<RisipBuddiesModel *> models = m_data->m_accountBuddyModels.values();
    return QQmlListProperty<RisipBuddiesModel>(this, models);
}

QQmlListProperty<RisipContactHistoryModel> RisipContactManager::contactHistoryModels()
{
    QList<RisipContactHistoryModel *> models = m_data->m_accountContactHistoryModels.values();
    return QQmlListProperty<RisipContactHistoryModel>(this, models);
}

QQmlListProperty<RisipPhoneContact> RisipContactManager::phoneContacts()
{
    QList<RisipPhoneContact *> contacts = m_data->m_phoneContacts.values();
    return QQmlListProperty<RisipPhoneContact> (this, contacts);
}

QList<RisipPhoneContact *> RisipContactManager::phoneContactList() const
{
    return m_data->m_phoneContacts.values();
}

/**
 * @brief RisipContactManager::phoneContactsModel
 * @return the phone contacts model
 *
 * The model containing the phone contacts. Initially the model is empty and it is only populated
 * after calling RisipContactManger::fetchPhoneContacts()
 *
 * @see RisipContactManager::fetchContacts
 */
RisipPhoneContactsModel *RisipContactManager::phoneContactsModel() const
{
    return m_data->m_phoneContactsModel;
}

/**
 * @brief RisipContactManager::createModelsForAccount
 * @param account
 *
 * Internal API.
 *
 * This function is used by Risip when new accounts are created.
 * @see Risip::createAccount
 */
void RisipContactManager::createModelsForAccount(RisipAccount *account)
{
    if(!account)
        return;

    if(!m_data->m_accountBuddyModels.contains(account->configuration()->uri())) {
        RisipBuddiesModel *buddiesModel = new RisipBuddiesModel(this);
        m_data->m_accountBuddyModels[account->configuration()->uri()] = buddiesModel;
        buddiesModel->setAccount(account);
    }

    if(!m_data->m_accountContactHistoryModels.contains(account->configuration()->uri())) {
        RisipContactHistoryModel *contactHistoryModel = new RisipContactHistoryModel(this);
        m_data->m_accountContactHistoryModels[account->configuration()->uri()] = contactHistoryModel;
        contactHistoryModel->setAccount(account);
    }
}

/**
 * @brief RisipContactManager::removeModelsForAccount
 * @param account
 *
 * Internal API.
 *
 * This function is used by Risip when an account is removed.
 * @see Risip::removeAccount
 */
void RisipContactManager::removeModelsForAccount(const RisipAccount *account)
{
    if(!account)
        return;

    if(m_data->m_accountBuddyModels.contains(account->configuration()->uri())) {
        delete m_data->m_accountBuddyModels.take(account->configuration()->uri());
    }
    if(m_data->m_accountContactHistoryModels.contains(account->configuration()->uri()))
        delete m_data->m_accountContactHistoryModels.take(account->configuration()->uri());
}

RisipBuddiesModel *RisipContactManager::buddyModelForAccount(const QString &account) const
{
    if(!account.isEmpty() && m_data->m_accountBuddyModels.contains(account))
        return qobject_cast<RisipBuddiesModel *>(m_data->m_accountBuddyModels[account]);

    return NULL;
}

RisipContactHistoryModel *RisipContactManager::contactHistoryModelForAccount(const QString &account) const
{
    if(!account.isEmpty() && m_data->m_accountContactHistoryModels.contains(account))
        return qobject_cast<RisipContactHistoryModel *>(m_data->m_accountContactHistoryModels[account]);

    return NULL;
}

/**
 * @brief RisipContactManager::fetchPhoneContacts
 *
 * Call this function to retrieve the phone contacts from the mobile device.
 * It will retrieve all contacts and populate a contact phone model that can be used
 * from the phoneContactsModel property.
 *
 * This function is called upon creation of RisipContactManager .
 * Can be called anywhere in the app.
 * @see RisipContactManger::phoneContactsModel
 */
void RisipContactManager::fetchPhoneContacts()
{
#ifdef Q_OS_IOS
    if(!m_data->m_iosContacts) {
        m_data->m_iosContacts = new RisipiOSContactAcessManager(this);

        connect(m_data->m_iosContacts, &RisipiOSContactAcessManager::phoneContactDiscovered,
                this, &RisipContactManager::phoneContactDiscovered, Qt::QueuedConnection);

        m_data->m_iosContacts->fetchContactsFromDevice();
    }
#endif

#ifdef Q_OS_ANDROID
    if(!m_data->androidContactAccessManager) {
        m_data->androidContactAccessManager = new RisipAndroidContactAccessManager(this);

        connect(m_data->androidContactAccessManager, &RisipAndroidContactAccessManager::phoneContactDiscovered,
                this, &RisipContactManager::phoneContactDiscovered, Qt::QueuedConnection);

        m_data->androidContactAccessManager->fetchContactsFromDevice();
    }
#endif

}

RisipPhoneContact *RisipContactManager::contactForName(const QString &name)
{
    if(!name.isEmpty()
            && m_data->m_phoneContacts.contains(name)) {
        return m_data->m_phoneContacts[name];
    }

    return NULL;
}

RisipPhoneContact *RisipContactManager::contactForIndex(int index)
{
    if(m_data->m_phoneContactsModel)
        return m_data->m_phoneContactsModel->contactForIndex(index);

    return NULL;
}

RisipPhoneNumber *RisipContactManager::phoneNumberForNumber(const QString &number)
{
    if(!number.isEmpty() && m_data->m_phoneNumbers.contains(number))
        return m_data->m_phoneNumbers[number];

    return NULL;
}

void RisipContactManager::phoneContactDiscovered(RisipPhoneContact *newContact)
{
    if(newContact) {
        QString contactFullname = newContact->fullName().trimmed();
        if(!contactFullname.isEmpty()
                && !m_data->m_phoneContacts.contains(contactFullname)) {

            RisipPhoneContact *newPhoneContact = new RisipPhoneContact(this);
            newPhoneContact->setFullName(contactFullname);
            newPhoneContact->setContactId(newContact->contactId());
            newPhoneContact->setContactImageData(newContact->contactImageData());
            newPhoneContact->setEmail(newContact->email());

            m_data->m_phoneContacts[contactFullname] = newPhoneContact;
            m_data->m_phoneContactsModel->addContact(newPhoneContact);

            QList<RisipPhoneNumber *> numbers = newContact->phoneNumberList();
            for(int i=0; i<numbers.count(); ++i) {
                numbers[i]->setParent(newPhoneContact);
                newPhoneContact->addPhoneNumber(numbers[i]);
                m_data->m_phoneNumbers.insert(numbers[i]->fullNumber(), numbers[i]);
            }
        }
    }

    newContact->deleteLater();
}

} //end of risip namespace
