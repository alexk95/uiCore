/*
 * ak_ui_qt_dock.cpp
 *
 *  Created on: August 10, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_dock.h>						// corresponding class
#include <ak_exception.h>						// error handling
#include <ak_ui_colorStyle.h>					// colorStyle

// Qt header
#include <qevent.h>								// QKeyEvent

// C++ header
#include <string>								// string

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::dock::dock(
	const QString &							_title,
	ak::ui::colorStyle *					_colorStyle,
	QWidget *								_parent,
	Qt::WindowFlags							_flags
) 
	: ak::ui::core::aWidget(ak::ui::core::objectType::oDock, _colorStyle),
	QDockWidget(_title, _parent, _flags)
{
	connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(slotDockLocationChanged(Qt::DockWidgetArea)));
}

ak::ui::qt::dock::~dock() {
	disconnect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(slotDockLocationChanged(Qt::DockWidgetArea)));
}

// #######################################################################################################

QWidget * ak::ui::qt::dock::widget(void) { return this; }

void ak::ui::qt::dock::setColorStyle(
	const ak::ui::colorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
			TYPE_COLORAREA::caBackgroundColorWindow, "#" + my_alias + "{", "}"));
	}
	else {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
			TYPE_COLORAREA::caBackgroundColorWindow));
	}
}

void ak::ui::qt::dock::setAlias(
	const QString &							_alias
) { ui::core::aObject::setAlias(_alias); setObjectName(my_alias); }

void ak::ui::qt::dock::addObjectSettingsToValue(
	rapidjson::Value &						_array,
	rapidjson::Document::AllocatorType &	_allocator
) {
	assert(_array.GetType() == rapidjson::Type::kArrayType); // Value is not an array type
}

void ak::ui::qt::dock::restoreSettings(
	const rapidjson::Value &				_settings
) {
	assert(_settings.IsObject()); // Value is not an object

}

void ak::ui::qt::dock::slotDockLocationChanged(
	Qt::DockWidgetArea						_area
) {
	switch (_area)
	{
	case Qt::DockWidgetArea::BottomDockWidgetArea: my_location = ui::core::dock_dockBottom; break;
	case Qt::DockWidgetArea::LeftDockWidgetArea: my_location = ui::core::dock_dockLeft; break;
	case Qt::DockWidgetArea::RightDockWidgetArea: my_location = ui::core::dock_dockRight; break;
	default:
		assert(0); // Can't happen?
		break;
	}
}