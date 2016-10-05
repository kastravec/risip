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

#ifndef UILOADER_H
#define UILOADER_H

#include <QObject>

class QQmlApplicationEngine;

class UiLoaderInfo : public QObject
{
    Q_OBJECT
public:
    UiLoaderInfo(QObject *parent = 0);
    ~UiLoaderInfo();
};

class UiLoader : public QObject
{
    Q_OBJECT

public:
    enum PlatformUI {
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
        SimpleDemo,
        Custom = -1
    };

    Q_ENUM(PlatformUI)
    Q_PROPERTY(int platformUi READ platformUi WRITE setPlatformUi NOTIFY platformUiChanged)

    UiLoader(QObject *parent = 0);
    ~UiLoader();

    void setPlatformUi(int platform);
    int platformUi() const;

    void start();

Q_SIGNALS:
    void platformUiChanged(int platform);

private:
    void updatePlatformUi();

    QQmlApplicationEngine *m_qmlEngine;
    int m_platformUi;
};

#endif // UILOADER_H
