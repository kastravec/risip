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

message("Running the ios-platform settings!")

#QMAKE_IOS_DEPLOYMENT_TARGET = 10.2

#QMAKE_MAC_SDK = macosx10.12

QMAKE_CXXFLAGS += -miphoneos-version-min=9.0
QMAKE_LFLAGS += -miphoneos-version-min=9.0
QMAKE_CXXFLAGS -= -fembed-bitcode-marker -fembed-bitcode
QMAKE_LFLAGS -= -fembed-bitcode-marker -fembed-bitcode

INCLUDEPATH += $$PWD/pjsip/include

#iOS headers and source files where ios specific functionality is implemented.
HEADERS += $$PWD/../../src/risipsdk/ios/risipioscontactaccessmanager.h

OBJECTIVE_SOURCES += \
    $$PWD/../../src/risipsdk/ios/risipioscontactaccessmanager.mm

LIBS += -L$$PWD/pjsip/staticlibs \
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
    -lwebrtc-arm64-apple-darwin_ios \
#    -lyuv-arm64-apple-darwin_ios \
    -framework CoreFoundation \
    -framework CFNetwork \
    -framework Contacts \
    -framework AVFoundation
