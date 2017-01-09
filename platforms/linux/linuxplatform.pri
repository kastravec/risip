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
DESTDIR = $$PWD/bin

# static linking with PJSIP
#LIBS += -L$$PWD/../../pjsip/linux-64/staticlibs \
#    -lpjsua2-x86_64-unknown-linux-gnu \
#    -lpjsua-x86_64-unknown-linux-gnu \
#    -lpjsip-ua-x86_64-unknown-linux-gnu \
#    -lpjsip-simple-x86_64-unknown-linux-gnu \
#    -lpjsip-x86_64-unknown-linux-gnu \
#    -lpjmedia-x86_64-unknown-linux-gnu \
#    -lpjmedia-audiodev-x86_64-unknown-linux-gnu \
#    -lpjmedia-x86_64-unknown-linux-gnu \
#    -lpjmedia-codec-x86_64-unknown-linux-gnu \
#    -lpj-x86_64-unknown-linux-gnu \
#    -lpjnath-x86_64-unknown-linux-gnu \
#    -lilbccodec-x86_64-unknown-linux-gnu \
#    -lgsmcodec-x86_64-unknown-linux-gnu \
#    -lspeex-x86_64-unknown-linux-gnu \
#    -lresample-x86_64-unknown-linux-gnu \
#    -lsrtp-x86_64-unknown-linux-gnu \
#    -lpjlib-util-x86_64-unknown-linux-gnu \
#    -lg7221codec-x86_64-unknown-linux-gnu

# dynamic linking with PJSIP
LIBS += -L$$PWD/../../pjsip/linux-64/dylibs \
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
