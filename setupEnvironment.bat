@ECHO OFF
REM This script requires the following environment variables to be set:
REM 1. UI_CORE_ROOT
REM 2. DEVENV_ROOT
REM 3. QTDIR_ROOT

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

IF "%QTDIR_ROOT%" == "" (
	ECHO Please specify the following environment variables: QTDIR_ROOT
	goto END
)

SET QTDIR=%QTDIR_ROOT%
SET QDIR=%QTDIR%

REM Set Tab Toolbar Root Directory
SET QT_TT_ROOT=%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar

REM Set Rapid JSON Directory
SET R_JSON_ROOT=%UI_CORE_ROOT%\Third_Party_Libraries\rapidjson

REM Set CURL Directory
SET CURL_ROOT=%UI_CORE_ROOT%\Third_Party_Libraries\curl
SET CURL_LIB=%UI_CORE_ROOT%\Third_Party_Libraries\curl\lib
SET CURL_INC=%UI_CORE_ROOT%\Third_Party_Libraries\curl\include
SET CURL_DLL=%UI_CORE_ROOT%\Third_Party_Libraries\curl\bin
SET CURL_LIBD=%UI_CORE_ROOT%\Third_Party_Libraries\curl\debug\lib
SET CURL_DLLD=%UI_CORE_ROOT%\Third_Party_Libraries\curl\debug\bin

SET ZLIB_ROOT=%SIM_PLAT_ROOT%\Third_Party_Libraries\zlib\zlib-1.2.11
SET ZLIB_LIB=%SIM_PLAT_ROOT%\Third_Party_Libraries\zlib\zlib-1.2.11\contrib\vstudio\vc14\x64\ZlibDllRelease

REM ##########################################################################################################

REM Set UI_CORE Project settings
SET UICORE_PROJ_ENV_INC=%QDIR%\include\QtCore\; %QDIR%\include\QtGui\; %QDIR%\include\QtWidgets\; %QT_TT_ROOT%\include\; %R_JSON_ROOT%\include\; %CURL_INC%
SET UICORE_ENV_INC=%UI_CORE_ROOT%\include; $(UICORE_PROJ_ENV_INC)

SET UICORE_PROJ_ENV_LIB_ROOT=%QT_TT_ROOT%\src\TabToolbar\Release; %QDIR%\lib; %CURL_LIB%; %ZLIB_LIB%
SET UICORE_PROJ_ENV_LIBD_ROOT=%QT_TT_ROOT%\src\TabToolbar\Debug; %QDIR%\lib; %CURL_LIB%; %ZLIB_LIB%
SET UICORE_ENV_LIB_ROOT=%UI_CORE_ROOT%\x64\Release;$(UICORE_PROJ_ENV_LIB_ROOT)
SET UICORE_ENV_LIBD_ROOT=%UI_CORE_ROOT%\x64\Debug;$(UICORE_PROJ_ENV_LIBD_ROOT)

SET UICORE_PROJ_ENV_LIB_INP=%QT_TT_ROOT%\src\TabToolbar\Release\TabToolbar.lib; %CURL_LIB%\libCurl.lib; zlibwapi.lib
SET UICORE_PROJ_ENV_LIBD_INP=%QT_TT_ROOT%\src\TabToolbar\Debug\TabToolbard.lib; %CURL_LIB%\libCurl.lib; zlibwapi.lib
SET UICORE_ENV_LIB_INP=%UI_CORE_ROOT%\x64\Release\uiCore.lib; $(UICORE_PROJ_ENV_LIB_INP)
SET UICORE_ENV_LIBD_INP=%UI_CORE_ROOT%\x64\Debug\uiCore.lib; $(UICORE_PROJ_ENV_LIBD_INP)

SET UICORE_ENV_DLL=%UI_CORE_ROOT%\x64\Release;%QT_TT_ROOT%\DLL;%CURL_DLL%; %ZLIB_LIB%
SET UICORE_ENV_DLLD=%UI_CORE_ROOT%\x64\Debug;%QT_TT_ROOT%\src\TabToolbar\Debug\;%CURL_DLL%;%ZLIB_LIB%

ECHO Environment was set up successfully.

:END

