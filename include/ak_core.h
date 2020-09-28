/*
 * ak_core.h
 *
 *  Created on: February 06, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

#if  defined(_WIN32) || defined (_WIN64)
#define OS_WINDOWS 1
#elif defined(__unix__) || defined (unix) || defined(__unix)
#define OS_UNIX 1
#endif

// C++ header
#include <vector>
#include <list>

// Qt header
#include <qstring.h>

namespace ak {

	namespace core {

		//! Describes the type of a message
		enum messageType {
			mCommand,
			mError,
			mEvent,
			mNotification,
			mText,
			mUnknownMessageType
		};
		
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
			eUnknownEvent				= 1 << 0,
			eActivated					= 1 << 1,
			eChanged					= 1 << 2,
			eCleared					= 1 << 3,
			eClicked					= 1 << 4,
			eClosing					= 1 << 5,
			eCollpased					= 1 << 6,
			eCursorPosotionChanged		= 1 << 7,
			eDestroyed					= 1 << 8,
			eDoubleClicked				= 1 << 9,
			eExpanded					= 1 << 10,
			eFocused					= 1 << 11,
			eFocusLeft					= 1 << 12,
			eInvalidEntry				= 1 << 13,
			eIndexChanged				= 1 << 14,
			eKeyPressed					= 1 << 15,
			eKeyReleased				= 1 << 16,
			eLocationChanged			= 1 << 17,
			eReleased					= 1 << 18,
			eSelectionChanged			= 1 << 19,
			eStateChanged				= 1 << 20,
			eTimeout					= 1 << 21,
			eToggeledChecked			= 1 << 22,
			eToggeledUnchecked			= 1 << 23
		};

		inline eventType operator | (eventType a, eventType b)
		{
			return static_cast<eventType>(static_cast<int>(a) | static_cast<int>(b));
		}

		//! Describes the type of an command
		enum commandType {
			cClearCreatedObjects
		};

		//! @brief Create a string representation of the provided ak::core::messageType
		//! @param _messageType The message type to be converted
		QString toQString(
			messageType								_messageType
		);

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

		// ########################################################################################
		// Graphics

		namespace graphics {

			template <class T, class U> class point2d {
			public:
				//! @brief Constructor
				//! @param _x The initial X value
				//! @param _y The initial Y value
				point2d(T _x, U _y);

				//! @brief Copy constructor
				//! @param _other The other 2D point
				point2d(const point2d & _other);

				point2d<T,U> & operator = (const point2d<T,U> & _other);
				point2d<T,U> & operator + (const point2d<T,U> & _other);
				bool operator == (const point2d<T,U> & _other);
				bool operator != (const point2d<T,U> & _other);

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
				point2d() {}

				T		my_x;			//! The current X value
				U		my_y;			//! The current Y value

			};

			template <class T, class U, class V> class point3d {
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
				point3d<T, U, V> & operator + (const point3d<T, U, V> & _other);
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
				point3d() {}

				T		my_x;			//! The current X value
				U		my_y;			//! The current Y value
				V		my_z;			//! The current Z value

			}; // class point3d


		} // namespace graphics

		//! Contains number related functions
		namespace numbers {

			// ####################################################################################################
			// Check type

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

	template <class T> std::vector<T> toVector(const std::list<T> & _list) {
		std::vector<T> ret;
		ret.reserve(_list.size());
		for (auto itm : _list) { ret.push_back(itm); }
		return ret;
	}

} // namespace ak