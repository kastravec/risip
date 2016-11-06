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

Page {
    id: contactsPage

    property alias buddyModel: sipListView.model
    property alias sipListView: sipListView
    property alias searchContactInput: searchContactInput
    property alias phoneListView: phoneListView
    property alias contactsListLayout: contactsListLayout

    footer: TabBar {
        id: tabBar
        currentIndex: 0

        TabButton {
            id: sipContacts
            text: qsTr("SIP")
        }

        TabButton {
            id: phoneContacts
            text: qsTr("Phone")
        }
    }

    ColumnLayout {
        anchors.fill: contactsPage.contentItem

        TextField {
            id: searchContactInput
            Layout.fillWidth: true
            placeholderText: qsTr("Search a contact..")
        }

        StackLayout {
            id: contactsListLayout
            x: 0

            currentIndex: tabBar.currentIndex

            ListView {
                id: sipListView
                snapMode: ListView.SnapToItem
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                clip: true

                delegate: Item {
                    width: parent.width
                    height: 50
                    Text {
                        anchors.fill: parent
                        text: contact
                        font.bold: true
                        font.pointSize: 12
                    }
                }
            }

            ListView {
                id: phoneListView
                snapMode: ListView.SnapToItem
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                clip: true

                delegate: Item {
                    width: parent.width
                    height: 50
                    Text {
                        anchors.fill: parent
                        text: fullName
                        font.bold: true
                    }
                }
            }
        }
    }
}
