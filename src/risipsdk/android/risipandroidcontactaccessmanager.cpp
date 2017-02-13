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
#include "risipandroidcontactaccessmanager.h"
#include "risipphonecontact.h"

#include <QXmlStreamReader>
#include <QJsonParseError>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroidExtras>
#include <QtAndroid>
#endif

namespace risip {

RisipAndroidContactAccessManager::RisipAndroidContactAccessManager(QObject *parent)
    :QObject(parent)
{
}

void RisipAndroidContactAccessManager::fetchContactsFromDevice()
{
#ifdef Q_OS_ANDROID
    //Request permissions
//    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
//    QAndroidJniObject::callStaticObjectMethod("com/risip/phonecontacts/RisipAndroidContacts", "requestPermissions", "(Landroid/app/Activity;)Ljava/lang/String;", activity.object<jobject>());

    //Get DATA from Java
//    QAndroidJniObject contacts = QtAndroid.androidActivity().callStaticObjectMethod("com/risip/phonecontacts/RisipAndroidContacts", "readContacts", "(Landroid/app/Activity;)Ljava/lang/String;"); /*, activity.object<jobject>()*/
//    QAndroidJniObject contacts = QAndroidJniObject::callStaticObjectMethod("com/risip/phonecontacts/RisipAndroidContacts", "readContacts", "(Landroid/app/Activity;)Ljava/lang/String;", QtAndroid.); /*, activity.object<jobject>()*/
//    QString xml=contacts.toString();

    //Parse DATA to Contact struct
//    qDebug()<<"PARSING CONTACTS FROM XML." <<xml;
//    parseXML(xml);

#endif
}

void RisipAndroidContactAccessManager::parseXML(const QString &xml)
{
    int index=1;

    QByteArray buff = QByteArray::fromStdString(xml.toStdString());
    QXmlStreamReader reader;
    reader.addData(buff);
    if (reader.readNextStartElement()) {
        if (reader.name() == "root"){
            RisipPhoneContact *phoneContact = NULL;
            while(reader.readNextStartElement()){
                if(reader.name() == "item"){
                    while(reader.readNextStartElement()) {
                        QString name=reader.name().toString();
                        QString value=reader.readElementText();
                        phoneContact = new RisipPhoneContact;
                        if(name.compare("name")==0) phoneContact->setFullName(value);
                        if(name.compare("id")==0) phoneContact->setContactId(value.toInt());
                        if(name.compare("phone")==0) phoneContact->addPhoneNumber(value);
                        if(name.compare("email")==0) phoneContact->setEmail(value);
//                        if(name.compare("etype")==0) phoneContact->set value;
//                        if(name.compare("note")==0) arr[index].note=value;

                        qDebug() << "PHONE CONTACT : " << phoneContact->fullName();

                        emit phoneContactDiscovered(phoneContact);
                    }
                    index++;
                }
                else
                    reader.skipCurrentElement();
            }
        }
    }
}

} //end of risip namespace
