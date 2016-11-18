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

import "settingspages"
import "callpages"
import "contactpages"
import "accountpages"

import Risip 1.0

ApplicationWindow {
    id: mainWindow
    width: 720
    height: 1280

    visibility: Window.AutomaticVisibility
    visible: false

    Material.theme: Material.Light
    Material.background: "#FFFFFF"
    Material.accent: "#db0000" //"#DB5AB9"
    Material.foreground: "#000000"
    Material.primary: "#FFFFFF"

    property string uiBasePath: "qrc:/ui/base/"
    property RisipEndpoint sipEndpoint: Risip.sipEndpoint
    property RisipAccount sipAccount: Risip.defaultAccount

    Component.onCompleted: { sipEndpoint.start(); RisipGeoPositionProvider.start();  }
    Component.onDestruction: { sipEndpoint.stop(); }

    header: MainToolBar { id: mainToolBar }
    footer: MainTabBar { id: mainTabBar }

    StackLayout {
        id: stackLayout
        currentIndex: mainTabBar.currentIndex
        anchors.fill: mainWindow.contentItem

        PageLoader {
            id: contactsPageLoader
            source: uiBasePath + "/contactpages/" + "ContactsPage.qml"
            active: true
        }

        PageLoader {
            id: dialPageLoader
            source: uiBasePath + "/callpages/" + "DialPage.qml"
            active: true
        }

        PageLoader {
            id: homePageLoader
            source: uiBasePath + "HomePage.qml"
            active: true
        }

        PageLoader {
            id: settingsPageLoader
            source: uiBasePath + "/settingspages/" + "SettingsPage.qml"
            active: true
        }
    }

    PageLoader {
        id: loginPageLoader
        source: uiBasePath + "/accountpages/" + "LoginPage.qml"
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
        onPresenceChanged: {
            console.log("BUDDY TOPTOP PRESENCE: " + presence);
        }
    }

    RisipBuddy {
        id: buddy2
        uri: "<sip:topatop@sip2sip.info>"
        onPresenceChanged: {
            console.log("BUDDY TOPATOP PRESENCE: " + presence);
        }
    }

    onVisibleChanged: {
        if(Risip.defaultAccount.status !== RisipAccount.SignedIn) {
            loginPageLoader.visible = true;
            mainTabBar.enabled = false;
        }
    }

    //connection on active SIP account to handle different account states
    Connections {
        target: sipAccount

        //handle signed in/out and errors
        onStatusChanged: {
            if(Risip.defaultAccount.status === RisipAccount.SignedIn) {
                loginPageLoader.active = false;
                mainWindow.footer.enabled = true;
                sipAccount.addRisipBuddy(buddy2);
                sipAccount.addRisipBuddy(buddy);
            } else if(Risip.defaultAccount.status === RisipAccount.SignedOut) {
                loginPageLoader.active = true;
                mainWindow.footer.enabled = false;
            }
        }
    }

    Connections {
        target: RisipGeoPositionProvider

        onCountryCodeChanged: {
            console.log("COUNTRY CODE: " + RisipGeoPositionProvider.countryCode);
        }
    }
}
