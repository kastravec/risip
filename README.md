Copyright (C) 2016 - 2017  Petref Saraci

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


## What is Risip ? ##

Risip provides a set of APIs, headers, source files and other utilities for creating and enabling RTC audio/video applications. 

Risip VoIP uses:
- Pjsip
- Qt


## What is PJSIP ? ##

Learn about Pjsip here https://www.pjsip.org/
Documentation on pjsip, how build and use: https://trac.pjsip.org/repos

quick tip -> configuring and building Pjsip for Linux

1. run the configure script

./configure --prefix="the install directory" 

2. make dep 
3. make
4. make install

-------------------------------------------------


### **How to start using Risip in my project?** ###

Using Risip in your Qt / QML project is very easy and straightforward.

1. Make sure you have Qt installed and running properly.

2. Create your project with Qt Creator

3. Inside your project folder, clone the risip-voipsdk repo.

4. Edit the .pro file of your project and add the following this line include(risip-voipsdk/risip-voipsdk.pri) on top.

[RisipApp](https://github.com/psaraci/risipapp) is based on Risip and is another great example to see how include the sdk in your project.


-------------------------------------------------

### **How to start developing and contributing your code for Risip?

**1. Download the latest Qt open source package and follow the instructions**. (http://qt.io)

Install Qt for your platforms that that you want to build apps for (Desktop,  and make sure everything is running smoothly with QtCreator IDE.


**2. Create a risi-voipsdk repo clone. **.

Risip provides Pjsip binaries for all supported platforms (Win, OSX, Linux, Android, iOS) already as part of the repositories. 

**3. Use QtCreator to load and build the Risip project**. 

You may use QtCreator to compile/build risip for all desktop platforms and Android, but for iOS you may need to use XCode too.


---------------------------------------------------


### **How to use the QML Risip ?** 

**Risip QML API**

**QML Components ( Singleton - non instantiatable )**

- Risip

One of the main QML Components in Risip SDK. Use this singleton to create Risip SIP accounts in the Risip engine, save and read settings locally. This component initializes and registers all the rest of QML components, it does this automatically.

- RisipEndpoint

Represents the SIP Endpoint / engine. You need to call *start()* and *stop()* in your application, upon creation and deletion respectively. Use this component to set various settings to your SIP endpoint. The default settings are typically suitable for most use cases.

- RisipCallManager

Use this singleton component to make and accept calls to any SIP contact or phone number. It provides a simple and intuitive API, see its documentation and the risipsdk-demo example for more details. It is recommended to use this component to handle calls in order to have call history and logging working out of the box. Otherwise you can handle calls yourself using RisipAccount and RisipCall.

- RisipContactManager

Use this singleton to access and manage your contacts and your contacts models that can be used directly into QML Views.

- RisipRateManger


**QML Components ( instantiatable components)**

- RisipAccount

This is a main component in Risip SDK. It provides a simple and intuitive API for creating and managing SIP accounts.
In Risip you do not create RisipAccounts manually, but you **must** use *Risip.createAccount()* method from Risip singleton.

- RisipAccountConfiguration

- RisipCall

- RisipMedia

- RispBuddy

- RisipMessage

- RisipPhoneNumber

- RisipPhoneContact


**How to use the API ?**

Use QtCreator and you will get code auto-completion for all Risip SDK APIs.

To refer to each component API details, to see its Properties and Methods, please refer to the documentation, or the respective C++ header file. See (risip-voipsdk/src/risipsdk/headers/*respective_file.h*).

The Risip Singleton components help you get start very easily with RIsip.


**First**:

You need to let Risip initiate itself, load the necessary plugins and modules.

To do that you call:

*RisipEndpoint.start();*

When your application is shutting down then you need let Risip know, like this:

*RisipEndpoint.stop();*


**Second**:

To register a SIP user account, it works like this in Risip.

Create a RisipAccountConfiguration object:

*RisipAccountConfiguration {
	id: myAccountConf
}*

Create the account using Risip Singleton:

*Risip.createAccount(myAccountConf);*

It is recommended to use Risip Default Account as a “default” SIP account, it makes the API easier to use:

*Risip.setDefaultAccount(myAccountConf.uri);*
