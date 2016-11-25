import QtQuick 2.7
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

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Page {
    id: root

    signal backClicked
    signal saveClicked

    header: Item {
        focus: true

        Row {
            id: rowLayout
            spacing: 3

            Image { source: "qrc:/images/icons/16/ArrowLeftRedx4.png"; Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter; }
            Label {
                text: qsTr("Settings")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Settings clicked..!");
                        root.backClicked();
                    }
                }
            }
        }

        Label {
            text: qsTr("Save")
            anchors.right: parent.right
            anchors.rightMargin: 5

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Save clicked..!");
                    root.saveClicked();
                }
            }
        }
    }
}
