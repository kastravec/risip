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

#include "uiloader.h"
#include <QQmlApplicationEngine>

UiLoaderInfo::UiLoaderInfo(QObject *parent)
    :QObject (parent)
{
}

UiLoaderInfo::~UiLoaderInfo()
{

}

UiLoader::UiLoader(QObject *parent)
    :QObject(parent)
    ,m_qmlEngine(NULL)
{
    updatePlatformUi();
}

UiLoader::~UiLoader()
{
    m_qmlEngine->deleteLater();
}

void UiLoader::setPlatformUi(int platform)
{
    if(m_platformUi != platform) {
        m_platformUi = platform;
        emit platformUiChanged(m_platformUi);
    }
}

int UiLoader::platformUi() const
{
    return m_platformUi;
}

void UiLoader::start()
{
    if(!m_qmlEngine)
        m_qmlEngine = new QQmlApplicationEngine(this);

    switch (m_platformUi) {
    case iOS:
    case iOS_iPad:
    case iOS_iPhone:
    case iOS_iWatch:
    case Android:
    case Android_Phone:
    case Android_Tablet:
    case Android_Watch:
    case Windows:
    case Windows_Phone:
    case Windows_Tablet:
    case Linux_Embedded:
        break;
    case OSX:
    case Linux_Desktop:
    case Windows_Desktop:
    case SimpleDemo:
        m_qmlEngine->load(QUrl(QLatin1String("qrc:/ui/simpledemo/SimpleDemo.qml")));
        break;
    }
}

void UiLoader::updatePlatformUi()
{

#if defined (Q_OS_ANDROID)
    m_platformUi = Android;
#endif

#if defined (Q_OS_IOS)
    m_platformUi = iOS;
#endif

#if defined (Q_OS_ANDROID)
    m_platformUi = Android_Phone;
#endif

#if defined (Q_OS_OSX)
    m_platformUi = OSX;
#endif

#if defined (Q_OS_WIN)
    m_platformUi = Windows;
#endif

#if defined (Q_OS_LINUX)
    m_platformUi = Linux_Desktop;
#endif

#if defined (Q_OS_WINRT)
    m_platformUi = WindowRT;
#endif

    emit platformUiChanged(m_platformUi);
}
