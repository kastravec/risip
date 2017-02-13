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

#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "risipsdkglobal.h"
#include <QObject>
#include <QSettings>

namespace risip {

class RISIP_VOIPSDK_EXPORT RisipApplicationSettings : public QObject
{
    Q_OBJECT

public:

    enum Platform {
        iOS,
        iOS_iPhone = 1,
        iOS_iPad,
        iOS_iWatch,
        OSX,
        Android,
        Android_Phone,
        Android_Tablet,
        Android_Watch,
        Windows,
        WindowRT,
        Windows_Phone,
        Windows_Tablet,
        Windows_Desktop,
        Linux_Desktop,
        Linux_Embedded,
        Desktop,
        SimpleDemo,
        Custom = -1
    };

    Q_ENUM(Platform)
    Q_PROPERTY(QString organizationName READ organizationName WRITE setOrganizationName NOTIFY organizationNameChanged)
    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName NOTIFY applicationNameChanged)
    Q_PROPERTY(QString organizationDomain READ organizationDomain WRITE setOrganizationDomain NOTIFY organizationDomainChanged)
    Q_PROPERTY(int platform READ platform CONSTANT)
    Q_PROPERTY(bool firstRun READ firstRun NOTIFY firstRunChanged)

    static RisipApplicationSettings *instance();
    ~RisipApplicationSettings();

    QString organizationName() const;
    void setOrganizationName(QString orgName);

    QString applicationName() const;
    void setApplicationName(QString name);

    QString organizationDomain() const;
    void setOrganizationDomain(QString name);

    int platform() const;
    bool firstRun() const;

    Q_INVOKABLE bool save();
    Q_INVOKABLE bool reset();
    Q_INVOKABLE bool read();

Q_SIGNALS:
    void firstRunChanged(bool first);
    void organizationNameChanged(QString name);
    void applicationNameChanged(QString name);
    void organizationDomainChanged(QString name);

private:
    RisipApplicationSettings(QObject *parent = 0);

    static RisipApplicationSettings *m_applicationSettingsInstance;
    int m_platform;
    QSettings m_settings;
};

} //end of risip namespace

#endif // APPLICATIONSETTINGS_H
