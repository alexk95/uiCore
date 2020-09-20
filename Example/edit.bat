@ECHO OFF

REM This script requires the following environment variables to be set:
REM 1. UI_CORE_ROOT
REM 2. DEVENV_ROOT
REM 3. QTDIR

IF "%UI_CORE_ROOT%" == "" (
	ECHO Please specify the following environment variables: UI_CORE_ROOT
	goto END
)

IF "%DEVENV_ROOT%" == "" (
	ECHO Please specify the following environment variables: DEVENV_ROOT
	goto END
)

IF "%QTDIR%" == "" (
	ECHO Please specify the following environment variables: QTDIR
	goto END
)

REM Setup eviroment
CALL "%UI_CORE_ROOT%\setupEnvironment.bat"

ECHO Launching development enviroment

REM Open project
START "" "%DEVENV_ROOT%\devenv.exe" "%UI_CORE_ROOT%\Example\Example.vcxproj"

:END

