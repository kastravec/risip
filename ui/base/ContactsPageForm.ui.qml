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
    id: root

    footer: TabBar {
        id: contactsTabBar

        TabButton {
            id: phoneContacts
            text: qsTr("Phone")
        }

        TabButton {
            id: sipContacts
            text: qsTr("SIP")
        }
    }

    StackLayout {
        id: stackLayout
        currentIndex: contactsTabBar.currentIndex

        ListView {
            id: listView1

            delegate: Item {
                width: 80
                height: 40
                Row {
                    id: row1
                    Rectangle {
                        width: 40
                        height: 40
                        color: colorCode
                    }

                    Text {
                        text: name
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    spacing: 10
                }
            }

            model: ListModel {
                ListElement {
                    name: "Grey"
                    colorCode: "grey"
                }
            }
        }


        ListView {
            id: listView2

            delegate: Item {
                width: 80
                height: 40
                Row {
                    id: row2
                    Rectangle {
                        width: 40
                        height: 40
                        color: colorCode
                    }

                    Text {
                        text: name
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    spacing: 10
                }
            }

            model: ListModel {
                ListElement {
                    name: "Grey"
                    colorCode: "grey"
                }
            }
        }
    }
}
