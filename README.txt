FreeSSM
=======

FreeSSM is a free and easy to use diagnostic and adjustment tool for SUBARU®
vehicles. It currently supports the models LEGACY®, LIBERTY®, OUTBACK®, BAJA®,
IMPREZA®, FORESTER® and TRIBECA® starting with model year 1999 and provides
access to the engine and transmission control units.

PLEASE NOTE:
This program is NOT A PRODUCT OF FUJI HEAVY INDUSTRIES LTD. OR ANY SUBARU®-
ASSOCIATED COMPANY. It is a free reengineering project which is not contributed,
provided or supported by any company in any way.

All trademarks are property of Fuji Heavy Industries Ltd. or their respective
owners.

--------------------------------------------------------------------------------

0. TABLE OF CONTENTS:

   1. Licence
   2. Supported Platforms
   3. Requirements
   4. Compilation
   5. Installation
   6. Starting FreeSSM

--------------------------------------------------------------------------------

1. LICENCE:

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
You should have received a copy of the GNU General Public License along with
this program (see file LICENCE.txt). If not, see <http://www.gnu.org/licenses/>.

The use of this program is AT YOUR OWN RISK. The author is NOT LIABLE FOR ANY
EFFECTS caused by usage, including UNEXPECTED VEHICLE BEHAVIOR or DAMAGES.
This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of FUNCTIONALITY ON YOUR VEHICLE,
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.


TinyXML is released under the zlib license, see src/tinyxml/readme.txt and
www.sourceforge.net/projects/tinyxml for further informations.

--------------------------------------------------------------------------------

2. SUPPORTED PLATFORMS:

   - Linux/X11
   - Windows

With minor modifications, it should be possible to build FreeSSM on MacOS X
and other Unix systems, too, but there is currently no offical support.

--------------------------------------------------------------------------------

3. REQUIREMENTS:
   1.) FreeSSM source code (https://github.com/Comer352L/FreeSSM)
   2.) Qt4 >= 4.4.0 (qt-project.org)
       - for Windows, the MinGW-version is required (not the VS version)
       - compilation has been tested only with Qt 4.8.x (which is the latest and 
         also the last release of the Qt4 framework)
       - the new Qt5 framework does NOT work !
   3.) MinGW (only for MS Windows; usually shipped with Qt4; see www.MinGW.org)
       PLEASE NOTE:
       - the Qt 4.8.5 installer currently available at qt-project.org does
         not include MinGW anymore. Qt 4.8 requires MinGW with g++ 4.4, which
         is unfortunately no longer available at MinGW.org.
       - MinGW with g++ 4.4 can be downloaded from:
            https://piece-of-c.googlecode.com/files/MinGW-gcc440_1.zip
       - newer MinGW versions will not work with the binary release of Qt 4.8.x

--------------------------------------------------------------------------------

4. COMPILATION:

Open a console window and switch to the FreeSSM-directory.

Preparation:
$ qmake

Compilation:
$ make release
or
$ make debug

Translation files:
$ make translation

NOTE (Windows only): depending on the used Qt4-version and system configuration,
                     'mingw32-make' must be called instead of 'make'.

--------------------------------------------------------------------------------

5. INSTALLATION:

$ make install

=> the application will be installed to 
	- Linux:	the users home-directory (/home/userXYZ/FreeSSM)
	- Windows:	C:\FreeSSM (can be moved after installation)

Un-installation:

$ make uninstall

--------------------------------------------------------------------------------

6. STARTING FreeSSM:

First, switch to the installation folder (see 5.).

Linux:
$ ./FreeSSM

Windows:
$ freessm

