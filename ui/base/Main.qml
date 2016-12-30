/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
**    http://risip.io
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
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

import Risip 1.0

Item {
    id: root
    width: 720
    height: 1280

    Material.theme: Material.Light
    Material.background: "#FFFFFF"
    Material.accent: "#db0000" //"#DB5AB9"
    Material.foreground: "#000000"
    Material.primary: "#FFFFFF"

    property string uiBasePath: "qrc:/ui/base/"
    property RisipAccount sipAccount: Risip.defaultAccount
    property RisipEndpoint sipEndpoint: Risip.sipEndpoint

    Component.onCompleted: { sipEndpoint.start(); }
    Component.onDestruction: { sipEndpoint.stop(); }

    Loader {
        id: splashScreenLoader
        source: uiBasePath + "SplashScreen.qml"
        active: true
        width: root.width
        height: height.height
        z:1
    }

    Loader {
        id: mainWindowLoader
        active: true
        asynchronous: true
        source: uiBasePath + "MainWindow.qml"
    }

    //handle splaschreen signals - when it times out what comes first!
    Connections {
        target: splashScreenLoader.item
        onTimeout: {
            mainWindowLoader.item.visible = true;
            splashScreenLoader.active = false;
        }
    }

    //check on sip endpoint statu - > default account can login now
    Connections {
        target: sipEndpoint

        onStatusChanged: {
            if(sipEndpoint.status === RisipEndpoint.Started)
                console.log("RISIP Engine started!");
        }
    }
}
