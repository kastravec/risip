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

#ifndef MORAPI_H
#define MORAPI_H

#include <QObject>

class RisipAccountProfile;
class RisipAccount;

class RisipMorApi : public QObject
{
    Q_OBJECT
public:
    enum Status {
        Ready = 202,
        Error = 504
    };

    enum ApiError {
        ErrorResponse
    };

    Q_ENUM(ApiError)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)

    static RisipMorApi *instance();
    ~RisipMorApi();

    int status() const;

    Q_INVOKABLE RisipAccountProfile *getUserProfile(const QString &username);
    Q_INVOKABLE void registerAccount(RisipAccountProfile *profile);
    Q_INVOKABLE void getUserBalance(const QString &username);
    Q_INVOKABLE void getUserBalance(RisipAccountProfile *profile);

Q_SIGNALS:
    void statusChanged(int status);
    void accountRegistered(RisipAccount *account);

private Q_SLOTS:
    void accountRegstrationHandler(const QByteArray &data);
    void accountRegistrationError(int errorCode);
    void userBalanceUpdateHandler(const QByteArray &data);
    void userBalanceUpdateError(int errorCode);

private:
    explicit RisipMorApi(QObject *parent = 0);

    static RisipMorApi *m_instance;
    int m_status;
};

#endif // MORAPI_H
