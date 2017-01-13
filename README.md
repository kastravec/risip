Copyright (C) 2016 - 2017  Petref Saraci
 http://risip.io
 petref.saraci@risip.io

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You have received a copy of the GNU General Public License
 along with this program. See LICENSE.GPLv3
 A copy of the license can be found also here <http://www.gnu.org/licenses/>.


## What is Risip VoIP SDK? ##

Risip VoIP SDK provides a set of APIs, headers, source files and other utilities for creating and enabling RTC audio/video applications. 

Risip VoIP uses:
- Pjsip
- Qt

Risip SDK APIs are for pure QML based applications and/or just with Qt/C++


## How do I start? How do I use Risip SDK in my Qt application? ##

**1. Download the latest Qt open source package and follow the instructions**. (http://qt.io)

Install Qt for your platforms that that you want to build apps for (Desktop,  and make sure everything is running smoothly with QtCreator IDE.

**2. Install Pjsip library**.

Risip provides Pjsip binaries for all supported platforms (Win, OSX, Linux, Android, iOS) and they can be found in the Download section of this repository. The link can be found here:

**https://bitbucket.org/psaraci/risip/downloads**

Inside the **risip-voipsdk** root folder, create a new folder and name it **pjsip**. 
Inside the **pjsip** folder, place the pjsip **include** directory and the pjsip **libraries** for each respective platform.

For a complete example see risipsdk-demo and risipapp repositories and the respective wikis.

You can always download and use any latest version of Pjsip, build it yourself and use the libs in Risip SDK.

**3. Use QtCreator to load and build the Risip SDK project**.

You may use QtCreator to compile/build risip for all desktop platforms and Android, but for iOS you may need
to use XCode too.