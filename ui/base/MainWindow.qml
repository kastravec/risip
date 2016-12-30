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

    Loader {
        id: accountRegistrationLoader
        source: uiBasePath + "/accountpages/" + "UserRegistration.qml";
        active: false
        asynchronous: true
        anchors.fill: parent
    }

    RisipBuddy {
        id: petiBuddy
        contact: "toptop"
    }

    Connections {
        target: accountRegistrationLoader.item

        onAccountRegistered: {
            if(sipAccount.status === RisipAccount.SignedIn) {
                loginPageLoader.active = false;
                mainPageLoader.item.visible = true;
            } else {
                loginPageLoader.active = true;
                mainPageLoader.item.visible = false;
            }

            accountRegistrationLoader.active = false;
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
                if(firstRun)
                    accountRegistrationLoader.active = true;
                else
                    loginPageLoader.active = true;

                mainPageLoader.item.visible = false; //main page is not visible because account is not signed int
            }

            welcomeScreenLoader.active = false; //disabling welcome screen on enter clicked
        }
    }

    //connection on active SIP account to handle different account states
    Connections {
        target: sipAccount

        //handle signed in/out and errors
        onStatusChanged: {
            if(sipAccount.status === RisipAccount.SignedIn) {
                loginPageLoader.active = false;
                mainPageLoader.item.visible = true;
                sipAccount.addBuddy(petiBuddy);
                petiBuddy.sendInstantMessage("koooott");
            } else if(Risip.defaultAccount.status === RisipAccount.SignedOut) {
                loginPageLoader.active = true;
                mainPageLoader.item.visible = false;
            }
        }

        onIncomingMessage: {
            var msg = message;
            console.log(msg.messageBody);
        }
    }
}
