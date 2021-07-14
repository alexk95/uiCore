/*
 * AppBase.cpp
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// Application header
#include "AppBase.h"			// Corresponding header
#include "TabToolbar.h"			// TabToolBar
#include "myCustomCentralWidget.h"

// AK header
#include <akAPI/uiAPI.h>		// The uiAPI
#include <akGui/aColorStyle.h>	// ColorStyle if needed for custom widgets

using namespace ak;

// We create some constant values for the icons
const QString c_dialogErrorIcon = "DialogError";
const QString c_dialogInfoIcon = "DialogInformation";
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

	// Add this object as a window event handler to the main window we have just created
	uiAPI::window::addEventHandler(m_mainWindow, this);

	// Create the tabToolbar
	m_ttb = new TabToolbar(this);

	// Create the central widget
	m_centralWidget = new myCustomCentralWidget(this);
	uiAPI::window::setCentralWidget(m_mainWindow, m_centralWidget);

	// Show window
	uiAPI::window::showMaximized(m_mainWindow);

	// Restore the window and color style settings from the last session
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
	// We store the window state and the color style in the settings so they can be restored on the next application start
	uiAPI::settings::setString(c_settingsWindowState, uiAPI::saveStateWindow(c_appVersion).c_str());
	uiAPI::settings::setString(c_settingsColorStyle, uiAPI::saveStateColorStyle(c_appVersion).c_str());
	return true;
}

// #########################################################################################################

// Test zone

#include <akNet/aCurlWrapper.h>

void AppBase::testFunction(void) {
	std::string response;
	if (aCurlWrapper::sendMail(
		"imap.mail.ru",
		"<kxxxbabbaalex@mail.ru>",
		std::list<std::string>({ "<kxxxbabbaalex@mail.ru>" }),
		std::list<std::string>(),
		"Test subject",
		"My first curl mail",
		"",
		response
	)) {
		uiAPI::promptDialog::show(response.c_str(), "Success", promptIconLeft, c_dialogInfoIcon, c_dialogIconPath);
	}
	else {
		// Exception will be catched in the tab toolbar and the message will be displayed in a promt dialog
		throw std::exception(response.c_str());
	}

}
