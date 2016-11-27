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

Rectangle {
    id: root
    color: "#ffffff"
    border.width: 1
    border.color: "#000000"//"#db0000"

    //default margins of button box and text
    width: label.implicitWidth + 25
    height: label.implicitHeight + 15

    property alias text: label.text
    property alias icon: icon
    property alias font: label.font
    property bool checkable: false
    property bool checked: false

    property string iconNormal
    property string iconAlternate

    signal clicked

    RowLayout {
        spacing: 2
        anchors.fill: parent

        Label {
            id: label
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Image {
            id: icon
            source: root.iconNormal
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }

    onEnabledChanged: {
        if(enabled)
            root.state = "base";
        else
            root.state = "disabled"
    }

    states: [
        State {
            name: "base"
            PropertyChanges {
                target: root
                border.color: "#000000"
                opacity: 1.0
            }

            PropertyChanges {
                target: label
                color: "#000000"
            }

            PropertyChanges {
                target: icon
                source: root.iconNormal
            }
        },

        State {
            name: "pressed"
            PropertyChanges {
                target: root
                border.color: "#db0000"
            }

            PropertyChanges {
                target: label
                color: "#db0000"
            }

            PropertyChanges {
                target: icon
                source: root.iconAlternate
            }
        },

        State {
            name: "disabled"
            PropertyChanges {
                target: root
                opacity: 0.6
                border.color: "#6c1000"
            }

            PropertyChanges {
                target: label
                color: "#6c1000"
            }

            PropertyChanges {
                target: icon
                source: root.iconNormal
                opacity: 0.6
            }
        }
    ]

    MouseArea {
        id: eventHandler
        anchors.fill: root

        onClicked: {

            if(root.checkable) {
                root.checked = !root.checked;
                if(root.state === "pressed")
                    root.state = "base";
                else
                    root.state = "pressed";
            } else {
                root.clicked();
            }
        }

        onPressed: {
            if(!root.checkable) {
                root.state = "pressed";
            }
        }

        onReleased: {
            if(!root.checkable) {
                root.state = "base";
            }
        }
    }

}
