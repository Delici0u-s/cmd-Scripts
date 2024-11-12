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
for /f "delims=" %%f in ('dir /b /a') do (

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

    Set str=%%f
    Call :$len str len
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

:$len
setlocal EnableDelayedExpansion
set s=#!%~1!
set c=0
set nums=32 16 8 4 2 1
for %%N in (%nums%) do if not "!s:~%%N,1!"=="" set /a c+=%%N& set s=!s:~%%N!
(endlocal & set %~2=%c%)& exit /b
