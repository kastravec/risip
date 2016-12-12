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
    id: mainContactsPage

    property alias buddyModel: sipListView.model
    property alias sipListView: sipListView
    property alias searchContactInput: searchContactInput
    property alias contactsListLayout: contactsListLayout
    property alias phoneListView: phoneListView
    property alias stackView: stackView
    property alias contactPages: contactPages
    property alias contactViewPage: contactViewPage

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: contactPages

        ContactPage { id: contactViewPage }

        Page {
            id: contactPages
            footer: TabBar {
                id: tabBar
                TabButton {
                    text: qsTr("Phone")
                }
                TabButton {
                    text: qsTr("Risip")
                }
                TabButton {
                    text: qsTr("Favorites")
                }
            }

            ColumnLayout {
                anchors.fill: parent

                LineEdit {
                    id: searchContactInput
                    height: 40
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    showMagnifyIcon: true
                    placeholderText: qsTr("Search Contacts")
                }

                StackLayout {
                    id: contactsListLayout
                    Layout.fillWidth: true
                    currentIndex: tabBar.currentIndex

                    PhoneContactListView {
                        id: phoneListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    ListView {
                        id: sipListView
                        Layout.fillWidth: true
                        snapMode: ListView.SnapToItem
                        Layout.fillHeight: true
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
                }
            }
        } //end page
    } //end of stackview
}
