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
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

import Risip 1.0

ApplicationWindow {
    id: root
    width: 720
    height: 1280

    visibility: Window.AutomaticVisibility
    visible: true

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    property string uiBasePath: "qrc:/ui/base/"
    property RisipEndpoint sipEndpoint: Risip.sipEndpoint

    Component.onCompleted: { sipEndpoint.start(); }
    Component.onDestruction: { sipEndpoint.stop(); }

    footer: TabBar {
        id: mainTabBar
        currentIndex: 1
        TabButton { text: qsTr("Contacts") }
        TabButton { text: qsTr("Home") }
        TabButton { text: qsTr("Dial") }
        TabButton { text: qsTr("Settings") }
    }

    StackLayout {
        id: stackLayout
        currentIndex: mainTabBar.currentIndex
        anchors.fill: root.contentItem

        PageLoader {
            id: contactsPageLoader
            source: uiBasePath + "ContactsPage.qml"
            active: true
        }

        PageLoader {
            id: homePageLoader
            source: uiBasePath + "HomePage.qml"
            active: true
        }

        PageLoader {
            id: dialPageLoader
            source: uiBasePath + "DialPage.qml"
            active: true
        }

        PageLoader {
            id: settingsPageLoader
            source: uiBasePath + "SettingsPage.qml"
            active: true
        }
    }

    //TODO ?
    PageLoader {
        id: addSipServicePageLoader
        source: uiBasePath + "AddSipServicePage.qml"
        active: false
    }

    Loader {
        id: splashScreenLoader
        source: uiBasePath + "SplashScreen.qml"
        active: true
        width: root.width
        height: root.height
    }

    PageLoader {
        id: loginPageLoader
        source: uiBasePath + "LoginPage.qml"
        active: true
        visible: false
    }

    //welcome screen is unloaded once is first used.
    // active only on first run.
    Loader {
        id: welcomeScreenLoader
        source: uiBasePath + "WelcomeScreen.qml"
        active: firstRun //firstRun is set in settings always true for testing
        visible: true
        width: root.width
        height: root.height
        z:1
    }

    Connections {
        target: splashScreenLoader.item
        onTimeout: {
            if(Risip.firstRun) {
                welcomeScreenLoader.visible = true;
            } else {
                if(Risip.defaultAccount.status === RisipAccount.SignedIn)
                    root.visible = true;
                else
                    loginPageLoader.visible = true;
            }

            splashScreenLoader.active = false;
        }
    }

    Connections {
        target: welcomeScreenLoader.item

        onEnterClicked: {
            welcomeScreenLoader.active = false;
            if(Risip.defaultAccount.status === RisipAccount.SignedIn)
                mainWindowLoader.item.visible = true;
            else {
                loginPageLoader.visible = true;
                root.footer.enabled = false;
            }
        }
    }

    Connections {
        target: homePageLoader.item

        onSignedOut: {
            loginPageLoader.active = true;
            root.footer.enabled = false;
        }
    }

    Connections {
        target: loginPageLoader.item

        onSignedIn: {
            loginPageLoader.active = false;
            root.footer.enabled = true;
        }

        onAddSipService: {
            addSipServicePageLoader.active = true;
            loginPageLoader.visible = false;
        }
    }

    Connections {
        target: addSipServicePageLoader.item

        onSipAccountAdded: {
            addSipServicePageLoader.active = false;
            loginPageLoader.visible = true;
        }

        onCanceled: {
            addSipServicePageLoader.active = false;
            loginPageLoader.visible = true;
        }
    }
}
