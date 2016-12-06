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

class RisipMorUser;
class RisipMorDevice;
class RisipUserProfile;

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
    Q_PROPERTY(int lcrId READ lcrId NOTIFY lcrIdChanged)
    Q_PROPERTY(QString uniqueHash READ uniqueHash WRITE setUniqueHash NOTIFY uniqueHashChanged)
    Q_PROPERTY(QString secretKey READ secretKey WRITE setSecretKey NOTIFY secretKeyChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)

    explicit RisipMorApi(QObject *parent = 0);
    ~RisipMorApi();

    int status() const;
    int lcrId() const;

    QString uniqueHash() const;
    void setUniqueHash(const QString &hash);

    QString secretKey() const;
    void setSecretKey(const QString &key);

    QString host() const;
    void setHost(const QString &host);

Q_SIGNALS:
    void lcrIdChanged(int lcrId);
    void uniqueHashChanged(const QString &uniqueHass);
    void secretKeyChanged(const QString &key);
    void hostChanged(const QString &host);

private:
    class Private;
    Private *m_data;
};

#endif // MORAPI_H
