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

import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    id: splash
//    width: 720
//    height: 1280
    color: "transparent"
    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen
    visibility: Window.AutomaticVisibility

    property int timeoutInterval: 1500
    signal timeout()

    Image {
        id: splashImage
        source: "qrc:/images/LaunchScreenImage.png"
        anchors.fill: parent
    }

    Timer {
        interval: timeoutInterval;
        running: true;
        repeat: false

        onTriggered: {
            visible = false
            splash.timeout()
        }
    }

    Component.onCompleted: visible = true
}
