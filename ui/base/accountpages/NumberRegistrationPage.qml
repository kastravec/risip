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
    property alias prefixLabel: prefixLabel
    property alias numberInput: numberInput
    property alias continueButton: continueButton

    signal continueButtonClicked

    ColumnLayout {
        id: columnLayout
        anchors.verticalCenter: parent.verticalCenter
        spacing: 60
        anchors.horizontalCenter: parent.horizontalCenter

        RowLayout {
            id: rowLayoutTop

            Image {
                id: image
                width: 24
                height: 18
                Layout.preferredWidth: width
                Layout.preferredHeight: height
                source: "image://countryFlags/de" //initial is 'de'
            }

            Label {
                id: countryLabel
                text: qsTr("Germany")
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            IconButton {
                id: arrowIcon
                width: 14
                height: 14
                Layout.preferredWidth: width
                Layout.preferredHeight: height
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                defaultIcon: "qrc:/images/icons/128/ArrowheadDownBlack.png" //from model

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: 20
                    onClicked: { countryPickerPopup.visible = true; }
                }
            }
        }

        RowLayout {
            id: rowLayoutBottom
            anchors.verticalCenter: parent.verticalCenter

            TextField {
                id: prefixLabel
                text: qsTr("+49")
                Layout.alignment: Qt.AlignLeft
                Layout.preferredWidth: 50
                Layout.preferredHeight: 40
                readOnly: true
            }

            TextField {
                id: numberInput
                Layout.fillWidth: true
                Layout.preferredWidth: 200
                Layout.preferredHeight: 40
                horizontalAlignment: Text.AlignLeft
            }
        }
    }

    RisipButton {
        id: continueButton
        text: qsTr("Continue")
        anchors.top: columnLayout.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked: root.continueButtonClicked();
    }

    Rectangle {
        id: countryPickerPopup
        anchors.fill: parent
        anchors.margins: 10
        focus: true
        z: parent.z + 1
        visible: false

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 15
            spacing: 20

            RowLayout {
                spacing: 10

                IconButton {
                    id: closeButton
                    width: 16
                    height: 16
                    defaultIcon: "qrc:/images/icons/128/CloseBlack.png"
                    activeIcon: "qrc:/images/icons/128/CloseRed.png"

                    onClicked: countryPickerPopup.visible = false;
                }

                Label {
                    text: qsTr("Close")

                    MouseArea {
                        anchors.fill: parent
                        onClicked: countryPickerPopup.visible = false;
                    }
                }

            }

            CountryListViewPage {
                id: countryListView
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

        } //end of column layout
    }

}
