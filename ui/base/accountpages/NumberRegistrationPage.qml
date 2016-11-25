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
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter

        RowLayout {
            id: rowLayoutTop

            Image {
                id: image
                Layout.preferredHeight: 29
                Layout.preferredWidth: 42
            }

            Label {
                id: countryLabel
                text: qsTr("Country")
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Image {
                id: image1
                width: 20
                height: 20
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
        }

        RowLayout {
            id: rowLayoutBottom

            Label {
                id: prefixLabel
                text: qsTr("+49")
                Layout.preferredHeight: 40
                Layout.preferredWidth: 41
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            TextField {
                id: numberInput
                text: qsTr("34343434")
                horizontalAlignment: Text.AlignHCenter
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

} //end of first page
