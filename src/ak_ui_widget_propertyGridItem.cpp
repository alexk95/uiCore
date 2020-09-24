/*
 * ak_ui_widget_propertyGridItem.cpp
 *
 *  Created on: March 30, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_widget_propertyGridItem.h"	// corresponding header
#include "ak_exception.h"					// error handling
#include "ak_ui_core.h"						// objectType

ak::ui::widget::propertyGridItem::propertyGridItem(
	ak::ID				_index,
	const QString &		_propertyName,
	ak::core::valueType	_valueType,
	ak::UID				_itemsUid,
	bool				_isMultivalued
) : my_bool(false),
	my_index(_index),
	my_widgetUid(ak::invalidUID),
	my_lastText(QString()),
	my_int(0),
	my_double(0.0),
	my_string(QString()),
	my_color(ak::ui::color()),
	my_propertyName(_propertyName),
	my_isMultivalued(_isMultivalued)
{
	switch (_valueType)
	{
	case ak::core::valueType::vBool: my_valueType = _valueType; break;
	case ak::core::valueType::vColor: my_valueType = _valueType; break;
	case ak::core::valueType::vDouble: my_valueType = _valueType; break;
	case ak::core::valueType::vInt: my_valueType = _valueType; break;
	case ak::core::valueType::vString: my_valueType = _valueType; break;
	case ak::core::valueType::vSelection: my_valueType = _valueType; break;
	default: throw ak::Exception("Invalid value type provided! Allowed: bool, int, double, string, color, selection", "ak::ui::widget::propertyGridItem::propertyGridItem()");
		break;
	}
}

ak::ui::widget::propertyGridItem::propertyGridItem(
	const ak::ui::widget::propertyGridItem &		_other
) :	my_bool(false),
	my_index(0),
	my_widgetUid(ak::invalidUID),
	my_int(0),
	my_double(0.0),
	my_string(QString()),
	my_color(ak::ui::color()),
	my_isMultivalued(_other.isMultivalued())
{
	my_valueType = _other.valueType();
	my_index = _other.index();
	my_bool = _other.getBool(false);
	my_color = _other.getColor(false);
	my_double = _other.getDouble(false);
	my_int = _other.getInt(false);
	my_string = _other.getString(false);
}

ak::ui::widget::propertyGridItem & ak::ui::widget::propertyGridItem::operator = (const ak::ui::widget::propertyGridItem & _other) {
	my_valueType = _other.valueType();
	my_index = _other.index();
	my_widgetUid = _other.getWidgetUid();
	my_bool = _other.getBool(false);
	my_color = _other.getColor(false);
	my_double = _other.getDouble(false);
	my_int = _other.getInt(false);
	my_string = _other.getString(false);
	return * this;
}

ak::ui::widget::propertyGridItem::~propertyGridItem() {}

// #############################################################################################
// Set values

void ak::ui::widget::propertyGridItem::setIndex(
	ak::ID									_index
) { my_index = _index; }

void ak::ui::widget::propertyGridItem::setWidgetUid(
	ak::UID									_widgetUid
) { my_widgetUid = _widgetUid; }

void ak::ui::widget::propertyGridItem::setPropertyName(
	const QString &							_propertyName
) { my_propertyName = _propertyName; }

void ak::ui::widget::propertyGridItem::setLastText(
	const QString &							_text
) { my_lastText = _text; }

void ak::ui::widget::propertyGridItem::setIsMultivalued(
	bool									_isMultivalued
) { my_isMultivalued = _isMultivalued; }

void ak::ui::widget::propertyGridItem::setBool(
	bool									_value,
	bool									_throwIfInvalidType
) {
	if (my_valueType == ak::core::valueType::vBool || !_throwIfInvalidType) { my_bool = _value; }
	else { throw ak::Exception("Object type is not [bool]", "ak::ui::widget::propertyGridItem::setBool()"); }
}

void ak::ui::widget::propertyGridItem::setInt(
	int										_value,
	bool									_throwIfInvalidType
) {
	if (my_valueType == ak::core::valueType::vInt || !_throwIfInvalidType) { my_int = _value; }
	else { throw ak::Exception("Object type is not [int]", "ak::ui::widget::propertyGridItem::setInt()"); }
}

void ak::ui::widget::propertyGridItem::setDouble(
	double									_value,
	bool									_throwIfInvalidType
) {
	if (my_valueType == ak::core::valueType::vDouble || !_throwIfInvalidType) { my_double = _value; }
	else { throw ak::Exception("Object type is not [double]", "ak::ui::widget::propertyGridItem::setDouble()"); }
}

void ak::ui::widget::propertyGridItem::setString(
	const QString &							_value,
	bool									_throwIfInvalidType
) {
	if (my_valueType == ak::core::valueType::vString || !_throwIfInvalidType) { my_string = _value; }
	else { throw ak::Exception("Object type is not [string]", "ak::ui::widget::propertyGridItem::setString()"); }
}

void ak::ui::widget::propertyGridItem::setColor(
	const ak::ui::color &					_value,
	bool									_throwIfInvalidType
) {
	if (my_valueType == ak::core::valueType::vColor || !_throwIfInvalidType) { my_color = _value; }
	else { throw ak::Exception("Object type is not [color]", "ak::ui::widget::propertyGridItem::setColor()"); }
}

void ak::ui::widget::propertyGridItem::setSelection(
	const QString &							_value,
	bool									_throwIfInvalidType
) {
	if (my_valueType == ak::core::valueType::vSelection || !_throwIfInvalidType) { my_selection = _value; }
	else { throw ak::Exception("Object type is not [selection]", "ak::ui::widget::propertyGridItem::setColor()"); }
}

void ak::ui::widget::propertyGridItem::setPossibleSelection(
	const std::vector<QString> &			_value,
	bool									_throwIfInvalidType
) {
	if (my_valueType == ak::core::valueType::vSelection || !_throwIfInvalidType) {
		my_selectionPossible.clear();
		my_selectionPossible = _value;
	}
	else { throw ak::Exception("Object type is not [selection]", "ak::ui::widget::propertyGridItem::setColor()"); }
}

// #############################################################################################
// Get

ak::ID ak::ui::widget::propertyGridItem::index(void) const { return my_index; }

ak::UID ak::ui::widget::propertyGridItem::getWidgetUid(void) const { return my_widgetUid; }

ak::core::valueType ak::ui::widget::propertyGridItem::valueType(void) const { return my_valueType; }

QString ak::ui::widget::propertyGridItem::propertyName(void) const { return my_propertyName; }

QString ak::ui::widget::propertyGridItem::lastText(void) const { return my_lastText; }

bool ak::ui::widget::propertyGridItem::isMultivalued(void) const { return my_isMultivalued; }

bool ak::ui::widget::propertyGridItem::getBool(
	bool				_throwIfInvalidType
) const {
	if (my_valueType == ak::core::valueType::vBool || !_throwIfInvalidType) { return my_bool; }
	else { throw ak::Exception("Object type is not bool", "ak::ui::widget::propertyGridItem::getBool()"); }
}

int ak::ui::widget::propertyGridItem::getInt(
	bool				_throwIfInvalidType
) const {
	if (my_valueType == ak::core::valueType::vInt || !_throwIfInvalidType) { return my_int; }
	else { throw ak::Exception("Object type is not int", "ak::ui::widget::propertyGridItem::getInt()"); }
}

double ak::ui::widget::propertyGridItem::getDouble(
	bool				_throwIfInvalidType
) const {
	if (my_valueType == ak::core::valueType::vDouble || !_throwIfInvalidType) { return my_double; }
	else { throw ak::Exception("Object type is not double", "ak::ui::widget::propertyGridItem::getDouble()"); }
}

QString ak::ui::widget::propertyGridItem::getString(
	bool				_throwIfInvalidType
) const {
	if (my_valueType == ak::core::valueType::vString || !_throwIfInvalidType) { return my_string; }
	else { throw ak::Exception("Object type is not string", "ak::ui::widget::propertyGridItem::getString()"); }
}

ak::ui::color ak::ui::widget::propertyGridItem::getColor(
	bool				_throwIfInvalidType
) const {
	if (my_valueType == ak::core::valueType::vColor || !_throwIfInvalidType) { return my_color; }
	else { throw ak::Exception("Object type is not color", "ak::ui::widget::propertyGridItem::getColor()"); }
}

QString ak::ui::widget::propertyGridItem::getSelection(
	bool												_throwIfInvalidType
) const {
	if (my_valueType == ak::core::valueType::vSelection || !_throwIfInvalidType) { return my_selection; }
	else { throw ak::Exception("Object type is not selection", "ak::ui::widget::propertyGridItem::getColor()"); }
}

std::vector<QString> ak::ui::widget::propertyGridItem::getSelectionPossible(
	bool												_throwIfInvalidType
) const {
	if (my_valueType == ak::core::valueType::vSelection || !_throwIfInvalidType) { return my_selectionPossible; }
	else { throw ak::Exception("Object type is not selection", "ak::ui::widget::propertyGridItem::getColor()"); }
}
