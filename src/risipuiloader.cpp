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

#include "risipuiloader.h"
#include "applicationsettings.h"
#include <QQmlApplicationEngine>
#include <QQuickStyle>

static QObject *applicationSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return ApplicationSettings::instance();
}

UiLoader::UiLoader(QObject *parent)
    :QObject(parent)
    ,m_qmlEngine(NULL)
{
}

UiLoader::~UiLoader()
{
    delete ApplicationSettings::instance();
}

void UiLoader::start()
{
    if(!m_qmlEngine)
        m_qmlEngine = new QQmlApplicationEngine(this);

    QQuickStyle::setStyle("Material");

    qmlRegisterSingletonType<ApplicationSettings>("Application", 1, 0, "ApplicationSettings", applicationSingletonProvider);

//    TODO check platform before loading

    m_qmlEngine->load(QUrl(QLatin1String("qrc:/ui/base/Main.qml")));

//    m_qmlEngine->load(QUrl(QLatin1String("qrc:/ui/simpledemo/SimpleDemo.qml")));
}

ApplicationSettings *UiLoader::applicationSettings()
{
    return ApplicationSettings::instance();
}
