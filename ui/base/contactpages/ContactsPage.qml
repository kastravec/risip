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

import Risip 1.0

ContactsPageForm {
    id: contactsPage

    property RisipPhoneContact phoneContact: RisipPhoneContact {}
    property RisipPhoneNumbersModel phoneNumbersModel: phoneContact.phoneNumbersModel
    property string contactFullName: phoneContact.fullName

    buddyModel: RisipContactManager.activeBuddiesModel.proxy
    phoneContactModel: RisipContactManager.phoneContactsModel.proxy

    searchContactInput.onTextChanged: {
        if(contactsListLayout.currentIndex === 0) {
            phoneContactModel.setFilterRegExp(searchContactInput.text);
        } else if(contactsListLayout.currentIndex === 1) {
            RisipContactManager.activeBuddiesModel.proxy.filterRole = RisipBuddiesModel.ContactRole;
            RisipContactManager.activeBuddiesModel.proxy.setFilterRegExp(searchContactInput.text);
        }
    }

    phoneListView.onContactClicked: {
        contactPageLoader.active = true;
        phoneContact = RisipContactManager.contactForName(contactName);
        contactPageLoader.item.phoneNumbersModel = phoneNumbersModel
        contactPageLoader.item.contactName = contactFullName;
        stackView.push(contactPageLoader.item);
    }

    Loader {
        id: contactPageLoader
        active: false
        source: "qrc:/ui/base/contactpages/ContactPage.qml"
    }

    Connections {
        target: contactPageLoader.item
        onBackClicked: {
            stackView.pop();
            contactPageLoader.active = false;
        }
    }
}
