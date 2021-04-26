// AK header
#include <ak_uiAPI.h>

// Project header
#include "AppBase.h"

int main(int _argc, char ** _argv) {
	// Initialize the uiCore. This step is mandatory before any UI element is created!
	ak::uiAPI::ini(nullptr, nullptr, nullptr, nullptr);

	// Create a new Application base object that should manage the application and setup the UI in the constructor
	AppBase app{ _argc, _argv };

	// Start the QT event loop from now on only messages (and/or signals) will be received in case of the registered events
	return ak::uiAPI::exec();
}