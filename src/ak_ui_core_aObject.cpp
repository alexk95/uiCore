/*
 * ak_ui_core_aObject.cpp
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_core_aObject.h>			// corresponding header
#include <ak_exception.h>				// error handling

ak::ui::core::aObject::aObject(ak::ui::core::objectType _type, ak::UID _uid, int _references)
:	my_uid(_uid), my_references(0), my_objectType(_type)
{
	try { setReferences(_references); }
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::core::aObject::aObject()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::core::aObject::aObject()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::core::aObject::aObject()"); }
}

ak::ui::core::aObject::aObject(
	const ak::ui::core::aObject &			_other
) {
	my_uid = _other.uid();
	my_references = _other.references();
	my_objectType = _other.objectType();
}

ak::ui::core::aObject & ak::ui::core::aObject::operator = (
	const ak::ui::core::aObject &			_other
) {
	my_uid = _other.uid();
	my_references = _other.references();
	my_objectType = _other.objectType();
	return *this;
}

ak::ui::core::aObject::~aObject() {}

void ak::ui::core::aObject::setUid(ak::UID _UID) { my_uid = _UID; }

ak::UID ak::ui::core::aObject::uid(void) const { return my_uid; }

void ak::ui::core::aObject::setReferences(int _references) {
	if (_references < 0) { throw ak::Exception("Provided reference is negative", "ak::ui::core::aObject::setReferences()"); }
	if (_references >= INT16_MAX) { throw ak::Exception("Reference overflow", "ak::ui::core::aObject::setReferences()"); }
	my_references = _references;
}

int ak::ui::core::aObject::references(void) const { return my_references; }

int ak::ui::core::aObject::incrReferences(void) {
	if (my_references == INT16_MAX) { throw ak::Exception("References overflow", "ak::ui::core::aObject::incrReferences()"); }
	return ++my_references;
}

int ak::ui::core::aObject::decrReferences(void) {
	if (my_references > 0) { return --my_references; }
	else { return 0; }
}

ak::ui::core::objectType ak::ui::core::aObject::objectType(void) const { return my_objectType; }

bool ak::ui::core::aObject::isPaintableType(void) const { return false; }

bool ak::ui::core::aObject::isWidgetType(void) const { return false; }