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
#include "risipmodels.h"
#include "ios/risipioscontactaccessmanager.h"
#include "risipphonecontact.h"

#include <QSortFilterProxyModel>
#include <QDebug>

RisipContactManager *RisipContactManager::m_instance = NULL;
RisipContactManager *RisipContactManager::instance()
{
    if(!m_instance)
        m_instance = new RisipContactManager;

    return m_instance;
}

RisipContactManager::RisipContactManager(QObject *parent)
    :QObject(parent)
    ,m_activeAccount(Risip::instance()->defaultAccount())
    ,m_accountBuddyModels()
    ,m_accountContactHistoryModels()
    ,m_activeBuddiesModel(NULL)
    ,m_activeContactHistoryModel(NULL)
    ,m_phoneContactsModel(new RisipPhoneContactsModel(this))
    ,m_activePhoneContact(NULL)
{
    fetchPhoneContacts();
}

RisipContactManager::~RisipContactManager()
{
}

/**
 * @brief RisipContactManager::activeAccount
 * @return the current active SIP account
 *
 * This property holds the current active account.
 */
RisipAccount *RisipContactManager::activeAccount() const
{
    return m_activeAccount;
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
    if(m_activeAccount != account) {
        m_activeAccount = account;
        if(!m_activeAccount)
            m_activeAccount = Risip::instance()->defaultAccount();

        if(m_activeAccount) {
            if(buddyModelForAccount(m_activeAccount->configuration()->uri())) {
                setActiveBuddiesModel(buddyModelForAccount(m_activeAccount->configuration()->uri()));
            } else {
                //TODO create model for this new active account
            }

            if(contactHistoryModelForAccount(m_activeAccount->configuration()->uri())) {
                setActiveContactHistory(contactHistoryModelForAccount(m_activeAccount->configuration()->uri()));
            } else {
                //TODO create model for this new active account
            }
        }

        emit activeAccountChanged(m_activeAccount);
    }
}

RisipPhoneContact *RisipContactManager::activePhoneContact() const
{
    return m_activePhoneContact;
}

void RisipContactManager::setActivePhoneContact(const QString &contactName)
{
    if(!contactName.isEmpty() && m_phoneContacts.contains(contactName))
        setActivePhoneContact(m_phoneContacts[contactName]);
}

void RisipContactManager::setActivePhoneContact(RisipPhoneContact *phoneContact)
{
    if(m_activePhoneContact != phoneContact) {
        m_activePhoneContact = phoneContact;
        emit activePhoneContactChanged(m_activePhoneContact);
    }
}

RisipBuddiesModel *RisipContactManager::activeBuddiesModel() const
{
    return m_activeBuddiesModel;
}

void RisipContactManager::setActiveBuddiesModel(RisipBuddiesModel *model)
{
    if(m_activeBuddiesModel != model) {
        m_activeBuddiesModel = model;
        emit activeBuddiesModelChanged(m_activeBuddiesModel);
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
        return m_activeContactHistoryModel;
}

void RisipContactManager::setActiveContactHistory(RisipContactHistoryModel *history)
{
    if(m_activeContactHistoryModel != history) {
        m_activeContactHistoryModel = history;
        emit activeContactHistoryChanged(m_activeContactHistoryModel);
    }
}

QQmlListProperty<RisipBuddiesModel> RisipContactManager::buddyModels()
{
    QList<RisipBuddiesModel *> models = m_accountBuddyModels.values();
    return QQmlListProperty<RisipBuddiesModel>(this, models);
}

QQmlListProperty<RisipContactHistoryModel> RisipContactManager::contactHistoryModels()
{
    QList<RisipContactHistoryModel *> models = m_accountContactHistoryModels.values();
    return QQmlListProperty<RisipContactHistoryModel>(this, models);
}

QQmlListProperty<RisipPhoneContact> RisipContactManager::phoneContacts()
{
    QList<RisipPhoneContact *> contacts = m_phoneContacts.values();
    return QQmlListProperty<RisipPhoneContact> (this, contacts);
}

QList<RisipPhoneContact *> RisipContactManager::phoneContactList() const
{
    return m_phoneContacts.values();
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
     return m_phoneContactsModel;
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

    if(!m_accountBuddyModels.contains(account->configuration()->uri())) {
        RisipBuddiesModel *buddiesModel = new RisipBuddiesModel(this);
        m_accountBuddyModels[account->configuration()->uri()] = buddiesModel;
        buddiesModel->setAccount(account);
    }

    if(!m_accountContactHistoryModels.contains(account->configuration()->uri())) {
        RisipContactHistoryModel *contactHistoryModel = new RisipContactHistoryModel(this);
        m_accountContactHistoryModels[account->configuration()->uri()] = contactHistoryModel;
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

    if(m_accountBuddyModels.contains(account->configuration()->uri())) {
        delete m_accountBuddyModels.take(account->configuration()->uri());
    }
    if(m_accountContactHistoryModels.contains(account->configuration()->uri()))
        delete m_accountContactHistoryModels.take(account->configuration()->uri());
}

RisipBuddiesModel *RisipContactManager::buddyModelForAccount(const QString &account) const
{
    if(!account.isEmpty() && m_accountBuddyModels.contains(account))
        return qobject_cast<RisipBuddiesModel *>(m_accountBuddyModels[account]);

    return NULL;
}

RisipContactHistoryModel *RisipContactManager::contactHistoryModelForAccount(const QString &account) const
{
    if(!account.isEmpty() && m_accountContactHistoryModels.contains(account))
        return qobject_cast<RisipContactHistoryModel *>(m_accountContactHistoryModels[account]);

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
    if(!m_iosContacts) {
        m_iosContacts = new RisipiOSContactAcessManager(this);

        connect(m_iosContacts, &RisipiOSContactAcessManager::phoneContactDiscovered,
                this, &RisipContactManager::phoneContactDiscovered, Qt::QueuedConnection);

        m_iosContacts->fetchContactsFromDevice();
    }
#endif
}

RisipPhoneContact *RisipContactManager::contactForName(const QString &name)
{
    if(!name.isEmpty()
            && m_phoneContacts.contains(name)) {
        return m_phoneContacts[name];
    }

    return NULL;
}

RisipPhoneContact *RisipContactManager::contactForIndex(int index)
{
    if(m_phoneContactsModel)
        return m_phoneContactsModel->contactForIndex(index);

    return NULL;
}

RisipPhoneNumber *RisipContactManager::phoneNumberForNumber(const QString &number)
{
    if(!number.isEmpty() && m_phoneNumbers.contains(number))
        return m_phoneNumbers[number];

    return NULL;
}

void RisipContactManager::phoneContactDiscovered(RisipPhoneContact *contact)
{
    if(contact) {
        if(!contact->fullName().trimmed().isEmpty()) {
            m_phoneContacts.insert(contact->fullName(), contact);
            m_phoneContactsModel->addContact(contact);

            QList<RisipPhoneNumber *> numbers = contact->phoneNumberList();
            for(int i=0; i<numbers.count(); ++i)
                m_phoneNumbers.insert(numbers[i]->fullNumber(), numbers[i]);
        }
    }
}
