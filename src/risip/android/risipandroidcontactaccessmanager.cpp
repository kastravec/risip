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
#include <QJsonParseError>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroidExtras>
#endif

RisipAndroidContactAccessManager::RisipAndroidContactAccessManager(QObject *parent)
    :QObject(parent)
{

}

void RisipAndroidContactAccessManager::fetchContactsFromDevice()
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject contacts = QtAndroid::androidActivity().callObjectMethod("readContacts","()[Ljava/lang/String;");
    QAndroidJniEnvironment env;
    int size = env->GetArrayLength(contacts.object<jintArray>());
    for (int i=0; i < size; ++i) {
        QAndroidJniObject arrayElemen = env->GetObjectArrayElement(contacts.object<jobjectArray>(), i);
        QString jsonData = arrayElemen.toString();
        QByteArray rawData = jsonData.toUtf8();
        QJsonParseError p;
        QJsonDocument contacts = QJsonDocument::fromJson(rawData,&p);
        QJsonObject contactInfo = contacts.object();
        QString id;
        QString name;

        foreach (QString key, contactInfo.keys()) {
            QString value = contactInfo.value(key).toString();
            if(key.compare("id") == 0) {
                id = value;
            } else if(key.compare("name") == 0) {
                name = value;
            } else {
                qDebug()<<"NUMBER: " <<id <<name << key << value;
            }
        }

    }

#endif

}
