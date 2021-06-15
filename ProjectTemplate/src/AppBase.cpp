// Project header
#include "AppBase.h"

#include <akAPI/uiAPI.h>

AppBase::AppBase(int _argc, char ** _argv) {

	// Create a new UID for the application
	my_uid = ak::uiAPI::createUid();

	// Create the main window
	my_mainWindow = ak::uiAPI::createWindow(my_uid);

	// Show the main window
	ak::uiAPI::window::showMaximized(my_mainWindow);

}

AppBase::~AppBase() {

}