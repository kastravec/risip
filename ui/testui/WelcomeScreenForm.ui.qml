import QtQuick 2.7
import QtQuick.Controls 2.0

Page {
    id: page3
    width: 400
    height: 600
    property alias enterButton: enterButton

    SwipeView {
        id: swipeView
        height: 300
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
            id: rectangle1
            width: 200
            height: 200
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
        anchors.topMargin: 5
        anchors.top: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: swipeView.currentIndex
        spacing: 15
        count: 3
    }

    Button {
        id: enterButton
        x: 168
        y: 305
        text: qsTr("Enter")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        anchors.bottom: parent.bottom
    }
}
