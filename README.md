# ScienceMode4_c_library
C library for the ScienceMode 4 protocol

## Compiling with cmake

### Basic Compilation

To build the library using CMake, follow these steps:

```bash
mkdir build
cd build
cmake ..
make
```

This will build a static library named `libsmpt.a` by default.

### Build Options

The following CMake options are available:

- `BUILD_SHARED_LIBS` - Build shared library (.so/.dll) instead of static (.a/.lib)
- `SMPT_LOW_LEVEL` - Enable Low-Level protocol module (ON by default)
- `SMPT_MID_LEVEL` - Enable Mid-Level protocol module (ON by default)
- `SMPT_DYSCOM_LEVEL` - Enable Dyscom-Level protocol module (ON by default)

Example: Building a shared library with only the low-level module:

```bash
cmake .. -DBUILD_SHARED_LIBS=ON -DSMPT_MID_LEVEL=OFF -DSMPT_DYSCOM_LEVEL=OFF
make
```

### Running Tests

To build and run the tests:

```bash
cmake ..
make
./sciencemode_tests
```

or use CTest:

```bash
cmake ..
make
ctest
```

### Installing the Library

To install the library system-wide:

```bash
cmake ..
make
sudo make install
```

This will install the library, headers, and CMake config files.

### Using in Other CMake Projects

After installation, the library can be used in other CMake projects:

```cmake
find_package(ScienceMode REQUIRED)
target_link_libraries(your_project PRIVATE ScienceMode::smpt)
```

### Cross-Platform Notes

- **Linux**: Make sure you have development packages installed (`build-essential`, `cmake`)
- **Windows**: Both MSVC and MinGW compilers are supported
- **macOS**: Standard build process works with both Intel and ARM-based Macs


## Compiling with Qt

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

### Build library on MacOs
Qt5 Core and qmake needs to be installed first. Then the library can be build by:
```
qmake -r "CONFIG+=myRelease smpt_all macos" ScienceMode.pro
make
```
