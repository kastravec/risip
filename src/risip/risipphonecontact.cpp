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
#include "risipphonecontact.h"
#include "risipmodels.h"
#include "risipglobals.h"

#include <QDebug>

RisipPhoneNumber::RisipPhoneNumber(const QString &rawNumber, QObject *parent)
    :QObject(parent)
    ,m_phoneContact(NULL)
    ,m_rawNumber()
    ,m_countryPrefix()
    ,m_secondPrefix()
    ,m_number()
    ,m_fullNumber()
    ,m_countryCode()
    ,m_label()
{
    setRawNumber(rawNumber);
}

RisipPhoneNumber::~RisipPhoneNumber()
{
}

RisipPhoneContact *RisipPhoneNumber::phoneContact() const
{
    return m_phoneContact;
}

void RisipPhoneNumber::setPhoneContact(RisipPhoneContact *contact)
{
    if(m_phoneContact != contact) {
        m_phoneContact = contact;

        if(m_phoneContact)
            m_phoneContact->addPhoneNumber(this);

        emit phoneContactChanged(m_phoneContact);
    }
}

QString RisipPhoneNumber::rawNumber() const
{
    return m_rawNumber;
}

/**
 * @brief RisipPhoneNumber::setRawNumber
 * @param number a raw format number
 *
 * Initiating this number with a raw number as input. The raw input will be validated
 * and parsed. Check for validity of this number by using the @see RisipPhoneNumber::valid property.
 */
void RisipPhoneNumber::setRawNumber(const QString &number)
{
    if(m_rawNumber != number) {
        m_rawNumber = number;
        emit rawNumberChanged(m_rawNumber);
        RisipGlobals::validateNumber(this);
    }
}

/**
 * @brief RisipPhoneNumber::label
 * @return label for this phone number
 *
 * A label represents a tag, a category if you like, something like e.g. "home" number , or "mobile" , etc
 */
QString RisipPhoneNumber::label() const
{
    return m_label;
}

void RisipPhoneNumber::setLabel(const QString &label)
{
    if(m_label != label) {
        m_label = label;
        emit labelChanged(m_label);
    }
}

QStringList RisipPhoneNumber::fullNumberParts() const
{
    return m_fullNumberParts;
}

void RisipPhoneNumber::setNumberParts(const QStringList &parts)
{
    m_fullNumberParts = parts;
    emit fullNumberPartsChanged(m_fullNumberParts);
}

QString RisipPhoneNumber::countryPrefix() const
{
    return m_countryPrefix;
}

void RisipPhoneNumber::setCountryPrefix(const QString &prefix)
{
    if(m_countryPrefix != prefix) {
        m_countryPrefix = prefix;
        emit countryPrefixChanged(m_countryPrefix);
    }
}

QString RisipPhoneNumber::countryName() const
{
    return m_countryName;
}

void RisipPhoneNumber::setCountryName(const QString &name)
{
    if(m_countryName != name) {
        m_countryName = name;
        emit countryNameChanged(m_countryName);
    }
}

QString RisipPhoneNumber::secondPrefix() const
{
    return m_secondPrefix;
}

void RisipPhoneNumber::setSecondPrefix(const QString &prefix)
{
    if(m_secondPrefix != prefix) {
        m_secondPrefix = prefix;
        emit secondPrefixChanged(m_secondPrefix);
    }
}

QString RisipPhoneNumber::number() const
{
    return m_number;
}

void RisipPhoneNumber::setNumber(const QString &number)
{
    if(m_number != number) {
        m_number = number;
        emit numberChanged(m_number);
    }
}

QString RisipPhoneNumber::fullNumber() const
{
    return m_fullNumber;
}

void RisipPhoneNumber::setFullNumber(const QString &number)
{
    if(m_fullNumber != number) {
        m_fullNumber = number;
        emit fullNumberChanged(m_fullNumber);
    }
}

QString RisipPhoneNumber::countryCode() const
{
    return m_countryCode;
}

void RisipPhoneNumber::setCountryCode(const QString &code)
{
    if(m_countryCode != code) {
        m_countryCode = code;
        emit countryCodeChanged(m_countryCode);
    }
}

/**
 * @brief RisipPhoneNumber::valid
 * @return
 *
 * A valid phone number is ready to be called.
 * The country code must not be NULL for a phone number to be valid.
 * Use @see RisipPhoneNumber::validate to validate the phone number or
 * simply call @see RisipPhoneNumber::setRawNumber
 */
bool RisipPhoneNumber::valid() const
{
    return !m_countryCode.isNull();
}

/**
 * @brief RisipPhoneNumber::reset
 *
 * It wipes out all the number details. It simply leaves an empty shell object.
 * Use @see RisipPhoneNumber::setRawNumber to initiate it with a number.
 */
void RisipPhoneNumber::reset()
{
    m_countryCode.clear();
    m_countryPrefix.clear();
    m_secondPrefix.clear();
    m_label.clear();
    m_number.clear();
    m_rawNumber.clear();

    emit countryCodeChanged(m_countryCode);
    emit countryPrefixChanged(m_countryPrefix);
    emit secondPrefixChanged(m_secondPrefix);
    emit labelChanged(m_label);
    emit numberChanged(m_number);
    emit rawNumberChanged(m_rawNumber);
}


RisipPhoneContact::RisipPhoneContact(QObject *parent)
    :QObject(parent)
    ,m_id(-1)
    ,m_fullName()
    ,m_email()
    ,m_phoneNumbers()
    ,m_phoneNumbersModel(new RisipPhoneNumbersModel(this))
{
    m_phoneNumbersModel->setPhoneContact(this);
}

RisipPhoneContact::~RisipPhoneContact()
{
}

int RisipPhoneContact::contactId() const
{
    return m_id;
}

void RisipPhoneContact::setContactId(int id)
{
    if(m_id != id) {
        m_id = id;
        emit contactIdChanged(m_id);
    }
}

QString RisipPhoneContact::fullName() const
{
    return m_fullName;
}

void RisipPhoneContact::setFullName(QString name)
{
    if(m_fullName != name) {
        m_fullName = name;
        emit fullNameChanged(m_fullName);
    }
}

QString RisipPhoneContact::email() const
{
    return m_email;
}

void RisipPhoneContact::setEmail(const QString &email)
{
    if(m_email != email) {
        m_email = email;
        emit emailChanged(email);
    }
}

RisipPhoneNumbersModel *RisipPhoneContact::phoneNumbersModel() const
{
    return m_phoneNumbersModel;
}

QByteArray RisipPhoneContact::contactImageData() const
{
    return m_contactImageData;
}

void RisipPhoneContact::setContactImageData(const QByteArray &imageData)
{
    if(m_contactImageData != imageData) {
        m_contactImageData = imageData;
        emit contactImageDataChanged(m_contactImageData);
    }
}

QQmlListProperty<RisipPhoneNumber> RisipPhoneContact::phoneNumbers()
{
    QList<RisipPhoneNumber *> numbers = m_phoneNumbers.values();
    return QQmlListProperty<RisipPhoneNumber>(this, numbers);
}

QList<RisipPhoneNumber *> RisipPhoneContact::phoneNumberList() const
{
    return m_phoneNumbers.values();
}

void RisipPhoneContact::addPhoneNumber(const QString &number, const QString &label)
{
    RisipPhoneNumber *phoneNumber = new RisipPhoneNumber(number, this);
    phoneNumber->setLabel(label);
    if(!m_phoneNumbers.contains(phoneNumber->fullNumber()))
        m_phoneNumbers[phoneNumber->fullNumber()] = phoneNumber;
}

void RisipPhoneContact::addPhoneNumber(RisipPhoneNumber *number)
{
    if(number) {
        if(!m_phoneNumbers.contains(number->fullNumber()))
            m_phoneNumbers[number->fullNumber()] = number;
    }
}

void RisipPhoneContact::removePhoneNumber(RisipPhoneNumber *number)
{
    if(number) {
        if(m_phoneNumbers.contains(number->fullNumber()))
            m_phoneNumbers.take(number->fullNumber())->deleteLater();
    }
}

