# ScienceMode4_c_library
C library for the ScienceMode 4 protocol

### Build library on Windows
Please download the QT-Installer from https://www.qt.io/download-open-source and install Qt Creator and Qt5 / Qt6.
Besides QT, Qt Serial Port is necessary.

Open ScienceMode.pro with Qt Creator and go to Projects. Select your Compiler and QT-Version and then go to Builds Steps and
add the following to the addional arguments field:

For release:
![image](https://github.com/ScienceMode/ScienceMode4_c_library/assets/85743064/4bd06445-58c9-42a7-9d01-e65c2217f18c)

For debug:
![image](https://github.com/ScienceMode/ScienceMode4_c_library/assets/85743064/9ae1f8d9-2a26-47f0-a2f0-4f78fba662c6)

Possible settings for windows (Release):
* For a MingGW-based compiler: `"CONFIG+=myRelease smpt_all mingw"`
* For a x86 visual studio compiler: `"CONFIG+=myRelease smpt_all windows_static_x86"`
* For a amd64 visual studio compiler: `"CONFIG+=myRelease smpt_all windows_static_amd64"`

Possible settings for windows (Debug):
* For a MingGW-based compiler: `"CONFIG+=myDebug smpt_all mingw"`
* For a x86 visual studio compiler: `"CONFIG+=myDebug smpt_all windows_static_x86"`
* For a amd64 visual studio compiler: `"CONFIG+=myDebug smpt_all windows_static_amd64"`

When selecting mingw, it is possible to build the library for 32bit or for 64bit, depending on the installed MinGW.
windows_static_x86 or windows_static_amd64 requires an installed visual studio c++ compiler and a fitting Qt version.

### Build library on Linux
Qt5 Core and qmake needs to be installed first. Then the library can be build by:
```
sudo apt install libqt5core5a qt5-qmake
qmake -r "CONFIG+=myRelease smpt_all linux_x86_amd64_static" ScienceMode.pro
make
```

### Build library on MacOS
Qt5 Core and qmake needs to be installed first. 

#### Install Qt5
In a terminal, assuming [homebrew](https://brew.sh) is installed, run:
```
$ brew install qt@5
$ brew install qt-creator
```
#### Link QT-Creator to Qt
After opening QT-Creator, you can link the Qt installation by navigating to:
```
/opt/homebrew/Cask/qt@5/ 
```

#### ScienceMode_Library
```
$ qmake -r "CONFIG+=myRelease smpt_all macos" ScienceMode.pro
make
```

