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
Open folder in Visual Studio 2022 (Or newer). 
Let Visual Studio build the requirements (May take some time).
Hit `Ctrl+Shift+B` to build. (Or use the build button at the top)


## Updating
If Bethesda releases more new breaking skyrim versions, and [CommonlibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)is still being updated, making this version compatible only requires updating the `baseline` hash in `vcpkg-configuration.json`.
