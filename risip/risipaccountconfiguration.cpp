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
**    A copy of the license is also here <http://www.gnu.org/licenses/>.
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

QString RisipAccountConfiguration::uri() const
{
    return QString::fromStdString(m_accountConfig.idUri);
}

void RisipAccountConfiguration::setUri(QString accountUri)
{
    if(uri() != accountUri) {
        m_accountConfig.idUri = accountUri.toStdString();
        emit uriChanged(accountUri);
    }
}

QString RisipAccountConfiguration::userName() const
{
    return QString::fromStdString(m_accountCredentials.username);
}

void RisipAccountConfiguration::setUserName(QString &name)
{
    if(userName() != name) {
        m_accountCredentials.username = name.toStdString();
        emit userNameChanged(name);
    }
}

QString RisipAccountConfiguration::password() const
{
    return QString::fromStdString(m_accountCredentials.data);
}

void RisipAccountConfiguration::setPassword(QString &pass)
{
    if(password() != pass) {
        m_accountCredentials.data = pass.toStdString();
        m_accountCredentials.dataType = 0; //0 is for plain password
        emit passwordChanged(pass);
    }
}

QString RisipAccountConfiguration::scheme() const
{
    return QString::fromStdString(m_accountCredentials.scheme);
}

void RisipAccountConfiguration::setScheme(QString &credScheme)
{
    if(scheme() != credScheme) {
        m_accountCredentials.scheme = credScheme.toStdString();
        emit schemeChanged(credScheme);
    }
}

QString RisipAccountConfiguration::serverAddress() const
{
    //server address always is stored as a "sip:serveraddress" format in pjsip, so removing "sip:"
    //comes in handy for passing the just the server address around
    return (QString::fromStdString(m_accountConfig.regConfig.registrarUri)).remove("sip:");
}

void RisipAccountConfiguration::setServerAddress(QString &address)
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

void RisipAccountConfiguration::setProxyServer(QString &proxy)
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
    return m_transportConfiguration.port;
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
        case UDP6:
            proxyUri = proxyUri + QString("udp6");
            break;
        case TCP6:
            proxyUri = proxyUri + QString("tcp6");
            break;
        case TLS:
            proxyUri = proxyUri + QString("tls");
        break;
        default:
            break;
        }

        m_accountConfig.sipConfig.proxies = { proxyUri.toStdString() }; //FIXME add proxy port
    }

    //adding the account credentials (username + password)
    m_accountConfig.sipConfig.authCreds.push_back(m_accountCredentials);

    m_accountConfig.callConfig.timerMinSESec = 1200;
    m_accountConfig.callConfig.timerSessExpiresSec = 22000;

    return m_accountConfig;
}

TransportConfig RisipAccountConfiguration::pjsipTransportConfig()
{
    if(!m_randomLocalPort && localPort() != 0)
        m_transportConfiguration.port = localPort();
    else if(m_randomLocalPort)
        m_transportConfiguration.port = 0;

    return m_transportConfiguration;
}
