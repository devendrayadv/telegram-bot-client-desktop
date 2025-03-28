@echo off
echo Building Kotatogram Desktop with bot client features...

:: Set paths
set QT_PATH=C:\Qt\6.8.3\mingw_64
set PATH=%QT_PATH%\bin;%PATH%

:: Create build directory
if not exist build mkdir build
cd build

:: Configure with CMake
cmake -G "MinGW Makefiles" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_PREFIX_PATH="%QT_PATH%" ^
    -DTDESKTOP_API_ID=1237900 ^
    -DTDESKTOP_API_HASH=df86a84b0af32d9c97a2a3e85179a46d ^
    -DDESKTOP_APP_USE_PACKAGED=OFF ^
    ..

:: Build
mingw32-make -j%NUMBER_OF_PROCESSORS%

echo Build completed!
pause 