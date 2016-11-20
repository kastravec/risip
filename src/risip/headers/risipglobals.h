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

#ifndef RISIPGLOBALS_H
#define RISIPGLOBALS_H

#include <QString>
#include <QPixmap>
#include <QRunnable>

class RisipPhoneNumber;

struct RisipCurrencies {
};

struct RisipSettingsParam {
    static const char *QmlUri;
    static const QString FirstRun;
    static const QString DefaultAccount;
    static const QString AutoSignIn;
    static const QString AccountGroup;
    static const QString TotalAccounts;
    static const QString Uri;
    static const QString Username;
    static const QString Password;
    static const QString ServerAddress;
    static const QString NetworkType;
    static const QString Scheme;
    static const QString ProxyServer;
    static const QString LocalPort;
    static const QString RandomLocalPort;
};

struct Country {
    Country(const QString &id = QString(), const QString &name = QString(),
            const QString &code = QString(), const QString &prefix = QString());
    Country(const Country &country);

    Country &operator=(const Country &country);
    bool operator!=(const Country &country);
    bool operator==(const Country &country);

    QString countryId;
    QString name;
    QString code;
    QString prefix;
    QPixmap flag;
};

struct RisipGlobals {
    RisipGlobals();

    static QString formatToSip(const QString &contact, const QString &server);
    static QList<Country> countries();
    static const Country &countryForPrefix(const QString &prefix);
    static void initializeCountries();
    static bool countriesInitialized();
    static void validateNumber(RisipPhoneNumber *number);

private:
    static QHash<QString, Country> m_allCountries;
    static bool m_countriesIntialized;
    static void setCountryList(QHash<QString, Country> countryList);

    friend class RisipConfigisLoader;
};

#endif // RISIPGLOBALS_H
