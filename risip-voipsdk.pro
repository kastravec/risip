##################################################################################
#    Copyright (C) 2016 - 2017  Petref Saraci
#    http://risip.io
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You have received a copy of the GNU General Public License
#    along with this program. See LICENSE.GPLv3
#    A copy of the license can be found also here <http://www.gnu.org/licenses/>.
#
###################################################################################

macx {
include(platforms/macos/macplatform.pri)
}

ios {
include(platforms/ios/iosplatform.pri)
}

android {
include(platforms/android-platform/androidplatform.pri)
}

linux {
include(platforms/linux/linuxplatform.pri)
}

win32 {
include(platforms/win/winplatform.pri)
}

TARGET = risip-voip
TEMPLATE = lib

CONFIG += exceptions c++11
CONFIG += create_prl
CONFIG += qt
CONFIG += dll

QT = gui \
     core \
     network \
     qml \
     quick \
     quickcontrols2

### DEFINES ###
DEFINES += RISIP_SDK \
           PJ_IS_LITTLE_ENDIAN=1 \
           PJ_IS_BIG_ENDIAN=0 \
           PJ_AUTOCONF

### INCLUDES ###
INCLUDEPATH += $$PWD/src \
               $$PWD/src/risipsdk \
               $$PWD/src/risipsdk/headers \
               $$PWD/src/risipsdk/headers/models \
               $$PWD/src/risipsdk/headers/apploader \
               $$PWD/src/risipsdk/headers/sipprovision \
               $$PWD/src/risipsdk/headers/utils

INCLUDEPATH += $$PWD/pjsip/include

RESOURCES += \
             risipconfigs.qrc \
             flagicons.qrc

### SOURCE & HEADER FILES ###
SOURCES += src/risipsdk/risip.cpp \
    src/risipsdk/risipaccount.cpp \
    src/risipsdk/risipcall.cpp \
    src/risipsdk/risipbuddy.cpp \
    src/risipsdk/risipmedia.cpp \
    src/risipsdk/risipendpoint.cpp \
    src/risipsdk/risipmessage.cpp \
    src/risipsdk/risipaccountconfiguration.cpp \
    src/risipsdk/risipcallmanager.cpp \
    src/risipsdk/risipcontactmanager.cpp \
    src/risipsdk/models/risipmodels.cpp \
    src/risipsdk/risipphonecontact.cpp \
    src/risipsdk/risipglobals.cpp \
    src/risipsdk/risipcontactimageprovider.cpp \
    src/risipsdk/risipcountryflagimageprovider.cpp \
    src/risipsdk/risipratemanager.cpp \
    src/risipsdk/pjsipwrapper/pjsipaccount.cpp \
    src/risipsdk/pjsipwrapper/pjsipcall.cpp \
    src/risipsdk/pjsipwrapper/pjsipbuddy.cpp \
    src/risipsdk/pjsipwrapper/pjsipendpoint.cpp \
    src/risipsdk/risipphonenumber.cpp \
    src/risipsdk/models/risipabstractbuddymodel.cpp \
    src/risipsdk/models/risipcallhistorymodel.cpp \
    src/risipsdk/models/risipphonecontactsmodel.cpp \
    src/risipsdk/models/risipcountryratesmodel.cpp \
    src/risipsdk/models/risipphonenumbersmodel.cpp \
    src/risipsdk/android/risipandroidcontactaccessmanager.cpp \
    src/risipsdk/apploader/risipapplicationsettings.cpp \
    src/risipsdk/apploader/risipuiloader.cpp \
    src/risipsdk/risipuserprofile.cpp \
    src/risipsdk/sipprovision/risipmoruser.cpp \
    src/risipsdk/sipprovision/risipmordevice.cpp \
    src/risipsdk/sipprovision/risipmoruserbalance.cpp \
    src/risipsdk/sipprovision/risipmorapi.cpp \
    src/risipsdk/utils/qqmlsortfilterproxymodel.cpp \
    src/risipsdk/utils/httpnetworkrequest.cpp \
    src/risipsdk/utils/stopwatch.cpp

HEADERS += src/risipsdk/headers/risip.h \
    src/risipsdk/headers/risipaccount.h \
    src/risipsdk/headers/risipcall.h \
    src/risipsdk/headers/risipbuddy.h \
    src/risipsdk/headers/risipmedia.h \
    src/risipsdk/headers/risipendpoint.h \
    src/risipsdk/headers/risipmessage.h \
    src/risipsdk/headers/risipaccountconfiguration.h \
    src/risipsdk/headers/risipcallmanager.h \
    src/risipsdk/headers/risipcontactmanager.h \
    src/risipsdk/headers/models/risipmodels.h \
    src/risipsdk/headers/risipglobals.h \
    src/risipsdk/headers/risipphonecontact.h \
    src/risipsdk/headers/risipuserprofile.h \
    src/risipsdk/ios/risipcallkprovider.h \
    src/risipsdk/ios/risipioswifiprovider.h \
    src/risipsdk/headers/risipcontactimageprovider.h \
    src/risipsdk/headers/risipcountryflagimageprovider.h \
    src/risipsdk/headers/risipratemanager.h \
    src/risipsdk/pjsipwrapper/pjsipaccount.h \
    src/risipsdk/pjsipwrapper/pjsipcall.h \
    src/risipsdk/pjsipwrapper/pjsipbuddy.h \
    src/risipsdk/pjsipwrapper/pjsipendpoint.h \
    src/risipsdk/headers/risipphonenumber.h \
    src/risipsdk/headers/models/risipabstractbuddymodel.h \
    src/risipsdk/headers/models/risipcallhistorymodel.h \
    src/risipsdk/headers/models/risipphonecontactsmodel.h \
    src/risipsdk/headers/models/risipcountryratesmodel.h \
    src/risipsdk/headers/models/risipphonenumbersmodel.h \
    src/risipsdk/android/risipandroidcontactaccessmanager.h \
    src/risipsdk/headers/apploader/risipapplicationsettings.h \
    src/risipsdk/headers/apploader/risipuiloader.h \
    src/risipsdk/headers/sipprovision/risipmoruser.h \
    src/risipsdk/headers/sipprovision/risipmordevice.h \
    src/risipsdk/headers/sipprovision/risipmoruserbalance.h \
    src/risipsdk/headers/sipprovision/risipmorapi.h \
    src/risipsdk/headers/utils/httpnetworkrequest.h \
    src/risipsdk/headers/utils/qqmlsortfilterproxymodel.h \
    src/risipsdk/headers/utils/stopwatch.h \
    src/risipsdk/headers/risipsdkglobal.h

DISTFILES += \
    README \
    LICENSE.GPLv3 \
    LICENSE.HEADER.GPLv3 \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    platforms/android/gradle/wrapper/gradle-wrapper.jar \
    platforms/android/gradlew \
    platforms/android/res/values/libs.xml \
    platforms/android/gradle/wrapper/gradle-wrapper.properties \
    platforms/android/gradlew.bat

