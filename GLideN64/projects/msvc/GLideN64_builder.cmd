@echo off
setlocal enableextensions disabledelayedexpansion
cls
set DPROJ=GLideN64.sln
set DMN=REM
set ERR=if errorlevel 1 goto err
set MSG=%DPROJ% does not exist in the same directory
if not exist "%~dp0%DPROJ%" goto err

set ARCH=
set BOTH=0
set CONF=Release
set KAP=7z
set NO7=0
set DQTD=1
set REB=
set NOQT=0
set NOWTL=0
set NOM64=0
set ESIM=0
set EBQ=start "" explorer .
taskkill /im msbuild.exe /f 2>nul
taskkill /im vctip.exe /f 2>nul
taskkill /im mspdbsrv.exe /f 2>nul
if "%*"=="" goto help
for %%P in (%*) do (
	if /i "%%P"=="--clean" goto clean
	if /i "%%P"=="--x86" set ARCH=x86
	if /i "%%P"=="--x64" set ARCH=x64& set BOTH=0
	if /i "%%P"=="--all" set ARCH=x64& set BOTH=1
	if /i "%%P"=="--debug" set CONF=Debug
	if /i "%%P"=="--zip" set KAP=zip
	if /i "%%P"=="--nopak" set NO7=1
	if /i "%%P"=="--rebuild" set REB=/t:Rebuild
	if /i "%%P"=="--noqt" set NOQT=1
	if /i "%%P"=="--nowtl" set NOWTL=1
	if /i "%%P"=="--nom64" set NOM64=1
	if /i "%%P"=="--dlqt" set DQTD=0
	if /i "%%P"=="--sim" set ESIM=1
	if /i "%%P"=="--q" set EBQ=REM
)

if not defined ARCH goto help
set /a MOD=%NOQT%+%NOWTL%+%NOM64%
set MSG=All compilation tasks were disabled on request
if %MOD%==3 goto err
set MSG=7z was not found in the environment
set /a MOD=%NO7%+%DQTD%
if %MOD% NEQ 2 7z >nul 2>&1
%ERR%

set tVSPF=%ProgramFiles(x86)%
if "%PROCESSOR_ARCHITECTURE%"=="x86" set tVSPF=%ProgramFiles%
set "tVSPF=%tVSPF%\Microsoft Visual Studio"
set tVSDS=Common7\Tools\VsDevCmd.bat
set MOD=\Community\ \Enterprise\ \Professional\
goto vsbeg
:vsenv
	for %%V in (%MOD%) do (
		if exist "%tVSPF%%~1%%V%tVSDS%" set "tVSDS=%tVSPF%%~1%%V%tVSDS%"
	)
	goto:eof

:vsbeg
call :vsenv "\2019"
call :vsenv "\2017"
set MOD=\
call :vsenv " 14.0"
call :vsenv " 12.0"

set MSG=Visual Studio developer environment was not loaded
msbuild -version >nul 2>&1
if errorlevel 1 call "%tVSDS%"
%ERR%
set MSG=Git was not found in the environment
git --version >nul 2>&1
%ERR%

set "TARCH=%ARCH%"
:X86
pushd "%~dp0..\..\"
if "%ARCH%"=="x86" set TARCH=Win32

if %NOQT%==1 goto noqt
if defined QTDIR_%ARCH% goto nodl
set "QTVER=qt-5_7_1-%ARCH%-msvc2017-static"
if exist "..\Qt\%QTVER%\include\QtCore" goto nodl

set MSG=Path to Qt %ARCH% was not specified or detected
if %DQTD%==1 goto err
set MSG=cURL was not found in the environment
curl --version >nul 2>&1
%ERR%
del /f /q "..\%QTVER%.7z" 2>nul
set errorlevel=0
set QTURL=https://github.com/gonetz/GLideN64/releases/download/qt_build
set MSG=cURL failed to download:^& echo %QTURL%/%QTVER%.7z
curl -L -o "..\%QTVER%.7z" "%QTURL%/%QTVER%.7z"
%ERR%
set MSG=7z failed to extract:^& echo %QTVER%.7z
7z x -y "..\%QTVER%.7z" -o"..\Qt"
%ERR%

:nodl
set "QTDIR=%~dp0..\..\..\Qt\%QTVER%"
if "%ARCH%"=="x64" (
	if defined QTDIR_x64 set "QTDIR="%QTDIR_x64%""
) else (
	if defined QTDIR_x86 set "QTDIR="%QTDIR_x86%""
)
set QTDIR=%QTDIR:"=%
set MSG=Something went wrong when detecting Qt:^& echo %QTDIR%
if not exist "%QTDIR%\include\QtCore" goto err
:noqt

if not defined BUILDROUTE set "BUILDROUTE="%~dp0..\..\build""
set BUILDROUTE=%BUILDROUTE:"=%
set "PJ64QT=%BUILDROUTE%\QT_for_Project64_%ARCH%"
set "PJ64WTL=%BUILDROUTE%\WTL_for_Project64_%ARCH%"
set "M64CL=%BUILDROUTE%\for_Mupen64Plus_%ARCH%"
set JACK=
if "%ARCH%"=="x64" set JACK=_x64
set "PJ64PluginsDirQT%JACK%=%PJ64QT%"
set "PJ64PluginsDirWTL%JACK%=%PJ64WTL%"
set "Mupen64PluginsDir%JACK%=%M64CL%"

set MOD=%random:~0,1%
if %MOD% LEQ 4 (set MOD=Lylat) else (
if %MOD% GEQ 8 (set MOD=Kildean) else set MOD=Caryll)

set MBQT=msbuild
if %ESIM%==1 (
	set "MBQT=echo %MBQT%"
	md "translations\wtl" 2>nul
	cd.>"translations\wtl\%MOD%.Lang"
	del /f /q "%BUILDROUTE%\*_%ARCH%.%KAP%" 2>nul
	set errorlevel=0
)
set "MBWTL=%MBQT%"
set "MBM64=%MBQT%"
if %NOQT%==1 set MBQT=REM
if %NOWTL%==1 set MBWTL=REM
if %NOM64%==1 set MBM64=REM

set "PTS=Platform=%TARCH%"
if defined TOOLSET set "PTS="%PTS%;PlatformToolset=%TOOLSET%""
set PTS=%PTS:"=%

goto mbbeg
:mbcl
	%~1 "%~dp0%~2" /m /p:Configuration=%CONF%%~3;%PTS% %REB%
	%ERR%
	echo.
	goto:eof

:mbbeg
set MSG=Qt version, architecture and path are really correct?^& echo %QTDIR%
call :mbcl "%MBQT%" "GLideNUI.vcxproj" ""
%DMN%
call :mbcl "%MBQT%" "%DPROJ%" "_qt"
%DMN%

set MSG=ERROR!
call :mbcl "%MBWTL%" "GLideNUI-wtl.vcxproj" ""
%DMN%
call :mbcl "%MBWTL%" "%DPROJ%" "_wtl"
%DMN%

call :mbcl "%MBM64%" "%DPROJ%" "_mupenplus"
%DMN%

goto pjqt
:cini
	set MSG=Failed to copy some project files to:^& echo %~1
	if %ESIM%==1 md "%~1" 2>nul
	set errorlevel=0
	copy /y ini\GLideN64.custom.ini "%~1\"
	%ERR%
	del /f /q "%~1\*.lib" "%~1\*.exp" 2>nul
	set errorlevel=0
	goto:eof

:pjqt
if %NOQT%==1 goto pjwtl
call :cini "%PJ64QT%"
%DMN%
copy /y translations\release\*.qm "%PJ64QT%\"
%ERR%

:pjwtl
if %NOWTL%==1 goto mpcl
call :cini "%PJ64WTL%"
%DMN%
md "%PJ64WTL%\translations" 2>nul
set errorlevel=0
copy /y translations\wtl\*.Lang "%PJ64WTL%\translations\"
%ERR%

:mpcl
if %NOM64%==1 goto pkg
call :cini "%M64CL%"
%DMN%

:pkg
for /f "tokens=1" %%R in ('git rev-parse --short HEAD') do set "REV=%%R"
set MSG=The route could not be accessed:^& echo %BUILDROUTE%
pushd "%BUILDROUTE%"
%ERR%
set MOD=the plugins
if %NO7%==0 (
	set MOD=the compressed files
	for /f "tokens=*" %%Z in ('dir /ad /b *_%ARCH%') do 7z a -t%KAP% "GLideN64-%REV%-%%Z.%KAP%" ".\%%Z\*"
)

if "%ARCH%"=="x64" (
	if %BOTH%==1 set ARCH=x86& goto X86
)

set MSG=DONE!^& echo Path to %MOD%:^& echo %CD%
%EBQ%
:err
set WTF=%errorlevel%
set DMN=exit /b %WTF%
echo.
echo %MSG%
exit /b %WTF%

:clean
pushd "%~dp0"
for /f "tokens=*" %%E in ('dir /ad /b') do (
	if /i "%%E" NEQ "lib" rd /s /q "%%E"
)
cd ..\..
del /f /q "src\Revision.h" 2>nul
rd /s /q "build" 2>nul
rd /s /q "translations\wtl" 2>nul
exit /b 0

:help
echo.
echo GLideN64's simplified build and packaging script
echo.
echo Usage:
echo   set ^<variable^>
echo   %~nx0 ^<architecture^> ^<other^>
echo.
echo ^<Variables^>
echo   set BUILDROUTE=^<Custom build folder^>
echo     e.g.         "Z:\My share folder"
echo   set QTDIR_x86=^<Your Qt x86 path^>
echo     e.g.        "D:\Static Qt\qt-5.7.1-x86-msvc2013"
echo   set QTDIR_x64=^<Your Qt x64 path^>
echo     e.g.        F:\Qt\qt-5_7_1-x64-msvc2015-static
echo   set TOOLSET=^<Custom PlatformToolset^>
echo     e.g.      v141_xp
echo.
echo ^<Architectures^>
echo   --x86       To compile x86
echo   --x64       To compile x64
echo   --all       To compile x86 and x64
echo     n.b.      If more than one of these is used, only the last one will
echo               take effect
echo.
echo ^<Others^>
echo   --clean     Clean ALL auto-generated build files within the project
echo   --debug     For debug builds
echo   --dlqt      Auto download and configure Qt for VS2017-2019, it
echo               would have no effect if QTDIR_x* is used or if the
echo               same version has already been extracted
echo   --noqt      To build without Qt support, it will ignore the
echo               effects of QTDIR_x* and "--dlqt"
echo   --nowtl     To skip WTL builds
echo   --nom64     To skip mupen64plus builds
echo   --nopak     To skip packing the plugins, "--zip" will be ineffective
echo               It will disable 7-Zip completely if "--dlqt" isn't used
echo   --rebuild   To rebuild without cleaning
echo   --sim       Simulated build, quick environment check without compiling
echo               It's destructive to the final product by creating
echo               dummy files, make sure to use "--clean" afterwards
echo   --q         Don't interact with Windows Explorer at the end
echo   --zip       Pack to ZIP files instead of 7Z files
echo.
echo Usage examples:
echo.
echo   %~nx0 --all --dlqt
echo.
echo   %~nx0 --x86 --debug --rebuild
echo.
echo   set BUILDROUTE=H:\%USERNAME%\experiment
echo   set TOOLSET=ClangCL
echo   %~nx0 --noqt --x64
echo.
echo   set QTDIR_x86="G:\Static Qt\qt-5.7.1-x86-msvc2015"
echo   set QTDIR_x64=G:\Static Qt\qt-5.7.1-x64-msvc2015
echo   %~nx0 --nowtl --nom64 --all
exit /b 0
