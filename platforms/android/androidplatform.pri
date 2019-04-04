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
message("Running the android-platform settings!")

DESTDIR = $$PWD/bin

QT += androidextras

INCLUDEPATH += $$PWD/pjsip/include

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-build

DISTFILES += $$PWD/../../src/risipsdk/android/com/risip/phonecontacts/RisipAndroidContacts.java \
    $$PWD/android-build/AndroidManifest.xml \
    $$PWD/android-build/gradle/wrapper/gradle-wrapper.jar \
    $$PWD/android-build/android/gradlew \
    $$PWD/android-build/android/res/values/libs.xml \
    $$PWD/android-build/android/build.gradle \
    $$PWD/android-build/android/gradle/wrapper/gradle-wrapper.properties \
    $$PWD/android-build/android/gradlew.bat

LIBS += -L$$PWD/pjsip/staticlibs \
    #-lpjmedia-audiodev-armv7-unknown-linux-android \
    #-lpjmedia-videodev-armv7-unknown-linux-android \
    #-lpjsip-armv7-unknown-linux-android \
    -lpjsua2-armv7-unknown-linux-android \
    -lpjsua-armv7-unknown-linux-android \
    -lpjsip-simple-armv7-unknown-linux-android \
    #-lpjsdp-armv7-unknown-linux-android \
    -lpjmedia-armv7-unknown-linux-android \
    -lpjsip-armv7-unknown-linux-android \
    -lpjmedia-audiodev-armv7-unknown-linux-android \
    -lpjsip-ua-armv7-unknown-linux-android \
    -lpjnath-armv7-unknown-linux-android \
    -lpjmedia-codec-armv7-unknown-linux-android \
    -lpj-armv7-unknown-linux-android \
    -lpjmedia-armv7-unknown-linux-android \
    -lilbccodec-armv7-unknown-linux-android \
    -lgsmcodec-armv7-unknown-linux-android \
    -lspeex-armv7-unknown-linux-android \
    -lresample-armv7-unknown-linux-android \
    -lsrtp-armv7-unknown-linux-android \
    -lpj-armv7-unknown-linux-android \
    -lpjlib-util-armv7-unknown-linux-android \
    -lwebrtc-armv7-unknown-linux-android \
    -lg7221codec-armv7-unknown-linux-android
