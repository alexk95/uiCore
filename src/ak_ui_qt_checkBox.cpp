/*
 * ak_ui_qt_checkBox.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_checkBox.h>					// corresponding header
#include <ak_ui_colorStyle.h>					// colorStyle
#include <ak_exception.h>						// error handling

// Qt header
#include <qevent.h>								// QKeyEvent

// C++ header
#include <string>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::checkBox::checkBox(
	QWidget *								_parent
) : QCheckBox(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oCheckBox) {}
ak::ui::qt::checkBox::checkBox(
	const QString &							_text,
	QWidget *								_parent
) : QCheckBox(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oCheckBox) {}

ak::ui::qt::checkBox::~checkBox() { A_OBJECT_DESTROYING }

// #######################################################################################################

// Event handling

void ak::ui::qt::checkBox::keyPressEvent(
	QKeyEvent *								_event
) {
	QCheckBox::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::checkBox::keyReleaseEvent(
	QKeyEvent *								_event
) {
	QCheckBox::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::checkBox::widget(void) { return this; }

void ak::ui::qt::checkBox::setColorStyle(
	const ak::ui::colorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls | TYPE_COLORAREA::caBackgroundColorControls, "#" + my_alias + "{", "}"));
	}
	else {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls | TYPE_COLORAREA::caBackgroundColorControls));
	}
}

void ak::ui::qt::checkBox::setAlias(
	const QString &							_alias
) { ui::core::aObject::setAlias(_alias); setObjectName(my_alias); }

void ak::ui::qt::checkBox::addObjectSettingsToValue(
	rapidjson::Value &						_array,
	rapidjson::Document::AllocatorType &	_allocator
) {
	assert(_array.GetType() == rapidjson::Type::kArrayType); // Value is not an array type

	// Initialize object
	rapidjson::Value root;
	root.SetObject();

	// Add alias
	std::string str(objectName().toStdString());
	rapidjson::Value nAlias(str.c_str(), _allocator);
	root.AddMember(RESTORABLE_NAME_ALIAS, nAlias, _allocator);

	// Add object type
	str = ak::ui::core::toQString(my_objectType).toStdString();
	rapidjson::Value nType(str.c_str(), _allocator);
	root.AddMember(RESTORABLE_NAME_TYPE, nType, _allocator);

	// Create settings
	rapidjson::Value settings;
	settings.SetObject();

	settings.AddMember(RESTORABLE_CFG_CHECKED, isChecked(), _allocator);
	
	// Add settings
	root.AddMember(RESTORABLE_NAME_SETTINGS, settings, _allocator);
	_array.PushBack(root, _allocator);
}

void ak::ui::qt::checkBox::restoreSettings(
	const rapidjson::Value &				_settings
) {
	assert(_settings.IsObject()); // Value is not an object

	if (_settings.HasMember(RESTORABLE_CFG_CHECKED)) {
		assert(_settings[RESTORABLE_CFG_CHECKED].IsBool());		// Not a boolean value stored
		setChecked(_settings[RESTORABLE_CFG_CHECKED].GetBool());
	}
}