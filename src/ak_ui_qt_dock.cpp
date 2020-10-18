/*
 * ak_ui_qt_dock.cpp
 *
 *  Created on: August 10, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_qt_dock.h>						// corresponding class
#include <ak_exception.h>						// error handling
#include <ak_ui_colorStyle.h>					// colorStyle

// Qt header
#include <qevent.h>								// QKeyEvent

// C++ header
#include <string>								// string

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
	try {
		assert(_colorStyle != nullptr); // Nullptr provided
		my_colorStyle = _colorStyle;
		setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sDock));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::dock::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::dock::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::dock::setColorStyle()"); }
}

void ak::ui::qt::dock::setAlias(
	const QString &							_alias
) {
	my_alias = _alias;
	setObjectName(_alias);
}

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