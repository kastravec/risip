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

#include "risipapplicationsettings.h"

#include <QCoreApplication>

namespace risip {

RisipApplicationSettings *RisipApplicationSettings::m_applicationSettingsInstance = NULL;
RisipApplicationSettings *RisipApplicationSettings::instance()
{
    if(!m_applicationSettingsInstance)
        m_applicationSettingsInstance = new RisipApplicationSettings;

    return m_applicationSettingsInstance;
}

RisipApplicationSettings::RisipApplicationSettings(QObject *parent)
    :QObject(parent)
    ,m_platform(Desktop)
{

#if defined (Q_OS_ANDROID)
    m_platform = Android;
#endif

#if defined (Q_OS_IOS)
    m_platform = iOS;
#endif

#if defined (Q_OS_ANDROID)
    m_platform = Android_Phone;
#endif

#if defined (Q_OS_OSX)
    m_platform = OSX;
#endif

#if defined (Q_OS_WIN)
    m_platform = Windows;
#endif

#if defined (Q_OS_LINUX)
    m_platform = Linux_Desktop;
#endif

#if defined (Q_OS_WINRT)
    m_platform = WindowRT;
#endif
}

RisipApplicationSettings::~RisipApplicationSettings()
{
    m_applicationSettingsInstance = NULL;
}

QString RisipApplicationSettings::organizationName() const
{
    return m_settings.organizationName();
}

void RisipApplicationSettings::setOrganizationName(QString orgName)
{
    if(organizationName() != orgName) {
        QCoreApplication::setOrganizationName(orgName);
        emit organizationNameChanged(orgName);
    }
}

QString RisipApplicationSettings::applicationName() const
{
    return m_settings.applicationName();
}

void RisipApplicationSettings::setApplicationName(QString name)
{
    if(applicationName() != name) {
        QCoreApplication::setApplicationName(name);
        emit applicationNameChanged(name);
    }
}

QString RisipApplicationSettings::organizationDomain() const
{
    return m_settings.organizationName();
}

void RisipApplicationSettings::setOrganizationDomain(QString name)
{
    if(organizationDomain() != name) {
        QCoreApplication::setOrganizationDomain(name);
        emit organizationDomainChanged(name);
    }
}

int RisipApplicationSettings::platform() const
{
    return m_platform;
}

bool RisipApplicationSettings::firstRun() const
{
    return true;
}

bool RisipApplicationSettings::save()
{
    return true;
}

bool RisipApplicationSettings::reset()
{
    return true;
}

bool RisipApplicationSettings::read()
{

    return true;
}

} //end of risip namespace
