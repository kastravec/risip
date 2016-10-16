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
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 400
    height: 600


    ColumnLayout {
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.horizontalCenter: parent.horizontalCenter

        RowLayout {

            ComboBox {
                id: comboBox1
                Layout.preferredHeight: 40
                Layout.preferredWidth: 227
            }

            CheckBox {
                id: checkBox1
                text: qsTr("GSM")
            }
        }

        TextField {
            id: textField1
            placeholderText: qsTr("Someone you want to talk to ?")
            Layout.preferredHeight: 40
            Layout.preferredWidth: 322
        }

        Button {
            id: button1
            text: qsTr("Call")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }


    ColumnLayout {
        x: 35
        y: 245
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 25
        anchors.horizontalCenter: parent.horizontalCenter

        ListView {
            id: listView1
            Layout.preferredHeight: 283
            Layout.preferredWidth: 322
            model: ListModel {
                ListElement {
                    name: "Some text message"
                }

                ListElement {
                    name: "Another one "
                }

                ListElement {
                    name: "More more more "
                }

                ListElement {
                    name: "Less ..."
                }
            }
            delegate: Item {
                x: 5
                width: 80
                height: 40
                Row {
                    id: row1
                    Rectangle {
                        width: 40
                        height: 40
                    }

                    Text {
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                    }
                    spacing: 10
                }
            }
        }

        RowLayout {

            TextField {
                id: textField2
                placeholderText: qsTr("Send a message..")
                Layout.preferredHeight: 40
                Layout.preferredWidth: 216
            }

            Button {
                id: button2
                text: qsTr("Button")
            }
        }
    }
}



