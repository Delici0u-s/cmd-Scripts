@echo off
REM Check if at least one argument is provided
if "%~1"=="" (
    echo Usage: compile.bat <source_file> [additional_args]
    exit /b 1
)

REM Extract the base name of the first argument (without extension)
for %%f in ("%~1") do set OutputName=%%~nf

REM Run g++ with all provided arguments and specify the output name
g++ %* -std=c++20 -o "%OutputName%"

REM Check if the compilation was successful
if "%ERRORLEVEL%"=="0" (
    echo Compilation successful. Output: %OutputName%
) else (
    echo Compilation failed.
)

