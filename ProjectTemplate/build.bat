@ECHO OFF

REM This script requires the following environment variables to be set:
REM 1. UI_CORE_ROOT
REM 2. DEVENV_ROOT
REM 3. QTDIR_ROOT
REM 4. Your own project root!		!! You need to specify where the prject is located at

IF "%UI_CORE_ROOT%" == "" (
	ECHO Please specify the following environment variables: UI_CORE_ROOT
	goto END
)

IF "%DEVENV_ROOT%" == "" (
	ECHO Please specify the following environment variables: DEVENV_ROOT
	goto END
)

IF "%QTDIR_ROOT%" == "" (
	ECHO Please specify the following environment variables: QTDIR_ROOT
	goto END
)

REM IF "%<YOUR PROJECT ENVIRONMENT VARIABLE>%" == "" (
REM 	ECHO Please specify the following environment variables: <YOUR PROJECT ENVIRONMENT VARIABLE>
REM 	goto END
REM )

rem Setup UI eviroment
CALL "%UI_CORE_ROOT%\setupEnvironment.bat"

ECHO Building Project

REM Open project

SET RELEASE=1
SET DEBUG=1

IF "%1"=="RELEASE" (
  SET RELEASE=1
  SET DEBUG=0
)

IF "%1"=="DEBUG" (
  SET RELEASE=0
  SET DEBUG=1
)

SET TYPE=/Rebuild
SET TYPE_NAME=REBUILD

IF "%2"=="BUILD" (
	SET TYPE=/Build
	SET TYPE_NAME=BUILD
)

IF %DEBUG%==1 (
	ECHO %TYPE_NAME% DEBUG
REM						!!! YOU NEED TO SPECIFY YOUR PROJECT DIR !!!
	"%DEVENV_ROOT%\devenv.exe" "%UI_CORE_ROOT%\ProjectTemplate\ProjectName.vcxproj" %TYPE% "Debug|x64" /Out buildLog_Debug.txt
REM	"%DEVENV_ROOT%\devenv.exe" "%<YOUR PROJECT ENVIRONMENT VARIABLE>%\uiCore.vcxproj" %TYPE% "Debug|x64" /Out buildLog_Debug.txt
)

IF %RELEASE%==1 (
	ECHO %TYPE_NAME% RELEASE
REM						!!! YOU NEED TO SPECIFY YOUR PROJECT DIR !!!
	"%DEVENV_ROOT%\devenv.exe" "%UI_CORE_ROOT%\ProjectTemplate\ProjectName.vcxproj" %TYPE% "Release|x64" /Out buildLog_Release.txt
REM	"%DEVENV_ROOT%\devenv.exe" "%<YOUR PROJECT ENVIRONMENT VARIABLE>%\uiCore.vcxproj" %TYPE% "Release|x64" /Out buildLog_Release.txt
) 
  
:END

