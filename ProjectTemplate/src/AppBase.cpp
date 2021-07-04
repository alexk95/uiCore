// Application header
#include "AppBase.h"			// Corresponding header

// AK header
#include <akAPI/uiAPI.h>		// The uiAPI
#include <akGui/aColorStyle.h>	// ColorStyle if needed for custom widgets

using namespace ak;

// We create some constant values for the icons
const QString c_dialogErrorIcon = "DialogError";
const QString c_dialogIconPath = "Dialog";

const std::string c_appVersion = "1.0";
const QString c_settingsWindowState = "WindowState";
const QString c_settingsColorStyle = "ColorStyle";

AppBase::AppBase(int _argc, char ** _argv)
{
	// Create own UID
	m_uid = ak::uiAPI::createUid();

	// Setup icon manager
	// We need to place the addIconSearchPath in a try block since a exception will be thrown if there is no valid search path

	int pathFoundCounter{ 0 };

	// Installation directory
	try { ak::uiAPI::addIconSearchPath(".\\Icons\\"); ++pathFoundCounter; }
	catch (...) {}
	// Repository icon folder
	try { ak::uiAPI::addIconSearchPath(QString(qgetenv("UI_CORE_ROOT") + "\\Icons\\")); ++pathFoundCounter; }
	catch (...) {}

	// If no repository was found throw an exception
	if (pathFoundCounter == 0) {
		throw std::exception("No icon path found. Please reinstall the application.");
	}

	// Create the main window
	m_mainWindow = uiAPI::createWindow(m_uid);

	// Create your elements

	// ...

	// Show window
	uiAPI::window::showMaximized(m_mainWindow);

	// Restore the window settings
	uiAPI::restoreStateColorStyle(uiAPI::settings::getString(c_settingsColorStyle, "").toStdString(), c_appVersion);
	uiAPI::restoreStateWindow(uiAPI::settings::getString(c_settingsWindowState, "").toStdString(), c_appVersion);

}

AppBase::~AppBase() {
	if (m_ttb != nullptr) { delete m_ttb; }
}

void AppBase::notify(
	ak::UID					_sender,
	ak::eventType			_eventType,
	int						_info1,
	int						_info2
) {
	try {

	}
	catch (const std::exception & e) {
		// Show an error dialog
		ak::uiAPI::promptDialog::show(e.what(), "Error", promptIconLeft, c_dialogErrorIcon, c_dialogIconPath);
	}
}

bool AppBase::closeEvent(void) {
	uiAPI::settings::setString(c_settingsWindowState, uiAPI::saveStateWindow(c_appVersion).c_str());
	uiAPI::settings::setString(c_settingsColorStyle, uiAPI::saveStateColorStyle(c_appVersion).c_str());
	return true;
}