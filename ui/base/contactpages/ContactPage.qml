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
import Risip 1.0

Page {
    id: root

    property RisipPhoneContact phoneContact: RisipContactManager.activePhoneContact

    signal backClicked
    signal favoriteClicked

    header: ToolBar {
        focus: true
                background: Rectangle {
                    implicitHeight: 40
                    color: "#ffffff"
                }

        Row{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            Arrow {
                orientation: "left"
                Layout.alignment: Qt.AlignVCenter
            }

            Label {
                text: qsTr("Contacts")

                MouseArea {
                    anchors.fill: parent
                    onClicked: { root.backClicked(); }
                }

                Layout.alignment: Qt.AlignVCenter
            }
        }

        Label {
            text: qsTr("Favorite")
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent
                onClicked: { root.favoriteClicked(); }
            }
        }

        // } //end of toolbar layout

    } // end of header

    ColumnLayout {
        id: columnLayout
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top : parent.top
        anchors.topMargin: 20

        Rectangle {
            id: contactIconBox
            width: 80
            height: 80
            radius: width * 0.5
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: phoneContact.initials
                anchors.centerIn: parent
            }

            Image {
                id: contactIcon
                width: 80
                height: 80
                anchors.centerIn: parent
                source: "image://contactIcon/" + phoneContact.fullName
            }
        }

        Text {
            id: contactNameLabel
            font.bold: true
            text: phoneContact.fullName
            Layout.alignment: Qt.AlignHCenter
        }
    } //end of contact header

    ListView {
        id: phoneNumberViewer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: columnLayout.bottom
        anchors.topMargin: 30
        anchors.bottom: parent.bottom
        clip: true
        snapMode: ListView.SnapToItem
        width: parent.width

        model: phoneContact.phoneNumbersModel
        delegate: phoneNumberDelegate
    }

    Component {
        id: phoneNumberDelegate
        Rectangle {
            id: phoneNumberFrame
            width: parent.width
            height: phoneNumberBox.height + 15

            CountryFlag {
                id: countryFlagIcon
                code: countryCode
                anchors.right:  phoneNumberBox.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            RisipButton {
                id: phoneNumberBox
                height: 40
                text:  fullNumber
                anchors.centerIn: parent
                border.width: 0
                onClicked: RisipCallManager.callPhone(fullNumber);
            }

        } //end of delegate
    }
}
