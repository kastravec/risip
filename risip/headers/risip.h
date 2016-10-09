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

#ifndef RISIP_H
#define RISIP_H

#include <QObject>
#include <QQmlListProperty>

class RisipAccount;
class RisipEndpoint;
class RisipAccountConfiguration;

class Risip: public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QQmlListProperty<RisipAccount> accounts READ accounts NOTIFY accountsChanged)
    Q_PROPERTY(RisipEndpoint * sipEndpoint READ sipEndpoint CONSTANT)

    static Risip *instance();
    Risip(QObject *parent = 0);
    ~Risip();

    QQmlListProperty<RisipAccount> accounts();
    RisipEndpoint *sipEndpoint();

    static void registerToQml();

    Q_INVOKABLE RisipAccount *getAccount(QString &accountUri);
    Q_INVOKABLE RisipAccount *getAccount(RisipAccountConfiguration *configuration);
    Q_INVOKABLE bool removeAccount(QString &accountUri);
    Q_INVOKABLE bool removeAccount(RisipAccountConfiguration *configuration);

Q_SIGNALS:
    void accountsChanged(QQmlListProperty<RisipAccount> accounts);

private:

    static Risip *m_risipInstance;
    QHash<QString, RisipAccount *> m_accounts;
    RisipEndpoint *m_sipEndpoint;
};

#endif // RISIP_H
