@echo off
REM Check if at least one argument is provided
if "%~1"=="" (
    echo Usage: compile.bat <source_file> [additional_args]
    exit /b 1
)

REM Initialize variables
set DebugFlag=false
set EnableWarnings=false
set OutputFile=
set SourceFile=
set FileArgs=

REM Parse arguments to find -g, -w, and -o flags, and determine the source file
setlocal enabledelayedexpansion
set NextArg=
for %%a in (%*) do (
    if "%%a"=="-g" (
        set DebugFlag=true
    ) else if "%%a"=="-w" (
        set EnableWarnings=true
    ) else if "%%a"=="-o" (
        set NextArg=o
    ) else if "!NextArg!"=="o" (
        set OutputFile=%%a
        set NextArg=
    ) else if not defined SourceFile (
        set SourceFile=%%a
        set FileArgs=!FileArgs! "%%a"
    ) else (
        set FileArgs=!FileArgs! "%%a"
    )
)

REM If no source file is found, display an error and exit
if not defined SourceFile (
    echo No source file provided. Usage: compile.bat <source_file> [additional_args]
    exit /b 1
)

REM If no output file was specified, use the base name of the source file
if not defined OutputFile (
    for %%f in ("%SourceFile%") do set OutputFile=%%~nf.exe
)

REM Check the extension of the source file to determine if it's C or C++
for %%f in ("%SourceFile%") do set FileExtension=%%~xf

REM C file compilation (using gcc)
if /i "%FileExtension%"==".c" (
    if "%DebugFlag%"=="true" (
        if "%EnableWarnings%"=="true" (
            gcc %FileArgs% -o "%OutputFile%" -g
        ) else (
            gcc %FileArgs% -o "%OutputFile%" -g -Wall -Wextra
        )
    ) else (
        if "%EnableWarnings%"=="true" (
            gcc %FileArgs% -o "%OutputFile%" -s
        ) else (
            gcc %FileArgs% -o "%OutputFile%" -s -Wall -Wextra
        )
    )
) else if /i "%FileExtension%"==".cpp" (
    REM C++ file compilation (using g++)
    if "%DebugFlag%"=="true" (
        if "%EnableWarnings%"=="true" (
            g++ %FileArgs% -std=c++23 -o "%OutputFile%" -g
        ) else (
            g++ %FileArgs% -std=c++23 -o "%OutputFile%" -g -Wall -Wextra
        )
    ) else (
        if "%EnableWarnings%"=="true" (
            g++ %FileArgs% -std=c++23 -o "%OutputFile%" -s
        ) else (
            g++ %FileArgs% -std=c++23 -o "%OutputFile%" -s -Wall -Wextra
        )
    )
) else (
    echo Unsupported file type: %FileExtension%. Only .c and .cpp files are supported.
    exit /b 1
)

REM Check if the compilation was successful
if "%ERRORLEVEL%"=="0" (
    echo Compilation successful. Output: %OutputFile%
) else (
    echo Compilation failed.
)