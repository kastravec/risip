/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
**    http://risip.io
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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../risipcomponents"

import Risip 1.0

GenericSettingsPage {
    id: root

    ListView {
        id: listView
        smooth: true
        cacheBuffer: 300
        snapMode: ListView.SnapToItem
        clip: true
        anchors.fill: parent

        model: RisipRateManager.countryRatesModel
        delegate: listViewDelegate
    }

    Component {
        id: listViewDelegate

        Rectangle {
            id: countryBox
            width: listView.width
            height: 40

            Image {
                width: 40
                height: 40
                id: countryFlag
                source: "image://countryFlags/" + countryPrefix //from model

                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
            }

            RisipButton {
                id: countryNameButton
                labelText.text: countryName
                anchors.left: countryFlag.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }


        }
    }
}
