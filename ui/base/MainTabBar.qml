import QtQuick 2.7
import QtQuick.Controls 2.0

TabBar {
    id: root
    height: 50
    currentIndex: 0
    property int previousIndex

    onCurrentIndexChanged: {
        togleButtonIcon(previousIndex, currentIndex);
        previousIndex = currentIndex;
    }

    Component.onCompleted: {
        togleButtonIcon(root.currentIndex, root.currentIndex);
        root.previousIndex = root.currentIndex
    }

    TabButton {
        id: phoneBookButton
        property bool active: false

        onActiveChanged: {
            if(active)
                phoneBookIcon.source = "qrc:/images/icons/24/PhoneBookRed.png";
            else
                phoneBookIcon.source = "qrc:/images/icons/24/PhoneBookBlack.png";
        }

        Image {
            id: phoneBookIcon
            source: "qrc:/images/icons/24/PhoneBookBlack.png";
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 24
            height: 24
        }
    }

    TabButton {
        id: phoneDialButton

        property bool active: false

        onActiveChanged: {
            if(active)
                phoneDialIcon.source = "qrc:/images/icons/24/PhoneRed.png";
            else
                phoneDialIcon.source = "qrc:/images/icons/24/PhoneBlack.png";
        }

        Image {
            id: phoneDialIcon
            source: "qrc:/images/icons/24/PhoneBlack.png";
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 24
            height: 24
        }
    }

    TabButton {
        id: messageButton

        property bool active: false

        onActiveChanged: {
            if(active)
                messageIcon.source = "qrc:/images/icons/24/MessageRed.png";
            else
                messageIcon.source = "qrc:/images/icons/24/MessageBlack.png";
        }

        Image {
            id: messageIcon
            source: "qrc:/images/icons/24/MessageBlack.png";
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 24
            height: 24
        }
    }

    TabButton {
        id: settingsButton

        property bool active: false

        onActiveChanged: {
            if(active)
                settingsIcon.source = "qrc:/images/icons/24/SettingsRed.png";
            else
                settingsIcon.source = "qrc:/images/icons/24/SettingsBlack.png";
        }

        Image {
            id: settingsIcon
            source: "qrc:/images/icons/24/SettingsBlack.png";
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 24
            height: 24
        }
    }

    //function to toggle the "active" property of the tab so that we change the icon respective for (non)current tab
    function togleButtonIcon(previousIndex, currentIndex) {
        switch(previousIndex) {
        case 0:
            phoneBookButton.active = false;
            break;
        case 1:
            phoneDialButton.active = false;
            break;
        case 2:
            messageButton.active = false;
            break;
        case 3:
            settingsButton.active = false;
            break;
        }

        switch(currentIndex) {
        case 0:
            phoneBookButton.active = true;
            break;
        case 1:
            phoneDialButton.active = true;
            break;
        case 2:
            messageButton.active = true;
            break;
        case 3:
            settingsButton.active = true;
            break;
        }
    }
}
