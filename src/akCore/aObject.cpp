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
#include <akCore/aObject.h>
#include <akCore/aException.h>

ak::aObject::aObject(objectType _type, UID _uid)//, int _references)
	: my_uid(_uid), my_references(0), my_objectType(_type), my_alias(""), my_parentObject(nullptr), my_owner(nullptr)
{}

ak::aObject::aObject(
	const ak::aObject &			_other
) {
	my_uid = _other.uid();
	my_objectType = _other.type();
	my_alias = _other.alias();
}

ak::aObject & ak::aObject::operator = (
	const ak::aObject &			_other
) {
	my_uid = _other.uid();
	my_objectType = _other.type();
	my_alias = _other.alias();
	return *this;
}

ak::aObject::~aObject() {}

// ################################################################################

void ak::aObject::removeChildObject(
	aObject *								_child
) {
	assert(_child != nullptr); // Nullptr provided
	assert(my_childObjects.find(_child->uid()) != my_childObjects.end()); // Child is not registered
	my_childObjects.erase(_child->uid());
}

void ak::aObject::addChildObject(
	aObject *								_child
) {
	assert(_child != nullptr);
	assert(my_childObjects.find(_child->uid()) == my_childObjects.end()); // Child with the specified UID already exists
	my_childObjects.insert_or_assign(_child->uid(), _child);
}

void ak::aObject::setAlias(
	const QString &							_alias
) { my_alias = _alias; }

// ################################################################################

QString ak::aObject::alias(void) const { return my_alias; }

void ak::aObject::setParentObject(
	aObject *								_parentObject
) { my_parentObject = _parentObject; }

ak::aObject * ak::aObject::parentObject(void) const { return my_parentObject; }

int ak::aObject::childObjectCount(void) const { return my_childObjects.size(); }

ak::aObject * ak::aObject::childObject(
	UID									_childUID
) {
	auto itm = my_childObjects.find(_childUID);
	assert(itm != my_childObjects.end()); // Invalid object UID
	assert(itm->second != nullptr);	// Nullptr stored
	return itm->second;
}

void ak::aObject::setOwner(
	aObject *								_object
) { my_owner = _object; }

ak::aObject * ak::aObject::owner(void) const { return my_owner; }

// ################################################################################

void ak::aObject::setUid(UID _UID) { my_uid = _UID; }

ak::UID ak::aObject::uid(void) const { return my_uid; }

ak::objectType ak::aObject::type(void) const { return my_objectType; }

bool ak::aObject::isPaintableType(void) const { return false; }

bool ak::aObject::isWidgetType(void) const { return false; }