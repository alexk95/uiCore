/*
 * ak_core.h
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <vector>					// vector<T>
#include <list>						// list<T>
#include <string>
#include <sstream>

// Qt header
#include <qstring.h>				// QString

// AK header
#include <ak_globalDataTypes.h>		// UICORE_API

namespace ak {

	namespace core {

		enum valueType {
			vArray,
			vBool,
			vColor,
			vDouble,
			vFloat,
			vInt,
			vInt64,
			vLosslessDouble,
			vLosslessFloat,
			vNull,
			vNumber,
			vObject,
			vSelection,
			vString,
			vUint,
			vUint64
		};

		//! Describes the type of an event
		enum eventType {
			eUnknownEvent					= 1 << 0,
			eActivated						= 1 << 1,
			eChanged						= 1 << 2,
			eCleared						= 1 << 3,
			eClicked						= 1 << 4,
			eClosing						= 1 << 5,
			eCollpased						= 1 << 6,
			eCursorPosotionChanged			= 1 << 7,
			eDestroyed						= 1 << 8,
			eDoubleClicked					= 1 << 9,
			eExpanded						= 1 << 10,
			eFocused						= 1 << 11,
			eFocusLeft						= 1 << 12,
			eInvalidEntry					= 1 << 13,
			eIndexChanged					= 1 << 14,
			eKeyPressed						= 1	 << 15,
			eKeyReleased					= 1 << 16,
			eLocationChanged				= 1 << 17,
			eReleased						= 1 << 18,
			eSelectionChanged				= 1 << 19,
			eStateChanged					= 1 << 20,
			eTabToolbarChanged				= 1 << 21,
			eTabToolbarClicked				= 1 << 22,
			eTimeout						= 1 << 23,
			eToggeledChecked				= 1 << 24,
			eToggeledUnchecked				= 1 << 25,
			eContextMenuItemClicked			= 1 << 26,
			eContextMenuItemCheckedChanged	= 1 << 27,
			eVisibilityChanged				= 1 << 28
		};

		inline eventType operator | (eventType a, eventType b)
		{
			return static_cast<eventType>(static_cast<int>(a) | static_cast<int>(b));
		}

		//! Describes the type of an command
		enum commandType {
			cClearCreatedObjects
		};

		//! @brief Create a string representation of the provided ak::core::eventType
		//! @param _eventType The event type to be converted
		QString toQString(
			eventType								_eventType
		);

		//! @brief Create a string representation of the provided ak::core::valueType
		//! @param _valueType The value type to be converted
		QString toQString(
			valueType								_valueType
		);

		// ############################################################################################################################################
		// Graphics

		namespace graphics {

			template <class T, class U> class UICORE_API point2d {
			public:
				//! @brief Constructor
				//! @param _x The initial X value
				//! @param _y The initial Y value
				point2d(T _x, U _y);

				//! @brief Copy constructor
				//! @param _other The other 2D point
				point2d(const point2d & _other);

				point2d<T, U> & operator = (const point2d<T, U> & _other);
				point2d<T, U> operator + (const point2d<T, U> & _other);
				point2d<T, U> & operator += (const point2d<T, U> & _other);
				bool operator == (const point2d<T, U> & _other);
				bool operator != (const point2d<T, U> & _other);

				//! @brief Returns the current X value
				T x(void) const;

				//! @brief Returns the current Y value
				U y(void) const;

				//! @brief Will set the X value
				//! @param _x The X value to set
				void setX(T _x);

				//! @brief Will set the Y value
				//! @param _y The Y value to set
				void setY(U _y);

				//! @brief Will add the provided value to the X value and return the new X value
				//! @param _v The value to add
				T addX(T _v);

				//! @brief Will add the provided value to the Y value and return the new Y value
				//! @param _v The value to add
				U addY(U _v);

				//! @brief Will subtract the provided value from the X value and return the new X value
				//! @param _v The value to subtract
				T subX(T _v);

				//! @brief Will subtract the provided value from the Y value and return the new Y value
				//! @param _v The value to subtract
				U subY(U _v);

			private:
				// Block default constructor
				point2d() = delete;

				T		my_x;			//! The current X value
				U		my_y;			//! The current Y value

			};

			template <class T, class U, class V> class UICORE_API point3d {
			public:
				//! @brief Constructor
				//! @param _x The initial X value
				//! @param _y The initial Y value
				//! @param _z The initial Z value
				point3d(T _x, U _y, V _z);

				//! @brief Copy constructor
				//! @param _other The other 2D point
				point3d(const point3d & _other);

				point3d<T, U, V> & operator = (const point3d<T, U, V> & _other);
				point3d<T, U, V> operator + (const point3d<T, U, V> & _other);
				point3d<T, U, V> & operator += (const point3d<T, U, V> & _other);
				bool operator == (const point3d<T, U, V> & _other);
				bool operator != (const point3d<T, U, V> & _other);

				//! @brief Returns the current X value
				T x(void) const;

				//! @brief Returns the current Y value
				U y(void) const;

				//! @brief Returns the current Z value
				V z(void) const;

				//! @brief Will set the X value
				//! @param _x The X value to set
				void setX(T _x);

				//! @brief Will set the Y value
				//! @param _y The Y value to set
				void setY(U _y);

				//! @brief Will set the Z value
				//! @param _z The Z value to set
				void setZ(V _z);

				//! @brief Will add the provided value to the X value and return the new X value
				//! @param _v The value to add
				T addX(T _v);

				//! @brief Will add the provided value to the Y value and return the new Y value
				//! @param _v The value to add
				U addY(U _v);

				//! @brief Will add the provided value to the Z value and return the new Z value
				//! @param _v The value to add
				V addZ(V _z);

				//! @brief Will subtract the provided value from the X value and return the new X value
				//! @param _v The value to subtract
				T subX(T _v);

				//! @brief Will subtract the provided value from the Y value and return the new Y value
				//! @param _v The value to subtract
				U subY(U _v);

				//! @brief Will subtract the provided value from the Z value and return the new Z value
				//! @param _v The value to subtract
				V subZ(V _v);

			private:
				// Block default constructor
				point3d() = delete;

				T		my_x;			//! The current X value
				U		my_y;			//! The current Y value
				V		my_z;			//! The current Z value

			}; // class point3d


		} // namespace graphics

		//! Contains number related functions
		namespace numbers {

			// ############################################################################################################################################
			// Check type

			template <class T> T validateNumber(std::string _string, bool & _failed) {
				std::stringstream ss(_string);
				T v;
				ss >> v;
				_failed = false;
				if (ss.fail()) { _failed = true; }
				std::string rest;
				ss >> rest;
				if (!rest.empty()) { _failed = true; }
				return v;
			}

			//! @brief Returns true if the provided C-String is in a format ready to be converted to a double or float
			//! @param str The C-String to be checked
			bool isDecimal(const char *str);

			//! @brief Returns true if the provided C++ String is in a format ready to be converted to a double or float
			//! @param str The C++ String to be checked
			bool isDecimal(const std::string &str);

			//! @brief Returns true if the provided Qt String is in a format ready to be converted to a double or float
			//! @param str The Qt String to be checked
			bool isDecimal(const QString &str);

			//! @brief Returns true if the provided C-String is in a format ready to be converted to an integer
			//! @param str The C-String to be checked
			bool isInteger(const char *str);

			//! @brief Returns true if the provided C++ String is in a format ready to be converted to an integer
			//! @param str The C++ String to be checked
			bool isInteger(const std::string &str);

			//! @brief Returns true if the provided Qt String is in a format ready to be converted to an integer
			//! @param str The Qt String to be checked
			bool isInteger(const QString &str);

			//! @brief Returns true if the provided C-String consists only of numerical characters
			//! @param str The C-String to be checked
			bool isNumericOnly(const char *str);

			//! @brief Returns true if the provided C++ String consists only of numerical characters
			//! @param str The C++ String to be checked
			bool isNumericOnly(const std::string &str);

			//! @brief Returns true if the provided Qt String consists only of numerical characters
			//! @param str The Qt String to be checked
			bool isNumericOnly(const QString &str);

		} // namespace numbers

		namespace str {
#if (OS_WINDOWS)
			const char charNewLine = '\n';
#elif (OS_UNIX)
			const char charNewLine = '\n';
#endif
			std::vector<int> toCharIndex(
				const char *					str
			);
		}

	} // namespace core

	template <class T> UICORE_API std::vector<T> toVector(const std::list<T> & _list) {
		std::vector<T> ret;
		ret.reserve(_list.size());
		for (auto itm : _list) { ret.push_back(itm); }
		return ret;
	}

} // namespace ak

// ############################################################################################################################################

// ############################################################################################################################################

// point 2d

template <class T, class U> ak::core::graphics::point2d<T, U>::point2d(T _x, U _y) { my_x = _x; my_y = _y; }

template <class T, class U> ak::core::graphics::point2d<T, U>::point2d(const point2d & _other) { my_x = _other.x(); my_y = _other.y(); }

template <class T, class U> ak::core::graphics::point2d<T, U> & ak::core::graphics::point2d<T, U>::operator = (const point2d<T, U> & _other) {
	my_x = _other.x(); my_y = _other.y();
	return *this;
}
template <class T, class U> ak::core::graphics::point2d<T, U> ak::core::graphics::point2d<T, U>::operator + (const point2d<T, U> & _other) {
	point2d<T, U> ret(my_x, my_y);
	ret.addX(_other.x());
	ret.addY(_other.y());
	return ret;

}
template <class T, class U> ak::core::graphics::point2d<T, U> & ak::core::graphics::point2d<T, U>::operator += (const point2d<T, U> & _other) {
	my_x = my_x + _other.x();
	my_y = my_y + _other.y();
	my_z = my_z + _other.z();
	return *this;
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

// ############################################################################################################################################

// point 3d

template <class T, class U, class V> ak::core::graphics::point3d<T, U, V>::point3d(T _x, U _y, V _z) { my_x = _x; my_y = _y; my_z = _z; }

template <class T, class U, class V> ak::core::graphics::point3d<T, U, V>::point3d(const point3d & _other) { my_x = _other.x(); my_y = _other.y(); my_z = _other.z(); }

template <class T, class U, class V> ak::core::graphics::point3d<T, U, V> & ak::core::graphics::point3d<T, U, V>::operator = (const point3d<T, U, V> & _other) {
	my_x = _other.x(); my_y = _other.y(); my_z = _other.z();
	return *this;
}
template <class T, class U, class V> ak::core::graphics::point3d<T, U, V> ak::core::graphics::point3d<T, U, V>::operator + (const point3d<T, U, V> & _other) {
	point3d<T, U, V> ret(my_x, my_y, my_z);
	ret.addX(_other.x());
	ret.addY(_other.y());
	ret.addZ(_other.z());
	return ret;

}
template <class T, class U, class V>  ak::core::graphics::point3d<T, U, V> & ak::core::graphics::point3d<T, U, V>::operator += (const point3d<T, U, V> & _other) {
	my_x = my_x + _other.x();
	my_y = my_y + _other.y();
	my_z = my_z + _other.z();
	return *this;
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