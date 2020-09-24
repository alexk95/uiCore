/*
 * ak_ui_widget_propertyGridItem.h
 *
 *  Created on: March 30, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <vector>

// Qt header
#include <qstring.h>

// AK header
#include "ak_core.h"			// value type
#include "ak_ui_color.h"		// color
#include "ak_globalDataTypes.h"	// UID and ID type

namespace ak {
	namespace ui {
		namespace widget {


			//! @brief Stores the information about an property grid item
			//! The get and set functions may only be called corresponding to the value type set on creation
			//! The allowed value types for a propertyGridObject are: Bool, Color, Double, Int, String, Selection
			class propertyGridItem {
			public:

				//! @brief Contructor
				//! @param _index The index this item is located at
				//! @param _propertyName The name of this property item
				//! @param _valueType The value type of this property (may not be changed later!)
				//! @param _itemsUid The UID of this property item
				//! @param _isMultivalued If true, this item will be assumed to be multivalued
				//! @throw ak::Exception if the provided value type is invalid
				propertyGridItem(
					ak::ID									_index,
					const QString &							_propertyName,
					ak::core::valueType						_valueType,
					ak::UID									_itemsUid = ak::invalidUID,
					bool									_isMultivalued = false
				);

				//! @brief Copy constructor
				//! @param _other The other property grid item
				propertyGridItem(
					const ak::ui::widget::propertyGridItem &		_other
				);

				//! @brief Assigment operator
				//! @param _other The other property grid item
				ak::ui::widget::propertyGridItem & operator = (const ak::ui::widget::propertyGridItem & _other);

				//! @brief Deconstructor
				virtual ~propertyGridItem();

				// #############################################################################################
				// Set values

				//! @brief Will set the current index of this item
				//! @param _index The items index
				void setIndex(
					ak::ID												_index
				);

				//! @brief Will set the UID of the properties widget
				void setWidgetUid(
					ak::UID												_widgetUid
				);

				//! @brief Will set the property name of this item
				//! @param _propertyName The property name to be set
				void setPropertyName(
					const QString &										_propertyName
				);

				//! @brief Will set the last text of this item
				//! @param _text The last text to be set
				void setLastText(
					const QString &										_text
				);

				//! @brief Will set weather this item is multivalued or not
				//! @pram _isMultivalued If true, this item will be assumed to be multivalued
				void setIsMultivalued(
					bool												_isMultivalued = true
				);

				//! @brief Will set the Boolean value of this object
				//! @param _value The value to be set
				//! @param _throwIfInvalidType If true, a Exception will be thrown if the item type of this object is not Boolean
				//! @throw ak::Exception if the item type is not Boolean and throwIfInvalidType was true
				void setBool(
					bool												_value,
					bool												_throwIfInvalidType = true
				);

				//! @brief Will set the Integer value of this object
				//! @param _value The value to be set
				//! @param _throwIfInvalidType If true, a Exception will be thrown if the item type of this object is not Integer
				//! @throw ak::Exception if the item type is not Integer and throwIfInvalidType was true
				void setInt(
					int													_value,
					bool												_throwIfInvalidType = true
				);

				//! @brief Will set the Double value of this object
				//! @param _value The value to be set
				//! @param _throwIfInvalidType If true, a Exception will be thrown if the item type of this object is not Double
				//! @throw ak::Exception if the item type is not Double and throwIfInvalidType was true
				void setDouble(
					double													_value,
					bool												_throwIfInvalidType = true
				);

				//! @brief Will set the String value of this object
				//! @param _value The value to be set
				//! @param _throwIfInvalidType If true, a Exception will be thrown if the item type of this object is not String
				//! @throw ak::Exception if the item type is not String and throwIfInvalidType was true
				void setString(
					const QString &										_value,
					bool												_throwIfInvalidType = true
				);

				//! @brief Will set the Color value of this object
				//! @param _value The value to be set
				//! @param _throwIfInvalidType If true, a Exception will be thrown if the item type of this object is not Color
				//! @throw ak::Exception if the item type is not Color and throwIfInvalidType was true
				void setColor(
					const ak::ui::color &								_value,
					bool												_throwIfInvalidType = true
				);

				//! @brief Will set the Selection (currently selected value) value of this object
				//! @param _value The value to be set
				//! @param _throwIfInvalidType If true, a Exception will be thrown if the item type of this object is not Selection
				//! @throw ak::Exception if the item type is not Selection and throwIfInvalidType was true
				void setSelection(
					const QString &										_value,
					bool												_throwIfInvalidType = true
				);

				//! @brief Will set the possoble selection value of this object
				//! @param _value The value to be set
				//! @param _throwIfInvalidType If true, a Exception will be thrown if the item type of this object is not Selection
				//! @throw ak::Exception if the item type is not Selection and throwIfInvalidType was true
				void setPossibleSelection(
					const std::vector<QString> &						_value,
					bool												_throwIfInvalidType = true
				);

				// #############################################################################################
				// Get

				//! @brief Returns the currently set index of this item
				ak::ID index(void) const;

				//! @brief Returns the UID of the items widget
				ak::UID getWidgetUid(void) const;

				//! @brief Returns the value type of this item
				ak::core::valueType valueType(void) const;

				//! @brief Returns the currently set property name of this item
				QString propertyName(void) const;

				//! @brief Returns the currently set last text of this item
				QString lastText(void) const;

				//! @brief Returns true if this item is currently set to is multivalued
				bool isMultivalued(void) const;

				//! @brief Returns the currently set Boolean value
				//! @param _throwIfInvalidType If true, an exception will be thrown if the item type is not Boolean
				//! @throw ak::Exception if the item type is not Boolean
				bool getBool(
					bool												_throwIfInvalidType = true
				) const;

				//! @brief Returns the currently set Integer value
				//! @param _throwIfInvalidType If true, an exception will be thrown if the item type is not Integer
				//! @throw ak::Exception if the item type is not Integer
				int getInt(
					bool												_throwIfInvalidType = true
				) const;

				//! @brief Returns the currently set Double value
				//! @param _throwIfInvalidType If true, an exception will be thrown if the item type is not Double
				//! @throw ak::Exception if the item type is not Double
				double getDouble(
					bool												_throwIfInvalidType = true
				) const;

				//! @brief Returns the currently set String value
				//! @param _throwIfInvalidType If true, an exception will be thrown if the item type is not String
				//! @throw ak::Exception if the item type is not String
				QString getString(
					bool												_throwIfInvalidType = true
				) const;

				//! @brief Returns the currently set Color value
				//! @param _throwIfInvalidType If true, an exception will be thrown if the item type is not Color
				//! @throw ak::Exception if the item type is not Color
				ak::ui::color getColor(
					bool												_throwIfInvalidType = true
				) const;

				//! @brief Returns the currently set Selection value
				//! @param _throwIfInvalidType If true, an exception will be thrown if the item type is not Selection
				//! @throw ak::Exception if the item type is not Selection
				QString getSelection(
					bool												_throwIfInvalidType = true
				) const;

				//! @brief Returns the currently set possibleSelection value
				//! @param _throwIfInvalidType If true, an exception will be thrown if the item type is not Selection
				//! @throw ak::Exception if the item type is not Selection
				std::vector<QString> getSelectionPossible(
					bool												_throwIfInvalidType = true
				) const;

			private:
				ak::core::valueType						my_valueType;			//! The value type of this object
				ak::ID									my_index;				//! The index of this object
				QString									my_propertyName;		//! The property name of this object
				QString									my_lastText;			//! The last text of this object
				bool									my_isMultivalued;		//! If true, this item is assumed to be multivalued
				ak::UID									my_widgetUid;

				bool									my_bool;				//! The Boolean value
				int										my_int;					//! The Integer value
				double									my_double;				//! The Double value
				QString									my_string;				//! The String value
				ak::ui::color							my_color;				//! The color value
				QString									my_selection;			//! The selection value
				std::vector<QString>					my_selectionPossible;	//! The possible selection value
			};

		} // namespace widget
	} // namespace ui
} // namespace ak
