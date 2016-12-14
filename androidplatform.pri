QT += androidextras
INCLUDEPATH += $$PWD/pjsip/android-armeabi/include

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
        android/AndroidManifest.xml \
        android/res/values/libs.xml \
        android/build.gradle \
        src/risip/android/com/risip/phonecontacts/RisipAndroidContacts.java

LIBS += -L$$PWD/pjsip/android-armeabi \
    #-lpjmedia-audiodev-arm-unknown-linux-androideabi \
    #-lpjmedia-videodev-arm-unknown-linux-androideabi \
    #-lpjsip-arm-unknown-linux-androideabi \
    -lpjsua2-arm-unknown-linux-androideabi \
    -lpjsua-arm-unknown-linux-androideabi \
    -lpjsip-simple-arm-unknown-linux-androideabi \
#    -lpjsdp-arm-unknown-linux-androideabi \
    -lpjmedia-arm-unknown-linux-androideabi \
    -lpjsip-arm-unknown-linux-androideabi \
    -lpjmedia-audiodev-arm-unknown-linux-androideabi \
    -lpjsip-ua-arm-unknown-linux-androideabi \
    -lpjnath-arm-unknown-linux-androideabi \
    -lpjmedia-codec-arm-unknown-linux-androideabi \
    -lpj-arm-unknown-linux-androideabi \
    -lpjmedia-arm-unknown-linux-androideabi \
    -lilbccodec-arm-unknown-linux-androideabi \
    -lgsmcodec-arm-unknown-linux-androideabi \
    -lspeex-arm-unknown-linux-androideabi \
    -lresample-arm-unknown-linux-androideabi \
    -lsrtp-arm-unknown-linux-androideabi \
    -lpj-arm-unknown-linux-androideabi \
    -lpjlib-util-arm-unknown-linux-androideabi \
    -lwebrtc-arm-unknown-linux-androideabi \
    -lg7221codec-arm-unknown-linux-androideabi
