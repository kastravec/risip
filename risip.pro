TARGET = risip
TEMPLATE = app

#CONFIG -= app_bundle
CONFIG += exceptions c++11

android: QT += androidextras
QT = gui core network qml quick quickcontrols2 positioning location

### DEFINES ###
DEFINES += PJ_IS_LITTLE_ENDIAN=1 \
           PJ_IS_BIG_ENDIAN=0 \
           PJ_AUTOCONF

### INCLUDES ###
INCLUDEPATH += $$PWD/src \
               $$PWD/src/risip/headers \
               $$PWD/src/utils \
               $$PWD/src/risip/models

# this include path depends on the directory of your pjsip headers/libs
# it could be different directory names for different platform where you put the pjsip headers
# e.g. Linux desktop -> pjsip/linux-desktop , Mac desktop -> pjsip/mac-desktop, Android-arm64 -> pjsip/android-arm64
#INCLUDEPATH += $$PWD/pjsip/linux-desktop/include
INCLUDEPATH += $$PWD/pjsip/mac-desktop/include
INCLUDEPATH += $$PWD/pjsip/ios-arm64/include

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
    src/risip/risipaccountprofile.cpp \
    src/risip/risipcallmanager.cpp \
    src/risip/risipcontactmanager.cpp \
    src/risip/models/risipmodels.cpp \
    src/risip/risipphonecontact.cpp \
    src/app/risipuiloader.cpp \
    src/app/applicationsettings.cpp \
    src/utils/httpnetworkrequest.cpp \
    src/risip/risipgeopositionprovider.cpp \
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
    src/risip/models/risipphonenumbersmodel.cpp

HEADERS += src/risip/headers/risip.h \
    src/risip/headers/risipaccount.h \
    src/risip/headers/risipcall.h \
    src/risip/headers/risipbuddy.h \
    src/risip/headers/risipmedia.h \
    src/risip/headers/risipendpoint.h \
    src/risip/headers/risipmessage.h \
    src/risip/headers/risipaccountconfiguration.h \
    src/risip/headers/risipaccountprofile.h \
    src/risip/headers/risipcallmanager.h \
    src/risip/headers/risipcontactmanager.h \
    src/risip/models/risipmodels.h \
    src/risip/headers/risipglobals.h \
    src/risip/headers/risipphonecontact.h \
    src/risip/ios/risipcallkprovider.h \
    src/app/risipuiloader.h \
    src/app/applicationsettings.h \
    src/utils/httpnetworkrequest.h \
    src/risip/headers/risipgeopositionprovider.h \
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
    src/risip/models/risipphonenumbersmodel.h

#iOS headers and source files where ios specific functionality is implemented.
ios {
HEADERS += src/risip/ios/risipioscontactaccessmanager.h

OBJECTIVE_SOURCES += \
    src/risip/ios/risipcallkitprovider.mm \
    src/risip/ios/risipioscontactaccessmanager.mm
}


########## LIBS -- linking with PJSIP libraries different platforms could be dynamic or static ##########

macx {
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12
}

linux {
LIBS += -L$$PWD/pjsip/linux-desktop/lib \
    -lpjsua2 \
    -lpjsua \
    -lpjsip-ua \
    -lpjsip-simple \
    -lpjsip \
    -lpjmedia \
    -lpjmedia-audiodev \
    -lpjmedia \
    -lpjmedia-codec \
    -lpj \
    -lpjnath \
    -lilbccodec \
    -lgsmcodec \
    -lspeex \
    -lresample \
    -lsrtp \
    -lpjlib-util \
    -lg7221codec
}

ios {
LIBS += -L$$PWD/pjsip/ios-64/lib \
    -lpjsua2-arm64-apple-darwin_ios \
    -lpjsua-arm64-apple-darwin_ios \
    -lpjsip-ua-arm64-apple-darwin_ios \
    -lpjsip-simple-arm64-apple-darwin_ios \
    -lpjsip-arm64-apple-darwin_ios \
    -lpjmedia-arm64-apple-darwin_ios \
    -lpjmedia-audiodev-arm64-apple-darwin_ios \
    -lpjmedia-arm64-apple-darwin_ios \
    -lpjmedia-codec-arm64-apple-darwin_ios \
    -lpjmedia-arm64-apple-darwin_ios \
    -lpj-arm64-apple-darwin_ios \
    -lpjnath-arm64-apple-darwin_ios \
    -lilbccodec-arm64-apple-darwin_ios \
    -lgsmcodec-arm64-apple-darwin_ios \
    -lspeex-arm64-apple-darwin_ios \
    -lresample-arm64-apple-darwin_ios \
    -lsrtp-arm64-apple-darwin_ios \
    -lpjlib-util-arm64-apple-darwin_ios \
    -framework CoreFoundation \
    -framework CFNetwork \
    -framework Contacts \
    -framework AVFoundation
}

macx {
LIBS += -L$$PWD/pjsip/mac-desktop/lib \
#    -lpjsua2 \
#    -lpjsua \
#    -lpjsip-ua \
#    -lpjsip-simple \
#    -lpjsip \
##    -lpjsdp \
#    -lpjmedia \
#    -lpjmedia-audiodev \
#    -lpjmedia \
#    -lpjmedia-codec \
#    -lpj \
#    -lpjnath \
#    -lilbccodec \
#    -lgsmcodec \
#    -lspeex \
#    -lresample \
#    -lsrtp \
#    -lpjlib-util \
#    -lg7221codec \
    -lpjsua2-x86_64-apple-darwin15.6.0 \
    -lpjsua-x86_64-apple-darwin15.6.0 \
    -lpjsip-simple-x86_64-apple-darwin15.6.0 \
#    -lpjsdp-x86_64-apple-darwin15.6.0 \
    -lpjmedia-x86_64-apple-darwin15.6.0 \
    -lpjsip-x86_64-apple-darwin15.6.0 \
    -lpjmedia-audiodev-x86_64-apple-darwin15.6.0 \
    -lpjsip-ua-x86_64-apple-darwin15.6.0 \
    -lpjnath-x86_64-apple-darwin15.6.0 \
    -lpjmedia-x86_64-apple-darwin15.6.0 \
    -lpj-x86_64-apple-darwin15.6.0 \
    -lpjmedia-x86_64-apple-darwin15.6.0 \
    -lpjmedia-codec-x86_64-apple-darwin15.6.0 \
    -lpjmedia-videodev-x86_64-apple-darwin15.6.0 \
    -lilbccodec-x86_64-apple-darwin15.6.0 \
    -lgsmcodec-x86_64-apple-darwin15.6.0 \
    -lspeex-x86_64-apple-darwin15.6.0 \
    -lresample-x86_64-apple-darwin15.6.0 \
    -lsrtp-x86_64-apple-darwin15.6.0 \
    -lpjlib-util-x86_64-apple-darwin15.6.0 \
    -lg7221codec-x86_64-apple-darwin15.6.0 \
    -framework CoreAudio \
    -framework AudioToolbox \
    -framework AudioUnit
}

#win32 {
#LIBS += -L$$PWD/pjsip/iphone-lib \
#        -llibpjproject-i386-Win32-vc8-Release-Dynamic \
#        -lpjlib-util-i386-Win32-vc8-Release-Dynamic \
#        -lpjsua2-lib-i386-Win32-vc8-Release-Dynamic \
#        -lpjsua-lib-i386-Win32-vc8-Release-Dynamic \
#        -lpjsip-ua-i386-Win32-vc8-Release-Dynamic \
#        -lpjsip-simple-i386-Win32-vc8-Release-Dynamic \
#        -lpjsip-core-i386-Win32-vc8-Release-Dynamic \
#        -lpjmedia-codec-i386-Win32-vc8-Release-Dynamic \
#        -lpjmedia-i386-Win32-vc8-Release-Dynamic \
#        -lpjmedia-codec-i386-Win32-vc8-Release-Dynamic \
#        -lpjmedia-audiodev-i386-Win32-vc8-Release-Dynamic \
#        -lpjnath-i386-Win32-vc8-Release-Dynamic \
#        -lpjlib-i386-Win32-vc8-Release-Dynamic \
#        -llibbaseclasses-i386-Win32-vc8-Release-Dynamic \
#        -llibg7221codec-i386-Win32-vc8-Release-Dynamic \
#        -llibportaudio-i386-Win32-vc8-Release-Dynamic \
#        -llibgsmcodec-i386-Win32-vc8-Release-Dynamic \
#        -llibilbccodec-i386-Win32-vc8-Release-Dynamic \
#        -llibspeex-i386-Win32-vc8-Release-Dynamic \
#        -llibresample-i386-Win32-vc8-Release-Dynamic \
#        -llibmilenage-i386-Win32-vc8-Release-Dynamic \
#        -llibsrtp-i386-Win32-vc8-Release-Dynamic \
#        -lwinmm \
#        -lole32 \
#        -lws2_32 \
#        -lwsock32 \
#        -lgdi32
#}

#android {
#LIBS += -L$$PWD/pjsip/lib \
#    #-lpjmedia-audiodev-arm-unknown-linux-androideabi \
#    #-lpjmedia-videodev-arm-unknown-linux-androideabi \
#    #-lpjsip-arm-unknown-linux-androideabi \
#    -lpjsua2-arm-unknown-linux-androideabi \
#    -lpjsua-arm-unknown-linux-androideabi \
#    -lpjsip-simple-arm-unknown-linux-androideabi \
#    -lpjsdp-arm-unknown-linux-androideabi \
#    -lpjmedia-arm-unknown-linux-androideabi \
#    -lpjsip-arm-unknown-linux-androideabi \
#    -lpjmedia-audiodev-arm-unknown-linux-androideabi \
#    -lpjsip-ua-arm-unknown-linux-androideabi \
#    -lpjnath-arm-unknown-linux-androideabi \
#    -lpjmedia-codec-arm-unknown-linux-androideabi \
#    -lpj-arm-unknown-linux-androideabi \
#    -lpjmedia-arm-unknown-linux-androideabi \
#    -lilbccodec-arm-unknown-linux-androideabi \
#    -lgsmcodec-arm-unknown-linux-androideabi \
#    -lspeex-arm-unknown-linux-androideabi \
#    -lresample-arm-unknown-linux-androideabi \
#    -lsrtp-arm-unknown-linux-androideabi \
#    -lpj-arm-unknown-linux-androideabi \
#    -lpjlib-util-arm-unknown-linux-androideabi
#    #-lg7221codec-arm-unknown-linux-androideabi \
#}

android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    DISTFILES += \
        android/AndroidManifest.xml \
        android/res/values/libs.xml \
        android/build.gradle \
        src/risip/android/com/risip/phonecontacts/RisipAndroidContacts.java
}

RESOURCES += qml.qrc \
    images.qrc \
    risipconfigs.qrc \
    flagicons.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = risip/

DISTFILES += \
    README \
    utils/runOnMac.sh \
    LICENSE.GPLv3 \
    LICENSE.HEADER.GPLv3 \
    risip_backlog \
    src/utils/SortFilterProxyModel/qpm.json \
    src/utils/SortFilterProxyModel/SortFilterProxyModel.pri \
    src/utils/SortFilterProxyModel/LICENSE \
    src/utils/SortFilterProxyModel/README.md

OBJECTIVE_SOURCES += \
    src/risip/ios/risipioswifiprovider.mm
