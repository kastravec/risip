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

#include "risipphonenumber.h"

#include "risipphonecontact.h"
#include "risipglobals.h"

namespace risip {

class RisipPhoneNumber::Private
{
public:
    RisipPhoneContact *phoneContact;
    QString rawNumber;
    QString countryPrefix;
    QString secondPrefix;
    QString number;
    QString fullNumber;
    QString countryCode;
    QString countryName;
    QString label;
    QStringList fullNumberParts;
};

RisipPhoneNumber::RisipPhoneNumber(const QString &rawNumber, QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->phoneContact = NULL;
    setRawNumber(rawNumber);
}

RisipPhoneNumber::~RisipPhoneNumber()
{
    delete m_data;
    m_data = NULL;
}

RisipPhoneContact *RisipPhoneNumber::phoneContact() const
{
    return m_data->phoneContact;
}

void RisipPhoneNumber::setPhoneContact(RisipPhoneContact *contact)
{
    if(m_data->phoneContact != contact) {
        m_data->phoneContact = contact;

        if(m_data->phoneContact)
            m_data->phoneContact->addPhoneNumber(this);

        emit phoneContactChanged(m_data->phoneContact);
    }
}

QString RisipPhoneNumber::rawNumber() const
{
    return m_data->rawNumber;
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
    if(m_data->rawNumber != number) {
        m_data->rawNumber = number;
        emit rawNumberChanged(m_data->rawNumber);
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
    return m_data->label;
}

void RisipPhoneNumber::setLabel(const QString &label)
{
    if(m_data->label != label) {
        m_data->label = label;
        emit labelChanged(m_data->label);
    }
}

QStringList RisipPhoneNumber::fullNumberParts() const
{
    return m_data->fullNumberParts;
}

void RisipPhoneNumber::setNumberParts(const QStringList &parts)
{
    m_data->fullNumberParts = parts;
    emit fullNumberPartsChanged(m_data->fullNumberParts);
}

QString RisipPhoneNumber::countryPrefix() const
{
    return m_data->countryPrefix;
}

void RisipPhoneNumber::setCountryPrefix(const QString &prefix)
{
    if(m_data->countryPrefix != prefix) {
        m_data->countryPrefix = prefix;
        emit countryPrefixChanged(m_data->countryPrefix);
    }
}

QString RisipPhoneNumber::countryName() const
{
    return m_data->countryName;
}

void RisipPhoneNumber::setCountryName(const QString &name)
{
    if(m_data->countryName != name) {
        m_data->countryName = name;
        emit countryNameChanged(m_data->countryName);
    }
}

QString RisipPhoneNumber::secondPrefix() const
{
    return m_data->secondPrefix;
}

void RisipPhoneNumber::setSecondPrefix(const QString &prefix)
{
    if(m_data->secondPrefix != prefix) {
        m_data->secondPrefix = prefix;
        emit secondPrefixChanged(m_data->secondPrefix);
    }
}

QString RisipPhoneNumber::number() const
{
    return m_data->number;
}

void RisipPhoneNumber::setNumber(const QString &number)
{
    if(m_data->number != number) {
        m_data->number = number;
        emit numberChanged(m_data->number);
    }
}

QString RisipPhoneNumber::fullNumber() const
{
    return m_data->fullNumber;
}

void RisipPhoneNumber::setFullNumber(const QString &number)
{
    if(m_data->fullNumber != number) {
        m_data->fullNumber = number;
        emit fullNumberChanged(m_data->fullNumber);
    }
}

QString RisipPhoneNumber::countryCode() const
{
    return m_data->countryCode;
}

void RisipPhoneNumber::setCountryCode(const QString &code)
{
    if(m_data->countryCode != code) {
        m_data->countryCode = code;
        emit countryCodeChanged(m_data->countryCode);
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
    return !m_data->countryCode.isNull();
}

/**
 * @brief RisipPhoneNumber::reset
 *
 * It wipes out all the number details. It simply leaves an empty shell object.
 * Use @see RisipPhoneNumber::setRawNumber to initiate it with a number.
 */
void RisipPhoneNumber::reset()
{
    m_data->countryCode.clear();
    m_data->countryPrefix.clear();
    m_data->secondPrefix.clear();
    m_data->label.clear();
    m_data->number.clear();
    m_data->rawNumber.clear();

    emit countryCodeChanged(m_data->countryCode);
    emit countryPrefixChanged(m_data->countryPrefix);
    emit secondPrefixChanged(m_data->secondPrefix);
    emit labelChanged(m_data->label);
    emit numberChanged(m_data->number);
    emit rawNumberChanged(m_data->rawNumber);
}

} //end of risip namespace
