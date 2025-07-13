@echo off
REM Set CMake generator to MinGW Makefiles and build the project

set BUILD_DIR=build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

cmake -G "MinGW Makefiles" ..
cmake --build .

@REM if %ERRORLEVEL% EQU 0 (
@REM     echo Build succeeded. Running tests...
@REM     ctest --output-on-failure
@REM ) else (
@REM     echo Build failed. Tests will not run.
@REM )

cd ..