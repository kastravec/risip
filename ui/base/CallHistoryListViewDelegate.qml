import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: root

    RowLayout {
        spacing: 20

        Text {
            id: contactLabel
            text: model.callContact
            color: "#000000"
        }

        Text {
            id: callDuration
            text: model.callDuration
            color: "#000000"
        }

        Text {
            id: callDirection
            text: model.callDirection
            color: "#000000"
        }

        Text {
            id: callTimestamp
            text: model.callTimestamp
            color: "#000000"
        }
    }
}
