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
**    A copy of the license is also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/

import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../base"

//importing the Risip QML module - pjsip wrapper api
import Risip 1.0

Item {
    id: root

    SplashScreen {
        id: splashScreen
        onTimeout: mainWindow.visible = true
    }

    MainWindow {
        id: mainWindow

        // ===== UI PART ====
        ColumnLayout {
            anchors.centerIn: mainWindow.contentItem

            spacing: 20

            TextField {
                id: userNameInput
                placeholderText: "Username"
                text: "topatop"
            }

            TextField {
                id: passwordInput
                placeholderText: "Password"
                echoMode: TextInput.Password
            }

            TextField {
                id: serverAddressInput
                placeholderText: "SIP Server (e.g. sip2sip.info)"
                text: "sip2sip.info"
            }

            TextField {
                id: proxyInput
                placeholderText: "SIP Proxy"
                text: "proxy.sipthor.net"
            }

            Button {
                id: registerButton
                text: qsTr("Register")

                onClicked: {
                    if(myEndpoint.status === RisipEndpoint.Started)
                        myAccount.login();
                }
            }

            Button {
                id: unregisterButton
                text: qsTr("Unregister")

                onClicked: { myAccount.logout(); }
            }

            RowLayout {
                Label { text: "Engine status: "}
                Label { text: myEndpoint.status }
            }

            RowLayout {
                Label { text: "Account status: " }
                Label { text: myAccount.statusText }
            }

            RowLayout {
                TextField {
                    id: calleeURI
                    placeholderText: "sip account to call"
                }

                Button {
                    text: "Call"
                    onClicked: { myCall.makeCall(calleeURI.text); }
                }

                Button {
                    id: answerButton
                    text: "Answer"
                    enabled: false

                    onClicked: {
                        console.log("Call status: " + myCall.status)
                        myCall.answer();
                        answerButton.highlighted = false;
                        answerButton.enabled = false;
                    }
                }

                Button {
                    text: "End"
                    onClicked: { myCall.hangup(); answerButton.highlighted = false; answerButton.enabled = false; }
                }
            }
        }

        // ====== Application Logic ======

        //endpoint represents the SIP/Voip client / in practice a pjsip library instance
        RisipEndpoint {
            id: myEndpoint
            Component.onCompleted: { myEndpoint.startEngine(); }
        }

        //risipaccount is a SIP account/user that is logged in and can chat, make/receive calls
        RisipAccount {
            id: myAccount

            //associating the account with the pjsip endpoint/library instance
            sipEndPoint: myEndpoint

            //status is a property of the account that represent the state of it
            onStatusChanged: {
                switch (status) {
                case RisipAccount.SignedIn:
                    console.log("Logged in!")
                    break;
                case RisipAccount.SignedOut:
                    console.log("Logged out!")
                    break;
                case RisipAccount.NotConfigured:
                    console.log("Account details/setting missing? !")
                    break;
                case RisipAccount.AccountError:
                    console.log("Some error..better restart!")
                    break;
                }
            }
        }

        RisipCall {
            id: myCall
            account: myAccount
            onIncomingCall: {
                answerButton.enabled = true;
                answerButton.highlighted = true;
            }
        }

        //account details/settings, in order to login.
        RisipAccountConfiguration {
            id: myAccountConfiguration
            account: myAccount
            scheme: "digest"
            userName: userNameInput.text
            password: passwordInput.text
            serverAddress: "sip2sip.info" //free sip server
            proxyServer: "proxy.sipthor.net" //always use proxy to be sure for connectivity
            randomLocalPort: false
            localPort: 5060
            networkProtocol: RisipAccountConfiguration.UDP
        }

    }
}
//"88.198.93.189"
