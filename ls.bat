@echo off
setlocal EnableDelayedExpansion

set maxItems=40

if "%1"=="" goto skipInput

set /a test=%1 2>nul
if %errorlevel% neq 0 (
    echo Error: The input is not a valid number.
    exit /b
) else (
    set maxItems=%1
)

:skipInput

set total=0
for /f "delims=" %%f in ('dir /b /a:-h') do (

    set /a total+=len

    if !total! GEQ !maxItems! (
        echo:
        set total=0
    )

    if exist "%%f\" (
        call :printFolder %%f
    ) else (
        set "filename=%%~nf"  & rem Extracts the name without extension
        set "extension=%%~xf" & rem Extracts the extension
        call :printFile !filename! !extension!
    )
    set "str=%%f"
    call :$len str len
)

set total=0
for /f "delims=" %%f in ('dir /b /a:h') do (

    set /a total+=len

    if !total! GEQ !maxItems! (
        echo:
        set total=0
    )

    if exist "%%f\" (
        call :printHiddenFolder %%f
    ) else (
        set "filename=%%~nf"  & rem Extracts the name without extension
        set "extension=%%~xf" & rem Extracts the extension
        call :printHiddenFile !filename! !extension!
    )
    set "str=%%f"
    call :$len str len
)

echo.
endlocal
goto :eof

:printFile
<nul set /p="[92m%1[0m%2  "
goto :eof

:printFolder
<nul set /p="[93m%1[0m  "
goto :eof


:printHiddenFile
<nul set /p="[35m%1[0m%2  "
goto :eof

:printHiddenFolder
<nul set /p="[33m%1[0m  "
goto :eof

:$len
setlocal EnableDelayedExpansion
set s=#!%~1!
set c=0
for /l %%N in (0, 1, 1000) do (
    if "!s:~%%N,1!"=="" goto lenDone
    set /a c+=1
)
:lenDone
endlocal & set %~2=%c%
exit /b

