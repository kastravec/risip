#include <QtQml>

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

#include "risip.h"

#include "globals.h"
#include "risipaccount.h"
#include "risipbuddy.h"
#include "risipcall.h"
#include "risipaccountconfiguration.h"
#include "risipmedia.h"
#include "risipmessage.h"
#include "risipcallhistorymodel.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

static QObject *risipSingletontypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return Risip::instance();
}

Risip *Risip::m_risipInstance = NULL;
Risip *Risip::instance()
{
    if(!m_risipInstance)
        m_risipInstance = new Risip;

    return m_risipInstance;
}

Risip::Risip(QObject *parent)
    :QObject (parent)
{
    readSettings();
}

Risip::~Risip()
{
    saveSettings();
    while (!m_accounts.isEmpty())
        delete m_accounts.take(m_accounts.keys().takeFirst());

    m_sipEndpoint.stopEngine();
}

QQmlListProperty<RisipAccount> Risip::accounts()
{
    QList<RisipAccount *> allAccounts = m_accounts.values();
    return QQmlListProperty<RisipAccount>(this, allAccounts);
}

QStringList Risip::accountNames() const
{
    return m_accounts.keys();
}

RisipEndpoint *Risip::sipEndpoint()
{
    return &m_sipEndpoint;
}

void Risip::registerToQml()
{
    qmlRegisterSingletonType<Risip>(RisipSettingsParam::QmlUri, 1, 0, "Risip", risipSingletontypeProvider);
    qmlRegisterType<RisipEndpoint>(RisipSettingsParam::QmlUri, 1, 0, "RisipEndpoint");
    qmlRegisterType<RisipAccount>(RisipSettingsParam::QmlUri, 1, 0, "RisipAccount");
    qmlRegisterType<RisipBuddy>(RisipSettingsParam::QmlUri, 1, 0, "RisipBuddy");
    qmlRegisterType<RisipCall>(RisipSettingsParam::QmlUri, 1, 0, "RisipCall");
    qmlRegisterType<RisipAccountConfiguration>(RisipSettingsParam::QmlUri, 1, 0, "RisipAccountConfiguration");
    qmlRegisterType<RisipMedia>(RisipSettingsParam::QmlUri, 1, 0, "RisipMedia");
    qmlRegisterType<RisipMessage>(RisipSettingsParam::QmlUri, 1, 0, "RisipMessage");
    qmlRegisterType<RisipCallHistoryModel>(RisipSettingsParam::QmlUri, 1, 0, "RisipCallHistoryModel");
}

RisipAccount *Risip::accountForUri(const QString &accountUri)
{
    if(!accountUri.isEmpty() && m_accounts.contains(accountUri))
        return m_accounts[accountUri];

    return new RisipAccount(this);
}

//sip2sip.info
RisipAccount *Risip::accountForConfiguration(RisipAccountConfiguration *configuration)
{
    if(configuration) {
        RisipAccount *account;
        //return an existing account and update its configuration too
        if(m_accounts.contains(configuration->uri())) {
            account = m_accounts[configuration->uri()];
            account->setConfiguration(configuration);
            return account;
        }
    }

    return createAccount(configuration);
}

RisipAccount *Risip::createAccount(RisipAccountConfiguration *configuration)
{
    if(!configuration)
        return new RisipAccount(this);

    //create a new account with the given configuration
    RisipAccount *account = new RisipAccount(this);
    account->setSipEndPoint(sipEndpoint());
    account->setConfiguration(configuration);
    m_accounts[account->configuration()->uri()] = account;
    emit accountsChanged();
    emit accountNamesChanged();

    return account;
}

bool Risip::removeAccount(QString &accountUri)
{
    if(m_accounts.contains(accountUri)) {
        m_accounts.take(accountUri)->deleteLater();
        return true;
    }

    return false;
}

bool Risip::removeAccount(RisipAccountConfiguration *configuration)
{
    QString accountUri;
    if(configuration)
        accountUri = configuration->uri();
    return removeAccount(accountUri);
}

bool Risip::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    int totaltAccounts = settings.value(RisipSettingsParam::TotalAccounts).toInt();

    RisipAccount *account = NULL;
    settings.beginGroup(RisipSettingsParam::AccountGroup);
    for(int i=0; i<totaltAccounts; ++i) {
        settings.beginReadArray(QString("account" + QString::number(i))); //settings array for account
        account = new RisipAccount(this);
        account->configuration()->setUri(settings.value(RisipSettingsParam::Uri).toString());
        account->configuration()->setUserName(settings.value(RisipSettingsParam::Username).toString());
        account->configuration()->setPassword(settings.value(RisipSettingsParam::Password).toString());
        account->configuration()->setNetworkProtocol(settings.value(RisipSettingsParam::NetworkType).toInt());
        account->configuration()->setServerAddress(settings.value(RisipSettingsParam::ServerAddress).toString());
        account->configuration()->setProxyServer(settings.value(RisipSettingsParam::ProxyServer).toString());
        account->configuration()->setLocalPort(settings.value(RisipSettingsParam::LocalPort).toInt());
        account->configuration()->setRandomLocalPort(settings.value(RisipSettingsParam::RandomLocalPort).toInt());
        account->configuration()->setScheme( settings.value(RisipSettingsParam::Scheme).toString() );
        m_accounts[account->configuration()->uri()] = account;
        settings.endArray(); //end array
    }

    return true;
}

bool Risip::saveSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(RisipSettingsParam::TotalAccounts, m_accounts.size());
    settings.beginGroup(RisipSettingsParam::AccountGroup);

    RisipAccount *account = NULL;
    for(int i=0; i<m_accounts.count(); ++i) {
        settings.beginWriteArray(QString("account" + QString::number(i)), 9); //settings array for account
        account = m_accounts[m_accounts.keys()[i]];
        settings.setValue(RisipSettingsParam::Uri, m_accounts.keys()[i]);
        settings.setValue(RisipSettingsParam::Username, account->configuration()->userName());
        settings.setValue(RisipSettingsParam::Password, account->configuration()->password());
        settings.setValue(RisipSettingsParam::NetworkType, account->configuration()->networkProtocol());
        settings.setValue(RisipSettingsParam::ServerAddress, account->configuration()->serverAddress());
        settings.setValue(RisipSettingsParam::ProxyServer, account->configuration()->proxyServer());
        settings.setValue(RisipSettingsParam::Scheme, account->configuration()->scheme());
        settings.setValue(RisipSettingsParam::LocalPort, account->configuration()->localPort());
        settings.setValue(RisipSettingsParam::RandomLocalPort, account->configuration()->randomLocalPort());
        settings.endArray(); //end settings array for account
    }

    settings.endGroup();
    return true;
}

bool Risip::resetSettings()
{

    return true;
}
