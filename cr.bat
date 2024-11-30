@echo off
:: Check if a filename was provided as an argument
if "%~1"=="" (
    echo Usage: %0 filename
    exit /b
)

:: Check if the file already exists
if exist "%~1" (
    echo Error: The file "%~1" already exists.
    exit /b
)

:: Create an empty file with the specified name in the current directory
copy nul "%~1" > nul

:: Notify the user
echo File "%~1" created in %cd%
