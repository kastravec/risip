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
#ifndef RISIPMORDEVICE_H
#define RISIPMORDEVICE_H

#include <QObject>

class RisipMorDevice : public QObject
{
    Q_OBJECT
public:
    enum Status {
        Ready = 1,
        DeviceCreationRequestSent,
        UpdateRequestInProcess,
        Error,
        Null = 0
    };

    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString extension READ extension WRITE setExtension NOTIFY extensionChanged)
    Q_PROPERTY(QString callerId READ callerId WRITE setCallerId NOTIFY callerIdChanged)

    explicit RisipMorDevice(QObject *parent = 0);
    ~RisipMorDevice();

    int status() const;

    int type() const;
    void setType(int type);

    QString username() const;
    void setUsername(const QString &name);

    QString password() const;
    void setPassword(const QString &pass);

    QString extension() const;
    void setExtension(const QString &ext);

    QString callerId() const;
    void setCallerId(const QString &id);

    Q_INVOKABLE void create();
    Q_INVOKABLE void update();

Q_SIGNALS:
    void statusChanged(int status);
    void typeChanged(int type);
    void usernameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void extensionChanged(const QString &extension);
    void callerIdChanged(const QString &callerId);

private Q_SLOTS:
    void creationHandler();
    void updateHandler();

private:
    void setStatus(int status);

    class Private;
    Private *m_data;
};

#endif // RISIPMORDEVICE_H
