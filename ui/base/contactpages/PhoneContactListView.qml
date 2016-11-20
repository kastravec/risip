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
import QtGraphicalEffects 1.0

//import Risip 1.0

Item {
    id: root

    property alias model: contactListView.model

    signal contactClicked(var contactName)

    ListView {
        id: contactListView
        clip: true
        cacheBuffer: 500
        snapMode: ListView.SnapToItem
        anchors.fill: parent

        delegate: contactListViewDelegate
    }

    //delegate
    Component {
        id: contactListViewDelegate
        Rectangle {
            id: rectangle
            width: parent.width
            height: 70

            MouseArea {
                anchors.fill: parent
                onClicked:{ root.contactClicked(fullName); }
            }

            Rectangle {
                id: contactIconBox
                width: 45
                height: 45
                radius: width * 0.5
                border.width: 0
                border.color: "#000000" //"#db0000"
                opacity: 0.7
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
            }

            Text {
                id: initialsTxt
                anchors.centerIn: contactIconBox
                text: initials
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                textFormat: Text.PlainText
            }

            Image {
                id: contactPic
                width: 45
                height: 45
                anchors.centerIn: contactIconBox
                source: "image://contactIcon/" + fullName;
                visible: false;
            }

            Rectangle {
                id: contactPicMask
                width: 45
                height: 45
                radius: width * 0.5
                visible: false
                clip: true
                border.width: 0
                anchors.centerIn: contactIconBox
            }

            OpacityMask {
                id: opacityMask
                anchors.fill: contactPicMask
                source: contactPic
                maskSource: contactPicMask
            }

            Text {
                id: fullNameLabel
                text: fullName
                font.bold: true
                anchors.top: parent.top
                anchors.topMargin: 15
                anchors.leftMargin: 10
                anchors.left: contactIconBox.right
                textFormat: Text.PlainText
            }

            Text {
                id: countryLabel
                text: qsTr(" Country")
                font.pointSize: 9
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.leftMargin: 10
                anchors.left: contactIconBox.right
                textFormat: Text.PlainText
            }
        }
    }
}
