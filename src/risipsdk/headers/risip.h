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

#include "risipsdkglobal.h"
#include "risipendpoint.h"
#include "risipaccount.h"

#include <QQmlListProperty>
#include <QAbstractItemModel>

namespace risip {

class RisipEndpoint;
class RisipAccountConfiguration;
class RisipCall;
class RisipBuddy;
class RisipContactManager;
class RisipCallManager;

class RISIP_VOIPSDK_EXPORT Risip: public QObject
{
    Q_OBJECT

public:
    enum SipResponseCode {
        PJSIP_SC_TRYING = 100,
        PJSIP_SC_RINGING = 180,
        PJSIP_SC_CALL_BEING_FORWARDED = 181,
        PJSIP_SC_QUEUED = 182,
        PJSIP_SC_PROGRESS = 183,

        PJSIP_SC_OK = 200,
        PJSIP_SC_ACCEPTED = 202,

        PJSIP_SC_MULTIPLE_CHOICES = 300,
        PJSIP_SC_MOVED_PERMANENTLY = 301,
        PJSIP_SC_MOVED_TEMPORARILY = 302,
        PJSIP_SC_USE_PROXY = 305,
        PJSIP_SC_ALTERNATIVE_SERVICE = 380,

        PJSIP_SC_BAD_REQUEST = 400,
        PJSIP_SC_UNAUTHORIZED = 401,
        PJSIP_SC_PAYMENT_REQUIRED = 402,
        PJSIP_SC_FORBIDDEN = 403,
        PJSIP_SC_NOT_FOUND = 404,
        PJSIP_SC_METHOD_NOT_ALLOWED = 405,
        PJSIP_SC_NOT_ACCEPTABLE = 406,
        PJSIP_SC_PROXY_AUTHENTICATION_REQUIRED = 407,
        PJSIP_SC_REQUEST_TIMEOUT = 408,
        PJSIP_SC_GONE = 410,
        PJSIP_SC_REQUEST_ENTITY_TOO_LARGE = 413,
        PJSIP_SC_REQUEST_URI_TOO_LONG = 414,
        PJSIP_SC_UNSUPPORTED_MEDIA_TYPE = 415,
        PJSIP_SC_UNSUPPORTED_URI_SCHEME = 416,
        PJSIP_SC_BAD_EXTENSION = 420,
        PJSIP_SC_EXTENSION_REQUIRED = 421,
        PJSIP_SC_SESSION_TIMER_TOO_SMALL = 422,
        PJSIP_SC_INTERVAL_TOO_BRIEF = 423,
        PJSIP_SC_TEMPORARILY_UNAVAILABLE = 480,
        PJSIP_SC_CALL_TSX_DOES_NOT_EXIST = 481,
        PJSIP_SC_LOOP_DETECTED = 482,
        PJSIP_SC_TOO_MANY_HOPS = 483,
        PJSIP_SC_ADDRESS_INCOMPLETE = 484,
        PJSIP_AC_AMBIGUOUS = 485,
        PJSIP_SC_BUSY_HERE = 486,
        PJSIP_SC_REQUEST_TERMINATED = 487,
        PJSIP_SC_NOT_ACCEPTABLE_HERE = 488,
        PJSIP_SC_BAD_EVENT = 489,
        PJSIP_SC_REQUEST_UPDATED = 490,
        PJSIP_SC_REQUEST_PENDING = 491,
        PJSIP_SC_UNDECIPHERABLE = 493,

        PJSIP_SC_INTERNAL_SERVER_ERROR = 500,
        PJSIP_SC_NOT_IMPLEMENTED = 501,
        PJSIP_SC_BAD_GATEWAY = 502,
        PJSIP_SC_SERVICE_UNAVAILABLE = 503,
        PJSIP_SC_SERVER_TIMEOUT = 504,
        PJSIP_SC_VERSION_NOT_SUPPORTED = 505,
        PJSIP_SC_MESSAGE_TOO_LARGE = 513,
        PJSIP_SC_PRECONDITION_FAILURE = 580,

        PJSIP_SC_BUSY_EVERYWHERE = 600,
        PJSIP_SC_DECLINE = 603,
        PJSIP_SC_DOES_NOT_EXIST_ANYWHERE = 604,
        PJSIP_SC_NOT_ACCEPTABLE_ANYWHERE = 606,

        PJSIP_SC_TSX_TIMEOUT = PJSIP_SC_REQUEST_TIMEOUT,
        /*PJSIP_SC_TSX_RESOLVE_ERROR = 702,*/
        PJSIP_SC_TSX_TRANSPORT_ERROR = PJSIP_SC_SERVICE_UNAVAILABLE,

        /* This is not an actual status code, but rather a constant
         * to force GCC to use 32bit to represent this enum, since
         * we have a code in PJSUA-LIB that assigns an integer
         * to this enum (see pjsua_acc_get_info() function).
         */
        PJSIP_SC__force_32bit = 0x7FFFFFFF
    };

    Q_ENUM(SipResponseCode)
    Q_PROPERTY(RisipEndpoint *sipEndpoint READ sipEndpoint CONSTANT)
    Q_PROPERTY(RisipAccount *defaultAccount READ defaultAccount NOTIFY defaultAccountChanged)
    Q_PROPERTY(QAbstractItemModel *allAccountsModel READ allAccountsModel NOTIFY allAccountsModelChanged)
    Q_PROPERTY(bool firstRun READ firstRun CONSTANT)

    ~Risip();
    static Risip *instance();
    static void registerToQml();

    RisipEndpoint *sipEndpoint();
    risip::RisipAccount *defaultAccount() const;

    bool firstRun() const;
    QAbstractItemModel *allAccountsModel() const;

    Q_INVOKABLE risip::RisipAccount *accountForUri(const QString &accountUri);
    Q_INVOKABLE risip::RisipAccount *accountForConfiguration(RisipAccountConfiguration *configuration);
    Q_INVOKABLE risip::RisipAccount *createAccount(RisipAccountConfiguration *configuration);
    Q_INVOKABLE bool removeAccount(const QString &accountUri);
    Q_INVOKABLE bool removeAccount(RisipAccountConfiguration *configuration);
    Q_INVOKABLE void setDefaultAccount(const QString &uri);

    Q_INVOKABLE bool readSettings();
    Q_INVOKABLE bool saveSettings();
    Q_INVOKABLE bool resetSettings();
    Q_INVOKABLE void accessPhoneContacts();
    Q_INVOKABLE void accessPhoneMedia();
    Q_INVOKABLE void accessPhoneLocation();

Q_SIGNALS:
    void defaultAccountChanged(RisipAccount *account);
    void allAccountsModelChanged(QAbstractItemModel *model);

private:
    explicit Risip(QObject *parent = 0);
    static Risip *m_risipInstance;

    class Private;
    Private *m_data;
};

} //end of risip namespace

#endif // RISIP_H
