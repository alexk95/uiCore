/*
 * ak_ui_core_aObject.cpp
 *
 *  Created on: July 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_core_aObject.h>			// corresponding header
#include <ak_exception.h>				// error handling

ak::ui::core::aObject::aObject(ak::ui::core::objectType _type, ak::UID _uid, int _references)
:	my_uid(_uid), my_references(0), my_objectType(_type), my_alias("")
{ setReferences(_references); }

ak::ui::core::aObject::aObject(
	const ak::ui::core::aObject &			_other
) {
	my_uid = _other.uid();
	my_references = _other.references();
	my_objectType = _other.objectType();
	my_alias = _other.alias();
}

ak::ui::core::aObject & ak::ui::core::aObject::operator = (
	const ak::ui::core::aObject &			_other
) {
	my_uid = _other.uid();
	my_references = _other.references();
	my_objectType = _other.objectType();
	my_alias = _other.alias();
	return *this;
}

ak::ui::core::aObject::~aObject() {}

void ak::ui::core::aObject::setAlias(
	const QString &							_alias
) { my_alias = _alias; }

QString ak::ui::core::aObject::alias(void) const { return my_alias; }

void ak::ui::core::aObject::setUid(ak::UID _UID) { my_uid = _UID; }

ak::UID ak::ui::core::aObject::uid(void) const { return my_uid; }

void ak::ui::core::aObject::setReferences(int _references) {
	assert(_references >= 0 && _references <= INT16_MAX); // Reference out of range
	my_references = _references;
}

int ak::ui::core::aObject::references(void) const { return my_references; }

int ak::ui::core::aObject::incrReferences(void) {
	assert(my_references < INT16_MAX); // Maximum references reached
	return ++my_references;
}

int ak::ui::core::aObject::decrReferences(void) {
	if (my_references > 0) { return --my_references; }
	else { return 0; }
}

ak::ui::core::objectType ak::ui::core::aObject::objectType(void) const { return my_objectType; }

bool ak::ui::core::aObject::isPaintableType(void) const { return false; }

bool ak::ui::core::aObject::isWidgetType(void) const { return false; }