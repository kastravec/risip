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

#include <QGuiApplication>

#include "uiloader.h"
#include "risip.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    //risi wrapper object - registering the c++ classes to the QML engine
    Risip::registerToQml();

    //the ui loader resolves which ui to be loaded for the platform/operating sytem
    UiLoader ui;
    ui.setPlatformUi(UiLoader::SimpleDemo); //demo platform for now , check SimpleDemo.qml
    ui.start();

    return app.exec();
}
