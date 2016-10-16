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

Item {
    id: item1
    width: 400
    height: 600
    property alias networkTypeInput: networkTypeInput
    property alias proxyServerInput: proxyServerInput
    property alias serverAddressInput: serverAddressInput
    property alias localPortInput: localPortInput
    property alias saveButton: saveButton
    property alias cancelButton: cancelButton
    property alias passwordInput: passwordInput
    property alias usernameInput: usernameInput

    ColumnLayout {
        id: mainColumnLayout
        anchors.verticalCenter: parent.verticalCenter
        spacing: 15
        anchors.horizontalCenter: parent.horizontalCenter

        TextField {
            id: usernameInput
            Layout.fillWidth: true
            placeholderText: qsTr("SIP Username e.g tom")
        }

        TextField {
            id: passwordInput
            placeholderText: qsTr("Password")
        }

        TextField {
            id: serverAddressInput
            placeholderText: qsTr("SIP Server address or IP")
        }

        TextField {
            id: proxyServerInput
            placeholderText: qsTr("Proxy Server or IP")
        }

        ComboBox {
            id: networkTypeInput
            textRole: ""
            Layout.fillWidth: true
            model: ["UDP", "TCP", "TLS"]
            currentIndex: 0
        }

        TextField {
            id: localPortInput
            Layout.fillWidth: true
            placeholderText: qsTr("Local Port e.g -1 for random")
        }

        RowLayout {
            id: buttonRowLayout

            spacing: 10
            Button {
                id: cancelButton
                text: qsTr("Cancel")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignLeft
                Layout.fillWidth: false
            }

            Button {
                id: saveButton
                text: qsTr("Save")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignRight
                Layout.fillWidth: false
            }
        }

    }
}
