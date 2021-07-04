/*
 * main.cpp
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include <akAPI/uiAPI.h>
#include "AppBase.h"			// The example class managing the UI

using namespace ak;

int main(int _argc, char * _argv[])
{
	// Initialize the API. This function call is mandatory,
	// otherwise the API will not be able to create objects.
	// The initialization will create all core objects required for the API to work
	uiAPI::ini("<Your organization name>", "<Your application name>");

	try {
		// Create the main class that is managing the functions of the UI
		AppBase e(_argc, _argv);

		// Start the Qt event queue
		return uiAPI::exec();
	}
	catch (const std::exception & e) {
		uiAPI::promptDialog::show(e.what(), "Fatal error");
		return -1;
	}
	catch (...) {
		uiAPI::promptDialog::show("Unknown error", "Fatal error");
		return -2;
	}

	
}
