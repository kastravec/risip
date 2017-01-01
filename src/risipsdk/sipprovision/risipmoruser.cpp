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
#include "risipmoruser.h"
#include "risipuserprofile.h"
#include "risipmordevice.h"
#include "risipmorapi.h"
#include "risipmoruserbalance.h"
#include <QXmlStreamReader>

#include "utils/httpnetworkrequest.h"

#include <QDebug>

class RisipMorUser::Private
{
public:
    int status;
    RisipUserProfile *profile;
    RisipMorDevice *defaultDevice;
    RisipMorUserBalance *balance;
    QHash<QString, RisipMorDevice *> devices;
    RisipMorApi *morApi;
    HttpNetworkRequest *httprequest;
};

RisipMorUser::RisipMorUser(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->profile = new RisipUserProfile(this);
    m_data->balance = new RisipMorUserBalance(this);
    m_data->defaultDevice = new RisipMorDevice(this);
    m_data->morApi = new RisipMorApi(this);
    setStatus(Null);
}

RisipMorUser::~RisipMorUser()
{
    delete m_data;
    m_data = NULL;
}

RisipUserProfile *RisipMorUser::profile() const
{
    return m_data->profile;
}

void RisipMorUser::setUserProfile(RisipUserProfile *profile)
{
    if(m_data->profile != profile) {
        m_data->profile = profile;
        emit userProfileChanged(m_data->profile);
    }
}

RisipMorDevice *RisipMorUser::defaultDevice() const
{
    return m_data->defaultDevice;
}

void RisipMorUser::setDefaultDevice(RisipMorDevice *device)
{
    if(m_data->defaultDevice != device) {
        m_data->defaultDevice = device;
        emit defaultDeviceChanged(m_data->defaultDevice);
    }
}

/**
 * @brief RisipMorUser::balance
 * @return user's balance object
 *
 * Use this to retrieve the respective Risip object for user's balance.
 *
 * Retrieve the object with this method and perform different functions
 * with RisipMorUserBalance API, i.e. query the balance of the user, update it,
 * check currency, etc.
 *
 */
RisipMorUserBalance *RisipMorUser::balance() const
{
    return m_data->balance;
}

int RisipMorUser::status() const
{
    return m_data->status;
}

QQmlListProperty<RisipMorDevice> RisipMorUser::devices()
{
    QList<RisipMorDevice *> alldevices = m_data->devices.values();
    return QQmlListProperty<RisipMorDevice> (this, alldevices);
}

/**
 * @brief RisipMorUser::create
 *
 * Call this method to create the user in the MOR billing server of Risip.
 * It will also create a default SIP device too, using the same credentials as
 * provided for the username.
 *
 * Check the status property to track the state of the Risip Mor User, whether it is created
 * or default device is being created and when it actually is ready to be used.
 */
void RisipMorUser::registerUser()
{
    setStatus(RequestUserCreationInProcess);
    //creating a new httprequest object
    m_data->httprequest = new HttpNetworkRequest(this);

    //http headers
    QJsonObject headers;
    headers.insert(QString("Content-type"), QJsonValue(QString("application/x-www-form-urlencoded")));

    //generating hash for MOR
    QString hashString = m_data->profile->email()
            + m_data->morApi->uniqueHash()
            + QString("SIP") // profile->deviceType() FIXME check device type, for now use SIP
            + m_data->profile->username()
            + m_data->profile->firstName()
            + m_data->profile->lastName()
            + m_data->morApi->secretKey();
    QByteArray hashvalue = QCryptographicHash::hash(hashString.toLatin1(), QCryptographicHash::Sha1);

    //URL parameters
    QJsonObject parameters;
    parameters.insert(QString("email"), QJsonValue(m_data->profile->email()));
    parameters.insert(QString("id"), QJsonValue(m_data->morApi->uniqueHash()));
    parameters.insert(QString("device_type"), QJsonValue(QString("SIP")));
    parameters.insert(QString("username"), QJsonValue(m_data->profile->username()));
    parameters.insert(QString("first_name"), QJsonValue(m_data->profile->firstName()));
    parameters.insert(QString("last_name"), QJsonValue(m_data->profile->lastName()));
    parameters.insert(QString("API_Secret_Key"), QJsonValue(m_data->morApi->secretKey()));
    parameters.insert(QString("hash"), QJsonValue(QString(hashvalue.toHex())));
    parameters.insert(QString("password"), QJsonValue(m_data->profile->password()));
    parameters.insert(QString("password2"), QJsonValue(m_data->profile->password()));
    parameters.insert(QString("country_id"), QJsonValue(m_data->profile->country().countryId));

    m_data->httprequest->setBaseUrl(QString("http://") + m_data->morApi->host() + QString("/billing/api/user_register?"));
    m_data->httprequest->setHttpHeaders(headers);
    m_data->httprequest->setUrlParameters(parameters);

    connect(m_data->httprequest, &HttpNetworkRequest::replyReady,
            this, &RisipMorUser::registrationHandler, Qt::DirectConnection);
    connect(m_data->httprequest, &HttpNetworkRequest::replyError,
            this, &RisipMorUser::registrationErrorHandler, Qt::DirectConnection);

    m_data->httprequest->post(QByteArray());
}

void RisipMorUser::update()
{

}

/**
 * @brief RisipMorUser::userCreationHandler
 *
 * Internal API.
 *
 * Private slot for handling the status change of the Risip MOR API which performs
 * user registration on the Risip MOR server.
 *
 */
void RisipMorUser::registrationHandler(const QByteArray &data)
{
    qDebug()<<"User Creation status: " << data;

    setStatus(UserCreated);
    m_data->httprequest->deleteLater();
}

void RisipMorUser::registrationErrorHandler(int errorCode, const QByteArray &data)
{
    qDebug()<<"HTTP REPLY ERROR: " << errorCode << data;
}

/**
 * @brief RisipMorUser::deviceCreationHandler
 *
 * Internal API.
 *
 * Privte slot handler for status change of the Risip MOR APO which performs
 * the device creation on the Risip MOR server.
 */
void RisipMorUser::defaultDeviceStatusHandler()
{
    setStatus(DeviceCreated);
    setStatus(Ready);
}

/**
 * @brief RisipMorUser::setStatus
 * @param status new status
 *
 * Internal API.
 *
 * Sets the status of this Risip MOR user.
 */
void RisipMorUser::setStatus(int status)
{
    if(m_data->status != status) {
        m_data->status = status;
        emit statusChanged(m_data->status);
    }
}

void RisipMorUser::createDefaultDevice()
{
    setStatus(RequestDeviceCreationInProcess);
    m_data->defaultDevice->create();
}
