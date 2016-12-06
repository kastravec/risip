/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
**    http://risip.io
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
import QtQuick.Layouts 1.0

import "../risipcomponents"

Page {
    id: root

    property alias registerButton: registerButton
    property alias emailInput: emailInput
    property alias usernameInput: usernameInput
    property alias passwordInput: passwordInput

    signal registerButtonClicked
    signal backClicked

    header: ToolBar {

        background: Rectangle {
            implicitHeight: 40
            color: "#ffffff"

            Line {
                id: horizontalLine
                anchors.bottom: parent.bottom
            }
        }

        ToolButton {
            id: backButton
            text: qsTr("Back")
            onClicked: root.backClicked()
        }
    }

    Label {
        id: label
        text: qsTr("Email associated with this account.")
        anchors.bottom: columnLayout.top
        anchors.bottomMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ColumnLayout {
        id: columnLayout
        width: parent.width *0.65
        anchors.verticalCenter: parent.verticalCenter
        spacing: 15
        anchors.horizontalCenter: parent.horizontalCenter

        TextField {
            id: usernameInput
            height: 37
            Layout.fillWidth: true
            placeholderText: qsTr("Username")
        }

        TextField {
            id: emailInput
            Layout.fillWidth: true
            placeholderText: qsTr("Email address")
        }

        TextField {
            id: passwordInput
            Layout.fillWidth: true
            echoMode: TextInput.Password
            placeholderText: qsTr("Password")
        }
    }

    RisipButton {
        id: registerButton
        text: "Register"
        anchors.topMargin: 40
        anchors.top: columnLayout.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked: root.registerButtonClicked();
    }

}
