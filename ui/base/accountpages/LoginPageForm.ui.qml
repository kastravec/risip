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

    property alias loginButton: loginButton
    property alias addSipServiceButton: addSipServiceButton
    property alias uernameInput: uernameInput
    property alias sipServicesInput: sipServicesInput
    property alias passwordInput: passwordInput
    property alias stackView: stackView

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginForm

        Pane {
            id: loginForm
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            ColumnLayout {
                id: inputLayout
                anchors.top: parent.top
                anchors.topMargin: 100
                spacing: 15
                anchors.horizontalCenter: parent.horizontalCenter

                ComboBox {
                    id: sipServicesInput
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                }

                TextField {
                    id: uernameInput
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    placeholderText: qsTr("Enter your username")
                }

                TextField {
                    id: passwordInput
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    echoMode: TextInput.Password

                    placeholderText: qsTr("Enter your password")
                }
            }

            RisipButton {
                id: loginButton
                width: 80
                anchors.topMargin: 20
                anchors.top: inputLayout.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                labelText.text: qsTr("Sign in")
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: false
            }

            RowLayout {
                id: bottomRowLayout

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVBottom
                spacing: 15

                Label {
                    text: qsTr("Add a new SIP Account service? ")
                }

                RisipButton {
                    id: addSipServiceButton
                    Layout.fillWidth: true
                    labelText.text: qsTr("Add SIP Service")
                }
            }
        }
    }
}
