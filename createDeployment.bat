@ECHO OFF

REM The first argument of the shell defines whether a release or debug build shall be performed. "BOTH" (default) , "RELEASE", "DEBUG" 
REM The second argument of hte shell defines whetehr a full rebuild or just a build is performed. "BUILD" (default), "REBUILD"

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

REM Clean up the Deployment directory

RMDIR /S /Q "%UI_CORE_ROOT%\Deployment"
MKDIR "%UI_CORE_ROOT%\Deployment"

REM ===========================================================================
REM Copy the library files
REM ===========================================================================

REM QtTabToolbar
COPY "%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar\DLL\TabToolbar.dll" "%UI_CORE_ROOT%\Deployment"

REM Qt
COPY "%QTDIR%\bin\Qt5Core.dll" "%UI_CORE_ROOT%\Deployment"
COPY "%QTDIR%\bin\Qt5Gui.dll" "%UI_CORE_ROOT%\Deployment"
COPY "%QTDIR%\bin\Qt5Widgets.dll" "%UI_CORE_ROOT%\Deployment"

REM COPY "%QTDIR%\bin\libEGL.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\libGLESv2.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\opengl32sw.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DAnimation.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DCore.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DExtras.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DInput.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DLogic.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DQuick.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DQuickAnimation.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DQuickExtras.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DQuickInput.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DQuickRender.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DQuickScene2D.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt53DRender.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Bluetooth.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Charts.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Concurrent.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5DataVisualization.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5DBus.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Gamepad.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Help.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Location.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Multimedia.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5MultimediaQuick.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5MultimediaWidgets.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Network.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5NetworkAuth.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Nfc.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5OpenGL.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Positioning.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5PositioningQuick.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5PrintSupport.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Purchasing.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Qml.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Quick.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5QuickControls2.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5QuickParticles.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5QuickTemplates2.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5QuickTest.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5QuickWidgets.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5RemoteObjects.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Script.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5ScriptTools.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Scxml.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Sensors.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5SerialBus.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5SerialPort.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Sql.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Svg.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Test.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5TextToSpeech.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5WebChannel.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5WebEngine.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5WebEngineCore.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5WebEngineWidgets.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5WebSockets.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5WebView.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5WinExtras.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5Xml.dll" "%UI_CORE_ROOT%\Deployment"
REM COPY "%QTDIR%\bin\Qt5XmlPatterns.dll" "%UI_CORE_ROOT%\Deployment"

MKDIR "%UI_CORE_ROOT%\Deployment\plugins"
XCOPY /S "%QTDIR%\plugins" "%UI_CORE_ROOT%\Deployment\plugins"

REM ===========================================================================
REM Copy the build files
REM ===========================================================================

COPY "%UI_CORE_ROOT%\x64\Release\uiCore.dll" "%UI_CORE_ROOT%\Deployment"
COPY "%UI_CORE_ROOT%\x64\Release\qt.conf" "%UI_CORE_ROOT%\Deployment"

ECHO [Paths] > "%UI_CORE_ROOT%\Deployment\qt.conf"
ECHO Plugins = .\\plugins >> "%UI_CORE_ROOT%\Deployment\qt.conf"

:END