TARGET = risip
TEMPLATE = app

CONFIG += app_bundle
CONFIG += exceptions c++11

QT = gui \
     core \
     network \
     qml \
     quick \
     quickcontrols2 \
     positioning \
     location

### DEFINES ###
DEFINES += PJ_IS_LITTLE_ENDIAN=1 \
           PJ_IS_BIG_ENDIAN=0 \
           PJ_AUTOCONF

### INCLUDES ###
INCLUDEPATH += $$PWD/src \
               $$PWD/src/risip/headers \
               $$PWD/src/utils \
               $$PWD/src/risip/models
               $$PWD/src/modules

macx {
include(platforms/macplatform.pri)
}

ios {
include(platforms/iosplatform.pri)
}

android {
include(platforms/androidplatform.pri)
}

linux {
include(platforms/linuxplatform.pri)
}

win32 {
include(platforms/winplatform.pri)
}

RESOURCES += ui.qrc \
             images.qrc \
             risipconfigs.qrc \
             flagicons.qrc

### SOURCE & HEADER FILES ###
SOURCES += src/app/main.cpp \
    src/risip/risip.cpp \
    src/risip/risipaccount.cpp \
    src/risip/risipcall.cpp \
    src/risip/risipbuddy.cpp \
    src/risip/risipmedia.cpp \
    src/risip/risipendpoint.cpp \
    src/risip/risipmessage.cpp \
    src/risip/risipaccountconfiguration.cpp \
    src/risip/risipcallmanager.cpp \
    src/risip/risipcontactmanager.cpp \
    src/risip/models/risipmodels.cpp \
    src/risip/risipphonecontact.cpp \
    src/app/risipuiloader.cpp \
    src/app/applicationsettings.cpp \
    src/utils/httpnetworkrequest.cpp \
    src/risip/risipglobals.cpp \
    src/sipregistrars/mor/risipmorapi.cpp \
    src/risip/risipcontactimageprovider.cpp \
    src/risip/risipcountryflagimageprovider.cpp \
    src/risip/risipratemanager.cpp \
    src/utils/qqmlsortfilterproxymodel.cpp \
    src/risip/pjsipwrapper/pjsipaccount.cpp \
    src/risip/pjsipwrapper/pjsipcall.cpp \
    src/risip/pjsipwrapper/pjsipbuddy.cpp \
    src/risip/pjsipwrapper/pjsipendpoint.cpp \
    src/risip/risipphonenumber.cpp \
    src/risip/models/risipabstractbuddymodel.cpp \
    src/risip/models/risipcallhistorymodel.cpp \
    src/risip/models/risipphonecontactsmodel.cpp \
    src/risip/models/risipcountryratesmodel.cpp \
    src/risip/models/risipphonenumbersmodel.cpp \
    src/sipregistrars/mor/risipmoruser.cpp \
    src/sipregistrars/mor/risipmordevice.cpp \
    src/sipregistrars/mor/risipmoruserbalance.cpp \
    src/risip/risipuserprofile.cpp \
    src/modules/location/opencagedataapi.cpp \
    src/modules/location/opencagedata.cpp \
    src/modules/location/risiplocation.cpp \
    src/modules/location/risipgeopositionprovider.cpp \
    src/risip/android/risipandroidcontactaccessmanager.cpp \
    src/utils/stopwatch.cpp

HEADERS += src/risip/headers/risip.h \
    src/risip/headers/risipaccount.h \
    src/risip/headers/risipcall.h \
    src/risip/headers/risipbuddy.h \
    src/risip/headers/risipmedia.h \
    src/risip/headers/risipendpoint.h \
    src/risip/headers/risipmessage.h \
    src/risip/headers/risipaccountconfiguration.h \
    src/risip/headers/risipcallmanager.h \
    src/risip/headers/risipcontactmanager.h \
    src/risip/models/risipmodels.h \
    src/risip/headers/risipglobals.h \
    src/risip/headers/risipphonecontact.h \
    src/risip/ios/risipcallkprovider.h \
    src/app/risipuiloader.h \
    src/app/applicationsettings.h \
    src/utils/httpnetworkrequest.h \
    src/sipregistrars/mor/risipmorapi.h \
    src/risip/ios/risipioswifiprovider.h \
    src/risip/headers/risipcontactimageprovider.h \
    src/risip/headers/risipcountryflagimageprovider.h \
    src/risip/headers/risipratemanager.h \
    src/utils/qqmlsortfilterproxymodel.h \
    src/risip/pjsipwrapper/pjsipaccount.h \
    src/risip/pjsipwrapper/pjsipcall.h \
    src/risip/pjsipwrapper/pjsipbuddy.h \
    src/risip/pjsipwrapper/pjsipendpoint.h \
    src/risip/headers/risipphonenumber.h \
    src/risip/models/risipabstractbuddymodel.h \
    src/risip/models/risipcallhistorymodel.h \
    src/risip/models/risipphonecontactsmodel.h \
    src/risip/models/risipcountryratesmodel.h \
    src/risip/models/risipphonenumbersmodel.h \
    src/sipregistrars/mor/risipmoruser.h \
    src/sipregistrars/mor/risipmordevice.h \
    src/sipregistrars/mor/risipmoruserbalance.h \
    src/risip/headers/risipuserprofile.h \
    src/modules/location/opencagedataapi.h \
    src/modules/location/opencagedata.h \
    src/modules/location/risiplocation.h \
    src/modules/location/risipgeopositionprovider.h \
    src/risip/android/risipandroidcontactaccessmanager.h \
    src/utils/stopwatch.h

DISTFILES += \
    README \
    utils/runOnMac.sh \
    LICENSE.GPLv3 \
    LICENSE.HEADER.GPLv3 \
    risip_backlog \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    platforms/android/AndroidManifest.xml \
    platforms/android/gradle/wrapper/gradle-wrapper.jar \
    platforms/android/gradlew \
    platforms/android/res/values/libs.xml \
    platforms/android/build.gradle \
    platforms/android/gradle/wrapper/gradle-wrapper.properties \
    platforms/android/gradlew.bat

