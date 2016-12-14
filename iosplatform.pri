
#iOS headers and source files where ios specific functionality is implemented.
HEADERS += src/risip/ios/risipioscontactaccessmanager.h

OBJECTIVE_SOURCES += \
    src/risip/ios/risipcallkitprovider.mm \
    src/risip/ios/risipioscontactaccessmanager.mm \
    src/risip/ios/risipioswifiprovider.mm

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
