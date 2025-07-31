@echo off
REM Set CMake generator to MinGW Makefiles and build the project

set BUILD_DIR=build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_C_FLAGS="--coverage" ..
cmake --build .
ctest --output-on-failure

REM Run gcov from the build directory
gcov CMakeFiles\md2rtf-cpp.dir\src\block_detector.cc.gcno

cd ..