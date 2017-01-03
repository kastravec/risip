##################################################################################
#    Copyright (C) 2016  Petref Saraci
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
DESTDIR = $$PWD/bin

QT += androidextras
INCLUDEPATH += $$PWD/pjsip/armeabi/include

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
        $$PWD/../../src/risipsdk/android/com/risip/phonecontacts/RisipAndroidContacts.java

LIBS += -L$$PWD/pjsip/armeabi/lib \
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
