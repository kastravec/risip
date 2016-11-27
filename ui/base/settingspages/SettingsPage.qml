/***********************************************************************************
**    Copyright (C) 2016  Petref Saraci
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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "../risipcomponents"
import Risip 1.0

Page {
    id: root

    StackView {
        id: stackView
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 20

        initialItem: settingListView
    }

    ListView {
        id: settingListView
        cacheBuffer: 30
        spacing: 10
        focus: true
        highlightRangeMode: ListView.NoHighlightRange
        snapMode: ListView.NoSnap

        delegate: settingsListDelegate
        model: settingsListModel

        section.property: "category"
        section.criteria: ViewSection.FullString
        section.delegate: settingsCategoryDelegate
    }

    //menu display delegate within the list
    Component {
        id: settingsListDelegate
        Item {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            height: 30

            RowLayout {
                anchors.fill: parent
                Label { text: settingsName }

                Arrow {
                    Layout.preferredWidth: width
                    Layout.preferredHeight: height
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    orientation: "right"
                }
            }

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    settingListView.currentIndex = index;
                    switch(index) {
                    case 0: //Profile
                        profileSettingsPageLoader.active = true;
                        stackView.push(profileSettingsPageLoader.item)
                        break;
                    case 1: //Balance
                        balanceSettingsPageLoader.active = true;
                        stackView.push(balanceSettingsPageLoader.item);
                        break;
                    case 2: //Rates
                        countryRatesPageLoader.active = true;
                        stackView.push(countryRatesPageLoader);
                        break;
                    case 3: //Earn Credits
                        break;
                    case 4: //Languages
                        languageSettingsPageLoader.active = true;
                        stackView.push(languageSettingsPageLoader.item);
                        break;
                    case 5: //Help
                        break;
                    case 6: //About
                        break;
                    case 7: //Sign out
                        Risip.defaultAccount.logout();
                        break;
                    }
                }
            }
        }
    }

    //"a section" delegate to just display a black horizontal line
    Component {
        id: settingsCategoryDelegate
        Item {
            width: settingListView.width
            height: 3

            Rectangle {
                anchors.bottom: parent.bottom
                width: settingListView.width
                height: 1
                color: "#000000"
            }
        }
    }

    Loader {
        id: profileSettingsPageLoader
        source: "qrc:/ui/base/settingspages/ProfileSettingsPage.qml"
        active: false
    }

    Loader {
        id: balanceSettingsPageLoader
        source: "qrc:/ui/base/settingspages/BalanceSettingsPage.qml"
        active: false
    }

    Loader {
        id: languageSettingsPageLoader
        source: "qrc:/ui/base/settingspages/LanguageSettingsPage.qml"
        active: false
    }

    Loader {
        id: countryRatesPageLoader
        source: "qrc:/ui/base/settingspages/CountryRatesPage.qml"
        active: false
    }

    Connections {
        target: profileSettingsPageLoader.item
        onBackClicked: { stackView.pop(); profileSettingsPageLoader.active = false; }

    }

    Connections {
        target: balanceSettingsPageLoader.item
        onBackClicked: { stackView.pop(); balanceSettingsPageLoader.active = false; }
    }

    Connections {
        target: languageSettingsPageLoader.item
        onBackClicked: { stackView.pop(); languageSettingsPageLoader.active = false; }
    }

    Connections {
        target: countryRatesPageLoader.item
        onBackClicked: { stackView.pop(); countryRatesPageLoader.active = false; }
    }

    //menu model - just simple menu text items
    ListModel {
        id: settingsListModel

        ListElement {
            settingsName: qsTr("Profile")
            category: "Account"
        }

        ListElement {
            settingsName: qsTr("Balance")
            category: "Account"
        }

        ListElement {
            settingsName: qsTr("Rates")
            category: "Account"
        }

        ListElement {
            settingsName: qsTr("Earn Credits")
            category: "Account"
        }

        ListElement {
            settingsName: qsTr("Languages")
            category: "Settings"
        }

        ListElement {
            settingsName: qsTr("Help")
            category: "Settings"
        }

        ListElement {
            settingsName: qsTr("About")
            category: "Settings"
        }

        ListElement {
            category: "Misc"
            settingsName: qsTr("Sign out")
        }
    }
}
