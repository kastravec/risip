import QtQuick 2.7

ListView {
    id: root

    delegate: CallHistoryListViewDelegate {
        width: root.width
        height: 50
    }
}
