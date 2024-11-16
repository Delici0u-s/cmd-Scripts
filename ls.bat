
@echo off
setlocal EnableDelayedExpansion

set maxItems=29

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

for /f "delims=" %%f in ('dir /b /a:-d-h') do (
    set "str=%%f"
    call :$len str len
    if !total! GEQ !maxItems! (
        echo:
        set total=0
    )
    set "filename=%%~nf"
    set "extension=%%~xf"
    <nul set /p="[92m!filename![0m!extension! "
    set /a total+=len
    if !total! LSS !maxItems! (
        <nul set /p="| "
    )
)

for /f "delims=" %%f in ('dir /b /a:d-h') do (
    set "str=%%f"
    call :$len str len
    if !total! GEQ !maxItems! (
        echo:
        set total=0
    )

    <nul set /p="[93m%%f[0m "

    set /a total+=len
    if !total! LSS !maxItems! (
        <nul set /p="| "
    )
)
for /f "delims=" %%f in ('dir /b /a:-dh') do (
    set "str=%%f"
    call :$len str len
    if !total! GEQ !maxItems! (
        echo:
        set total=0
    )
    set "filename=%%~nf"
    set "extension=%%~xf"
    <nul set /p="[35m!filename![0m!extension! "
    set /a total+=len
    if !total! LSS !maxItems! (
        <nul set /p="| "
    )
)

for /f "delims=" %%f in ('dir /b /a:dh') do (
    set "str=%%f"
    call :$len str len
    if !total! GEQ !maxItems! (
        echo:
        set total=0
    )

    <nul set /p="[33m%%f[0m "

    set /a total+=len
    if !total! LSS !maxItems! (
        <nul set /p="| "
    )
)




endlocal
goto :eof

:$len
setlocal EnableDelayedExpansion
set "s=#!%~1!"
set c=0
for /l %%N in (0, 1, 1000) do (
    if "!s:~%%N,1!"=="" goto lenDone
    if "!s:~%%N,1!"=="" (
        set /a %%N+=3
    ) else (
        set /a c+=1
    )
)
:lenDone
endlocal & set %~2=%c%
exit /b
