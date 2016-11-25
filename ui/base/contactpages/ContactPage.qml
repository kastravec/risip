import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../risipcomponents"
import Risip 1.0

Page {
    id: root

    property RisipPhoneContact phoneContact: RisipContactManager.activePhoneContact

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
                height: 20
                MouseArea {
                    anchors.fill: parent
                    onClicked: { root.backClicked(); }
                }
            }
        }

        Label {
            text: qsTr("Favorite")
            anchors.right: parent.right
            anchors.rightMargin: 5

            MouseArea {
                anchors.fill: parent
                onClicked: { root.favoriteClicked(); }
            }
        }
    } // end of header

    ColumnLayout {
        id: columnLayout
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top : parent.top
        anchors.topMargin: 20

        Rectangle {
            id: contactIconBox
            width: 80
            height: 80
            radius: width * 0.5
            color: "#ffffff"
            border.color: "#000000"
            border.width: 0

            Text {
                id: initialsTxt
                anchors.centerIn: contactIconBox
                text: "..."
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Image {
                id: contactIcon
                width: 80
                height: 80
                anchors.centerIn: parent
                source: "image://contactIcon/" + phoneContact.fullName
            }
        }

        Text {
            id: contactNameLabel
            font.bold: true
            text: phoneContact.fullName
        }
    }

    ListView {
        id: phoneNumberViewer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: columnLayout.bottom
        anchors.topMargin: 30
        anchors.bottom: parent.bottom
        width: parent.width
        clip: true
        snapMode: ListView.SnapToItem

        model: phoneContact.phoneNumbersModel
        delegate: Rectangle {
            id: phoneNumberDelegate
            width: parent.width/2 - 20
            height: phoneNumberBox.height + 15
            property RisipPhoneNumber phoneNumber: RisipContactManager.phoneNumberForNumber(fullNumber)

            Image {
                id: countryFlagIcon
                width: 28
                height: 28
                source: "image://countryFlags/" + phoneNumber.countryCode
                anchors.right: phoneNumberBox.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            RisipButton {
                id: phoneNumberBox
                width: phoneNumberDelegate.width - 10
                height: 40
                anchors.centerIn: parent
                text: fullNumber
                onClicked: RisipCallManager.callRisipPhoneNumber(phoneNumber);
            }

//            Text {
//                text: RisipContactManager.phoneNumberForNumber(fullNumber).countryCode
//            }
        } //end of delegate
    }
}
