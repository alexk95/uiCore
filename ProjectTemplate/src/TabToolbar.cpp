
// Project header
#include "TabToolbar.h"
#include "AppBase.h"

// uiCore header
#include <akAPI/uiAPI.h>

using namespace ak;

TabToolbar::TabToolbar(AppBase * _app)
	: m_app(_app)
{


	// Show the toolBar
	uiAPI::window::setTabToolBarVisible(m_app->m_mainWindow, true);
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

	}
	catch (const std::exception & e) {
		assert(0);
		uiAPI::promptDialog::show(e.what(), "Error");
	}
	catch (...) {
		assert(0);
		uiAPI::promptDialog::show("Unknown error", "Error");
	}
}