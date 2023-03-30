# whofetch installer for Windows

This directory contains the Inno Setup script that defines the whofetch installer for Windows OS.

Note: This installer is only valid when whofetch is built with cmake and Visual C++.

## Building installer

Inno Setup 6 (>=6.1.2) is required to compile main.iss into an installer. To install, perform the following steps

1. compile whofetch with the release build.
    ```
    > cmake -S . -B build
    > cmake --build build --config Release
    ```
2. Install Inno Setup 6 either using winget or [manually](https://jrsoftware.org/isinfo.php).
    ```
    > winget install JRSoftware.InnoSetup
    ```
3. Add the Inno Setup 6 installation folder to the `PATH` environment variable so that the iscc command can be used.
4. you can compile the installer with the following command.
    ```
    > iscc main.iss
    ```