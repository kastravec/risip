/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
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

#include "risipaccountconfiguration.h"
#include "risipaccount.h"

#include <QDebug>

namespace risip {

class RisipAccountConfiguration::Private
{
public:
    RisipAccount *risipAccount;
    AccountConfig accountConfig;
    AuthCredInfo accountCredentials;
    TransportConfig transportConfiguration;
    int networkProtocol;
    QString proxyAddress;
    int proxyPort;
    bool randomLocalPort;
};

RisipAccountConfiguration::RisipAccountConfiguration(QObject *parent)
    :QObject(parent)
    ,m_data(new Private)
{
    m_data->risipAccount = NULL;
    m_data->networkProtocol = UDP;
    m_data->randomLocalPort = true;

    setTransportId(-1);
    setLocalPort(0); //setting port to 0 means that any available random port will be used
//    setEncryptCalls(true);
}

RisipAccountConfiguration::~RisipAccountConfiguration()
{
    delete m_data;
    m_data = NULL;
}

RisipAccount *RisipAccountConfiguration::account() const
{
    return m_data->risipAccount;
}

void RisipAccountConfiguration::setAccount(RisipAccount *account)
{
    if(m_data->risipAccount != account) {
        m_data->risipAccount = account;

        if(m_data->risipAccount != NULL)
            m_data->risipAccount->setConfiguration(this);

        emit accountChanged(m_data->risipAccount);
    }
}

QString RisipAccountConfiguration::uri()
{
    if(m_data->accountConfig.idUri.empty()) {
        QString uri = QString("sip:") + userName() + QString("@") + serverAddress();
        setUri(uri);
        return uri;
    }

    return QString::fromStdString(m_data->accountConfig.idUri);
}

void RisipAccountConfiguration::setUri(const QString &accountUri)
{
    string accountUristr = accountUri.toStdString();
    if(m_data->accountConfig.idUri != accountUristr) {
        m_data->accountConfig.idUri = accountUristr;
        emit uriChanged(accountUri);
    }
}

QString RisipAccountConfiguration::userName()
{
    QString username;
    if(!m_data->accountCredentials.username.empty())
        username = QString::fromStdString(m_data->accountCredentials.username);

    return username;
}

void RisipAccountConfiguration::setUserName(const QString &name)
{
    if(userName() != name) {
        m_data->accountCredentials.username = name.toStdString();
        emit userNameChanged(name);
    }
}

QString RisipAccountConfiguration::password() const
{
    if(!m_data->accountCredentials.data.empty())
        return QString::fromStdString(m_data->accountCredentials.data);

    return QString();
}

void RisipAccountConfiguration::setPassword(const QString &pass)
{
    if(password() != pass) {
        m_data->accountCredentials.data = pass.toStdString();
        m_data->accountCredentials.dataType = 0; //0 is for plain password
        emit passwordChanged(pass);
    }
}

QString RisipAccountConfiguration::scheme() const
{
    if(!m_data->accountCredentials.scheme.empty())
        return QString::fromStdString(m_data->accountCredentials.scheme);

    return QString();
}

void RisipAccountConfiguration::setScheme(const QString &credScheme)
{
    if(scheme() != credScheme) {
        m_data->accountCredentials.scheme = credScheme.toStdString();
        emit schemeChanged(credScheme);
    }
}

QString RisipAccountConfiguration::serverAddress()
{
    //server address always is stored as a "sip:serveraddress" format in pjsip, so removing "sip:"
    //comes in handy for passing the just the server address around
    if(!m_data->accountConfig.regConfig.registrarUri.empty())
        return (QString::fromStdString(m_data->accountConfig.regConfig.registrarUri)).remove("sip:");

    return QString();
}

void RisipAccountConfiguration::setServerAddress(const QString &address)
{
    //always add the "sip:" prefix to properly store server address inside pjsip.
    if(serverAddress() != address) {
        m_data->accountConfig.regConfig.registrarUri = "sip:" + address.toStdString();
        emit serverAddressChanged(address);
    }
}

QString RisipAccountConfiguration::proxyServer() const
{
    return m_data->proxyAddress;
}

void RisipAccountConfiguration::setProxyServer(const QString &proxy)
{
    if(m_data->proxyAddress != proxy) {
        m_data->proxyAddress = proxy;
        emit proxyServerChanged(proxy);
    }
}

int RisipAccountConfiguration::proxyPort() const
{
    return m_data->proxyPort;
}

void RisipAccountConfiguration::setProxyPort(int port)
{
    if(m_data->proxyPort != port) {
        m_data->proxyPort = port;
        emit proxyPortChanged(m_data->proxyPort);
    }
}

int RisipAccountConfiguration::networkProtocol() const
{
    return m_data->networkProtocol;
}

void RisipAccountConfiguration::setNetworkProtocol(int protocol)
{
    if(m_data->networkProtocol != protocol) {
        m_data->networkProtocol = protocol;
        emit networkProtocolChanged(m_data->networkProtocol);
    }
}

int RisipAccountConfiguration::localPort() const
{
    return (int)m_data->transportConfiguration.port;
}

/**
 * @brief RisipAccountConfiguration::setLocalPort
 * @param port is local port that the engine will bind to for sending/accepting data
 *
 * Use this function to bind the sip client to a desired port and also make sure to disable
 * "randomLocalPort property. @see RisipAccountConfiguration::randomLocalPort
 *
 * Setting the port to 0 will simply enable the randomLocalPort property and the sip engine will
 * use any available port.
 */
void RisipAccountConfiguration::setLocalPort(int port)
{
    if(m_data->transportConfiguration.port != port) {
        m_data->transportConfiguration.port = port;
        emit localPortChanged(port);
    }
}

bool RisipAccountConfiguration::randomLocalPort() const
{
    return m_data->randomLocalPort;
}

void RisipAccountConfiguration::setRandomLocalPort(bool random)
{
    if(m_data->randomLocalPort != random) {
        m_data->randomLocalPort = random;
        emit randomLocalPortChanged(random);
    }
}

bool RisipAccountConfiguration::encryptCalls() const
{
    if(m_data->accountConfig.mediaConfig.srtpUse == PJMEDIA_SRTP_DISABLED)
        return false;
    else if(m_data->accountConfig.mediaConfig.srtpUse == PJMEDIA_SRTP_OPTIONAL )
        return true;

    return false;
}

void RisipAccountConfiguration::setEncryptCalls(bool encrypt)
{
    if(m_data->accountConfig.mediaConfig.srtpUse != encrypt) {
        if(encrypt)
            m_data->accountConfig.mediaConfig.srtpUse = PJMEDIA_SRTP_OPTIONAL;
        else
            m_data->accountConfig.mediaConfig.srtpUse = PJMEDIA_SRTP_DISABLED;

        emit encryptCallsChanged(encrypt);
    }
}

bool RisipAccountConfiguration::valid()
{
    if(serverAddress().isEmpty()
            || uri().isEmpty()
            || userName().isEmpty()
            || password().isEmpty()) {

        qDebug()<<"Invalid Configuration: " << serverAddress() <<uri() <<userName() << password();
        return false;
    }

    return true;
}

int RisipAccountConfiguration::transportId() const
{
    return m_data->accountConfig.sipConfig.transportId;
}

void RisipAccountConfiguration::setTransportId(int transId)
{
    if(transportId() != transId) {
        m_data->accountConfig.sipConfig.transportId = transId;
        emit transportIdChanged(transId);
    }
}

/**
 * @brief RisipAccountConfiguration::pjsipAccountConfig
 * @return AccountConfig
 *
 * An internal function that actually returns an AccountConfig object, that the C++ PJSIP API understands and we can
 * pass around in PJSIP library.
 *
 * Basically the RisipAccountConfiguration class has an internal AccountConfig object that populates it with the
 * respective account settings.
 */
AccountConfig RisipAccountConfiguration::pjsipAccountConfig()
{
    //setting the final sip account URI in a proper SIP format
    if(uri().isEmpty())
        setUri(QString("sip:") + userName() + QString("@") + serverAddress());

    //add the proxy and relevant network type
    if(!m_data->proxyAddress.isEmpty() || !m_data->proxyAddress.isNull()) {
        QString proxyUri = QString("sip:") + proxyServer() + QString(";transport=");
        switch (networkProtocol()) {
        case UDP:
            proxyUri = proxyUri + QString("udp");
            break;
        case TCP:
            proxyUri = proxyUri + QString("tcp");
            break;
        case TLS:
            proxyUri = proxyUri + QString("tls");
            break;
//        case UDP6:
//            proxyUri = proxyUri + QString("udp6");
//            break;
//        case TCP6:
//            proxyUri = proxyUri + QString("tcp6");
//            break;
        default:
            break;
        }

        m_data->accountConfig.sipConfig.proxies = { proxyUri.toStdString() }; //FIXME add proxy port
    }

    //adding the account credentials (username + password)
    m_data->accountConfig.sipConfig.authCreds.push_back(m_data->accountCredentials);

    m_data->accountConfig.callConfig.timerMinSESec = 1200;
    m_data->accountConfig.callConfig.timerSessExpiresSec = 22000;

//    m_data->accountConfig.natConfig.iceEnabled = true;

    return m_data->accountConfig;
}

void RisipAccountConfiguration::setPjsipAccountConfig(AccountConfig pjsipConfig)
{
    m_data->accountConfig = pjsipConfig;
}

TransportConfig RisipAccountConfiguration::pjsipTransportConfig()
{
    if(!m_data->randomLocalPort && localPort() != 0)
        m_data->transportConfiguration.port = localPort();
    else if(m_data->randomLocalPort)
        m_data->transportConfiguration.port = 0;

//    m_data->accountConfig.natConfig.sipStunUse
    return m_data->transportConfiguration;
}

void RisipAccountConfiguration::setPjsipTransportConfig(TransportConfig pjsipConfig)
{
    m_data->transportConfiguration = pjsipConfig;
}

} //end of risip namespace
