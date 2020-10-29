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
:	my_uid(_uid), my_references(0), my_objectType(_type), my_alias(""), my_parentObject(nullptr), my_owner(nullptr)
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

// ################################################################################

void ak::ui::core::aObject::removeChildObject(
	aObject *								_child
) {
	assert(_child != nullptr); // Nullptr provided
	assert(my_childObjects.find(_child->uid()) != my_childObjects.end()); // Child is not registered
	my_childObjects.erase(_child->uid());
}

void ak::ui::core::aObject::addChildObject(
	aObject *								_child
) {
	assert(_child != nullptr);
	assert(my_childObjects.find(_child->uid()) == my_childObjects.end()); // Child with the specified UID already exists
	my_childObjects.insert_or_assign(_child->uid(), _child);
}

void ak::ui::core::aObject::setAlias(
	const QString &							_alias
) { my_alias = _alias; }

// ################################################################################

QString ak::ui::core::aObject::alias(void) const { return my_alias; }

void ak::ui::core::aObject::setParentObject(
	aObject *								_parentObject
) { my_parentObject = _parentObject; }

ak::ui::core::aObject * ak::ui::core::aObject::parentObject(void) const { return my_parentObject; }

int ak::ui::core::aObject::childObjectCount(void) const { return my_childObjects.size(); }

ak::ui::core::aObject * ak::ui::core::aObject::childObject(
	ak::UID									_childUID
) {
	my_childObjectsIterator itm = my_childObjects.find(_childUID);
	assert(itm != my_childObjects.end()); // Invalid object UID
	assert(itm->second != nullptr);	// Nullptr stored
	return itm->second;
}

void ak::ui::core::aObject::setOwner(
	aObject *								_object
) { my_owner = _object; }

ak::ui::core::aObject * ak::ui::core::aObject::owner(void) const { return my_owner; }

// ################################################################################

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