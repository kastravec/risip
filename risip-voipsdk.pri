macx:!iphoneos: {
include($$PWD/platforms/macos/macplatform.pri)
CONFIG += app_bundle
}

iphoneos {
include($$PWD/platforms/ios/iosplatform.pri)
}

android {
include($$PWD/platforms/android/androidplatform.pri)
}

linux:!android:!macx {
include($$PWD/platforms/linux/linuxplatform.pri)
}

win32 {
include($$PWD/platforms/win/winplatform.pri)
}

CONFIG += exceptions c++11

DEFINES += RISIP_SDK \
           PJ_IS_LITTLE_ENDIAN=1 \
           PJ_IS_BIG_ENDIAN=0 \
           PJ_AUTOCONF \
           QT_DEPRECATED_WARNINGS

QT = gui \
     core \
     network \
     qml \
     quick \
     quickcontrols2

INCLUDEPATH += $$PWD/src/risipsdk/headers \
               $$PWD/src/risipsdk/headers/models \
               $$PWD/src/risipsdk/headers/apploader \
               $$PWD/src/risipsdk/headers/location \
               $$PWD/src/risipsdk/headers/sipprovision \
               $$PWD/src/risipsdk/headers/utils

RESOURCES += $$PWD/risipconfigs.qrc \
             $$PWD/flagicons.qrc

### SOURCE & HEADER FILES ###
SOURCES += $$PWD/src/risipsdk/risip.cpp \
    $$PWD/src/risipsdk/risipaccount.cpp \
    $$PWD/src/risipsdk/risipcall.cpp \
    $$PWD/src/risipsdk/risipbuddy.cpp \
    $$PWD/src/risipsdk/risipmedia.cpp \
    $$PWD/src/risipsdk/risipendpoint.cpp \
    $$PWD/src/risipsdk/risipmessage.cpp \
    $$PWD/src/risipsdk/risipaccountconfiguration.cpp \
    $$PWD/src/risipsdk/risipcallmanager.cpp \
    $$PWD/src/risipsdk/risipcontactmanager.cpp \
    $$PWD/src/risipsdk/models/risipmodels.cpp \
    $$PWD/src/risipsdk/risipphonecontact.cpp \
    $$PWD/src/risipsdk/risipglobals.cpp \
    $$PWD/src/risipsdk/risipcontactimageprovider.cpp \
    $$PWD/src/risipsdk/risipcountryflagimageprovider.cpp \
    $$PWD/src/risipsdk/risipratemanager.cpp \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipaccount.cpp \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipcall.cpp \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipbuddy.cpp \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipendpoint.cpp \
    $$PWD/src/risipsdk/risipphonenumber.cpp \
    $$PWD/src/risipsdk/models/risipabstractbuddymodel.cpp \
    $$PWD/src/risipsdk/models/risipcallhistorymodel.cpp \
    $$PWD/src/risipsdk/models/risipphonecontactsmodel.cpp \
    $$PWD/src/risipsdk/models/risipcountryratesmodel.cpp \
    $$PWD/src/risipsdk/models/risipphonenumbersmodel.cpp \
    $$PWD/src/risipsdk/android/risipandroidcontactaccessmanager.cpp \
    $$PWD/src/risipsdk/apploader/risipapplicationsettings.cpp \
    $$PWD/src/risipsdk/apploader/risipuiloader.cpp \
    $$PWD/src/risipsdk/utils/qqmlsortfilterproxymodel.cpp \
    $$PWD/src/risipsdk/utils/filter.cpp \
    $$PWD/src/risipsdk/utils/sorter.cpp \
    $$PWD/src/risipsdk/utils/stopwatch.cpp \
    $$PWD/src/risipsdk/models/risipaccountlistmodel.cpp

HEADERS += $$PWD/src/risipsdk/headers/risip.h \
    $$PWD/src/risipsdk/headers/risipaccount.h \
    $$PWD/src/risipsdk/headers/risipcall.h \
    $$PWD/src/risipsdk/headers/risipbuddy.h \
    $$PWD/src/risipsdk/headers/risipmedia.h \
    $$PWD/src/risipsdk/headers/risipendpoint.h \
    $$PWD/src/risipsdk/headers/risipmessage.h \
    $$PWD/src/risipsdk/headers/risipaccountconfiguration.h \
    $$PWD/src/risipsdk/headers/risipcallmanager.h \
    $$PWD/src/risipsdk/headers/risipcontactmanager.h \
    $$PWD/src/risipsdk/headers/models/risipmodels.h \
    $$PWD/src/risipsdk/headers/risipglobals.h \
    $$PWD/src/risipsdk/headers/risipphonecontact.h \
    $$PWD/src/risipsdk/ios/risipcallkprovider.h \
    $$PWD/src/risipsdk/headers/risipcontactimageprovider.h \
    $$PWD/src/risipsdk/headers/risipcountryflagimageprovider.h \
    $$PWD/src/risipsdk/headers/risipratemanager.h \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipaccount.h \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipcall.h \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipbuddy.h \
    $$PWD/src/risipsdk/pjsipwrapper/pjsipendpoint.h \
    $$PWD/src/risipsdk/headers/risipphonenumber.h \
    $$PWD/src/risipsdk/headers/models/risipabstractbuddymodel.h \
    $$PWD/src/risipsdk/headers/models/risipcallhistorymodel.h \
    $$PWD/src/risipsdk/headers/models/risipphonecontactsmodel.h \
    $$PWD/src/risipsdk/headers/models/risipcountryratesmodel.h \
    $$PWD/src/risipsdk/headers/models/risipphonenumbersmodel.h \
    $$PWD/src/risipsdk/android/risipandroidcontactaccessmanager.h \
    $$PWD/src/risipsdk/headers/apploader/risipapplicationsettings.h \
    $$PWD/src/risipsdk/headers/apploader/risipuiloader.h \
    $$PWD/src/risipsdk/headers/utils/qqmlsortfilterproxymodel.h \
    $$PWD/src/risipsdk/headers/utils/filter.h \
    $$PWD/src/risipsdk/headers/utils/sorter.h \
    $$PWD/src/risipsdk/headers/utils/stopwatch.h \
    $$PWD/src/risipsdk/headers/risipsdkglobal.h \
    $$PWD/src/risipsdk/headers/models/risipaccountlistmodel.h

DISTFILES += \
    $$PWD/README \
    $$PWD/LICENSE.GPLv3 \
    $$PWD/LICENSE.HEADER.GPLv3
