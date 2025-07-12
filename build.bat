@echo off
REM Set CMake generator to MinGW Makefiles and build the project

set BUILD_DIR=build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

cmake -G "MinGW Makefiles" ..
mingw32-make

cd ..