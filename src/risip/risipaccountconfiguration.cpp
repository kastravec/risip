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

#include <QDebug>

RisipAccountConfiguration::RisipAccountConfiguration(QObject *parent)
    :QObject(parent)
    ,m_risipAccount(NULL)
    ,m_networkProtocol(UDP)
    ,m_proxyAddress()
    ,m_randomLocalPort(true)
{
    setTransportId(-1);
    setLocalPort(0); //setting port to 0 means that any available random port will be used
//    setEncryptCalls(true);
}

RisipAccountConfiguration::~RisipAccountConfiguration()
{}

RisipAccount *RisipAccountConfiguration::account() const
{
    return m_risipAccount;
}

void RisipAccountConfiguration::setAccount(RisipAccount *account)
{
    if(m_risipAccount != account) {
        m_risipAccount = account;

        if(m_risipAccount != NULL)
            m_risipAccount->setConfiguration(this);

        emit accountChanged(m_risipAccount);
    }
}

QString RisipAccountConfiguration::uri()
{
    if(m_accountConfig.idUri.empty()) {
        QString uri = QString("sip:") + userName() + QString("@") + serverAddress();
        setUri(uri);
        return uri;
    }

    return QString::fromStdString(m_accountConfig.idUri);
}

void RisipAccountConfiguration::setUri(const QString &accountUri)
{
    string accountUristr = accountUri.toStdString();
    if(m_accountConfig.idUri != accountUristr) {
        m_accountConfig.idUri = accountUristr;
        emit uriChanged(accountUri);
    }
}

QString RisipAccountConfiguration::userName()
{
    QString username;
    if(!m_accountCredentials.username.empty())
        username = QString::fromStdString(m_accountCredentials.username);

    return username;
}

void RisipAccountConfiguration::setUserName(const QString &name)
{
    if(userName() != name) {
        m_accountCredentials.username = name.toStdString();
        emit userNameChanged(name);
    }
}

QString RisipAccountConfiguration::password() const
{
    if(!m_accountCredentials.data.empty())
        return QString::fromStdString(m_accountCredentials.data);

    return QString();
}

void RisipAccountConfiguration::setPassword(const QString &pass)
{
    if(password() != pass) {
        m_accountCredentials.data = pass.toStdString();
        m_accountCredentials.dataType = 0; //0 is for plain password
        emit passwordChanged(pass);
    }
}

QString RisipAccountConfiguration::scheme() const
{
    if(!m_accountCredentials.scheme.empty())
        return QString::fromStdString(m_accountCredentials.scheme);

    return QString();
}

void RisipAccountConfiguration::setScheme(const QString &credScheme)
{
    if(scheme() != credScheme) {
        m_accountCredentials.scheme = credScheme.toStdString();
        emit schemeChanged(credScheme);
    }
}

QString RisipAccountConfiguration::serverAddress()
{
    //server address always is stored as a "sip:serveraddress" format in pjsip, so removing "sip:"
    //comes in handy for passing the just the server address around
    if(!m_accountConfig.regConfig.registrarUri.empty())
        return (QString::fromStdString(m_accountConfig.regConfig.registrarUri)).remove("sip:");

    return QString();
}

void RisipAccountConfiguration::setServerAddress(const QString &address)
{
    //always add the "sip:" prefix to properly store server address inside pjsip.
    if(serverAddress() != address) {
        m_accountConfig.regConfig.registrarUri = "sip:" + address.toStdString();
        emit serverAddressChanged(address);
    }
}

QString RisipAccountConfiguration::proxyServer() const
{
    return m_proxyAddress;
}

void RisipAccountConfiguration::setProxyServer(const QString &proxy)
{
    if(m_proxyAddress != proxy) {
        m_proxyAddress = proxy;
        emit proxyServerChanged(proxy);
    }
}

int RisipAccountConfiguration::proxyPort() const
{
    return m_proxyPort;
}

void RisipAccountConfiguration::setProxyPort(int port)
{
    if(m_proxyPort != port) {
        m_proxyPort = port;
        emit proxyPortChanged(m_proxyPort);
    }
}

int RisipAccountConfiguration::networkProtocol() const
{
    return m_networkProtocol;
}

void RisipAccountConfiguration::setNetworkProtocol(int protocol)
{
    if(m_networkProtocol != protocol) {
        m_networkProtocol = protocol;
        emit networkProtocolChanged(m_networkProtocol);
    }
}

int RisipAccountConfiguration::localPort() const
{
    return (int)m_transportConfiguration.port;
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
    if(m_transportConfiguration.port != port) {
        m_transportConfiguration.port = port;
        emit localPortChanged(port);
    }
}

bool RisipAccountConfiguration::randomLocalPort() const
{
    return m_randomLocalPort;
}

void RisipAccountConfiguration::setRandomLocalPort(bool random)
{
    if(m_randomLocalPort != random) {
        m_randomLocalPort = random;
        emit randomLocalPortChanged(random);
    }
}

bool RisipAccountConfiguration::encryptCalls() const
{
    if(m_accountConfig.mediaConfig.srtpUse == PJMEDIA_SRTP_DISABLED)
        return false;
    else if(m_accountConfig.mediaConfig.srtpUse == PJMEDIA_SRTP_OPTIONAL )
        return true;
}

void RisipAccountConfiguration::setEncryptCalls(bool encrypt)
{
    if(m_accountConfig.mediaConfig.srtpUse != encrypt) {
        if(encrypt)
            m_accountConfig.mediaConfig.srtpUse = PJMEDIA_SRTP_OPTIONAL;
        else
            m_accountConfig.mediaConfig.srtpUse = PJMEDIA_SRTP_DISABLED;

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
    return m_accountConfig.sipConfig.transportId;
}

void RisipAccountConfiguration::setTransportId(int transId)
{
    if(transportId() != transId) {
        m_accountConfig.sipConfig.transportId = transId;
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
    if(!m_proxyAddress.isEmpty() || !m_proxyAddress.isNull()) {
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

        m_accountConfig.sipConfig.proxies = { proxyUri.toStdString() }; //FIXME add proxy port
    }

    //adding the account credentials (username + password)
    m_accountConfig.sipConfig.authCreds.push_back(m_accountCredentials);

    m_accountConfig.callConfig.timerMinSESec = 1200;
    m_accountConfig.callConfig.timerSessExpiresSec = 22000;

//    m_accountConfig.natConfig.iceAggressiveNomination
    return m_accountConfig;
}

void RisipAccountConfiguration::setPjsipAccountConfig(AccountConfig pjsipConfig)
{
    m_accountConfig = pjsipConfig;
}

TransportConfig RisipAccountConfiguration::pjsipTransportConfig()
{
    if(!m_randomLocalPort && localPort() != 0)
        m_transportConfiguration.port = localPort();
    else if(m_randomLocalPort)
        m_transportConfiguration.port = 0;

//    m_accountConfig.natConfig.sipStunUse
    return m_transportConfiguration;
}

void RisipAccountConfiguration::setPjsipTransportConfig(TransportConfig pjsipConfig)
{
    m_transportConfiguration = pjsipConfig;
}
