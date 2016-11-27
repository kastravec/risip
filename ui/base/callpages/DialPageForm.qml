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
    id: item1

    property alias callButton: callButton
    property alias contactInput: contactInput

    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        TextField {
            id: contactInput
            placeholderText: qsTr("Someone you want to talk to ?")
            Layout.preferredHeight: 40
            Layout.preferredWidth: 322
        }

        RisipButton {
            id: callButton
            text: qsTr("Call")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }

    RowLayout {
        y: 527
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        Label {
            id: label1
            text: qsTr("Note: ")
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: 12
        }

        TextArea {
            id: textArea1
            text: qsTr("Simply enter the username of the contact you want to call of the same SIP registrar. ")
            font.pointSize: 11
            font.italic: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            wrapMode: Text.WordWrap
        }
    }
}
