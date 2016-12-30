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
import QtQuick.Controls.Material 2.0

import "settingspages"
import "callpages"
import "contactpages"
import "accountpages"
import "risipcomponents"

import Risip 1.0

Page {
    id: mainPage
    width: 720
    height: 1280
    visible: false

    Material.theme: Material.Light
    Material.background: "#FFFFFF"
    Material.accent: "#db0000" //"#DB5AB9"
    Material.foreground: "#000000"
    Material.primary: "#FFFFFF"

    property string uiBasePath: "qrc:/ui/base/"
    property RisipAccount sipAccount: Risip.defaultAccount

    RisipCall {
        id: calling
    }


    header: MainToolBar { id: mainToolBar }
    footer: MainTabBar { id: mainTabBar }

    StackLayout {
        id: stackLayout
        currentIndex: mainTabBar.currentIndex
        anchors.fill: mainPage.contentItem

        PageLoader {
            id: contactsPageLoader
            source: uiBasePath + "/contactpages/" + "ContactsPage.qml"
            active: true
        }

        PageLoader {
            id: dialPageLoader
            source: uiBasePath + "/callpages/" + "DialPage.qml"
            active: true
        }

        PageLoader {
            id: homePageLoader
            source: uiBasePath + "HomePage.qml"
            active: true
        }

        PageLoader {
            id: settingsPageLoader
            source: uiBasePath + "/settingspages/" + "SettingsPage.qml"
            active: true
        }
    }

    CallPage {
        id: callPage
        anchors.fill: mainPage.contentItem
    }
}
