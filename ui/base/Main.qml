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
import Risip 1.0

Item {
    id: root

    width: 720
    height: 1280

    property string uiBasePath: "qrc:/ui/base/"
    property bool firstRun: Risip.firstRun

    Loader {
        id: splashScreenLoader
        source: uiBasePath + "SplashScreen.qml"
        active: true
        width: root.width
        height: height.height
        z:1
    }

    //welcome screen is unloaded once is first used.
    // active only on first run.
    Loader {
        id: welcomeScreenLoader
        source: uiBasePath + "WelcomeScreen.qml"
        active: firstRun //firstRun is set in settings always true for testing
        visible: false
        width: root.width
        height: root.height
        z:1
    }

    //MainWindow loader - main window of the app
    Loader {
        id: mainWindowLoader
        active: true
        source: "MainWindow.qml"
        asynchronous: true
        anchors.fill: parent
    }

    //handle splaschreen signals - when it times out what comes first!
    Connections {
        target: splashScreenLoader.item
        onTimeout: {
            if(Risip.firstRun) {
                welcomeScreenLoader.visible = true;
            } else {
                if(Risip.defaultAccount.status === RisipAccount.SignedIn)
                    mainWindowLoader.item.visible = true;
                else
                    loginPageLoader.visible = true;
            }

            splashScreenLoader.active = false;
        }
    }

    //welcome screen singal handler, from which we go to either
    //the Login page or if autologin is enabled then directly to
    //the Home page.
    Connections {
        target: welcomeScreenLoader.item
        onEnterClicked: {
            welcomeScreenLoader.active = false;
            mainWindowLoader.item.visible = true;
        }
    }
}
