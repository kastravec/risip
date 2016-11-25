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

#include "risipmodels.h"
#include "risip.h"
#include "risipaccount.h"
#include "risipbuddy.h"
#include "risipcall.h"
#include "risipphonecontact.h"
#include "risipphonenumber.h"
#include "risipglobals.h"

#include <QSortFilterProxyModel>
#include <QDebug>

RisipAbstractBuddyModel::RisipAbstractBuddyModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_account(Risip::instance()->defaultAccount())
{
}

RisipAbstractBuddyModel::~RisipAbstractBuddyModel()
{
}

RisipAccount *RisipAbstractBuddyModel::account() const
{
    return m_account;
}

void RisipAbstractBuddyModel::setAccount(RisipAccount *account)
{
    if(m_account != account) {
        m_account = account;
        emit accountChanged(m_account);
    }
}

QHash<int, QByteArray> RisipAbstractBuddyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UriRole] = "uri";
    roles[ContactRole] = "contact";
    roles[TypeRole] = "type";

    return roles;
}

int RisipAbstractBuddyModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_buddies.count();
}

QVariant RisipAbstractBuddyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    RisipBuddy *buddy = m_buddies.at(index.row());
    if(!buddy) {
        qDebug()<<"no buddy found!";
        return QVariant();
    }

    switch (role) {
    case UriRole:
        return buddy->uri();
        break;
    case ContactRole:
        return buddy->contact();
        break;
    case TypeRole:
        return buddy->type();
        break;
    default:
        return QVariant();
    }

    return QVariant();
}

void RisipAbstractBuddyModel::addBuddy(RisipBuddy *buddy)
{
    if(buddy) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_buddies.append(buddy);
        endInsertRows();
    }
}

void RisipAbstractBuddyModel::removeBuddy(RisipBuddy *buddy)
{
    if(buddy) {
        beginRemoveRows(QModelIndex(), m_buddies.indexOf(buddy), 1);
        m_buddies.removeAll(buddy);
        buddy->releaseFromAccount();
        endRemoveRows();
    }
}

RisipBuddiesModel::RisipBuddiesModel(QObject *parent)
    :RisipAbstractBuddyModel(parent)
{}

RisipBuddiesModel::~RisipBuddiesModel()
{}

RisipContactHistoryModel::RisipContactHistoryModel(QObject *parent)
    :RisipAbstractBuddyModel(parent)
{}

RisipContactHistoryModel::~RisipContactHistoryModel()
{}

RisipCallHistoryModel::RisipCallHistoryModel(QObject *parent)
    : QAbstractListModel(parent)
    ,m_account(Risip::instance()->defaultAccount())
{
}

RisipCallHistoryModel::~RisipCallHistoryModel()
{
}

RisipAccount *RisipCallHistoryModel::account()
{
    return m_account;
}

void RisipCallHistoryModel::setAccount(RisipAccount *account)
{
    if(m_account != account) {
        m_account = account;
        emit accountChanged(m_account);
    }
}

QHash<int, QByteArray> RisipCallHistoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CallContactRole] = "callContact";
    roles[CallDirectionRole] = "callDirection";
    roles[CallDurationRole] = "callDuration";
    roles[CallTimestampRole] = "callTimestamp";
    return roles;
}

int RisipCallHistoryModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_calls.count();
}

QVariant RisipCallHistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    RisipCall *call = m_calls.at(index.row());
    if(!call) {
        qDebug()<<"No call data found in the history!";
        return QVariant();
    }

    switch (role) {
    case CallDirectionRole:
        return call->callDirection();
    case CallDurationRole:
        return call->callDuration();
    case CallTimestampRole:
        return call->timestamp();
    case CallContactRole:
        return call->buddy()->contact();
    }

    return QVariant();
}

void RisipCallHistoryModel::addCallRecord(RisipCall *call)
{
    if(call) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_calls.append(call);
        endInsertRows();
    }
}

void RisipCallHistoryModel::removeCallRecord(RisipCall *call)
{
    if(call) {
        beginRemoveRows(QModelIndex(), m_calls.indexOf(call), 1);
        m_calls.removeAll(call);
        endRemoveRows();
    }
}

RisipPhoneContactsModel::RisipPhoneContactsModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_phoneContacts()
{
}

RisipPhoneContactsModel::~RisipPhoneContactsModel()
{
}

QHash<int, QByteArray> RisipPhoneContactsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ContactId] = "contactId";
    roles[FullName] = "fullName";
    roles[Initials] = "initials";
    roles[PhoneNumberList] = "phoneNumberList";
    return roles;
}

int RisipPhoneContactsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_phoneContacts.count();
}

QVariant RisipPhoneContactsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()
            || index.row() >= m_phoneContacts.count())
        return QVariant();

    RisipPhoneContact *contact = m_phoneContacts.at(index.row());
    if(!contact) {
        qDebug()<<"No contact found in model..!";
        return QVariant();
    }

    switch (role) {
    case ContactId:
        return contact->contactId();
    case FullName:
        if(contact)
            return contact->fullName();
        return QVariant();
    case Initials: {
        QStringList firstLast = contact->fullName().split(" ");
        if(firstLast.count() == 0)
            return QString(".");
        if(firstLast.count() == 1)
            return firstLast.at(0).left(1).toUpper();
        return firstLast.at(0).left(1) + QString(".") + firstLast.at(1).left(1);
    }
    case PhoneNumberList:
        return QVariant::fromValue(contact->phoneNumberList());
    }

    return QVariant();
}

RisipPhoneContact *RisipPhoneContactsModel::contactForIndex(int index)
{
    if(m_phoneContacts.count() >= index >= 0)
        return m_phoneContacts[index];

    return NULL;
}

void RisipPhoneContactsModel::addContact(RisipPhoneContact *contact)
{
    if(!contact)
        return;

    if(!m_phoneContacts.contains(contact)) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_phoneContacts.append(contact);
        endInsertRows();
    }
}

void RisipPhoneContactsModel::removeContact(RisipPhoneContact *contact)
{
    if(!contact && !m_phoneContacts.contains(contact))
        return;

    beginRemoveRows(QModelIndex(), m_phoneContacts.indexOf(contact), 1);
    m_phoneContacts.removeAll(contact);
    contact->deleteLater();
    endRemoveRows();
}

RisipPhoneNumbersModel::RisipPhoneNumbersModel(QObject *parent)
    :QAbstractListModel(parent)
    ,m_phoneContact(NULL)
{
}

RisipPhoneNumbersModel::~RisipPhoneNumbersModel()
{
}

RisipPhoneContact *RisipPhoneNumbersModel::phoneContact() const
{
    return m_phoneContact;
}

void RisipPhoneNumbersModel::setPhoneContact(RisipPhoneContact *contact)
{
    if(m_phoneContact != contact) {
//        beginResetModel();
        m_phoneContact = contact;
//        endResetModel();
        emit phoneContactChanged(m_phoneContact);

    }
}

QHash<int, QByteArray> RisipPhoneNumbersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FullNumber] = "fullNumber";
    roles[CountryPrefix] = "countryPrefix";
    roles[CountryCode] = "countryCode";
    roles[RegionPrefix] = "regionPrefix";
    roles[Number] = "number";
    roles[RawNumber] = "rawNumber";
    return roles;
}

int RisipPhoneNumbersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_phoneContact)
        return 0;

    return m_phoneContact->phoneNumberList().count();
}

QVariant RisipPhoneNumbersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_phoneContact)
        return QVariant();

    switch (role) {
    case FullNumber:
        return m_phoneContact->phoneNumberList()[index.row()]->fullNumber();
    case CountryPrefix:
        return m_phoneContact->phoneNumberList()[index.row()]->countryPrefix();
    case CountryCode:
        return m_phoneContact->phoneNumberList()[index.row()]->countryCode();
    case RegionPrefix:
        return QVariant();
    case Number:
        return m_phoneContact->phoneNumberList()[index.row()]->number();
    case RawNumber:
        return m_phoneContact->phoneNumberList()[index.row()]->rawNumber();
    default:
        return QVariant();
    }

    return QVariant();
}

RisipCountryRatesModel::RisipCountryRatesModel(QObject *parent)
    :QAbstractListModel(parent)
{
}

RisipCountryRatesModel::~RisipCountryRatesModel()
{
}

QHash<int, QByteArray> RisipCountryRatesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CountryName] = "countryName";
    roles[CountryCode] = "countryCode";
    roles[CountryRate] = "countryRate";
    roles[CountryPrefix] = "countryPrefix";
    roles[ValidFromDate] = "validFromDate";
    roles[ValidTillDate] = "validTillDate";
    return roles;
}

int RisipCountryRatesModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_allCountries.count();
}

QVariant RisipCountryRatesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()
            || index.row() > m_allCountries.count())
        return QVariant();

    Country country = m_allCountries[index.row()];
    switch (role) {
    case CountryName:
        return country.name;
    case CountryCode:
        return country.code.toLower();
    case CountryPrefix:
        return country.prefix;
    case CountryRate:
        return country.rate.actualRate + country.rate.timeMeasure;
    case ValidFromDate:
        return country.rate.validFromDate;
    case ValidTillDate:
        return country.rate.validTillDate;
    default:
        return QVariant();
    }

    return QVariant();
}
