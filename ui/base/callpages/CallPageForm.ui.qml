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

import "../risipcomponents"

Page {
    id: page1

    property alias endCallButton: endCallButton
    property alias answerCallButton: answerCallButton
    property alias usernameLabel: usernameLabel
    property alias statusLabel: statusLabel
    property alias micButton: micButton
    hoverEnabled: true

    Label {
        id: statusLabel
        text: qsTr("status")
        font.pointSize: 15
        font.italic: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Label {
        id: usernameLabel
        text: "User"
        font.bold: true
        font.pointSize: 20
        anchors.bottom: statusLabel.top
        anchors.bottomMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
    }

    RowLayout {
        id: callButtonLayout
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 25
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        RisipButton {
            id: answerCallButton
            Layout.preferredWidth: 80
            labelText.text: qsTr("Answer")
        }

        RisipButton {
            id: endCallButton
            Layout.preferredWidth: 80
            labelText.text: qsTr("End")
        }
    }

    RowLayout {
        id: callMediaButtonLayout

        anchors.bottom: callButtonLayout.top
        anchors.bottomMargin: 60
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        RisipButton {
            id: micButton
            Layout.preferredWidth: 40
            iconNormal: "qrc:/images/icons/32/MicrophoneBlack.png"
            iconAlternate: "qrc:/images/icons/32/MicrophoneRed.png"
            checkable: true
            width: 40
            height: 40
            radius: width *0.5
        }

//        RisipButton {
//            id: speakerButton
//            Layout.preferredWidth: 60
//            labelText.text: qsTr("Speaker")
//            checkable: true
//        }
    }

    states: [
        State {
            name: "incoming"
        },
        State {
            name: "outgoing"

            PropertyChanges {
                target: answerCallButton
                enabled: false
            }
        },
        State {
            name: "incall"
            PropertyChanges {
                target: answerCallButton
                enabled: false
            }
        }
    ]
}
