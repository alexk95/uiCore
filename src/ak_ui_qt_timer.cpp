#include <ak_ui_qt_timer.h>		// Corresponding header

ak::ui::qt::timer::timer()
	: QTimer(), ak::ui::core::aObject(ak::ui::core::objectType::oTimer)
{}

ak::ui::qt::timer::~timer() {}