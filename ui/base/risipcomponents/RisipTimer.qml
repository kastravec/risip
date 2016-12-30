import QtQuick 2.7

Timer {
    id: root
    interval: 1000
    repeat: true

    property int elapsedTime: 0
    property string elapsedTimeText: "00:00"

    onTriggered: {
        root.elapsedTime += 1;
        if(root.elapsedTime / 60 < 1) {
            if(root.elapsedTime / 10 < 1)
                root.elapsedTimeText = "00:0" + root.elapsedTime;
             else
                root.elapsedTimeText = "00:" + root.elapsedTime;
        } else if(root.elapsedTime === 60) {
            root.elapsedTimeText = "01:00";
        } else if((root.elapsedTime - 60) / 10 < 1) {
            root.elapsedTimeText = "01:0" + root.elapsedTime;
        } else {
            root.elapsedTimeText = "01:" + root.elapsedTime;
        }
    }

    onRunningChanged: {
        if(!root.running)
            reset();
    }

    function reset() {
        root.elapsedTime = 0;
    }
}
