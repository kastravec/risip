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
import QtQuick.Controls.Material 2.0

import "risipcomponents"

Page {
    id: root

    Material.theme: Material.Light
    Material.background: "#FFFFFF"
    Material.accent: "#db0000" //"#DB5AB9"
    Material.foreground: "#000000"
    Material.primary: "#FFFFFF"

    signal enterClicked

    SwipeView {
        id: swipeView
        height: parent.height *0.80
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        Rectangle {
            id: page1

            Image {
                id: image1
                fillMode: Image.PreserveAspectCrop
                anchors.fill: parent
                source: "qrc:/images/businessman-calling-phone.jpg"
            }
        }

        Rectangle {
            id: page2

            Image {
                id: image2
                fillMode: Image.PreserveAspectCrop
                anchors.fill: parent
                source: "qrc:/images/multiscreen.png"
            }
        }

        Rectangle {
            id: page3

            color: "#ffffff"

            Image {
                id: image3
                fillMode: Image.PreserveAspectCrop
                anchors.fill: parent
                source: "qrc:/images/calling.jpeg"
            }
        }
    }

    PageIndicator {
        id: pageIndicator
        anchors.topMargin: 10
        anchors.top: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: swipeView.currentIndex
        spacing: 15
        count: 3
        interactive: true
    }

    RisipButton {
        id: enterButton
        text: qsTr("Let's start")

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 50
        anchors.bottom: parent.bottom

        onClicked: { root.enterClicked(); }
    }
}
