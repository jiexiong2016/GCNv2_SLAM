rem See if cmake can be found. If this fails, first try a fallback (default position)
rem If the fallback fails, report an error and die.

cmake --version >NUL 2>NUL

if errorlevel 1 (
    echo Cannot find cmake.exe on the command line; calling default vsvarsall.bat to install
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
    cmake --version >NUL 2>NUL
)

if errorlevel 1 (
    echo.
    echo Cannot find cmake.exe; check that the cmake command line extensions are installed and
    echo run vcvarsall.bat to register the build system
    echo.
    echo An example command line is:
    echo.
    echo "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
    echo.
    exit /b 1
)

rem Check VCPKG directory set up
if "%VCPKG_ROOT%"=="" (
    echo.
    echo Please set VCPKG_ROOT to the vcpkg installation directory.
    echo.
    exit /b 1
)

rem Check the right triplet is set up
if not "%VCPKG_DEFAULT_TRIPLET%"=="x64-windows" (
    echo .
    echo Please set %VCPKG_DEFAULT_TRIPLET to x64-windows.
    echo .
    exit /b 1
)

exit /b 0