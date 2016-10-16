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

Rectangle {
    id: root

    width: 720
    height: 1280

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
                source: "qrc:/images/hello.jpeg"

                Button {
                    id: nextButton1
                    text: qsTr("Next")
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 15
                    anchors.right: parent.right
                    anchors.rightMargin: 15

                    onClicked: swipeView.currentIndex = swipeView.currentIndex +1
                }
            }
        }

        Rectangle {
            id: page2

            Image {
                id: image2
                fillMode: Image.PreserveAspectCrop
                anchors.fill: parent
                source: "qrc:/images/multiscreen.png"

                Button {
                    id: nextButton2
                    text: qsTr("Next")
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 15
                    anchors.right: parent.right
                    anchors.rightMargin: 15

                    onClicked: swipeView.currentIndex = swipeView.currentIndex +1
                }
            }
        }

        Rectangle {
            id: page3
            width: 200
            height: 200
            color: "#ffffff"

            Image {
                id: image3
                fillMode: Image.PreserveAspectCrop
                anchors.fill: parent
                source: "qrc:/images/calling.jpeg"

                Button {
                    id: nextButton3
                    text: qsTr("Next")
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 15
                    anchors.right: parent.right
                    anchors.rightMargin: 15

                    onClicked: swipeView.currentIndex = swipeView.currentIndex -2
                }
            }
        }
    }

    PageIndicator {
        id: pageIndicator
        anchors.topMargin: 5
        anchors.top: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: swipeView.currentIndex
        spacing: 15
        count: 3
        interactive: true
    }

    Button {
        id: enterButton
        text: qsTr("Enter")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        anchors.bottom: parent.bottom

        onClicked: {
            root.enterClicked();
        }
    }
}
