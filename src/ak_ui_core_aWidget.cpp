/*
 * ak_ui_core_aWidget.cpp
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_core_aWidget.h>		// corresponding header
#include <ak_ui_colorStyle.h>		// colorStyle

ak::ui::core::aWidget::aWidget(
	ak::ui::core::objectType	_type,
	ak::ui::colorStyle *		_colorStyle,
	ak::UID						_UID,
	int							_references
) : ak::ui::core::aPaintable(_type, _colorStyle, _UID, _references) {}

ak::ui::core::aWidget::~aWidget() {}

bool ak::ui::core::aWidget::isWidgetType(void) const { return true; }