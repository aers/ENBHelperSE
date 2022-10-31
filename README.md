## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH` or install it as part of Visual Studio.
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
	* (If you're on windows, you have this installed already)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Clone vcpkg, run the script, then run `vcpkg integrate install` to make visual studio use vcpkg.
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++

## Register Visual Studio as a Generator
Open folder in Visual Studio, and newest version will automatically find CMake.

## Building
```
git clone https://github.com/aers/ENBHelperSE
cd ENBHelperSE
cmake build .
```
