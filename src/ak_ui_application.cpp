

#include <ak_ui_application.h>			// Corresponding header

static int argc = 0;

ak::ui::application::application()
	: QApplication(argc, nullptr)
{}

ak::ui::application::~application() {}