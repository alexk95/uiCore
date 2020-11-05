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
#include <qtabbar.h>

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
	A_OBJECT_DESTROYING
	disconnect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(slotDockLocationChanged(Qt::DockWidgetArea)));
}

// #######################################################################################################

QWidget * ak::ui::qt::dock::widget(void) { return this; }

void ak::ui::qt::dock::setColorStyle(
	const ak::ui::colorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet;
	sheet = my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow, "QDockWidget{", "titlebar-close-icon: none; titlebar-normal-icon: none;}\n");
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorHeader |
		TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caBorderColorHeader, "QDockWidget::title{border-width: 1px;", "}\n"));
	
	if (!sheet.isEmpty()) {
		sheet.append("QDockWidget::close-button{border: none; background: transparent; icon-size: 12px; image: url(");
		sheet.append(my_colorStyle->getFilePath("Dock/Dock_Close.png"));
		sheet.append(");}");

		sheet.append("QDockWidget::close-button:hover:!pressed{border: none; color: transparent; background: transparent; icon-size: 12px; image: url(");
		sheet.append(my_colorStyle->getFilePath("Dock/Dock_Close_Focus.png"));
		sheet.append(");}");

		sheet.append("QDockWidget::close-button:pressed{border: none; background: transparent; icon-size: 12px; image: url(");
		sheet.append(my_colorStyle->getFilePath("Dock/Dock_Close_Pressed.png"));
		sheet.append(");}");

		sheet.append("QDockWidget::float-button{border: none; background: transparent; icon-size: 12px; image: url(");
		sheet.append(my_colorStyle->getFilePath("Dock/Dock_Float.png"));
		sheet.append(");}");

		sheet.append("QDockWidget::float-button:hover:!pressed{border: none; background: transparent; icon-size: 12px; image: url(");
		sheet.append(my_colorStyle->getFilePath("Dock/Dock_Float_Focus.png"));
		sheet.append(");}");

		sheet.append("QDockWidget::float-button:pressed{border: none; background: transparent; icon-size: 12px; image: url(");
		sheet.append(my_colorStyle->getFilePath("Dock/Dock_Float_Pressed.png"));
		sheet.append(");}");
	}
	this->setStyleSheet(sheet);

	this->setStyleSheet(sheet);
}

void ak::ui::qt::dock::removeChildObject(
	aObject *									_child
) {
	ui::core::aObject::removeChildObject(_child);
	setWidget(nullptr);
}

// #######################################################################################################

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