@echo off
REM Run all *test.exe files in build directory for md2rtf-cpp project

set BUILD_DIR=build
if not exist %BUILD_DIR% (
    echo Build directory does not exist. Please run build.bat first.
    exit /b 1
)
cd /d "%BUILD_DIR%"

REM Loop through all *test.exe files and run them
for %%F in (*test.exe) do (
    echo Running %%F...
    "%%F"
    if errorlevel 1 (
        echo Test failed: %%F
    )
)

REM Return to original directory
cd /d "%~dp0"
