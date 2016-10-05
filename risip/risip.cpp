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

Risip::Risip(QObject *parent)
    :QObject (parent)
{
//    qmlRegisterType<RisipEndpoint>(RISIP_QML_IMPORT_URI, 1, 0, "Risip");
    qmlRegisterType<RisipEndpoint>(RISIP_QML_IMPORT_URI, 1, 0, "RisipEndpoint");
    qmlRegisterType<RisipAccount>(RISIP_QML_IMPORT_URI, 1, 0, "RisipAccount");
    qmlRegisterType<RisipBuddy>(RISIP_QML_IMPORT_URI, 1, 0, "RisipBuddy");
    qmlRegisterType<RisipCall>(RISIP_QML_IMPORT_URI, 1, 0, "RisipCall");
    qmlRegisterType<RisipAccountConfiguration>(RISIP_QML_IMPORT_URI, 1, 0, "RisipAccountConfiguration");
    qmlRegisterType<RisipMedia>(RISIP_QML_IMPORT_URI, 1, 0, "RisipMedia");
    qmlRegisterType<RisipMessage>(RISIP_QML_IMPORT_URI, 1, 0, "RisipMessage");
    qmlRegisterType<RisipCallHistoryModel>(RISIP_QML_IMPORT_URI, 1, 0, "RisipCallHistoryModel");

}

Risip::~Risip()
{
    while (!m_accounts.isEmpty())
        m_accounts[m_accounts.keys().takeFirst()]->deleteLater();

    m_sipEndpoint->deleteLater();
}

QQmlListProperty<RisipAccount> Risip::accounts()
{
    QList<RisipAccount *> allAccounts = m_accounts.values();
    return QQmlListProperty<RisipAccount>(this, allAccounts);
}

RisipEndpoint *Risip::sipEndpoint() const
{
    return m_sipEndpoint;
}

RisipAccount *Risip::getAccount(QString &accountUri)
{
    return new RisipAccount();
}

RisipAccount *Risip::getAccount(RisipAccountConfiguration *configuration)
{
    if(m_accounts.contains(configuration->uri()))
            return m_accounts[configuration->uri()];

    return new RisipAccount();
}

bool Risip::removeAccount(QString &accountUri)
{
    return true;
}

bool Risip::removeAccount(RisipAccountConfiguration *configuration)
{
    return true;
}
