import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../risipcomponents"
import Risip 1.0

Page {
    id: root

    property alias contactName: contactNameLabel.text
    property alias phoneNumbersModel: phoneNumberViewer.model

    signal backClicked
    signal favoriteClicked

    header: Item {
        focus: true

        Row {
            id: headerLayout
            spacing: 2

            Image { source: "qrc:/images/icons/16/ArrowLeftRedx4.png"; Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter; }
            Label {
                text: qsTr("Contacts")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Back to contact..!");
                        root.backClicked();
                    }
                }
            }
        }

        Label {
            text: qsTr("Favorite")
            anchors.right: parent.right
            anchors.rightMargin: 5

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Favorite clicked..!");
                    root.favoriteClicked();
                }
            }
        }

    }

    ColumnLayout {
        id: columnLayout
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top : parent.top
        anchors.topMargin: 20

        Rectangle {
            id: rectangle
            width: 80
            height: 80
            color: "#ffffff"
            radius: width * 0.5
            border.color: "#000000"
            border.width: 1
        }

        Text {
            id: contactNameLabel
            font.bold: true
        }
    }

    ListView {
        id: phoneNumberViewer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: columnLayout.bottom
        anchors.topMargin: 30
        anchors.bottom: parent.bottom
        width: parent.width/2
        clip: true
        snapMode: ListView.SnapOneItem

        delegate: Rectangle {
            id: phoneNumberDelegate
            width: parent.width
            height: phoneNumberBox.height + 10

            RisipButton {
                id: phoneNumberBox
                width: phoneNumberViewer.width
                anchors.centerIn: parent
                labelText.text: fullNumber

                onClicked: RisipCallManager.callPhone(labelText.text);
            }
        }

    }
}
