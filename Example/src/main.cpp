/*
 * main.cpp
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include <ak_uiAPI.h>			// uiAPI
#include <Example.h>			// The example class managing the UI

using namespace ak;

int main(int argc, char *argv[])
{
	// Initialize the API. This function call is mandatory,
	// otherwise the API will not be able to create objects.
	// The initialization will create all core objects required for the API to work
	uiAPI::ini(true);

	// Create the main class that is managing the functions of the UI
	Example e;

	return 0;
}
