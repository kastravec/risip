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

#include "risipuserprofile.h"

class RisipUserProfile::Private
{
public:
    int userId = -1;
    QString userName;
    QString password;
    QString balance;
    QString email;
    QString firstName;
    QString lastName;
    Country country;
    QString city;
    QString address;
    QString postcode;
    QString vatNumber;
    int currency;
    QString phoneNumber;
    QString mobileNumber;
    int deviceType;
    bool valid = false;
    int tariffId;
    int lcrId;
};

RisipUserProfile::RisipUserProfile(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->firstName = QString("Unknown");
    m_data->lastName = QString("Unknown");
}

RisipUserProfile::~RisipUserProfile()
{
    delete m_data;
    m_data = NULL;
}

int RisipUserProfile::userId() const
{
    return m_data->userId;
}

bool RisipUserProfile::valid() const
{
    if(m_data->userName.isEmpty()
            && m_data->firstName.isEmpty()
            && m_data->lastName.isEmpty()
            && m_data->password.isEmpty()
            && m_data->country.code.isEmpty()
            && m_data->email.isEmpty())
        return false;

    return false;
}

QString RisipUserProfile::username() const
{
    return m_data->userName;
}

void RisipUserProfile::setUsername(const QString &username)
{
    if(m_data->userName != username) {
        m_data->userName = username;
        emit usernameChanged(username);
    }
}

QString RisipUserProfile::password() const
{
    return m_data->password;
}

void RisipUserProfile::setPassword(const QString &password)
{
    if(m_data->password != password) {
        m_data->password = password;
        emit passwordChanged(password);
    }
}

QString RisipUserProfile::creditBalance() const
{
    return m_data->balance;
}

QString RisipUserProfile::email() const
{
    return m_data->email;
}

void RisipUserProfile::setEmail(const QString &email)
{
    if(m_data->email != email) {
        m_data->email = email;
        emit emailChanged(email);
    }
}

QString RisipUserProfile::firstName() const
{
    return m_data->firstName;
}

void RisipUserProfile::setFirstName(const QString &firstname)
{
    if(m_data->firstName != firstname) {
        m_data->firstName = firstname;
        emit firstNameChanged(firstname);
    }
}

QString RisipUserProfile::lastName() const
{
    return m_data->lastName;
}

void RisipUserProfile::setLastName(const QString &lastname)
{
    if(m_data->lastName != lastname) {
        m_data->lastName = lastname;
        emit lastNameChanged(lastname);
    }
}

QString RisipUserProfile::countryCode() const
{
    return m_data->country.name;
}

void RisipUserProfile::setCountryCode(const QString &code)
{
    if(m_data->country.code != code) {

        m_data->country.code = code;
        emit countryCodeChanged(m_data->country.code);
    }
}

QString RisipUserProfile::countryPrefix() const
{
    return m_data->country.prefix;
}

void RisipUserProfile::setCountryPrefix(const QString &prefix)
{
    if(prefix.isEmpty())
        return;

    QString tmpPrefix(prefix);
    tmpPrefix.remove("+");
    if(m_data->country.prefix != tmpPrefix) {
        m_data->country = RisipGlobals::countryForPrefix(tmpPrefix);
        emit countryPrefixChanged(tmpPrefix);
    }
}

QString RisipUserProfile::city() const
{
    return m_data->city;
}

void RisipUserProfile::setCity(const QString &city)
{
    if(m_data->city != city) {
        m_data->city = city;
        emit cityChanged(city);
    }
}

QString RisipUserProfile::address() const
{
    return m_data->address;
}

void RisipUserProfile::setAddress(const QString &address)
{
    if(m_data->address != address) {
        m_data->address = address;
        emit addressChanged(address);
    }
}

QString RisipUserProfile::postCode() const
{
    return m_data->postcode;
}

void RisipUserProfile::setPostCode(const QString &code)
{
    if(m_data->postcode != code) {
        m_data->postcode = code;
        emit postCodeChanged(code);
    }
}

QString RisipUserProfile::vatNumber() const
{
    return m_data->vatNumber;
}

void RisipUserProfile::setVatNumber(const QString &number)
{
    if(m_data->vatNumber != number) {
        m_data->vatNumber = number;
        emit vatNumberChanged(number);
    }
}

int RisipUserProfile::currency() const
{
    return m_data->currency;
}

void RisipUserProfile::setCurrency(int currency)
{
    if(m_data->currency != currency) {
        m_data->currency = currency;
        emit currencyChanged(currency);
    }
}

QString RisipUserProfile::phoneNumber() const
{
    return m_data->phoneNumber;
}

void RisipUserProfile::setPhoneNumber(const QString &phone)
{
    if(m_data->phoneNumber != phone) {
        m_data->phoneNumber = phone;
        emit phoneNumberChanged(phone);
    }
}

QString RisipUserProfile::mobileNumber() const
{
    return m_data->mobileNumber;
}

void RisipUserProfile::setMobileNumber(const QString &mobile)
{
    if(m_data->mobileNumber != mobile) {
        m_data->mobileNumber = mobile;
        emit mobileNumberChanged(mobile);
    }
}

int RisipUserProfile::deviceType() const
{
    return m_data->deviceType;
}

void RisipUserProfile::setDeviceType(int type)
{
    if(m_data->deviceType != type) {
        m_data->deviceType = type;
        emit deviceTypeChanged(type);
    }
}

void RisipUserProfile::reset()
{
    delete m_data;
    m_data = NULL;

    m_data = new Private;
}

Country RisipUserProfile::country() const
{
    return m_data->country;
}

void RisipUserProfile::setCountry(Country country)
{
    if(m_data->country != country) {
        m_data->country = country;
    }
}
