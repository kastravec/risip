import QtQuick 2.7

//import Risip 1.0

QtObject {
    id: application

    property RisipAccount sipAccount: Risip.defaultAccount
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

    Connections {
        target: sipAccount

        onStatusChanged: {
            if(sipAccount.status === RisipAccount.SignedIn)
                online = true;
            else
                online = false;
        }
    }


    function addSipService() {
    }
}
