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

#ifndef RISIP_H
#define RISIP_H

#include "risipendpoint.h"
#include <QQmlListProperty>

class RisipAccount;
class RisipEndpoint;
class RisipAccountConfiguration;
class RisipCall;
class RisipBuddy;

class Risip: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QQmlListProperty<RisipAccount> accounts READ accounts NOTIFY accountsChanged)
    Q_PROPERTY(QStringList accountNames READ accountNames NOTIFY accountNamesChanged)
    Q_PROPERTY(RisipEndpoint * sipEndpoint READ sipEndpoint CONSTANT)
    Q_PROPERTY(RisipAccount * defaultAccount READ defaultAccount NOTIFY defaultAccountChanged)
    Q_PROPERTY(bool firstRun READ firstRun CONSTANT)

    ~Risip();
    static Risip *instance();
    static void registerToQml();

    QQmlListProperty<RisipAccount> accounts();
    QStringList accountNames() const;
    RisipEndpoint *sipEndpoint();
    RisipAccount *defaultAccount();
    bool firstRun() const;

    Q_INVOKABLE RisipAccount *accountForUri(const QString &accountUri);
    Q_INVOKABLE RisipAccount *accountForConfiguration(RisipAccountConfiguration *configuration);
    Q_INVOKABLE RisipAccount *createAccount(RisipAccountConfiguration *configuration);
    Q_INVOKABLE bool removeAccount(QString &accountUri);
    Q_INVOKABLE bool removeAccount(RisipAccountConfiguration *configuration);
    Q_INVOKABLE void setDefaultAccount(const QString &uri);

    Q_INVOKABLE bool readSettings();
    Q_INVOKABLE bool saveSettings();
    Q_INVOKABLE bool resetSettings();

Q_SIGNALS:
    void accountsChanged();
    void accountNamesChanged();
    void defaultAccountChanged(RisipAccount *account);

private:
    Risip(QObject *parent = 0);
    static Risip *m_risipInstance;
    QHash<QString, RisipAccount *> m_accounts;
    RisipEndpoint m_sipEndpoint;
    QString m_defaultAccountUri;
};

#endif // RISIP_H
