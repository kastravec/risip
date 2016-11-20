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
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

import Risip 1.0

ApplicationWindow {
    id: root
    width: 720
    height: 1280
    visibility: Window.AutomaticVisibility
    visible: true

//    Material.theme: Material.Light
//    Material.background: "#FFFFFF"
//    Material.accent: "#db0000" //"#DB5AB9"
//    Material.foreground: "#000000"
//    Material.primary: "#FFFFFF"

    property string uiBasePath: "qrc:/ui/base/"
    property bool firstRun: Risip.firstRun
    property RisipAccount sipAccount: Risip.defaultAccount

    Component.onCompleted: {
        if(firstRun) {
            welcomeScreenLoader.active = true;
        } else {
            if(sipAccount.status === RisipAccount.SignedIn) {
                loginPageLoader.active = false;
                mainPageLoader.visible = true;
            } else {
                loginPageLoader.active = true;
                mainPageLoader.visible = false;
            }
        }
    }

    Loader {
        id: mainPageLoader
        source: uiBasePath + "MainPage.qml"
        active: true
        anchors.fill: parent
    }

    //welcome screen is unloaded once is first used.
    // active only on first run.
    Loader {
        id: welcomeScreenLoader
        source: uiBasePath + "WelcomeScreen.qml"
        active: false
        anchors.fill: parent
    }

    Loader {
        id: loginPageLoader
        source: uiBasePath + "/accountpages/" + "LoginPage.qml"
        active: false
        asynchronous: true
        anchors.fill: parent
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

    RisipBuddy {
        id: buddy3
        uri: "<sip:pasaatop@sip2sip.info>"
        onPresenceChanged: {
            console.log("BUDDY TOPATOP PRESENCE: " + presence);
        }
    }

    RisipBuddy {
        id: buddy4
        uri: "<sip:opop@sip2sip.info>"
        onPresenceChanged: {
            console.log("BUDDY TOPATOP PRESENCE: " + presence);
        }
    }
    //welcome screen singal handler, from which we go to either
    //the Login page or if autologin is enabled then directly to
    //the Home page.
    Connections {
        target: welcomeScreenLoader.item
        onEnterClicked: {
            if(sipAccount.status === RisipAccount.SignedIn) {
                loginPageLoader.active = false;
                mainPageLoader.item.visible = true;
            } else {
                loginPageLoader.active = true;
                mainPageLoader.item.visible = false;
            }

            welcomeScreenLoader.active = false;
        }
    }

    //connection on active SIP account to handle different account states
    Connections {
        target: sipAccount

        //handle signed in/out and errors
        onStatusChanged: {
            if(Risip.defaultAccount.status === RisipAccount.SignedIn) {
                loginPageLoader.active = false;
                mainPageLoader.item.visible = true;
                sipAccount.addBuddy(buddy);
                sipAccount.addBuddy(buddy2);
                sipAccount.addBuddy(buddy3);
                sipAccount.addBuddy(buddy2);
                sipAccount.addBuddy(buddy4);
                sipAccount.addBuddy(buddy4);

            } else if(Risip.defaultAccount.status === RisipAccount.SignedOut) {
                loginPageLoader.active = true;
                mainPageLoader.item.visible = false;
            }
        }
    }
}
