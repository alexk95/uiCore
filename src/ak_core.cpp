/*
 * ak_core.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_core.h"		// corresponding class

QString ak::core::toQString(
	messageType				_messageType
) {
	switch (_messageType)
	{
	case ak::core::mError: return QString("Error");
	case ak::core::mEvent: return QString("Event");
	case ak::core::mNotification: return QString("Notification");
	case ak::core::mText: return QString("Text");
	case ak::core::mUnknownMessageType:  return QString("UnknownMessageType");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::core::toQString(
	eventType				_eventType
) {
	switch (_eventType)
	{
	case ak::core::eUnknownEvent: return QString("UnknownEvent");
	case ak::core::eActivated: return QString("Activated");
	case ak::core::eChanged: return QString("Changed");
	case ak::core::eCleared: return QString("Cleared");
	case ak::core::eClicked: return QString("Clicked");
	case ak::core::eClosing: return QString("Closing");
	case ak::core::eCollpased: return QString("Collapsed");
	case ak::core::eCursorPosotionChanged: return QString("CursorPositionChanged");
	case ak::core::eDestroyed: return QString("Destroyed");
	case ak::core::eDoubleClicked: return QString("DoubleClicked");
	case ak::core::eExpanded: return QString("Expanded");
	case ak::core::eFocused: return QString("Focused");
	case ak::core::eFocusLeft: return QString("FocusLeft");
	case ak::core::eIndexChanged: return QString("IndexChanged");
	case ak::core::eInvalidEntry: return QString("InvalidEntry");
	case ak::core::eKeyPressed: return QString("KeyPressed");
	case ak::core::eKeyReleased: return QString("KeyReleased");
	case ak::core::eLocationChanged: return QString("LocationChanged");
	case ak::core::eReleased: return QString("Released");
	case ak::core::eSelectionChanged: return QString("SelectionChanged");
	case ak::core::eStateChanged: return QString("StateChanged");
	case ak::core::eTimeout: return QString("Timeout");
	case ak::core::eToggeledChecked: return QString("ToggeledToChecked");
	case ak::core::eToggeledUnchecked: return QString("ToggeledToUnchecked");
	default: assert(0); // not implemented yet
	}
	return QString("");
}

QString ak::core::toQString(
	valueType				_valueType
) {
	switch (_valueType)
	{
	case ak::core::valueType::vArray: return QString("Array"); break;
	case ak::core::valueType::vBool: return QString("Bool"); break;
	case ak::core::valueType::vColor: return QString("Color"); break;
	case ak::core::valueType::vDouble: return QString("Double"); break;
	case ak::core::valueType::vFloat: return QString("Float"); break;
	case ak::core::valueType::vInt: return QString("Integer"); break;
	case ak::core::valueType::vInt64: return QString("64bitInteger"); break;
	case ak::core::valueType::vLosslessDouble: return QString("LosslessDouble"); break;
	case ak::core::valueType::vLosslessFloat: return QString("LosslessFloat"); break;
	case ak::core::valueType::vNull: return QString("Null"); break;
	case ak::core::valueType::vNumber: return QString("Number"); break;
	case ak::core::valueType::vObject: return QString("Object"); break;
	case ak::core::valueType::vSelection: return QString("Selection"); break;
	case ak::core::valueType::vString: return QString("String"); break;
	case ak::core::valueType::vUint: return QString("UnsignedInteger"); break;
	case ak::core::valueType::vUint64: return QString("Unsigned64bitInteger"); break;
	default: assert(0); // not implemented yet
	}
	return QString("");
}

bool ak::core::numbers::isDecimal(const char *str) {
	if (str != nullptr) {
		int i = 0;
		bool gotSign = false;
		bool gotDecimal = false;
		for (; str[i] != 0; i++) {
			if (str[i] == '-' || str[i] == '+') {
				// sign
				if (i != 0) { return false; }
				gotSign = true;
			}
			else if (str[i] == '.') {
				if (gotDecimal) { return false; }
				if (gotSign && i == 1) { return false; }
				if (i == 0) { return false; }
				gotDecimal = true;
			}
			else if (str[i] < '0' || str[i] > '9') { return false; }
		}
		if (gotSign && i == 1) { return false; }
	}
	return true;
}

bool ak::core::numbers::isDecimal(const std::string &str) {
	return isDecimal(str.c_str());
}

bool ak::core::numbers::isDecimal(const QString &str) {
	return isDecimal(str.toStdString());
}

bool ak::core::numbers::isInteger(const char *str) {
	if (str != nullptr) {
		bool gotSign = false;
		int i = 0;
		for (; str[i] != 0; i++) {
			if (str[i] == '+' || str[i] == '-') {
				if (i != 0) { return false; }
				gotSign = true;
			}
			else if (str[i] < '0' || str[i] > '9') { return false; }
		}
		if (gotSign && i == 1) { return false; }
	}
	return true;
}

bool ak::core::numbers::isInteger(const std::string &str) {
	return isInteger(str.c_str());
}

bool ak::core::numbers::isInteger(const QString &str) {
	return isInteger(str.toStdString());
}

bool ak::core::numbers::isNumericOnly(const char *str) {
	if (str != nullptr) {
		for (int i = 0; str[i] != 0; i++) {
			if (str[i] < '0' || str[i] > '9') { return false; }
		}
	}
	return true;
}

bool ak::core::numbers::isNumericOnly(const std::string &str) {
	return isNumericOnly(str.c_str());
}

bool ak::core::numbers::isNumericOnly(const QString &str) {
	return isNumericOnly(str.toStdString());
}

// ########################################################################################
// Graphics

// point 2d

template <class T, class U> ak::core::graphics::point2d<T, U>::point2d(T _x, U _y) { my_x = _x; my_y = _y; }

template <class T, class U> ak::core::graphics::point2d<T, U>::point2d(const point2d & _other) { my_x = _other.x(); my_y = _other.y(); }

template <class T, class U> ak::core::graphics::point2d<T, U> & ak::core::graphics::point2d<T,U>::operator = (const point2d<T, U> & _other) {
	my_x = _other.x(); my_y = _other.y();
	return *this;
}
template <class T, class U> ak::core::graphics::point2d<T, U> & ak::core::graphics::point2d<T,U>::operator + (const point2d<T, U> & _other) {
	point2d<T,U> ret(my_x, my_y);
	ret.addX(_other.x());
	ret.addY(_other.y());
	return ret;
	
}
template <class T, class U> bool ak::core::graphics::point2d<T, U>::operator == (const point2d & _other) {
	if (my_x != _other.x()) { return false; }
	if (my_y != _other.y()) { return false; }
	return true;
}
template <class T, class U> bool ak::core::graphics::point2d<T, U>::operator != (const point2d & _other) { return !(*this == _other); }

template <class T, class U> T ak::core::graphics::point2d<T, U>::x(void) const { return my_x; }

template <class T, class U> U ak::core::graphics::point2d<T, U>::y(void) const { return my_y; }

template <class T, class U> void ak::core::graphics::point2d<T, U>::setX(T _x) { my_x = _x; }

template <class T, class U> void ak::core::graphics::point2d<T, U>::setY(U _y) { my_y = _y; }

template <class T, class U> T ak::core::graphics::point2d<T, U>::addX(T _v) { my_x = my_x + v; return my_x; }

template <class T, class U> U ak::core::graphics::point2d<T, U>::addY(U _v) { my_y = my_y + v; return my_y; }

template <class T, class U> T ak::core::graphics::point2d<T, U>::subX(T _v) { my_x = my_x - _v; return my_x; }

template <class T, class U> U ak::core::graphics::point2d<T, U>::subY(U _v) { my_y = my_y - _v; return my_y; }

// point 3d

template <class T, class U, class V> ak::core::graphics::point3d<T, U, V>::point3d(T _x, U _y, V _z) { my_x = _x; my_y = _y; my_z = _z; }

template <class T, class U, class V> ak::core::graphics::point3d<T, U, V>::point3d(const point3d & _other) { my_x = _other.x(); my_y = _other.y(); my_z = _other.z(); }

template <class T, class U, class V> ak::core::graphics::point3d<T, U, V> & ak::core::graphics::point3d<T, U, V>::operator = (const point3d<T, U, V> & _other) {
	my_x = _other.x(); my_y = _other.y(); my_z = _other.z();
	return *this;
}
template <class T, class U, class V> ak::core::graphics::point3d<T, U, V> & ak::core::graphics::point3d<T, U, V>::operator + (const point3d<T, U, V> & _other) {
	point3d<T, U, V> ret(my_x, my_y, my_z);
	ret.addX(_other.x());
	ret.addY(_other.y());
	ret.addZ(_other.z());
	return ret;

}
template <class T, class U, class V> bool ak::core::graphics::point3d<T, U, V>::operator == (const point3d & _other) {
	if (my_x != _other.x()) { return false; }
	if (my_y != _other.y()) { return false; }
	if (my_z != _other.z()) { return false; }
	return true;
}
template <class T, class U, class V> bool ak::core::graphics::point3d<T, U, V>::operator != (const point3d & _other) { return !(*this == _other); }

template <class T, class U, class V> T ak::core::graphics::point3d<T, U, V>::x(void) const { return my_x; }

template <class T, class U, class V> U ak::core::graphics::point3d<T, U, V>::y(void) const { return my_y; }

template <class T, class U, class V> V ak::core::graphics::point3d<T, U, V>::z(void) const { return my_z; }

template <class T, class U, class V> void ak::core::graphics::point3d<T, U, V>::setX(T _x) { my_x = _x; }

template <class T, class U, class V> void ak::core::graphics::point3d<T, U, V>::setY(U _y) { my_y = _y; }

template <class T, class U, class V> void ak::core::graphics::point3d<T, U, V>::setZ(V _z) { my_z = _z; }

template <class T, class U, class V> T ak::core::graphics::point3d<T, U, V>::addX(T _v) { my_x = my_x + v; return my_x; }

template <class T, class U, class V> U ak::core::graphics::point3d<T, U, V>::addY(U _v) { my_y = my_y + v; return my_y; }

template <class T, class U, class V> V ak::core::graphics::point3d<T, U, V>::addZ(V _v) { my_z = my_z + v; return my_z; }

template <class T, class U, class V> T ak::core::graphics::point3d<T, U, V>::subX(T _v) { my_x = my_x - _v; return my_x; }

template <class T, class U, class V> U ak::core::graphics::point3d<T, U, V>::subY(U _v) { my_y = my_y - _v; return my_y; }

template <class T, class U, class V> V ak::core::graphics::point3d<T, U, V>::subZ(V _z) { my_z = my_z - _v; return my_z; }

// ########################################################################################
// String

std::vector<int> ak::core::str::toCharIndex(
	const char *					str
) {
	std::vector<int> out;
	if (str != nullptr) {
		while (*str != 0) { out.push_back((int)*str); str++; }
	}
	out.push_back(0);
	return out;
}