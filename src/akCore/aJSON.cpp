/*
 *	File:		aJSON.cpp
 *	Package:	akCore
 *
 *  Created on: June 15, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akCore/aJSON.h>

#include <qstring.h>
#include <qstringlist.h>

#include <cassert>

ak::aJSON::aJSON(jsonType _type)
	: m_type(_type), m_bool(false), m_double(0.0), m_int(0)
{}

ak::aJSON::aJSON(bool _v)
	: m_type(Boolean), m_bool(_v), m_double(0.0), m_int(0)
{}

ak::aJSON::aJSON(double _v)
	: m_type(Double), m_bool(false), m_double(_v), m_int(0)
{}

ak::aJSON::aJSON(int _v)
	: m_type(Integer), m_bool(false), m_double(_v), m_int(_v)
{}

ak::aJSON::aJSON(char _v)
	: m_type(String), m_bool(false), m_double(0.0), m_int(0)
{
	char v[2]{ _v, 0 };
	m_string = v;
}

ak::aJSON::aJSON(const char * _v)
	: m_type(String), m_bool(false), m_double(0.0), m_int(0), m_string(_v)
{}

ak::aJSON::aJSON(const std::string & _v)
	: m_type(String), m_bool(false), m_double(0.0), m_int(0), m_string(_v)
{}

ak::aJSON::aJSON(const aJSON & _other)
	: m_type(_other.type()), m_bool(false), m_double(0.0), m_int(0)
{
	switch (m_type)
	{
	case ak::aJSON::Array: m_array = _other.m_array; break;
	case ak::aJSON::Boolean: m_bool = _other.m_bool; break;
	case ak::aJSON::Double: m_double = _other.m_double; break;
	case ak::aJSON::Integer: m_int = _other.m_int; break;
	case ak::aJSON::Null: break;
	case ak::aJSON::Object: m_obj = _other.m_obj; break;
	case ak::aJSON::String: m_string = _other.m_string; break;
	default:
		assert(0);
		break;
	}
}

ak::aJSON::~aJSON() {

}

// ########################################################################################

// Setter

void ak::aJSON::setString(char _v) {
	char v[2]{ _v, 0 };
	m_string = v;
}

void ak::aJSON::reserve(size_t _size) {
	if (m_type == Array) {
		m_array.reserve(_size);
	}
}

void ak::aJSON::pushBack(bool _v) {
	if (m_type == Array) { m_array.push_back(aJSON(_v)); }
}

void ak::aJSON::pushBack(int _v) {
	if (m_type == Array) { m_array.push_back(aJSON(_v)); }
}

void ak::aJSON::pushBack(double _v) {
	if (m_type == Array) { m_array.push_back(aJSON(_v)); }
}

void ak::aJSON::pushBack(char _v) {
	if (m_type == Array) { m_array.push_back(aJSON(_v)); }
}

void ak::aJSON::pushBack(const char * _v) {
	if (m_type == Array) { m_array.push_back(aJSON(_v)); }
}

void ak::aJSON::pushBack(const std::string & _v) {
	if (m_type == Array) { m_array.push_back(aJSON(_v)); }
}

void ak::aJSON::pushBack(const aJSON & _v) {
	if (m_type == Array) { m_array.push_back(aJSON(_v)); }
}

void ak::aJSON::addMember(const char * _member, bool _v) {
	if (m_type == Object) { m_obj.insert_or_assign(std::string(_member), aJSON(_v)); }
}

void ak::aJSON::addMember(const char * _member, int _v) {
	if (m_type == Object) { m_obj.insert_or_assign(std::string(_member), aJSON(_v)); }
}

void ak::aJSON::addMember(const char * _member, double _v) {
	if (m_type == Object) { m_obj.insert_or_assign(std::string(_member), aJSON(_v)); }
}

void ak::aJSON::addMember(const char * _member, char _v) {
	if (m_type == Object) { m_obj.insert_or_assign(std::string(_member), aJSON(_v)); }
}

void ak::aJSON::addMember(const char * _member, const char * _v) {
	if (m_type == Object) { m_obj.insert_or_assign(std::string(_member), aJSON(_v)); }
}

void ak::aJSON::addMember(const char * _member, const std::string & _v) {
	if (m_type == Object) { m_obj.insert_or_assign(std::string(_member), aJSON(_v)); }
}

void ak::aJSON::addMember(const char * _member, const aJSON & _v) {
	if (m_type == Object) { m_obj.insert_or_assign(std::string(_member), aJSON(_v)); }
}

void ak::aJSON::addMember(const std::string & _member, bool _v) {
	if (m_type == Object) { m_obj.insert_or_assign(_member, aJSON(_v)); }
}

void ak::aJSON::addMember(const std::string & _member, int _v) {
	if (m_type == Object) { m_obj.insert_or_assign(_member, aJSON(_v)); }
}

void ak::aJSON::addMember(const std::string & _member, double _v) {
	if (m_type == Object) { m_obj.insert_or_assign(_member, aJSON(_v)); }
}

void ak::aJSON::addMember(const std::string & _member, char _v) {
	if (m_type == Object) { m_obj.insert_or_assign(_member, aJSON(_v)); }
}

void ak::aJSON::addMember(const std::string & _member, const char * _v) {
	if (m_type == Object) { m_obj.insert_or_assign(_member, aJSON(_v)); }
}

void ak::aJSON::addMember(const std::string & _member, const std::string & _v) {
	if (m_type == Object) { m_obj.insert_or_assign(_member, aJSON(_v)); }
}


void ak::aJSON::addMember(const std::string & _member, const aJSON & _v) {
	if (m_type == Object) { m_obj.insert_or_assign(_member, aJSON(_v)); }
}

// ########################################################################################

// Getter

std::string ak::aJSON::toMultilineString(void) const {
	return toMultilineString("");
}

std::string ak::aJSON::toString(void) const {
	switch (m_type)
	{
	case ak::aJSON::Array:
	{
		std::string ret{ "[" };
		bool first{ true };
		for (auto itm : m_array) {
			if (first) {
				first = false;
			}
			else {
				ret.append(", ");
			}
			ret.append(itm.toString());
		}
		ret.append("]");
		return ret;
	}
		break;
	case ak::aJSON::Boolean:
		if (m_bool) { return "true"; }
		else { return "false"; }
	case ak::aJSON::Double: return std::to_string(m_double);
	case ak::aJSON::Integer: return std::to_string(m_int);
	case ak::aJSON::Null: return "null";
	case ak::aJSON::Object:
	{
		std::string ret{ "{" };
		bool first{ true };
		for (auto itm : m_obj) {
			if (first) {
				ret.append("\"");
				first = false;
			}
			else {
				ret.append(", \"");
			}
			ret.append(itm.first).append("\": ").append(itm.second.toString());
		}
		ret.append("}");
		return ret;
	}
		break;
	case ak::aJSON::String: return "\"" + toValidStringItem(m_string) + "\"";
	default:
		assert(0);
	}
	return "";
}

bool ak::aJSON::hasMember(const std::string & _key) const {
	if (m_type != Object) { return false; }
	auto it = m_obj.find(_key);
	return it != m_obj.end();
}

ak::aJSON ak::aJSON::getMember(const std::string & _key) const {
	if (m_type != Object) { return aJSON(Null); }
	auto it = m_obj.find(_key);
	if (it != m_obj.end()) {
		return it->second;
	}
	return aJSON(Null);
}

ak::aJSON ak::aJSON::getAt(size_t _index) {
	if (m_type != Array || _index < 0 || _index >= m_array.size()) { return aJSON(Null); }
	return m_array[_index];
}

size_t ak::aJSON::size(void) const {
	if (m_type == Array) { return m_array.size(); }
	else if (m_type == Object) { return m_obj.size(); }
	else { return 0; }
}

// ########################################################################################

// Private functions

std::string ak::aJSON::toMultilineString(const std::string & _prefix) const {

	switch (m_type)
	{
	case ak::aJSON::Array:
	{
		std::string ret{ "[" };
		bool first{ true };

		for (auto it : m_array) {
			if (first) {
				ret.append("\n"); first = false;
			}
			else {
				ret.append(",\n");
			}
			ret.append(_prefix).append("\t");
			ret.append(it.toMultilineString(_prefix + "\t"));
		}
		ret.append("\n").append(_prefix).append("]");
		return ret;
	}
	case ak::aJSON::Boolean:
		if (m_bool) { return "true"; }
		else { return "false"; }
	case ak::aJSON::Double: return std::to_string(m_double);
	case ak::aJSON::Integer: return std::to_string(m_int);
	case ak::aJSON::Object:
	{
		std::string ret{ "{" };
		bool first{ true };

		for (auto it : m_obj) {
			if (first) {
				ret.append("\n"); first = false;
			}
			else {
				ret.append(",\n");
			}
			ret.append(_prefix).append("\t\"");
			ret.append(it.first).append("\": ");
			ret.append(it.second.toMultilineString(_prefix + "\t"));
		}
		ret.append("\n").append(_prefix).append("}");
		return ret;
	}
	case ak::aJSON::String: return "\"" + toValidStringItem(m_string) + "\"";
	case ak::aJSON::Null:
	default: return "null";
	}
}

std::string ak::aJSON::toValidStringItem(const std::string & _orig) const{
	QString str{ _orig.c_str() };
	QString ret;
	QStringList splitted = str.split('\\', QString::KeepEmptyParts);
	bool first{ true };
	for (auto s : splitted) {
		if (first) {
			first = false;
		}
		else {
			ret.append("\\\\");
		}
		ret.append(s);
	}
	
	str.clear();
	splitted = ret.split('\t', QString::KeepEmptyParts);
	first = true;
	for (auto s : splitted) {
		if (first) {
			first = false;
		}
		else {
			str.append("\\t");
		}
		str.append(s);
	}

	ret.clear();
	splitted = str.split('\n', QString::KeepEmptyParts);
	first = true;
	for (auto s : splitted) {
		if (first) {
			first = false;
		}
		else {
			ret.append("\\n");
		}
		ret.append(s);
	}

	str.clear();
	splitted = ret.split('\r', QString::KeepEmptyParts);
	first = true;
	for (auto s : splitted) {
		if (first) {
			first = false;
		}
		else {
			str.append("\\r");
		}
		str.append(s);
	}

	ret.clear();
	splitted = str.split('\"', QString::KeepEmptyParts);
	first = true;
	for (auto s : splitted) {
		if (first) {
			first = false;
		}
		else {
			ret.append("\\\"");
		}
		ret.append(s);
	}

	return ret.toStdString();
}