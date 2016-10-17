TARGET = risip
TEMPLATE = app

#CONFIG -= app_bundle
CONFIG += exceptions c++11 static

QT -= gui
QT = core network qml quick quickcontrols2

QT += svg
QTPLUGIN += qtvirtualkeyboardplugin

### DEFINES ###
DEFINES += PJ_IS_LITTLE_ENDIAN=1 \
           PJ_IS_BIG_ENDIAN=0 \
           PJ_AUTOCONF

### INCLUDES ###
INCLUDEPATH += $$PWD/src \
               $$PWD/risip/headers

# this include path depends on the directory of your pjsip headers/libs
# it could be different directory names for different platform where you put the pjsip headers
# e.g. Linux desktop -> pjsip/linux-desktop , Mac desktop -> pjsip/mac-desktop, Android-arm64 -> pjsip/android-arm64

#INCLUDEPATH += $$PWD/pjsip/linux-desktop/include
INCLUDEPATH += $$PWD/pjsip/mac-desktop/include
#INCLUDEPATH += $$PWD/pjsip/ios-arm64/include


### SOURCE & HEADER FILES ###
SOURCES += src/main.cpp \
    risip/risip.cpp \
    risip/risipaccount.cpp \
    risip/risipcall.cpp \
    risip/risipbuddy.cpp \
    risip/risipmedia.cpp \
    risip/risipendpoint.cpp \
    risip/risipmessage.cpp \
    risip/risipaccountconfiguration.cpp \
    src/uiloader.cpp \
    risip/risipcallhistorymodel.cpp \
    src/applicationsettings.cpp \
    risip/globals.cpp \
    sipregistrars/httpmanager.cpp \
    sipregistrars/httpresponse.cpp \
    sipregistrars/mor/morapi.cpp

HEADERS += risip/headers/risip.h \
    risip/headers/risipaccount.h \
    risip/headers/risipcall.h \
    risip/headers/risipbuddy.h \
    risip/headers/risipmedia.h \
    risip/headers/risipendpoint.h \
    risip/headers/globals.h \
    risip/headers/risipinterface.h \
    risip/headers/risipmessage.h \
    risip/headers/risipaccountconfiguration.h \
    src/uiloader.h \
    risip/headers/risipcallhistorymodel.h \
    src/applicationsettings.h \
    sipregistrars/httpmanager.h \
    sipregistrars/httpresponse.h \
    sipregistrars/mor/morapi.h

macx {
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12
}

########## LIBS ##########

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
    -lpjlib-util-arm64-apple-darwin_ios
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
        android/build.gradle
}

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = risip/

DISTFILES += \
    README \
    utils/runOnMac.sh \
    LICENSE.GPLv3 \
    LICENSE.HEADER.GPLv3 \
    ui/base/qtquickcontrols2.conf
