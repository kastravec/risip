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

RisipAccountProfile::RisipAccountProfile(QObject *parent)
    :QObject(parent)
    ,m_data(new RisipAccountProfileData)
    ,m_status(Empty)
{

}

RisipAccountProfile::~RisipAccountProfile()
{
    delete m_data;
    m_data = NULL;
}

int RisipAccountProfile::userId() const
{
    return m_data->m_userId;
}

bool RisipAccountProfile::valid() const
{
    return true;
}

QString RisipAccountProfile::username() const
{
    return m_data->m_userName;
}

void RisipAccountProfile::setUsername(const QString &username)
{
    if(m_data->m_userName != username) {
        m_data->m_userName = username;
        emit usernameChanged(username);
    }
}

QString RisipAccountProfile::password() const
{
    return m_data->m_password;
}

void RisipAccountProfile::setPassword(const QString &password)
{
    if(m_data->m_password != password) {
        m_data->m_password = password;
        emit passwordChanged(password);
    }
}

QString RisipAccountProfile::creditBalance() const
{
    return m_data->m_balance;
}

QString RisipAccountProfile::email() const
{
    return m_data->m_email;
}

void RisipAccountProfile::setEmail(const QString &email)
{
    if(m_data->m_email != email) {
        m_data->m_email = email;
        emit emailChanged(email);
    }
}

QString RisipAccountProfile::firstName() const
{
    return m_data->m_firstName;
}

void RisipAccountProfile::setFirstName(const QString &firstname)
{
    if(m_data->m_firstName != firstname) {
        m_data->m_firstName = firstname;
        emit firstNameChanged(firstname);
    }
}

QString RisipAccountProfile::lastName() const
{
    return m_data->m_lastName;
}

void RisipAccountProfile::setLastName(const QString &lastname)
{
    if(m_data->m_lastName != lastname) {
        m_data->m_lastName = lastname;
        emit lastNameChanged(lastname);
    }
}

QString RisipAccountProfile::country() const
{
    return m_data->m_country;
}

void RisipAccountProfile::setCountry(const QString &country)
{
    if(m_data->m_country != country) {
        m_data->m_country = country;
        emit countryChanged(country);
    }
}

QString RisipAccountProfile::city() const
{
    return m_data->m_city;
}

void RisipAccountProfile::setCity(const QString &city)
{
    if(m_data->m_city != city) {
        m_data->m_city = city;
        emit cityChanged(city);
    }
}

QString RisipAccountProfile::address() const
{
    return m_data->m_address;
}

void RisipAccountProfile::setAddress(const QString &address)
{
    if(m_data->m_address != address) {
        m_data->m_address = address;
        emit addressChanged(address);
    }
}

QString RisipAccountProfile::postCode() const
{
    return m_data->m_postcode;
}

void RisipAccountProfile::setPostCode(const QString &code)
{
    if(m_data->m_postcode != code) {
        m_data->m_postcode = code;
        emit postCodeChanged(code);
    }
}

QString RisipAccountProfile::vatNumber() const
{
    return m_data->m_vatNumber;
}

void RisipAccountProfile::setVatNumber(const QString &number)
{
    if(m_data->m_vatNumber != number) {
        m_data->m_vatNumber = number;
        emit vatNumberChanged(number);
    }
}

int RisipAccountProfile::currency() const
{
    return m_data->m_currency;
}

void RisipAccountProfile::setCurrency(int currency)
{
    if(m_data->m_currency != currency) {
        m_data->m_currency = currency;
        emit currencyChanged(currency);
    }
}

QString RisipAccountProfile::phoneNumber() const
{
    return m_data->m_phoneNumber;
}

void RisipAccountProfile::setPhoneNumber(const QString &phone)
{
    if(m_data->m_phoneNumber != phone) {
        m_data->m_phoneNumber = phone;
        emit phoneNumberChanged(phone);
    }
}

QString RisipAccountProfile::mobileNumber() const
{
    return m_data->m_mobileNumber;
}

void RisipAccountProfile::setMobileNumber(const QString &mobile)
{
    if(m_data->m_mobileNumber != mobile) {
        m_data->m_mobileNumber = mobile;
        emit mobileNumberChanged(mobile);
    }
}

int RisipAccountProfile::deviceType() const
{
    return m_data->m_deviceType;
}

void RisipAccountProfile::setDeviceType(int type)
{
    if(m_data->m_deviceType != type) {
        m_data->m_deviceType = type;
        emit deviceTypeChanged(type);
    }
}

int RisipAccountProfile::status() const
{
    return m_status;
}

void RisipAccountProfile::setStatus(int status)
{
    if(m_status != status) {
        m_status = status;
        emit statusChanged(m_status);
    }
}

void RisipAccountProfile::reset()
{
    delete m_data;
    m_data = NULL;

    m_data = new RisipAccountProfileData;
}
