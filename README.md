# ScienceMode4_c_library
C library for the ScienceMode 4 protocol

### Build library on Windows
Please download the QT-Installer from https://www.qt.io/download-open-source and install Qt Creator and Qt5.
Open ScienceMode.pro with Qt Creator and go to Projects. Select your Compiler and QT-Version and then go to Builds Steps and
add the following to Addional arguments:
* For a MingGW-based compiler: `"CONFIG+=myRelease smpt_all mingw"`
* For a x86 visual studio compiler: `"CONFIG+=myRelease smpt_all windows_static_x86"`
* For a amd64 visual studio compiler: `"CONFIG+=myRelease smpt_all windows_static_amd64"`

For Debug mode, replace myRelease with myDebug.

### Build library on Linux
Qt5 Core and qmake needs to be installed first. Then the library can be build by:
```
sudo apt install libqt5core5a qt5-qmake
qmake -r "CONFIG+=myRelease smpt_all linux_x86_amd64_static" ScienceMode.pro
make
```

### Build library on MacOs
Qt5 Core and qmake needs to be installed first. Then the library can be build by:
```
qmake -r "CONFIG+=myRelease smpt_all macos" ScienceMode.pro
make
```
