INCLUDEPATH += $$PWD/../pjsip/mac-desktop/include

QMAKE_MAC_SDK = macosx10.12
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12
QMAKE_CXXFLAGS += -mmacosx-version-min=10.12
QMAKE_LFLAGS += -mmacosx-version-min=10.12

LIBS += -L$$PWD/../pjsip/mac-desktop/lib \
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
