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
    id: mainWindow
    width: 720
    height: 1280

    visibility: Window.AutomaticVisibility
    visible: false

    property string uiBasePath: "qrc:/ui/base/"
    property RisipEndpoint sipEndpoint: Risip.sipEndpoint
    property RisipAccount sipAccount: Risip.defaultAccount

    Component.onCompleted: { sipEndpoint.start(); RisipGeoPositionProvider.start();  }
    Component.onDestruction: { sipEndpoint.stop(); }

    header: ToolBar {
        id: mainToolBar

        RowLayout {
            anchors.fill: parent

            Label {
                text: qsTr(" Risip")
                Layout.alignment: Qt.AlignCenter | Qt.AlignVCenter
            }
        }
    }

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
        anchors.fill: mainWindow.contentItem

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

    PageLoader {
        id: loginPageLoader
        source: uiBasePath + "LoginPage.qml"
        active: true
        visible: false
    }

    CallPage {
        id: callPage
        anchors.fill: mainWindow.contentItem
    }

    RisipBuddy {
        id: buddy
        uri: "<sip:toptop@sip2sip.info>"
    }

    RisipBuddy {
        id: buddy2
        uri: "<sip:topatop@sip2sip.info>"
    }

    onVisibleChanged: {
        if(Risip.defaultAccount.status != RisipAccount.SignedIn) {
            loginPageLoader.visible = true;
            mainWindowLoader.item.footer.enabled = false;
        }
    }

    //connection on active SIP account to handle different account states
    Connections {
        target: sipAccount

        //handle signed in/out and errors
        onStatusChanged: {
            if(Risip.defaultAccount.status === RisipAccount.SignedIn) {
                sipAccount.addRisipBuddy(buddy2);
                sipAccount.addRisipBuddy(buddy);
            }
        }
    }

    //Signal handler for Home page.
    Connections {
        target: homePageLoader.item

        onSignedOut: {
            loginPageLoader.active = true;
            mainWindow.footer.enabled = false;
        }
    }

    //Handling all signals from the Login Page
    Connections {
        target: loginPageLoader.item

        onSignedIn: {
            loginPageLoader.active = false;
            mainWindow.footer.enabled = true;
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
        target: RisipGeoPositionProvider

        onCountryCodeChanged: {
            console.log("COUNTRY CODE: " + RisipGeoPositionProvider.countryCode);
        }
    }
}
