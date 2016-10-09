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
import QtQuick.Layouts 1.3
import "../base"

//importing the Risip QML module - pjsip wrapper api
import Risip 1.0

Item {
    id: root

    property RisipEndpoint mySipEndpoint: Risip.sipEndpoint //sip endpoint - sip client instance
    property RisipAccount myAccount //risipaccount is a SIP account/user that is logged in and can chat, make/receive calls
    property RisipCall incomingCall // object for handling the incoming calls
    property RisipCall outgoingCall //object for handling the outgoing calls

    SplashScreen {
        id: splashScreen
        onTimeout: mainWindow.visible = true
    }

    // ===== UI PART ====

    MainWindow {
        id: mainWindow

        ColumnLayout {
            anchors.centerIn: mainWindow.contentItem

            spacing: 20

            TextField {
                id: userNameInput
                placeholderText: "Username";
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
                text: qsTr("Register")
                onClicked: {
                    if(mySipEndpoint.status === RisipEndpoint.Started) {
                        myAccount = Risip.getAccount(myAccountConfiguration) //Using the Risip API to get/create the account
                        myAccount.login();
                    }
                }
            }

            Button {
                text: qsTr("Unregister")
                onClicked: { myAccount.logout(); }
            }

            RowLayout {
                Label { text: "Engine status: "}
                Label { text: mySipEndpoint.status }
            }

            RowLayout {
                Label { text: "Account status: " }
                Label { text: myAccount.statusText }
            }

            RowLayout {
                TextField {
                    id: conctactUriInput
                    placeholderText: "sip account to call"
                }

                Button {
                    text: "Call"
                    onClicked: { root.outgoingCall = myBuddy.call(); }
                }

                Button {
                    id: answerButton
                    text: "Answer"
                    enabled: false

                    onClicked: {
                        console.log("Call status: " + incomingCall.status)
                        incomingCall.answer();
                        answerButton.highlighted = false;
                        answerButton.enabled = false;
                    }
                }

                Button {
                    text: "End"
                    onClicked: {
                        outgoingCall.hangup(); incomingCall.hangup();
                        answerButton.highlighted = false; answerButton.enabled = false; }
                }

                Button {
                    text: "Send test IM"
                    onClicked: { myBuddy.sendInstantMessage("uiiiii"); }
                }
            }

            Label { text: "Call History: " }

            CallHistoryListView {
                id: callHistoryList
                model: myAccount.callHistoryModel
            }
        }
    }

    // ====== Application Logic  using the Risip APIs ======

    //starting the sip endpoint engine
    //endpoint represents the SIP/Voip client / in practice a pjsip library instance
    Component.onCompleted: { mySipEndpoint.startEngine(); }

    //handling singals from the sip account instance
    Connections {
        target: myAccount

        onStatusChanged: {
            switch (status) {
            case RisipAccount.SignedIn:
                console.log("Logged in!")
                myAccount.presence = RisipBuddy.Online
                myBuddy.addToAccount();
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

        onIncomingCall: {
            answerButton.enabled = true;
            answerButton.highlighted = true;
            root.incomingCall = call;
        }

        onIncomingMessage: { console.log("incoming message: from: "
                                         + message.buddy.uri
                                         + message.messageBody) ; }
    }

    //a simple sip contact (within same sip sip2sip.info server)
    RisipBuddy {
        id: myBuddy
        uri: "<sip:toptop@sip2sip.info>"
        account: myAccount;
    }

    //account details/settings, in order to login.
    RisipAccountConfiguration {
        id: myAccountConfiguration
        account: myAccount
        scheme: "digest"
        userName: userNameInput.text
        password: passwordInput.text
        serverAddress: serverAddressInput.text.trim(); //"sip2sip.info" free sip server
        proxyServer: proxyInput.text.trim(); //"proxy.sipthor.net" //always use proxy to be sure for connectivity
        randomLocalPort: true //false
        //            localPort: 5060
        networkProtocol: RisipAccountConfiguration.UDP
    }
}

//"88.198.93.189"
