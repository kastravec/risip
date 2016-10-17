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
import QtQuick.Window 2.1
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
//    Material.color:
    Material.accent: Material.Purple

    property string uiBasePath: "qrc:/ui/base/"
    property RisipApplication risipApp: RisipApplication {}

    Component.onCompleted: { risipApp.start(); }
    Component.onDestruction: { risipApp.stop(); }

    footer: TabBar {
        id: mainTabBar
        currentIndex: 1
        TabButton { text: qsTr("Contacts") }
        TabButton { text: qsTr("Home") }
        TabButton { text: qsTr("Dial") }
        TabButton { text: qsTr("Settings") }
    }

    Loader {
        id: welcomeScreenLoader
        active: true
        source: uiBasePath + "WelcomeScreen.qml"
        width: root.width
        height: root.height
        asynchronous: true
        z:1
    }

    Loader {
        id: loginPageLoader
        source: uiBasePath + "LoginPage.qml"
        active: false
        anchors.fill: root.contentItem
    }

    Loader {
        id: addSipServicePageLoader
        source: uiBasePath + "AddSipServicePage.qml"
        active: false
        anchors.fill: root.contentItem
    }

    StackLayout {
        id: stackLayout
        currentIndex: mainTabBar.currentIndex
        anchors.fill: root.contentItem

        Loader {
            id: contactsPageLoader
            source: uiBasePath + "ContactsPage.qml"
            active: false
            asynchronous: true
        }

        Loader {
            id: homePageLoader
            source: uiBasePath + "HomePage.qml"
            active: false
            asynchronous: true
        }

        Loader {
            id: dialPageLoader
            source: uiBasePath + "DialPage.qml"
            active: false
            asynchronous: true
        }

        Loader {
            id: settingsPageLoader
            source: uiBasePath + "SettingsPage.qml"
            active: false
            asynchronous: true
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

    Connections {
        target: welcomeScreenLoader.item

        onEnterClicked: {
            welcomeScreenLoader.active = false;
            root.footer.enabled = false;

            loginPageLoader.active = true;

            contactsPageLoader.active = true;
            homePageLoader.active = true;
            dialPageLoader.active = true;
            settingsPageLoader.active = true;
        }
    }

    Connections {
        target: risipApp
        onReadyChanged: {console.log("READY!")}
    }
}
