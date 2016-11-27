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
import QtQuick.Layouts 1.3

Rectangle {
    id: root
    width: parent.width

    property alias text: searchInput.text
    property alias placeholderText: placeHolder.text

    Rectangle {
        id: frame
        border.width: 1
        width: parent.width - 20
        height: parent.height - 5
        anchors.centerIn: parent

        Image {
            id: searchIcon
            width: 22
            height: 22
            source: "qrc:/images/icons/128/MagnifyingGlassBlack.png"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }

        TextInput {
            id: searchInput
            anchors.left: searchIcon.right
            anchors.leftMargin: 10
            anchors.right: clearIcon.left
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            clip: true

            Text {
                id: placeHolder
                text: qsTr("Search")
                opacity: 0.5
                anchors.centerIn: searchInput
            }

            onTextChanged: {
                if(text.length > 0)
                    root.state = "base"; //placeHolder.visible = false;
                else
                    root.state = "active"; //placeHolder.visible = true;
            }
        }

        Image {
            id: clearIcon
            width: 12
            height: 12
            source: "qrc:/images/icons/128/CancelBlack.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10

            MouseArea {
                anchors.fill: parent
                anchors.margins: 20
                onPressed: { clearIcon.source = "qrc:/images/icons/128/CancelRed.png"; }
                onReleased: { clearIcon.source = "qrc:/images/icons/128/CancelBlack.png"; }
                onClicked:{
                    searchInput.clear();
                }
            }
        }
    }
    states: [
        State {
            name: "active"

            PropertyChanges {
                target: frame
                border.color: "#db0000"
            }

            PropertyChanges {
                target: placeHolder
                visible: false
            }

            PropertyChanges {
                target: searchIcon
                source: "qrc:/images/icons/128/MagnifyingGlassRed.png"
            }
        }
    ]

    onTextChanged: {
        if(text)
            root.state = "active"; //searchIcon.source = "qrc:/images/icons/128/MagnifyingGlassRed.png";
        else
            root.state = "base"; //searchIcon.source = "qrc:/images/icons/128/MagnifyingGlassBlack.png";
    }
}
