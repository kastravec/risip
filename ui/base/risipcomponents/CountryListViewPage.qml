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

import SortFilterProxyModel 0.1
import Risip 1.0

Page {
    id: root

    property alias delegate: listView.delegate

    signal countrySelected

    SortFilterProxyModel {
        id: proxyModel
        sourceModel: RisipRateManager.countryRatesModel
        filterRoleName: "countryName"
        filterCaseSensitivity: Qt.CaseInsensitive
        sortOrder: Qt.AscendingOrder
        sortRoleName: "countryName"
        filterPattern: countrySearchBox.text
    }

    ColumnLayout {
        id: columnLayout
        spacing: 10
        anchors.fill: root.contentItem
        anchors.topMargin: 10

        LineEdit {
            id: countrySearchBox
            Layout.fillWidth: true
            height: 40
            showMagnifyIcon: true
            placeholderText: qsTr("Search Country")
        }

        ListView {
            id: listView
            smooth: true
            cacheBuffer: 300
            snapMode: ListView.SnapToItem
            clip: true
            model: proxyModel
            delegate: listViewDelegate
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    } //end of column layout

    Component {
        id: listViewDelegate

        Rectangle {
            id: countryFrame
            height: 60
            width: parent.width

            Line {
                id: horizontalLine
                width: countryFrame.width - 20 //margins 10 each left and right
                anchors.bottom: countryFrame.bottom
                anchors.centerIn: countryFrame
                opacity: 0.5
            }

            Rectangle {
                id: countryBox
                width: parent.width
                height: 40
                anchors.centerIn: countryFrame

                Rectangle {
                    id: countryFlagBorder
                    width: 41
                    height: 41
                    border.color: "#aaaaaa"
                    border.width: 1
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Image {
                        id: countryFlag
                        width: 40
                        height: 24
                        source: "image://countryFlags/" + countryCode //from model
                        anchors.centerIn: parent
                    }
                }

                RisipButton {
                    id: countryNameButton
                    text: countryName //from model
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: countryFlagBorder.right
                    anchors.leftMargin: 20
                    border.width: 0

                    onClicked: { root.countrySelected(); }
                }

                Label {
                    id: prefixLabel
                    text: "(+" + countryPrefix + ")" //from model
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: countryNameButton.right
                    anchors.leftMargin: 4
                }
            }
        }

    }//end of component delegate
}
