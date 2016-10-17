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
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 400
    height: 400
    property alias userNameLabel: userNameLabel
    property alias statusLabel: statusLabel
    property alias signOutButton: signOutButton

    Pane {
        id: pane1
        x: 15
        width: 370
        height: 145
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        RowLayout {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            spacing: 30

            Label {
                id: userNameLabel
                text: qsTr("User")
            }

            Label {
                id: statusLabel
                text: qsTr("status")
                font.italic: true
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                id: signOutButton
                text: qsTr("Sign out")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
        }

    }

}
