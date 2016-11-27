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

import "../risipcomponents"

Page {
    id: root
    property alias networkTypeInput: networkTypeInput
    property alias proxyServerInput: proxyServerInput
    property alias serverAddressInput: serverAddressInput
    property alias localPortInput: localPortInput
    property alias passwordInput: passwordInput
    property alias usernameInput: usernameInput

    signal cancelClicked
    signal saveClicked

    header: Item {
        focus: true

        Row {
            id: rowLayout
            spacing: 2

            Arrow {
                orientation: "left"
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
            }

            Label {
                text: qsTr("Cancel")
                MouseArea {
                    anchors.fill: parent
                    onClicked: root.cancelClicked();
                }
            }
        }

        Label {
            text: qsTr("Save")
            anchors.right: parent.right
            anchors.rightMargin: 5

            MouseArea {
                anchors.fill: parent
                onClicked: root.saveClicked();
            }
        }

    }

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
            Layout.fillWidth: true
            placeholderText: qsTr("Password")
            echoMode: TextInput.Password
        }

        TextField {
            id: serverAddressInput
            Layout.fillWidth: true
            placeholderText: qsTr("SIP Server address or IP")
        }

        TextField {
            id: proxyServerInput
            Layout.fillWidth: true
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
    }
}
