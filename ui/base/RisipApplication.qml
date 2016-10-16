import QtQuick 2.7

import Risip 1.0

QtObject {
    id: application

    property RisipAccount sipAccount
    property RisipEndpoint sipEndpoint: Risip.sipEndpoint
    property bool ready: false
    property bool online: false

    signal start
    signal stop

    onStart: {
        sipEndpoint.startEngine();
        if(Risip.sipEndpoint.status === RisipEndpoint.Started)
            ready = true;
    }

    onStop: {
        sipEndpoint.stopEngine();
        if(Risip.sipEndpoint.status === RisipEndpoint.Started)
            ready = false;
    }

    onReadyChanged: {
//        if(ready === true)
    }

    function addSipService() {
    }
}
