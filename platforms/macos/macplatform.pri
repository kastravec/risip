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
CONFIG += app_bundle

INCLUDEPATH += $$PWD/pjsip/include

QMAKE_MAC_SDK = macosx10.12
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12
QMAKE_CXXFLAGS += -mmacosx-version-min=10.12
QMAKE_LFLAGS += -mmacosx-version-min=10.12

# static linking with PJSIP
LIBS += -L$$PWD/pjsip/lib \
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

# dynamic linking with PJSIP
#LIBS += -L$$PWD/pjsip/lib \
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
