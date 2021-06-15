/*
 *	File:		aFlags.h
 *	Package:	akCore
 *
 *  Created on: February 06, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

namespace ak {

	//! This class is used to manage a "bitfield"
	//! The type should be an enum where every value represents a single bit
	//! Like:
	//! enum enumName {
	//!		enumValue1	= 0x01,	//	0001
	//!		enumValue2	= 0x02,	//	0010
	//!		enumValue3	= 0x04,	//	0100
	//!			...
	//!	}
	template<class T> class aFlags {
	public:

		//! @brief Constructor
		aFlags() : my_field{ static_cast<T>(0) } {}

		//! @brief Copy constructor
		//! @param _other The other "bitfield"
		aFlags(const aFlags<T> & _other) : my_field{ _other.field() } {}

		//! @brief Constructor
		//! @param _initialState The initial state of the bitfield
		aFlags(T _initialState) : my_field{ _initialState } {}

		//! @brief Constructor
		//! @param _initialStateValue A value that can be casted to the bitfield
		aFlags(long long _initialStateValue) : my_field{ static_cast<T>(_initialStateValue) } {}

		//! @brief Deconstructor
		virtual ~aFlags() {}

		//! @brief Will return a copy of the bitfield
		T field(void) const { return my_field; }

		//! @brief Will return a value representing the current bitfield
		long long castField(void) const { return static_cast<long long>(my_field); }

		//! @brief Will set the provided flag
		void setFlag(T _flag) { my_field = static_cast<T>(static_cast<long long>(my_field) | static_cast<long long>(_flag)); }

		//! @brief Will set the provided flag
		void setFlag(T _flag, bool _flagIsSet) {
			if (_flagIsSet) { my_field = static_cast<T>(static_cast<long long>(my_field) | static_cast<long long>(_flag)); }
			else { my_field = static_cast<T>(static_cast<long long>(my_field) & (!static_cast<long long>(_flag))); }
		}

		//! @brief Will remove the provided flag
		void removeFlag(T _flag) { my_field = static_cast<T>(static_cast<long long>(my_field) & (!static_cast<long long>(_flag))); }

		//! @brief Will check if the provided flag is set
		bool flagIsSet(T _flag) const { return (static_cast<long long>(my_field) & static_cast<long long>(_flag)) == static_cast<long long>(_flag); }

		//! @brief Will copy the other bitfield
		aFlags<T> & operator = (const aFlags<T> & _other) { my_field = _other.field(); return this; }

		//! @brief Will take the other bitfield and perform a bitwise OR, will return the result
		aFlags<T> operator + (const aFlags<T> & _other) {
			flags<T> newField{ static_cast<T>(static_cast<long long>(_other.field()) | static_cast<long long>(my_field)) };
			return newField;
		}

		//! @brief Will remove all set bits set at the other bitfield from this bitfield
		aFlags<T> operator - (const aFlags<T> & _other) {
			flags<T> newField{ static_cast<T>(static_cast<long long>(_other.field()) & !static_cast<long long>(my_field)) };
			return newField;
		}

		//! @brief Will take the other bitfield and perform a bitwise OR, will return the result
		aFlags<T> operator += (const aFlags<T> & _other) {
			my_field = static_cast<T>(static_cast<long long>(_other.field()) | static_cast<long long>(my_field));
			return this;
		}

		//! @brief Will remove all set bits set at the other bitfield from this bitfield
		aFlags<T> & operator -= (const aFlags<T> & _other) {
			my_field = static_cast<T>(static_cast<long long>(_other.field()) & !static_cast<long long>(my_field));
			return this;
		}

		//! @brief Will return true if both fields are equal
		bool operator == (const aFlags<T> & _other) const { return static_cast<long long>(my_field) == static_cast<long long>(_other.field()); }

		//! @brief Will return true if both fields are not equal
		bool operator != (const aFlags<T> & _other) const { return static_cast<long long>(my_field) != static_cast<long long>(_other.field()); }

	private:
		T		my_field;
	};

}