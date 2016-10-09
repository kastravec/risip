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
#include "risipendpoint.h"
#include "risipcall.h"
#include "risipaccountconfiguration.h"
#include "risipmedia.h"
#include "risipmessage.h"
#include "risipcallhistorymodel.h"

#include <QDebug>

static QObject *risipSingletontypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new Risip;
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
    ,m_sipEndpoint(new RisipEndpoint(this))
{
}

Risip::~Risip()
{
    while (!m_accounts.isEmpty())
        delete m_accounts.take(m_accounts.keys().takeFirst());

    m_sipEndpoint->stopEngine();
    if(m_sipEndpoint)
        delete m_sipEndpoint;
}

QQmlListProperty<RisipAccount> Risip::accounts()
{
    QList<RisipAccount *> allAccounts = m_accounts.values();
    return QQmlListProperty<RisipAccount>(this, allAccounts);
}

RisipEndpoint *Risip::sipEndpoint()
{
    return m_sipEndpoint;
}

void Risip::registerToQml()
{
    qmlRegisterSingletonType<Risip>(RISIP_QML_IMPORT_URI, 1, 0, "Risip", risipSingletontypeProvider);
    qmlRegisterType<RisipEndpoint>(RISIP_QML_IMPORT_URI, 1, 0, "RisipEndpoint");
    qmlRegisterType<RisipAccount>(RISIP_QML_IMPORT_URI, 1, 0, "RisipAccount");
    qmlRegisterType<RisipBuddy>(RISIP_QML_IMPORT_URI, 1, 0, "RisipBuddy");
    qmlRegisterType<RisipCall>(RISIP_QML_IMPORT_URI, 1, 0, "RisipCall");
    qmlRegisterType<RisipAccountConfiguration>(RISIP_QML_IMPORT_URI, 1, 0, "RisipAccountConfiguration");
    qmlRegisterType<RisipMedia>(RISIP_QML_IMPORT_URI, 1, 0, "RisipMedia");
    qmlRegisterType<RisipMessage>(RISIP_QML_IMPORT_URI, 1, 0, "RisipMessage");
    qmlRegisterType<RisipCallHistoryModel>(RISIP_QML_IMPORT_URI, 1, 0, "RisipCallHistoryModel");
}

RisipAccount *Risip::getAccount(QString &accountUri)
{
    if(!accountUri.isEmpty() && m_accounts.contains(accountUri))
        return m_accounts[accountUri];

    return new RisipAccount();
}

RisipAccount *Risip::getAccount(RisipAccountConfiguration *configuration)
{
    if(!configuration)
        return new RisipAccount();

    RisipAccount *account;
    //return an existing account and update its configuration too
    if(m_accounts.contains(configuration->uri())) {
        account = m_accounts[configuration->uri()];
        account->setConfiguration(configuration);
        return account;
    }

    //create a new account with the given configuration
    account = new RisipAccount;
    account->setSipEndPoint(sipEndpoint());
    account->setConfiguration(configuration);
    m_accounts[account->configuration()->uri()] = account;
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
