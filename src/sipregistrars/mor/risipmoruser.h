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
#ifndef RISIPMORUSER_H
#define RISIPMORUSER_H

#include <QObject>

#include <QQmlListProperty>

class RisipUserProfile;
class RisipMorDevice;
class RisipMorUserBalance;

class RisipMorUser : public QObject
{
    Q_OBJECT
public:

    enum Status {
        Ready = 1,
        RequestUserCreationInProcess,
        RequestDeviceCreationInProcess,
        UpdateRequestInProcess,
        UserCreated,
        DeviceCreated,
        Error,
        Null = 0
    };

    Q_ENUM(Status)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(RisipUserProfile * profile READ profile WRITE setUserProfile NOTIFY userProfileChanged)
    Q_PROPERTY(RisipMorDevice * defaultDevice READ defaultDevice WRITE setDefaultDevice NOTIFY defaultDeviceChanged)
    Q_PROPERTY(RisipMorUserBalance * balance READ balance NOTIFY balanceChanged)
    Q_PROPERTY(QQmlListProperty<RisipMorDevice> devices READ devices NOTIFY devicesChanged)

    explicit RisipMorUser(QObject *parent = 0);
    ~RisipMorUser();

    int status() const;
    QQmlListProperty<RisipMorDevice> devices();

    RisipUserProfile *profile() const;
    void setUserProfile(RisipUserProfile *profile);

    RisipMorDevice *defaultDevice() const;
    void setDefaultDevice(RisipMorDevice *defaultDevice);

    RisipMorUserBalance *balance() const;

    Q_INVOKABLE void registerUser();
    Q_INVOKABLE void update();

Q_SIGNALS:
    void statusChanged(int status);
    void userProfileChanged(RisipUserProfile *profile);
    void defaultDeviceChanged(RisipMorDevice *defaultDevice);
    void balanceChanged(RisipMorUserBalance *balance);
    void devicesChanged();

private Q_SLOTS:
    void registrationHandler(const QByteArray &data);
    void registrationErrorHandler(int errorCode, const QByteArray &data);
    void defaultDeviceStatusHandler();

private:
    void setStatus(int status);
    void createDefaultDevice();

    class Private;
    Private *m_data;
};

#endif // RISIPMORUSER_H
