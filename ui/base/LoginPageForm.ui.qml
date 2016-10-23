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

Page {
    id: item1

    property alias loginButton: loginButton
    property alias addSipServiceButton: addSipServiceButton
    property alias uernameInput: uernameInput
    property alias signedInCheckbox: signedInCheckbox
    property alias sipServicesInput: sipServicesInput
    property alias passwordInput: passwordInput

    ColumnLayout {
        anchors.bottom: bottomRowLayout.top
        anchors.bottomMargin: 135
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 15

        TextField {
            id: uernameInput
            Layout.fillWidth: true
            placeholderText: qsTr("Enter your username")
        }

        TextField {
            id: passwordInput
            Layout.fillWidth: true
            echoMode: TextInput.Password

            placeholderText: qsTr("Enter your password")
        }

        ComboBox {
            id: sipServicesInput
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.preferredWidth: 200
        }

        Button {
            id: loginButton
            text: qsTr("Sign in")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
        }

        CheckBox {
            id: signedInCheckbox
            text: qsTr("Stay signed in")
            checked: true
        }
    }

    RowLayout {
        id: bottomRowLayout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        spacing: 15

        Label {
            id: label1
            text: qsTr("Add a new SIP Account service? ")
        }

        Button {
            id: addSipServiceButton
            text: qsTr("Add SIP Service")
        }
    }
}
