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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1

import "../base"

//importing the Risip QML module - pjsip wrapper api
import Risip 1.0

ApplicationWindow {
    id: mainWindow

    width: 720
    height: 1280

    visibility: Window.AutomaticVisibility
    visible: true

    //risipaccount is a SIP account/user that is logged in and can chat, make/receive calls
    property RisipAccount myAccount: Risip.defaultAccount

    // object for handling the incoming/outgoing calls
    property RisipCall activeCall

    // ===== UI PART ====

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
                //checking if sip endpoint is started!
                if(Risip.sipEndpoint.status === RisipEndpoint.Started) {
                    myAccount = Risip.accountForConfiguration(myAccountConfiguration) //Using the Risip API to get/create the account
                    //signing in using the given configuration
                    Risip.setDefaultAccount(myAccount.configuration.uri);
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
            Label { text: Risip.sipEndpoint.status }
        }

        RowLayout {
            Label { text: "Account status: " }
            Label { text: myAccount.statusText }
        }

        RowLayout {
            TextField {
                id: contactUriInput
                placeholderText: "sip account to call"
            }

            Button {
                text: "Call"
                onClicked: { mainWindow.activeCall = RisipCallManager.callContact(contactUriInput.text); }
            }

            Button {
                id: answerButton
                text: "Answer"
                enabled: false

                onClicked: {
                    console.log("Call status: " + activeCall.status)
                    activeCall.answer();
                    answerButton.highlighted = false;
                    answerButton.enabled = false;
                }
            }

            Button {
                text: "End"
                onClicked: {
                    activeCall.hangup();
                    answerButton.highlighted = false; answerButton.enabled = false;
                }
            }

            Button {
                text: "Send test IM"
                onClicked: { myBuddy.sendInstantMessage("message from risip client..uiiiiii"); }
            }
        }
    }

    // ====== Application Logic  using the Risip APIs ======

    //starting the sip endpoint engine
    //endpoint represents the SIP/Voip client / in practice a pjsip library instance
    Component.onCompleted: { Risip.sipEndpoint.start(); }
    Component.onDestruction: { Risip.sipEndpoint.stop(); }

    Connections {
        target: RisipCallManager

        onIncomingCall: {
            mainWindow.activeCall = call;
            answerButton.enabled = true;
            answerButton.highlighted = true;
        }

        onOutgoingCall: {
            mainWindow.activeCall = call;
            answerButton.enabled = false;
        }
    }

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

        onIncomingMessage: { console.log("incoming message: from: "
                                         + message.buddy.uri
                                         + message.messageBody) ; }
    }

    //a simple sip contact (within same sip sip2sip.info server)
    RisipBuddy {
        id: myBuddy
        contact: contactUriInput.text
        account: myAccount;
    }

    //account details/settings, in order to login.
    // for this demo a free SIP Service is used , check the the sip2sip.info, create sip accounts there
    // and simply use Risip to make your own clients.
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
