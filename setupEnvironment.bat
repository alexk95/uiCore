@ECHO OFF
REM This script requires the following environment variables to be set:
REM 1. UI_CORE_ROOT
REM 2. DEVENV_ROOT
REM 3. QTDIR

REM Ensure that the setup will only be performed once
if "%UI_CORE_LIB_ENV_DEFINED%"=="1" (
	goto END
)

SET UI_CORE_LIB_ENV_DEFINED=1

ECHO Setup uiCore lib environment

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

REM Set Tab Toolbar Root Directory
SET QT_TT_ROOT=%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar

REM Add the deployment directory to the path
SET PATH=%UI_CORE_ROOT%\Deployment;%PATH%

ECHO Environment was set up successfully.

:END

