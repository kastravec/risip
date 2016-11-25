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

#include "risipaccountprofile.h"

class RisipAccountProfile::Private
{
public:
    int status;
    int userId = -1;
    QString userName;
    QString password;
    QString balance;
    QString email;
    QString firstName;
    QString lastName;
    QString country;
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

RisipAccountProfile::RisipAccountProfile(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{

}

RisipAccountProfile::~RisipAccountProfile()
{
    delete m_data;
    m_data = NULL;
}

int RisipAccountProfile::userId() const
{
    return m_data->userId;
}

bool RisipAccountProfile::valid() const
{
    return true;
}

QString RisipAccountProfile::username() const
{
    return m_data->userName;
}

void RisipAccountProfile::setUsername(const QString &username)
{
    if(m_data->userName != username) {
        m_data->userName = username;
        emit usernameChanged(username);
    }
}

QString RisipAccountProfile::password() const
{
    return m_data->password;
}

void RisipAccountProfile::setPassword(const QString &password)
{
    if(m_data->password != password) {
        m_data->password = password;
        emit passwordChanged(password);
    }
}

QString RisipAccountProfile::creditBalance() const
{
    return m_data->balance;
}

QString RisipAccountProfile::email() const
{
    return m_data->email;
}

void RisipAccountProfile::setEmail(const QString &email)
{
    if(m_data->email != email) {
        m_data->email = email;
        emit emailChanged(email);
    }
}

QString RisipAccountProfile::firstName() const
{
    return m_data->firstName;
}

void RisipAccountProfile::setFirstName(const QString &firstname)
{
    if(m_data->firstName != firstname) {
        m_data->firstName = firstname;
        emit firstNameChanged(firstname);
    }
}

QString RisipAccountProfile::lastName() const
{
    return m_data->lastName;
}

void RisipAccountProfile::setLastName(const QString &lastname)
{
    if(m_data->lastName != lastname) {
        m_data->lastName = lastname;
        emit lastNameChanged(lastname);
    }
}

QString RisipAccountProfile::country() const
{
    return m_data->country;
}

void RisipAccountProfile::setCountry(const QString &country)
{
    if(m_data->country != country) {
        m_data->country = country;
        emit countryChanged(country);
    }
}

QString RisipAccountProfile::city() const
{
    return m_data->city;
}

void RisipAccountProfile::setCity(const QString &city)
{
    if(m_data->city != city) {
        m_data->city = city;
        emit cityChanged(city);
    }
}

QString RisipAccountProfile::address() const
{
    return m_data->address;
}

void RisipAccountProfile::setAddress(const QString &address)
{
    if(m_data->address != address) {
        m_data->address = address;
        emit addressChanged(address);
    }
}

QString RisipAccountProfile::postCode() const
{
    return m_data->postcode;
}

void RisipAccountProfile::setPostCode(const QString &code)
{
    if(m_data->postcode != code) {
        m_data->postcode = code;
        emit postCodeChanged(code);
    }
}

QString RisipAccountProfile::vatNumber() const
{
    return m_data->vatNumber;
}

void RisipAccountProfile::setVatNumber(const QString &number)
{
    if(m_data->vatNumber != number) {
        m_data->vatNumber = number;
        emit vatNumberChanged(number);
    }
}

int RisipAccountProfile::currency() const
{
    return m_data->currency;
}

void RisipAccountProfile::setCurrency(int currency)
{
    if(m_data->currency != currency) {
        m_data->currency = currency;
        emit currencyChanged(currency);
    }
}

QString RisipAccountProfile::phoneNumber() const
{
    return m_data->phoneNumber;
}

void RisipAccountProfile::setPhoneNumber(const QString &phone)
{
    if(m_data->phoneNumber != phone) {
        m_data->phoneNumber = phone;
        emit phoneNumberChanged(phone);
    }
}

QString RisipAccountProfile::mobileNumber() const
{
    return m_data->mobileNumber;
}

void RisipAccountProfile::setMobileNumber(const QString &mobile)
{
    if(m_data->mobileNumber != mobile) {
        m_data->mobileNumber = mobile;
        emit mobileNumberChanged(mobile);
    }
}

int RisipAccountProfile::deviceType() const
{
    return m_data->deviceType;
}

void RisipAccountProfile::setDeviceType(int type)
{
    if(m_data->deviceType != type) {
        m_data->deviceType = type;
        emit deviceTypeChanged(type);
    }
}

int RisipAccountProfile::status() const
{
    return m_data->status;
}

void RisipAccountProfile::setStatus(int status)
{
    if(m_data->status != status) {
        m_data->status = status;
        emit statusChanged(m_data->status);
    }
}

void RisipAccountProfile::reset()
{
    delete m_data;
    m_data = NULL;

    m_data = new Private;
}
