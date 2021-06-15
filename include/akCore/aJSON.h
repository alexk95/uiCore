/*
 *	File:		aJSON.h
 *	Package:	akCore
 *
 *  Created on: June 15, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <string>
#include <map>
#include <vector>

#include <akCore/globalDataTypes.h>

namespace ak {

	class UICORE_API_EXPORT aJSON {
	public:

		enum jsonType {
			Array,
			Boolean,
			Double,
			Integer,
			Null,
			Object,
			String
		};

		//! @brief Will create a empty item with the specified type
		//! The type can not be changed
		aJSON(jsonType _type);

		//! @brief Will create an item with the type Boolean
		//! @param _v The value to set
		aJSON(bool _v);

		//! @brief Will create an item with the type Double
		//! @param _v The value to set
		aJSON(double _v);

		//! @brief Will create an item with the type Integer
		//! @param _v The value to set
		aJSON(int _v);

		//! @brief Will create an item with the type String
		//! @param _v The value to set
		aJSON(char _v);

		//! @brief Will create an item with the type String
		//! @param _v The value to set
		aJSON(const char * _v);

		//! @brief Will create an item with the type String
		//! @param _v The value to set
		aJSON(const std::string & _v);

		//! @brief Will create a copy of the other json item
		//! @param _other The other item
		aJSON(const aJSON & _other);

		virtual ~aJSON();

		// #############################################################

		// Setter

		//! @brief Will set the Boolean value
		//! This function will have no effect if the item type is not Boolean
		//! @param _v The value to set
		void setBool(bool _v) { m_bool = _v; }

		//! @brief Will set the Integer value
		//! This function will have no effect if the item type is not Integer
		//! @param _v The value to set
		void setInt(int _v) { m_int = _v; }

		//! @brief Will set the Double value
		//! This function will have no effect if the item type is not Double
		//! @param _v The value to set
		void setDouble(double _v) { m_double = _v; }

		//! @brief Will set the String value
		//! This function will have no effect if the item type is not String
		//! @param _v The value to set
		void setString(char _v);

		//! @brief Will set the String value
		//! This function will have no effect if the item type is not String
		//! @param _v The value to set
		void setString(const char * _v) { m_string = _v; }

		//! @brief Will set the String value
		//! This function will have no effect if the item type is not String
		//! @param _v The value to set
		void setString(const std::string & _v) { m_string = _v; }

		//! @brief Will reserve memory for the array
		//! This function will have no effect if the item type is not Array
		//! @param _size The size to reserve
		void reserve(size_t _size);

		//! @brief Will add the provided value to the end of the array
		//! This function will have no effect if the item type is not Array
		//! @param _v The value to add
		void pushBack(bool _v);

		//! @brief Will add the provided value to the end of the array
		//! This function will have no effect if the item type is not Array
		//! @param _v The value to add
		void pushBack(int _v);

		//! @brief Will add the provided value to the end of the array
		//! This function will have no effect if the item type is not Array
		//! @param _v The value to add
		void pushBack(double _v);

		//! @brief Will add the provided value to the end of the array
		//! This function will have no effect if the item type is not Array
		//! @param _v The value to add
		void pushBack(char _v);

		//! @brief Will add the provided value to the end of the array
		//! This function will have no effect if the item type is not Array
		//! @param _v The value to add
		void pushBack(const char * _v);

		//! @brief Will add the provided value to the end of the array
		//! This function will have no effect if the item type is not Array
		//! @param _v The value to add
		void pushBack(const std::string & _v);

		//! @brief Will add the provided value to the end of the array
		//! This function will have no effect if the item type is not Array
		//! @param _v The value to add
		void pushBack(const aJSON & _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const char * _member, bool _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const char * _member, int _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const char * _member, double _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const char * _member, char _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const char * _member, const char * _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const char * _member, const std::string & _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const char * _member, const aJSON & _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const std::string & _member, bool _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const std::string & _member, int _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const std::string & _member, double _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const std::string & _member, char _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const std::string & _member, const char * _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const std::string & _member, const std::string & _v);

		//! @brief Will add the provided member to the object
		//! This function will have no effect if the item type is not Object
		//! The order in which the members are added may differ from the order at export
		//! @param _v The value to add
		void addMember(const std::string & _member, const aJSON & _v);

		// #############################################################

		// Getter

		//! @brief Will return the type of this json item
		inline jsonType type(void) const { return m_type; }

		//! @brief Will return a multiline JSON string representing this item
		std::string toMultilineString(void) const;

		//! @brief Will return the bool value
		//! This function may be used only for bool type item
		inline bool getBool(void) const { return m_bool; }

		//! @brief Will return the double value
		//! This function may be used only for double type item
		inline double getDouble(void) const { return m_double; }

		//! @brief Will return the integer value
		//! This function may be used only for integer type item
		inline int getInt(void) const { return m_int; }

		//! @brief Will return null
		//! This function may be used only for null type item
		inline void * getNull(void) const { return nullptr; }

		//! @brief Will return the string value
		//! This function may be used only for string type item
		inline std::string getString(void) { return m_string; }

		//! @brief Will return true if the object has a member with the specified key
		//! This function may be used only for object type item
		bool hasMember(const std::string & _key) const;

		//! @brief Will return the item with the specified member key
		//! This function may be used only for object type item
		aJSON getMember(const std::string & _key) const;

		//! @brief Will return the item at the specified index
		//! This function may be used only for array type item
		aJSON getAt(size_t _index);

		//! @brief Will return the ammount of members in the object or the array size
		//! This function has only an effect on an objct type or array type item, otherwise will return 0
		size_t size(void) const;
	private:

		std::string toMultilineString(const std::string & _prefix) const;

		std::string toValidStringItem(const std::string & _orig) const;

		jsonType				m_type;

		std::vector<aJSON>		m_array;
		bool					m_bool;
		double					m_double;
		int						m_int;
		std::map<std::string,
			aJSON>				m_obj;
		std::string				m_string;
	};

}