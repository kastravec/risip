/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
**    http://risip.io
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You have received a copy of the GNU General Public License
**    along with this program. See LICENSE.GPLv3
**    A copy of the license can be found also here <http://www.gnu.org/licenses/>.
**
************************************************************************************/

import QtQuick 2.7
import Risip 1.0

CallPageForm {
    id: callPage;
    visible: false

    property RisipCall activeCall
    usernameLabel.text: activeCall.buddy.uri;

    Connections {
        target: RisipCallManager

        onIncomingCall: {
            //first hangup if any call is active
            if(activeCall)
                activeCall.hangup();

            callPage.activeCall = call;
            callPage.state = "incoming";
            callPage.visible = true;
        }

        onOutgoingCall: {
            callPage.activeCall = call;
            callPage.state = "outgoing"
            callPage.visible = true;
        }
    }

    answerCallButton.onClicked: activeCall.answer();
    endCallButton.onClicked: activeCall.hangup();

    Connections {
        target: activeCall

        onStatusChanged: {
            switch (status) {
            case RisipCall.CallConfirmed:
                statusLabel.text = "Call connected, enjoy!";
                callPage.state = "incall";
                callPage.visible = true;
                break;
            case RisipCall.CallDisconnected:
                statusLabel.text = "Call disconnected, good bye!";
                callPage.visible = false;
                break;
            case RisipCall.CallEarly:
            case RisipCall.ConnectingToCall:
                statusLabel.text = "we are working to establish the call..and.."
                break;
            case RisipCall.IncomingCallStarted:
                statusLabel.text = "who may be calling now?.."
                break;
            case RisipCall.OutgoingCallStarted:
                statusLabel.text = "trying to reach..and calling"
                break;
            case RisipCall.Null:
                statusLabel.text = ".. trying reaching somebody";
                callPage.visible = false;
                break;
            }
        }
    }
}
