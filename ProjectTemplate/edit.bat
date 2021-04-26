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

REM Setup UI eviroment
CALL "%UI_CORE_ROOT%\setupEnvironment.bat"

ECHO Launching development enviroment
REM						!!! YOU NEED TO SPECIFY YOUR PROJECT DIR !!!
START "" "%DEVENV_ROOT%\devenv.exe" "%UI_CORE_ROOT%\ProjectTemplate\ProjectName.vcxproj"
REM START "" "%DEVENV_ROOT%\devenv.exe" "%<YOUR PROJECT ENVIRONMENT VARIABLE>%\ProjectName.vcxproj"

:END

