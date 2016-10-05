import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    id: item1
    property alias loginButton: loginButton
    property alias addSipServiceButton: addSipServiceButton

    ColumnLayout {
        x: 203
        y: 93
        anchors.bottom: bottomRowLayout.top
        anchors.bottomMargin: 135
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 15

        TextField {
            id: passwordInput
            Layout.fillWidth: true
            echoMode: TextInput.PasswordEchoOnEdit
            placeholderText: qsTr("Enter your password")
        }

        TextField {
            id: uernameInput
            Layout.fillWidth: true
            placeholderText: qsTr("Enter your username")
        }

        ComboBox {
            id: sipServiceInput
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.preferredWidth: 200
        }

        Button {
            id: loginButton
            text: qsTr("Sign in")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: false
        }
    }

    RowLayout {
        id: bottomRowLayout
        x: 191
        y: 413
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        spacing: 15

        Label {
            id: label1
            text: qsTr("Add a new SIP Account service? ")
        }

        Button {
            id: addSipServiceButton
            text: qsTr("Add SIP Service")
        }
    }
}
