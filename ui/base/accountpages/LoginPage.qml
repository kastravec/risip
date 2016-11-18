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
import Risip 1.0

LoginPageForm {
    id: root
    sipServicesInput.model: Risip.accountNames

    property RisipAccount sipAccount: Risip.defaultAccount
    signal signedIn

    sipServicesInput.onActivated: {
        sipAccount = Risip.accountForUri(sipServicesInput.currentText);
        uernameInput.text = sipAccount.configuration.userName
        passwordInput.text = sipAccount.configuration.password

        Risip.setDefaultAccount(sipAccount.configuration.uri);
    }

    addSipServiceButton.onClicked: {
        addSipServicePageLoader.active = true;
        stackView.push(addSipServicePageLoader.item)
    }

    loginButton.onClicked: { sipAccount.login(); }

    Loader {
        id: addSipServicePageLoader
        source: "qrc:/ui/base/accountpages/AddSipServicePage.qml"
        active: false
    }

    Connections {
        target: addSipServicePageLoader.item

        onCancelClicked: {
            stackView.pop();
            addSipServicePageLoader.active = false;
        }

        onSipAccountAdded: {
            stackView.pop();
            addSipServicePageLoader.active = false;
        }
    }
}
