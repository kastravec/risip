import QtQuick 2.7
import QtQuick.Window 2.2

import Risip 1.0
import "../base"

Item {
    id: root

    SplashScreen {
        id: splashScreen
        onTimeout: loginPageLoader.visible = true
    }

    Loader {
        id: mainWindowLoader
        source: "qrc:/ui/base/MainWindow.qml"
        active: false
    }

    Loader {
        id: loginPageLoader
        source: "qrc:/ui/base/LoginPage.qml"
        active: true
        visible: false
    }

    RisipEndpoint {
        id: myEndpoint
        Component.onCompleted: { myEndpoint.startEngine(); }
    }

    RisipAccount {
        id: myAccount
        sipEndPoint: myEndpoint

        onStatusChanged: {
            switch (status) {
            case RisipAccount.SignedIn:
                console.log("Logged in!")
                myAccount.presence = RisipBuddy.Online
                myBuddy.addToList();
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

        onIncomingMessage: {
            console.log("incoming message: from: "
                        + message.buddy.uri
                        + ""+ message.messageBody) ;
        }
    }

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
