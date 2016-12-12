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
    id: item1

    property alias callButton: callButton
    property alias contactInput: contactInput

    LineEdit {
        id: contactInput
        height: 40
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.bottom: phoneNumbersLayout.top
        anchors.bottomMargin: 40
        placeholderText: qsTr("Enter Name or Number")
        Layout.preferredHeight: 40
        frameBorder: 0
        font.pointSize: 32
        verticalBottomLine: true
    }

    GridLayout {
        id: phoneNumbersLayout
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 3
        rows: 4
        rowSpacing: 25
        columnSpacing: 60

        RisipButton {
            id: one
            text: "1"
            border.width: 0
            font.bold: true
            font.pointSize: 40

            onClicked: contactInput.text += "1";

            Line {
                anchors.top: one.bottom
            }

        }
        RisipButton {
            id: two
            text: "2"
            border.width: 0
            font.bold: true
            font.pointSize: 40

            onClicked: contactInput.text += "2";

            Line {
                anchors.top: two.bottom
            }
        }
        RisipButton {
            id: three
            text: "3"
            border.width: 0
            font.bold: true
            font.pointSize: 40

            onClicked: contactInput.text += "3";

            Line {
                anchors.top: three.bottom
            }
        }
        RisipButton {
            id: four
            text: "4"
            Layout.fillWidth: true
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "4";

            Line {
                anchors.top: four.bottom
            }
        }
        RisipButton {
            id: five
            text: "5"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "5";

            Line {
                anchors.top: five.bottom
            }
        }
        RisipButton {
            id: six
            text: "6"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "6";

            Line {
                anchors.top: six.bottom
            }
        }
        RisipButton {
            id: seven
            text: "7"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "7";

            Line {
                anchors.top: seven.bottom
            }
        }
        RisipButton {
            id: eight
            text: "8"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "8";

            Line {
                anchors.top: eight.bottom
            }
        }
        RisipButton {
            id: nine
            text: "9"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "9";

            Line {
                anchors.top: nine.bottom
            }
        }
        RisipButton {
            id: star
            text: "*"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "*";

            Line {
                anchors.top: star.bottom
            }
        }
        RisipButton {
            id: zero
            text: "0"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "0";

            Line {
                anchors.top: zero.bottom
            }
        }
        RisipButton {
            id: hash
            text: "#"
            font.bold: true
            font.pointSize: 40
            border.width: 0

            onClicked: contactInput.text += "#";

            Line {
                anchors.top: hash.bottom
            }
        }
    }

    RisipButton {
        id: callButton
        width: 80
        text: qsTr("Call")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
    }
}
