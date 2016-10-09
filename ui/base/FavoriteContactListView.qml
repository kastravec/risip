import QtQuick 2.7
import QtQuick.Controls 2.0

ListView {
    id: favoriteContactListview
    anchors.fill: parent

    delegate: Item {
        width: parent.width
        height: 50
        Row {
            id: row1
            spacing: 10

            Rectangle {
                width: 40
                height: 40
                radius: 90

                color: colorCode
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true

                text: name
            }
        }
    }

    model: ListModel {
        ListElement {
            name: "Kot Fare"
            colorCode: "grey"
        }

        ListElement {
            name: "Kot Fa Fare"
            colorCode: "red"
        }

        ListElement {
            name: "Fare Kot"
            colorCode: "blue"
        }

        ListElement {
            name: "Kof Tare"
            colorCode: "green"
        }
    }
}

