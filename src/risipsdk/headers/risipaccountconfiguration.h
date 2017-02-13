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

#ifndef RISIPACCOUNTCONFIGURATION_H
#define RISIPACCOUNTCONFIGURATION_H

#include "risipsdkglobal.h"
#include <pjsua2.hpp>
using namespace pj;

#include <QObject>

namespace risip {

class RisipAccount;

class RISIP_VOIPSDK_EXPORT RisipAccountConfiguration : public QObject
{
    Q_OBJECT
public:

    enum Codecs {
        iLBC30 = 22,
        iLBC20,
        G711U,
        G711A,
        G722,
        G729,
        Speex20,
        Speex30,
        Opus
    };

    enum NetworkProtocol {
        UDP = 0,
        TCP = 1,
        TLS = 2
//        UDP6,
//        TCP6,
//        SCTP,
//        TLS6,
//        LOOP
    };

    Q_ENUM(Codecs)
    Q_ENUM(NetworkProtocol)

    //pjsip-defined
    Q_PROPERTY(RisipAccount * account READ account WRITE setAccount NOTIFY accountChanged)
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString scheme READ scheme WRITE setScheme NOTIFY schemeChanged)
    Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(QString proxyServer READ proxyServer WRITE setProxyServer NOTIFY proxyServerChanged)
    Q_PROPERTY(int proxyPort READ proxyPort WRITE setProxyPort NOTIFY proxyPortChanged)
    Q_PROPERTY(int transportId READ transportId WRITE setTransportId NOTIFY transportIdChanged)
    Q_PROPERTY(bool encryptCalls READ encryptCalls WRITE setEncryptCalls NOTIFY encryptCallsChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

    //user-defined
    Q_PROPERTY(int networkProtocol READ networkProtocol WRITE setNetworkProtocol NOTIFY networkProtocolChanged)
    Q_PROPERTY(int localPort READ localPort WRITE setLocalPort NOTIFY localPortChanged)
    Q_PROPERTY(bool randomLocalPort READ randomLocalPort WRITE setRandomLocalPort NOTIFY randomLocalPortChanged)
//    Q_PROPERTY(QList<int> availableCodecs READ availableCodecs WRITE setAvailableCodecs NOTIFY availableCodecsChanged)

    RisipAccountConfiguration(QObject *parent = 0);
    ~RisipAccountConfiguration();

    RisipAccount *account() const;
    void setAccount(RisipAccount *account);

    QString uri();
    void setUri(const QString &accountUri);

    QString userName();
    void setUserName(const QString &name);

    QString password() const;
    void setPassword(const QString &pass);

    QString scheme() const;
    void setScheme(const QString &credScheme);

    QString serverAddress();
    void setServerAddress(const QString &address);

    QString proxyServer() const;
    void setProxyServer(const QString &proxy);

    int proxyPort() const;
    void setProxyPort(int port);

    int transportId() const;
    void setTransportId(int transId);

    int networkProtocol() const;
    void setNetworkProtocol(int protocol);

    int localPort() const;
    void setLocalPort(int port);

    bool randomLocalPort() const;
    void setRandomLocalPort(bool random);

    bool encryptCalls() const;
    void setEncryptCalls(bool encrypt);

    bool valid();

//    QList<int> availableCodecs() const;
//    void setAvailableCodecs(const QList<int> &codecs);

    AccountConfig pjsipAccountConfig();
    void setPjsipAccountConfig(AccountConfig pjsipConfig);

    TransportConfig pjsipTransportConfig();
    void setPjsipTransportConfig(TransportConfig pjsipConfig);

Q_SIGNALS:
    void accountChanged(RisipAccount *account);
    void uriChanged(const QString &uri);
    void userNameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void schemeChanged(const QString &scheme);
    void serverAddressChanged(const QString &serverAddress);
    void proxyServerChanged(const QString &proxy);
    void proxyPortChanged(int &port);
    void transportIdChanged(int transportId);
    void networkProtocolChanged(int protocol);
    void localPortChanged(int port);
    void randomLocalPortChanged(bool randomPort);
    void encryptCallsChanged(bool encrypt);
    void validChanged(bool valid);

private:
    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIPACCOUNTCONFIGURATION_H
