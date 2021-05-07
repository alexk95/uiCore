/*
 * ak_ui_core_aWidget.cpp
 *
 *  Created on: July 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_core_aWidget.h>		// corresponding header
#include <ak_ui_colorStyle.h>		// colorStyle
#include <ak_ui_qt_dock.h>

ak::ui::core::aWidget::aWidget(
	ak::ui::core::objectType	_type,
	const ak::ui::colorStyle *	_colorStyle,
	ak::UID						_UID
) : ak::ui::core::aPaintable(_type, _colorStyle, _UID) {}

ak::ui::core::aWidget::~aWidget() {}

bool ak::ui::core::aWidget::isWidgetType(void) const { return true; }
