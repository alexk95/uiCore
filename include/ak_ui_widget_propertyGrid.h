/*
 * ak_ui_widget_propertyGrid.h
 *
 *  Created on: March 29, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// Qt header
#include <qstring.h>						// QString

// AK header
#include <ak_ui_core_aWidgetManager.h>		// base class
#include <ak_core.h>						// eventType
#include <ak_globalDataTypes.h>				// UID and ID type
#include <ak_ui_qt_comboButtonItem.h>		// comboButtonItem
#include <ak_ui_color.h>					// color

namespace ak {
	
	// Forward declaration
	class notifierPropertyGrid;
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class color;
		class colorStyle;
		class objectManager;
		class signalLinker;

		namespace widget {

			// Forward declaration
			class propertyGridItem;
			class table;

			//! @brief This class represents a property grid which consists of a table with two columns
			//! The first column is the name of the property and the second one is its value
			class propertyGrid : public ak::ui::core::aWidgetManager {
			public:

				//! @brief Constructor
				//! @param _messenger The messenger used in this class
				//! @param _uidManager The UID manager used in this class
				//! @param _dock The dock widget the property grid will be displayed in
				//! @param _colorStyle The color style used in this property grid
				//! @throw ak::Exception if a creation failed or an error occurs when calling a subroutine
				propertyGrid(
					ak::messenger *									_messenger,
					ak::uidManager *								_uidManager,
					const ak::ui::colorStyle *						_colorStyle = nullptr
				);

				//! @brief Deconstructor
				~propertyGrid();

				// ##############################################################################################################
				// ak::widget Functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *						_colorStyle
				) override;

				// ############################################################################################################
				// Add object

				//! @brief Will create a new item with the value type Boolean at the property Grid
				//! @param _itemName The name of the property
				//! @param _value The value of the property
				//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
				//! @throw ak::Exception if a object creation failed
				void createItem(
					const QString &									_itemName,
					bool											_value,
					bool											_isMultipleValues = false
				);

				//! @brief Will create a new item with the value type Integer at the property Grid
				//! @param _itemName The name of the property
				//! @param _value The value of the property
				//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
				//! @throw ak::Exception if a object creation failed
				void createItem(
					const QString &									_itemName,
					int												_value,
					bool											_isMultipleValues = false
				);

				//! @brief Will create a new item with the value type Double at the property Grid
				//! @param _itemName The name of the property
				//! @param _value The value of the property
				//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
				//! @throw ak::Exception if a object creation failed
				void createItem(
					const QString &									_itemName,
					double											_value,
					bool											_isMultipleValues = false
				);

				//! @brief Will create a new item with the value type String at the property Grid
				//! @param _itemName The name of the property
				//! @param _value The value of the property
				//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
				//! @throw ak::Exception if a object creation failed
				void createItem(
					const QString &									_itemName,
					const char *									_value,
					bool											_isMultipleValues = false
				);

				//! @brief Will create a new item with the value type String at the property Grid
				//! @param _itemName The name of the property
				//! @param _value The value of the property
				//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
				//! @throw ak::Exception if a object creation failed
				void createItem(
					const QString &									_itemName,
					const QString &									_value,
					bool											_isMultipleValues = false
				);

				//! @brief Will create a new item with the value type Color at the property Grid
				//! @param _itemName The name of the property
				//! @param _value The value of the property
				//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
				//! @throw ak::Exception if a object creation failed
				void createItem(
					const QString &									_itemName,
					const ak::ui::color &								_value,
					bool											_isMultipleValues = false
				);

				//! @brief Will create a new item with the value type Selection at the property Grid
				//! @param _itemName The name of the property
				//! @param _selection The possible selection values
				//! @param _selectedValues The currently selected value
				//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
				//! @throw ak::Exception if a object creation failed
				void createItem(
					const QString &									_itemName,
					const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
					const QString &									_selectedValue,
					bool											_isMultipleValues = false
				);

				// ############################################################################################################
				// Manipulate existing

				//! @brief Clears the whole propertyGrid
				void clear(void);

				// ############################################################################################################
				// Information

				//! @brief Sets teh showMessageboxOnSyntaxError mode
				//! @param _show If true, a message box will be displayed in case of an error
				void setShowMessageboxOnSyntaxError(
					bool											_show
				);

				//! @brief Will set the focus to the row provided
				//! @param _row The row index to set the focus on
				//! @throw ak::Exception on any error during a subroutine call
				void focusRow(
					int												_row
				) const;

				//! @brief Will set the propvided row to "Sytax error" (Not for types: bool, color, selection)
				//! @param _row The row to set the syntax error at
				//! @throw ak::Exception if the value type of the provided row i bool, array or selection. Also if any error occurs during a subroutine call
				void setSyntaxError(
					int												_row,
					const QString &									_message = QString("")
				);

				// ############################################################################################################
				// Getter

				//! @brief Returns the ammount of items in the propertyGrid
				int itemCount(void) const;

				//! @brief Returns true if the showMessageboxOnSytaxError option is on
				bool showMessageboxOnSyntaxError(void) const;

				//! @brief Returns the value type at the specified row
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call
				ak::core::valueType getValueType(
					ak::ID												_row
				) const;

				//! @brief Returns the name of the property at the specified row
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call
				QString getPropertyName(
					ak::ID												_row
				) const;

				//! @brief Return true if the item is stil in a multi value state
				//! The value of an item can not be retreived if it is multivalued
				//! @param _row The row index of the requested item
				//! @throw ak::Exception if any error occurs during a subroutine call
				bool getIsMultivalued(
					ak::ID												_row
				) const;

				// ############################################################################################################
				// Get values

				//! @brief Return the Boolean value at the specified row
				//! The value type at the specified row must be a Boolean
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call or the multivalued state is true
				bool getBool(ak::ID _row) const;

				//! @brief Return the Integer value at the specified row
				//! The value type at the specified row must be a Integer
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call or the multivalued state is true
				int getInt(ak::ID _row) const;

				//! @brief Return the Double value at the specified row
				//! The value type at the specified row must be a Double
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call or the multivalued state is true
				double getDouble(ak::ID _row) const;

				//! @brief Return the String value at the specified row
				//! The value type at the specified row must be a String
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call or the multivalued state is true
				QString getString(ak::ID _row) const;

				//! @brief Return the Current Selection value at the specified row
				//! The value type at the specified row must be a Selection
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call or the multivalued state is true
				QString getSelection(ak::ID _row) const;

				//! @brief Return the Possible selection values value at the specified row
				//! The value type at the specified row must be a Selection
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call or the multivalued state is true
				std::vector<QString> getSelectionPossibleValues(ak::ID _row) const;

				//! @brief Return the Color value at the specified row
				//! The value type at the specified row must be a Color
				//! @param _row The row index
				//! @throw ak::Exception if any error occurs during a subroutine call or the multivalued state is true
				ak::ui::color getColor(ak::ID	_row) const;

				// ############################################################################################################
				// Event handling

				//! @brief Will send a event message for the row provided
				//! @param _row The row to raise the event for
				//! @param _eventType The type of the event
				//! @param _forceMessage If true, a message will always be sent even if the condition is not fullfilled
				//! @throw ak::Exception if any error occurs during a subroutine call
				void raiseCellEvent(
					int												_row,
					ak::core::eventType								_eventType,
					bool											_forceMessage = false
				) const;

				//! @brief Will send a event message fot the widget provided
				//! @param _widgetUid The UID of the widget
				//! @param _eventType The type of the event
				//! @throw ak::Exception if the UID is invalid or any error occurs during a subroutine call
				void raiseWidgetEvent(
					ak::UID											_widgetUid,
					ak::core::eventType								_eventType
				);

				//! @brief Will send a key event message with the provided key
				//! @param _key The key that was pressed/released
				//! @param _keyDown If true a key pressed event will be send, othwise released
				void keyEvent(
					ak::ui::core::keyType							_key,
					bool											_keyDown
				);

			protected:

				//! @brief Checks if the row index provided is in the index range
				//! @param _row The row index to check
				//! @param _throwException If true, an exception will be thrown if index out of range, otherwise returns false
				//! @throw ak::Exception if the index is out of range
				bool checkRow(
					int												_row,
					bool											_throwException = false
				) const;

				//! @brief Will return true if the value type of the provided row is equal to the value type provided
				//! @param _row The row index to check the value type at
				//! @param _throwException If true, an exception will be thrown instead of returning false if the value types differ
				//! @throw ak::Exception if the index is out of range. Also if the throwException is true and the object types differ
				bool checkItemTypeEqual(
					int												_row,
					ak::core::valueType								_requestedType,
					bool											_throwException = false
				) const;

			private:
				ak::messenger *												my_externalMessanger;					//! The globally used messaging system
				ak::ui::widget::table *										my_table;								//! The table this property grid is managing
				ak::uidManager *											my_externalUidManager;					//! The globally used UID manager
				ak::notifierPropertyGrid *									my_internalNotifier;					//! The internally used notifier
				ui::signalLinker *											my_signalLinker;

				bool														my_showMessageboxOnSyntaxError;			//! If true, a message box will be displayed on case of any syntax error

				std::vector<ak::ui::widget::propertyGridItem *>				my_items;								//! Contains all items displayed in this property grid
				std::map<ak::UID, size_t>									my_UIDmap;								//! Map mapping the UID to their index in the my_items vector
				typedef std::map<ak::UID, size_t>::iterator					my_UIDmapIterator;						//! Iterator used to iterate trough the UID map

				ak::ui::color												my_colorNormal;							//! The color used to display "correct" information
				ak::ui::color												my_colorInvalidInput;					//! The color used to display "wrong" information

			};

		} // namespace widget
	} // namespace ui
} // namespace ak