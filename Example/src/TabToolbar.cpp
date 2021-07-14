
// Project header
#include "TabToolbar.h"
#include "AppBase.h"

// uiCore header
#include <akAPI/uiAPI.h>

// C++ header
#include <exception>

// Here we define all icon names and the icon path
const QString c_icoPath = "ToolBar";
const QString c_icoExit = "ExitAppBlue";
const QString c_icoColorStyleBright = "Sun";
const QString c_icoColorStyleDark = "Moon";
const QString c_icoDocks = "Docks";
const QString c_icoDebug = "BugRed";

using namespace ak;

TabToolbar::TabToolbar(AppBase * _app) 
	: m_app(_app)
{
	// Here we create the main page and the group to place the buttons at
	m_pFile.page = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_app->m_mainWindow, "File");
	m_pFile.gDefault = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_pFile.page, "");

	// Now create the buttons
	m_pFile.gDefault_aExit = uiAPI::createToolButton(m_app->m_uid, "Exit", c_icoExit, c_icoPath);
	const QIcon & icoBright = uiAPI::getIcon(c_icoColorStyleBright, c_icoPath);
	const QIcon & icoDark = uiAPI::getIcon(c_icoColorStyleDark, c_icoPath);
	m_pFile.gDefault_aColorStyle = uiAPI::createColorStyleSwitch(m_app->m_uid, "Bright", "Dark", icoBright, icoDark);
	m_pFile.gDefault_aDocks = uiAPI::createDockWatcher(m_app->m_uid, c_icoDocks, c_icoPath, "Docks");
	m_pFile.gDefault_aDebug = uiAPI::createToolButton(m_app->m_uid, "Test", c_icoDebug, c_icoPath);

	// To display the buttons, add them to the group
	uiAPI::container::addObject(m_pFile.gDefault, m_pFile.gDefault_aExit);
	uiAPI::container::addObject(m_pFile.gDefault, m_pFile.gDefault_aColorStyle);
	uiAPI::container::addObject(m_pFile.gDefault, m_pFile.gDefault_aDocks);
	uiAPI::container::addObject(m_pFile.gDefault, m_pFile.gDefault_aDebug);

	// Show the toolBar in the main window
	uiAPI::window::setTabToolBarVisible(m_app->m_mainWindow);

	// Register callbacks for the exit button
	uiAPI::registerUidNotifier(m_pFile.gDefault_aExit, this);
	uiAPI::registerUidNotifier(m_pFile.gDefault_aDebug, this);
}

TabToolbar::~TabToolbar() {

}

void TabToolbar::notify(
	ak::UID					_sender,
	ak::eventType			_eventType,
	int						_info1,
	int						_info2
) {
	try {
		if (_sender == m_pFile.gDefault_aExit && _eventType == etClicked) {
			uiAPI::promptDialog::show("This is just a test to demonstrate how the error promt dialog looks like :-)", "Error", promptIconLeft, "DialogError", "Dialog");

			// Here we know that the exit button was clicked and we will now call the close window
			uiAPI::window::close(m_app->m_mainWindow);
		}
		else if (_sender == m_pFile.gDefault_aDebug && _eventType == etClicked) {
			m_app->testFunction();
		}
	}
	catch (const std::exception & e) {
		// We display a dialog with an error icon (see the aIconManager documentation)
		uiAPI::promptDialog::show(e.what(), "Error", promptIconLeft, "DialogError", "Dialog");
	}
	catch (...) {
		// We display a dialog with an error icon (see the aIconManager documentation)
		uiAPI::promptDialog::show("Unknown error", "Error", promptIconLeft, "DialogError", "Dialog");
	}
}