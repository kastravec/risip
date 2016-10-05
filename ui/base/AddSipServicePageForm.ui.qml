import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: item1
    width: 400
    height: 600
    property alias saveButton: saveButton

    ColumnLayout {
        id: mainColumnLayout
        anchors.verticalCenter: parent.verticalCenter
        spacing: 15
        anchors.horizontalCenter: parent.horizontalCenter

        TextField {
            id: usernameInput
            Layout.fillWidth: true
            placeholderText: qsTr("SIP Username e.g tom")
        }

        TextField {
            id: passwordInput
            placeholderText: qsTr("Password")
        }

        TextField {
            id: sipServerInput
            placeholderText: qsTr("SIP Server address or IP")
        }

        TextField {
            id: sipProxyInput
            placeholderText: qsTr("Proxy Server or IP")
        }

        ComboBox {
            id: networkProtocolInput
            textRole: ""
            Layout.fillWidth: true
            model: ["UDP", "TCP", "TLS"]
            currentIndex: 0
        }

        TextField {
            id: localPortInput
            Layout.fillWidth: true
            placeholderText: qsTr("Local Port e.g -1 for random")
        }

        Button {
            id: saveButton
            text: qsTr("Save")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: false
        }
    }
}
