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
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.0

import "../risipcomponents"

Page {
    id: root
    z: 1
    width: 720
    height: 1280

    Material.theme: Material.Light
    Material.background: "#FFFFFF"
    Material.accent: "#db0000" //"#DB5AB9"
    Material.foreground: "#000000"
    Material.primary: "#FFFFFF"

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
            width: parent.width *0.80
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            ColumnLayout {
                id: inputLayout
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                ComboBox {
                    id: sipServicesInput
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                }

                LineEdit {
                    id: uernameInput
                    height: 40
                    frameBorder: 0
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    placeholderText: qsTr("Username")
                    verticalBottomLine: true
                }

                LineEdit {
                    id: passwordInput
                    height: 40
                    frameBorder: 0
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                    verticalBottomLine: true
                    placeholderText: qsTr("Password")
                }
            }

            RisipButton {
                id: loginButton
                width: 80
                anchors.topMargin: 20
                anchors.top: inputLayout.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Sign in")
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: false
            }

            RowLayout {
                id: bottomRowLayout

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVBottom
                spacing: 15

                RisipButton {
                    id: registerButton
                    Layout.fillWidth: true
                    text: qsTr("Register your free account")
                }

                RisipButton {
                    id: addSipServiceButton
                    Layout.fillWidth: true
                    text: qsTr("Add SIP Service")
                }
            }
        }
    }
}
