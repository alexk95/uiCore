// Application header
#include "AppBase.h"			// Corresponding header
#include "TabToolbar.h"			// TabToolbar

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
	: m_ttb(nullptr)
{
	// Create own UID
	m_uid = ak::uiAPI::createUid();

	// Create the main window
	m_mainWindow = uiAPI::createWindow(m_uid);

	// Create the tabToolbar
	m_ttb = new TabToolbar(this);

	// Create your elements

	// ...

	// Add this object as an window event listener to the main window
	uiAPI::window::addEventHandler(m_mainWindow, this);

	// Show window
	uiAPI::window::showMaximized(m_mainWindow);

	// Restore the window settings
	uiAPI::restoreStateColorStyle(uiAPI::settings::getString(c_settingsColorStyle, "").toStdString(), c_appVersion);
	uiAPI::restoreStateWindow(uiAPI::settings::getString(c_settingsWindowState, "").toStdString(), c_appVersion);

}

AppBase::~AppBase() {
	if (m_ttb != nullptr) { delete m_ttb; }
}

bool AppBase::closeEvent(void) {
	uiAPI::settings::setString(c_settingsWindowState, uiAPI::saveStateWindow(c_appVersion).c_str());
	uiAPI::settings::setString(c_settingsColorStyle, uiAPI::saveStateColorStyle(c_appVersion).c_str());
	return true;
}