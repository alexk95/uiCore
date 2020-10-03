/*
 * ak_ui_objectManager.h
 *
 *  Created on: July 14, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <map>							// map
#include <vector>						// vector

// Qt header
#include <qicon.h>						// QIcon
#include <qstring.h>					// QString
#include <qtoolbutton.h>				// QToolButton

#include <ak_globalDataTypes.h>			// UID and ID type

// AK ui header
#include <ak_core.h>					// valueType
#include <ak_ui_core.h>					// objectType, dockLocation
#include <ak_ui_color.h>				// color
#include <ak_ui_qt_comboButtonItem.h>

// Forward declaration
class QWidget;
class QAction;

namespace ak {

	// Forward declaration
	class notifierObjectManager;
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class iconManager;
		class signalLinker;
		class colorStyle;
		namespace core { class aObject; }


		//! @brief The objectManager is used for creating and manipulating objects and widgets
		//! All objects created here a derived from ak::core::aObject or child
		class objectManager {
		public:
			//! @brief Constructor
			//! @param _messenger The globally used messaging system
			//! @param _uidManager The globally used uidManager
			//! @param _colorStyle The globally used color style
			//! @throw ak::Exception if one of the required arguments is a nullptr or a object creation failed
			objectManager(
				ak::messenger *										_messenger,
				ak::uidManager *									_uidManager,
				ak::ui::iconManager *								_iconManager,
				ak::ui::colorStyle *								_colorStyle = nullptr
			);

			//! @brief Deconstructor
			virtual ~objectManager();

			// ###############################################################################################################################################

			// Object creation

			//! @brief Will create a action and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _text The initial text of the action
			//! @param _icon The initial icon of the action
			//! @param _popupMode The popup mode of the action
			//! @throw ak::Exception if failed to create
			ak::UID createAction(
				ak::UID												_creatorUid,
				const QString &										_text,
				const QIcon &										_icon,
				QToolButton::ToolButtonPopupMode					_popupMode = QToolButton::ToolButtonPopupMode::InstantPopup
			);

			//! @brief Will create a check box and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _text The initial text of the check box
			//! @throw ak::Exception if failed to create
			ak::UID createCheckBox(
				ak::UID												_creatorUid,
				const QString &										_text = QString("")
			);

			//! @brief Will create a color edit button and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _color The initial color set to the color edit button
			//! @param _textOverride If a text is provided then the default color text will be overridden
			//! @throw ak::Exception if failed to create
			ak::UID createColorEditButton(
				ak::UID												_creatorUid,
				const ak::ui::color &								_color,
				const QString &										_textOverride = QString("")
			);

			//! @brief Will create a combo box and return its UID
			//! @param _creatorUid The UID of the creator
			//! @throw ak::Exception if failed to create
			ak::UID createComboBox(
				ak::UID												_creatorUid
			);

			//! @brief Will create a combo button and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _initialText The initial text of the combo button
			//! @param _possibleSelection The items that can be selected in the combo button
			//! @throw ak::Exception if failed to create
			ak::UID createComboButton(	
				ak::UID												_creatorUid,
				const QString &										_initialText = QString(""),
				const std::vector<ak::ui::qt::comboButtonItem> &	_possibleSelection = std::vector<ak::ui::qt::comboButtonItem>()
			);

			//! @brief Will create a combo button item and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _text The text of the combo button item
			//! @throw ak::Exception if failed to create
			ak::UID createComboButtonItem(
				ak::UID												_creatorUid,
				const QString &										_text = QString("")
			);

			//! @brief Will create a combo button item and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _icon The icon of the combo button item
			//! @throw ak::Exception if failed to create
			ak::UID createComboButtonItem(
				ak::UID												_creatorUid,
				const QIcon &										_icon,
				const QString &										_text = QString("")
			);

			//! @brief Will create a dock and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _text The text of the dock
			//! @throw ak::Exception if failed to create
			ak::UID createDock(
				ak::UID												_creatorUid,
				const QString &										_text = QString("")
			);

			//! @brief Will create a push button and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _text The text of the push button
			//! @throw ak::Exception if failed to create
			ak::UID createPushButton(
				ak::UID												_creatorUid,
				const QString &										_text = QString("")
			);

			//! @brief Will create a push button and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _icon The icon of the push button
			//! @param _text The text of the push button
			//! @throw ak::Exception if failed to create
			ak::UID createPushButton(
				ak::UID												_creatorUid,
				const QIcon &										_icon,
				const QString &										_text = QString("")
			);

			//! @brief Will create a property grid and return its UID
			//! @param _creatorUid The UID of the creator
			//! @throw ak::Exception if failed to create
			ak::UID createPropertyGrid(
				ak::UID												_creatorUid
			);

			//! @brief Will create a table and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _rows The initial row count of the table
			//! @param _columns The initial column count of the table
			//! @throw ak::Exception if failed to create
			ak::UID createTable(
				ak::UID												_creatorUid,
				int													_rows,
				int													_columns
			);

			//! @brief Will create a text edit and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _initialText The initial text of the text edit
			//! @throw ak::Exception if failed to create
			ak::UID createTextEdit(
				ak::UID												_creatorUid,
				const QString &										_initialText = QString("")
			);

			//! @brief WIll create a timer and return its UID
			//! @param _creatorUid The UID of the creator
			//! @throw ak::Exception if failed to create
			ak::UID createTimer(
				ak::UID												_creatorUid
			);

			//! @brief Will create a tree and return its UID
			//! @param _creatorUid The UID of the creator
			//! @throw ak::Exception if failed to create
			ak::UID createTree(
				ak::UID												_creatorUid
			);

			//! @brief Will create a ui manager and return its UID
			//! @param _creatorUid The UID of the creator
			//! @throw ak::Exception if failed to create
			ak::UID createUiManager(
				ak::UID												_creatorUid
			);

			//! @brief Will create a new tab view and return its UID
			//! @param _creatorUid The UID of the creator who creates this object
			//! @throw ak::Exception if the provided UID is invali
			ak::UID createTabView(
				ak::UID												_creatorUid
			);

			//! @brief Will create a tab toolbar sub container and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _parentUid The UID of the parent container or 
			//! @param _text The text of the new container
			//! @throw ak::Exception if failed to create or the parent UID is invalid
			ak::UID createTabToolBarSubContainer(
				ak::UID												_creatorUid,
				ak::UID												_parentUid,
				const QString &										_text = QString("")
			);

			//! @brief Will create a tab toolbar page and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _uiManagerUid The UID of the ui manager where to create the page at
			//! @param _text The text of the page
			//! @throw ak::Exception if failed to create or the uiManager UID is invalid
			ak::UID createTabToolBarPage(
				ak::UID												_creatorUid,
				ak::UID												_uiManagerUid,
				const QString &										_text = QString("")
			);

			// ###############################################################################################################################################
			
			// Object setter

			//! @brief Will set the alias for the specified object
			//! If a empty alias will be provided the old one will be erased (if exists)
			//! @param _objectUid The UID of the object
			//! @param _alias The alias to set
			void obj_setAlias(
				ak::UID												_objectUid,
				const QString &										_alias
			);

			//! @brief Will add the provided object to the provided container
			//! @param _
			//! @throw ak::Exception If one of the provided UIDs is invalid
			void obj_addObjectToContainer(
				ak::UID									_parentUid,
				ak::UID									_objectUid
			);

			void obj_setCentralWidget(
				ak::UID									_parentUid,
				ak::UID									_objectUid
			);

			void obj_setCentralWidget(
				ak::UID									_parentUid,
				QWidget *								_widget
			);

			void obj_setText(
				ak::UID									_objectUid,
				const QString &										_text
			);

			void obj_setChecked(
				ak::UID									_objectUid,
				bool												_checked
			);

			void obj_setTristate(
				ak::UID									_objectUid,
				bool												_isTristate
			);

			void obj_setFilterVisible(
				ak::UID												_objectUid,
				bool												_vis = true
			);

			void obj_setFilterCaseSensitive(
				ak::UID												_objectUid,
				bool												_caseSensitive,
				bool												_refresh = true
			);

			void obj_setFilterRefreshOnChange(
				ak::UID												_objectUid,
				bool												_refreshOnChange
			);

			void obj_setItems(
				ak::UID									_objectUid,
				const std::vector<ak::ui::qt::comboButtonItem> &	_items
			);

			void obj_setColor(
				ak::UID									_objectUid,
				const ak::ui::color &								_color
			);

			void obj_setIcon(
				ak::UID									_objectUid,
				const QIcon &										_icon
			);

			//! @brief Will set the read only option for the provided object
			//! @param _objectUid The UID of the object to set the option at
			//! @param _readOnly If true, the object will be read only
			//! @throw ak::Exception if the provided object UID is invalid
			void obj_setReadOnly(
				ak::UID									_objectUid,
				bool									_readOnly = true
			);

			//! @brief Will append the text to the provided object
			void obj_appendText(
				ak::UID									_objectUid,
				const QString &										_text
			);

			//! @brief Will set the auto scroll option for the provided object
			//! @param _objectUid The UID of the requested object
			//! @param _enabled If true, the auto scroll option for the object will be enabled
			//! @throw ak::Exception if the provided object UID is invalid
			void obj_setAutoScrollToBottomEnabled(
				ak::UID									_objectUid,
				bool									_enabled = true
			);

			//! @brief Will set the visible status of the tabToolbar
			//! @param _uiManagerUid The UID of the UI manager the tab toolbar is located at
			//! @param _vis If true, the tab toolBar will be visible
			void obj_setTabToolBarVisible(
				ak::UID											_uiManagerUid,
				bool											_vis = true
			);

			//! @brief Will set the tab location of the provided object
			//! @param _location The tab location to set
			//! @throw ak::Exception if the provided object UID is invalid
			void obj_setTabLocation(
				ak::UID											_objectUid,
				ak::ui::core::tabLocation						_location
			);

			//! @brief Will focus the tab at the specified object
			//! @param _objectUid The UID of the object to focus the tab at
			//! @param _tab The tab to focus
			//! @throw ak::Exception if the object UID is invalid or the tab is invalid
			void obj_setTabFocused(
				ak::UID											_objectUid,
				ak::ID											_tab
			);

			//! @brief Will add a new object with the value type Boolean to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if a object creation failed
			void obj_addProperty(
				ak::UID											_objectUid,
				const QString &									_itemName,
				bool											_value,
				bool											_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Integer to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if a object creation failed
			void obj_addProperty(
				ak::UID								_objectUid,
				const QString &									_itemName,
				int												_value,
				bool											_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Double to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if a object creation failed
			void obj_addProperty(
				ak::UID								_objectUid,
				const QString &									_itemName,
				double											_value,
				bool											_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type String to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if a object creation failed
			void obj_addProperty(
				ak::UID								_objectUid,
				const QString &									_itemName,
				const char *									_value,
				bool											_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type String to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if a object creation failed
			void obj_addProperty(
				ak::UID								_objectUid,
				const QString &									_itemName,
				const QString &									_value,
				bool											_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Color to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if a object creation failed
			void obj_addProperty(
				ak::UID								_objectUid,
				const QString &									_itemName,
				const ak::ui::color &								_value,
				bool											_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Selection to the property Grid
			//! @param _objectUid The UID of the object to add the property at
			//! @param _itemName The name of the property
			//! @param _selection The possible selection values
			//! @param _selectedValues The currently selected value
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if a object creation failed
			void obj_addProperty(
				ak::UID								_objectUid,
				const QString &									_itemName,
				const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
				const QString &									_selectedValue,
				bool											_isMultipleValues = false
			);

			//! @brief Will add a new item to the tree widget
			//! @param _objectUid The UID of the object
			//! @param _parentId The ID of the parent object
			//! @param _text The text of the object to add
			//! @param _textAlignment The text alignment of the very last child
			//! @param _foreColor The foreground color of the very last child
			//! @param _backColor The background color of the very last child
			//! @param _icon The icon to add to the very last child
			ak::ID obj_createItem(
				ak::UID											_objectUid,
				ak::ID											_parentId,
				const QString &									_text = QString(""),
				ak::ui::core::textAlignment						_textAlignment = ak::ui::core::textAlignment::align_alignLeft,
				QIcon											_icon = QIcon()
			);

			//! @brief Will add new items to the tree according to the provided command
			//! The command consists of the root item and the childs.
			//! The command may look like this:
			//! root|child|child2
			//! In this case the delimiter will be the '|' and the tree will get a root item with the text "root" (if doesnt exist before)
			//! The root gets a child item with the text "child" and the child gets a child with the text "child2", so the tree looks like this:
			//! ->root
			//! ->->child
			//! ->->->child2
			//! The very last item will also get the alignment, colors and icon set.
			//! Will return the UID of the very last item.
			//! @param _objectUid The UID of the object
			//! @param _cmd The tree command
			//! @param _delimiter The delimiter used in the command which seperates the items
			//! @param _textAlignment The text alignment of the very last child
			//! @param _foreColor The foreground color of the very last child
			//! @param _backColor The background color of the very last child
			//! @param _icon The icon to add to the very last child
			ak::ID obj_createItem(
				ak::UID											_objectUid,
				const QString &									_cmd,
				char											_delimiter = '|',
				ak::ui::core::textAlignment						_textAlignment = ak::ui::core::textAlignment::align_alignLeft,
				const QIcon  &									_icon = QIcon()
			);

			//! @brief Will expand all items of the provided object
			//! @param _objectUid The UID of the object
			void obj_expandAllItems(
				ak::UID											_objectUid
			);

			//! @brief Will collapse all items of the provided object
			//! @param _objectUid The UID of the object
			void obj_collapseAllItems(
				ak::UID											_objectUid
			);

			//! @brief Enables or disables the ability to select multiple items
			//! @param _objectUid The UID of the object
			//! @param _multiSelection Specify whether multiple items can be selected
			void obj_setMultiSelectionEnabled(
				ak::UID											_objectUid,
				bool											_multiSelection = true
			);

			//! @brief Enables or disables the ability to automatically select/deselect the childrens of an item
			//! @param _objectUid The UID of the object
			//! @param _enabled if true, the childs of an item will be selected/deselected automatically
			void obj_setAutoSelectAndDeselectChildrenEnabled(
				ak::UID											_objectUid,
				bool											_enabled = true
			);

			//! @brief Will set the auto expand selected items option for the specified object
			//! @param _objectUid The UID of the object
			//! @param _enabled If true the option is enabled
			//! @throw ak::Exception if the specified object is invalid
			void obj_setAutoExpandSelectedItems(
				ak::UID											_objectUid,
				bool											_enabled = true
			);

			//! @brief Will deselect all items
			//! @param _objectUid The UID of the object
			void obj_deselectAllItems(
				ak::UID											_objectUid
			);

			//! @brief Will create a new tab at the provided object
			//! @param _objectUid The UID of the object to add the tab at
			//! @param _widgetUid The UID of the widget to set as a central widget
			//! @param _title The title of the tab
			//! @param _icon The icon of the tab
			ak::ID obj_addTab(
				ak::UID											_objectUid,
				ak::UID											_widgetUid,
				const QString &									_title,
				const QIcon &									_icon = QIcon()
			);
			
			//! @brief Will create a new tab at the provided object
			//! @param _objectUid The UID of the object to add the tab at
			//! @param _widget The widget to set as the central widget
			//! @param _title The title of the tab
			//! @param _icon The icon of the tab
			ak::ID obj_addTab(
				ak::UID											_objectUid,
				QWidget *										_widget,
				const QString &									_title,
				const QIcon &									_icon = QIcon()
			);

			//! @brief Will create a new tab at the provided object
			//! @param _objectUid The UID of the object to add the tab at
			//! @param _widgetUid The UID of the widget to set as a central widget
			//! @param _title The title of the tab
			//! @param _icon The icon of the tab
			ak::ID obj_addTab(
				ak::UID											_objectUid,
				ak::UID											_widgetUid,
				const QString &									_title,
				const QString &									_iconName,
				const QString &									_iconSize
			);

			//! @brief Will create a new tab at the provided object
			//! @param _objectUid The UID of the object to add the tab at
			//! @param _widget The widget to set as the central widget
			//! @param _title The title of the tab
			//! @param _icon The icon of the tab
			ak::ID obj_addTab(
				ak::UID											_objectUid,
				QWidget *										_widget,
				const QString &									_title,
				const QString &									_iconName,
				const QString &									_iconSize
			);

			//! @brief Enables or disables the provided object
			//! @param _objectUid The UID of the object
			//! @param _enabled if true, the object will be enabled
			void obj_setEnabled(
				ak::UID											_objectUid,
				bool											_enabled = true
			);

			void obj_setVisible(
				ak::UID											_objectUid,
				bool											_visible = true
			);

			//! @brief Will clear the items of the provided object
			//! @param _objectUid The UID of the object to clear its items
			//! @throw ak::Exception if the provided object UID is invalid
			void obj_clear(
				ak::UID											_objectUid
			);

			//! @brief Will delete all provided items from the provided object
			//! @param _objectUid The UID of the object to delete the items from
			//! @param _items The items to delete
			//! @throw ak::Exception if the provided object UID or any item ID is invalid
			void obj_deleteItems(
				ak::UID												_objectUid,
				const std::vector<ak::ID> &							_items
			);

			//! @brief Will call the start function of the provided object with the given value
			//! @param _objectUid The UID of the object
			//! @param _value The value to set to the object
			//! @throw ak::Exception if the provided object UID is invalid
			void obj_start(
				ak::UID												_objectUid,
				int													_value
			);

			//! @brief Will call the stop function of the provided object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the provided object UID is invalid
			void obj_stop(
				ak::UID												_objectUid
			);

			//! @brief Will call the shoot function of the provided object
			//! @param _objectUid The UID of the object
			//! @param _value The value to set to the object
			//! @throw ak::Exception if the provided object UID is invalid
			void obj_shoot(
				ak::UID												_objectUid,
				int													_value
			);

			//! @brief Will delete the specified object
			//! @param _objectUid The UID of the object to delete
			//! @throw ak::Exception if the provieded UID is invalid
			void obj_delete(
				ak::UID												_objectUid
			);

			// ###############################################################################################################################################

			// Object getter

			//! @brief Will return the alias of the specified object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the UID is invalid
			QString obj_getAlias(
				ak::UID												_objectUid
			);

			//! @brief Will return the previously created object
			//! @param _objectUid The UID of the object to retreive
			//! @throw ak::Exception if the provided UID is invalid
			ak::ui::core::aObject * obj_get(
				ak::UID												_objectUid
			);

			//! @brief Will return QWidget of the specified Object.
			//! The created object must be a widgetType object to retreive its widget
			//! @param _objectUid The UID of the object to retreive
			//! @throw ak::Exception if the provided UID is invalid
			QWidget * obj_getWidget(
				ak::UID												_objectUid
			);

			//! @brief Will return QAction of the specified action
			//! @param _actionUid The UID of the action to retreive
			//! @throw ak::Exception if the provided UID is invalid
			QAction * obj_getAction(
				ak::UID												_actionUid
			);

			//! @brief Will return the object type of the provided object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided UID is invalid
			ak::ui::core::objectType obj_getType(
				ak::UID												_objectUid
			);

			QString obj_getText(
				ak::UID									_objectUid
			);

			bool obj_getTristate(
				ak::UID									_objectUid
			);

			bool obj_getChecked(
				ak::UID									_objectUid
			);

			//! @brief Will return the current objects color
			//! @param _objectUid The object to return the color from
			ak::ui::color obj_getColor(
				ak::UID									_objectUid
			);

			//! @brief Will return all selected objects of the specified object
			//! @param _objectUid The object to return the slection from
			std::vector<ak::ID> obj_getSelectedItems(
				ak::UID									_objectUid
			);

			//! @brief Will return the auto scroll option for the provided object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided object UID is invalid
			bool obj_getAutoScrollToBottomEnabled(
				ak::UID									_objectUid
			);

			//! @brief Will return the enabled state of the provided object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided object UID is invalid
			bool obj_getEnabled(
				ak::UID									_objectUid
			);

			//! @brief Will return the ID of the item at this object with the specified text
			//! Returns invalidID if the item does not exist
			ak::ID obj_getItem(
				ak::UID									_objectUid,
				const QString &							_text
			);

			//! @brief Will return the count of items at the specified object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided UID is invalid or the object type is invalid
			int obj_getItemCount(
				ak::UID									_objectUid
			);

			//! @brief Will return the currently focused tab at the specified object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided UID is invalid
			int obj_getFocusedTab(
				ak::UID									_objectUid
			);

			//! @brief Will get the auto expand selected items option for the specified object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the specified object is invalid
			bool obj_getAutoExpandSelectedItems(
				ak::UID									_objectUid
			);

			// ###############################################################################################################################################

			// Item setter

			//! @brief Will set the selected state of the provided item.
			//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item
			//! @param _selected The selected state of the item
			//! @throw ak::Exception if the ID is invalid
			void itm_setSelected(
				ak::UID											_objectUid,
				ak::ID											_itemId,
				bool											_selected = true
			);

			//! @brief Will set the selected state of the provided item.
			//! Will not change the selected state of the childs item even if the selectAndDeselectChilds option is true
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item
			//! @param _selected The selected state of the item
			//! @throw ak::Exception if the ID is invalid
			void itm_setSingleSelected(
				ak::UID											_objectUid,
				ak::ID											_itemId,
				bool											_selected = true
			);

			//! @brief Will toggle the selected state of the provided item.
			//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item
			//! @param _selected The selected state of the item
			//! @throw ak::Exception if the ID is invalid
			void itm_toggleSelection(
				ak::UID											_objectUid,
				ak::ID											_itemId
			);

			//! @brief Will set the text of the specified item
			//! @param _objectUid The UID of the object
			//! @param _tab The item to set the text at
			//! @param _text The text to set
			//! @throw ak::Exception if the specified UID or ID is invalid
			void itm_setText(
				ak::UID											_objectUid,
				ak::ID											_itemId,
				const QString &									_text
			);

			// ###############################################################################################################################################

			// Item getter

			//! @brief Will return all items from root to specified item as a vector where the first item is the root item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the requested item
			//! @throw ak::Exception if the provided object UID is invalid
			//! @throw ak::Exception if the provided item ID is invalid
			std::vector<QString> itm_getPath(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return all items from root to specified item as a string seperated with the provided delimiter where the first item is the root item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the requested item
			//! @param _delimiter The delimiter between the items
			//! @throw ak::Exception if the provided object UID is invalid
			//! @throw ak::Exception if the provided item ID is invalid
			QString itm_getPathString(
				ak::UID									_objectUid,
				ak::ID									_itemId,
				char									_delimiter = '|'
			);

			//! @brief Will return the text of the specified item at the specified object
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item to get the text of
			QString itm_getText(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the value type of the specified item at the specified object
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item to get the text of
			ak::core::valueType itm_getValueType(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the ID of the specified item in the specified object
			//! @param _objectUid The UID the specified item is located at
			//! @param _itemPath The path of the item
			//! @param _delimiter The of the item path
			ak::ID itm_getID(
				ak::UID									_objectUid,
				const QString &							_itemPath,
				char									_delimiter = '|'
			);

			//! @brief Will return the boolean value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			bool itm_getValueBoolean(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the color value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			ui::color itm_getValueColor(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the double value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			double itm_getValueDouble(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the integer value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			int itm_getValueInteger(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the boolean value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			std::vector<QString> itm_getValuePossibleSelection(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the selected value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			QString itm_getValueSelection(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the string value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			QString itm_getValueString(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the boolean value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			bool itm_getValueIsMultivalued(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			// ###############################################################################################################################################

			//! @brief Will add the dock to the uiManager at the specified dock location
			//! @param _uiManagerUid The UID of the UI manager to add the dock at
			//! @param _dockUid The UID of the existing dock to add to the UI manager
			//! @param _dockLocation The location where to put the dock at
			void addDock(
				ak::UID									_uiManagerUid,
				ak::UID									_dockUid,
				ak::ui::core::dockLocation				_dockLocation
			);

			//! @brief Will add the dock as a tab to the parent dock
			//! @param _parentUid The UID of the parent dock
			//! @param _dockUid The UID of the dock to tabify
			void tabifyDock(
				ak::UID									_uiManagerUid,
				ak::UID									_parentUid,
				ak::UID									_dockUid
			);

			//! @brief Will set the dock priority on the bottom left corner.
			//! The dock with the priority will expand into the corner.
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _dockLocation The dock location to set the priority for (allowed dockBottom and dockLeft)
			//! @throw ak::Exception if the uiManager UID or the dock location is invalid
			void setDockBottomLeftPriority(
				ak::UID									_uiManagerUid,
				ak::ui::core::dockLocation				_dockLocation
			);

			//! @brief Will set the dock priority on the bottom left corner.
			//! The dock with the priority will expand into the corner.
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _dockLocation The dock location to set the priority for (allowed dockBottom and dockRight)
			//! @throw ak::Exception if the uiManager UID or the dock location is invalid
			void setDockBottomRightPriority(
				ak::UID									_uiManagerUid,
				ak::ui::core::dockLocation				_dockLocation
			);

			//! @brief Will set the visible state of the status label of the provided uiManager.
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _visible If true the status label will be visible
			//! @throw ak::Exception if the provided UI manager UID is invalid
			void setStatusLabelVisible(
				ak::UID									_uiManagerUid,
				bool									_visible
			);

			//! @brief Will set the visible state of the status progress bar of the provided uiManager.
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _visible If true the status progress bar will be visible
			//! @throw ak::Exception if the provided UI manager UID is invalid
			void setStatusProgressVisible(
				ak::UID									_uiManagerUid,
				bool									_visible
			);

			//! @brief Will set the status label text at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _text The text to set
			//! @throw ak::Exception if the provided UI manager UID is invalid
			void setStatusLabelText(
				ak::UID									_uiManagerUid,
				const QString &							_text
			);

			//! @brief Will set the value of the status progress bar at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _value The value to set (0 - 100)
			//! @throw ak::Exception if the provided UI manager UID is invalid or the provided value is out of range
			void setStatusProgressValue(
				ak::UID									_uiManagerUid,
				int										_value
			);

			//! @brief Will set the continuous option of the status progress bar at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _continuous If true the status progress bar will be displayed as a continuous bar
			//! @throw ak::Exception if the provided UI manager UID is invalid
			void setStatusProgressContinuous(
				ak::UID									_uiManagerUid,
				bool									_continuous
			);

			//! @brief Will return true if the status label is visible at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @throw ak::Exception if the provided UI manager UID is invalid
			bool getStatusLabelVisible(
				ak::UID									_uiManagerUid
			);

			//! @brief Will return true if the status progress bar is visible at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @throw ak::Exception if the provided UI manager UID is invalid
			bool getStatusProgressVisible(
				ak::UID									_uiManagerUid
			);

			//! @brief Will return the status label text at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @throw ak::Exception if the provided UI manager UID is invalid
			QString getStatusLabelText(
				ak::UID									_uiManagerUid
			);

			//! @brief Will return the status progress bar value at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @throw ak::Exception if the provided UI manager UID is invalid
			int getStatusProgressValue(
				ak::UID									_uiManagerUid
			);

			//! @brief Will return the continuous option of the status progress bar at the provided uiManager
			//! @param _uiManagerUid The UID of the UI manager
			//! @throw ak::Exception if the provided UI manager UID is invalid
			bool getStatusProgressContinuous(
				ak::UID									_uiManagerUid
			);

			//! @brief Will close the provided uiManager
			//! @param _uiManagerUid The UID of the uiManager to close
			//! @throw ak::Exception if the provided UID is invalid
			void close(
				ak::UID												_uiManagerUid
			);

			// ###############################################################################################################################################

			//! @brief Will destroy all objects created by this creator
			//! @param _creatorUid The UID of the creator
			void creatorDestroyed(
				ak::UID									_creatorUid
			);

			void setColorStyle(
				ak::ui::colorStyle *								_colorStyle
			);

			//! @brief Will destroy all objects
			void destroyAll(void);

			//! @brief Will return the JSON settings string of all objects that have an alias set
			std::string getSettingsJSON(void);

			//! @brief Will setup the UI with the settings provided in the settings JSON string
			//! @param _json The JSON string containing the settings
			//! @throw ak::Exception on syntax mismatch
			void setupSettings(
				const char *										_json
			);

		private:

			objectManager() = delete;
			objectManager(const ak::ui::objectManager &) = delete;

			// ###############################################################################################################################################

			//! @brief Will register the created UID for the creator
			//! @param _creatorUid The UID of the creator
			//! @param _createdUid THe UID of the created object
			void addCreatedUid(
				ak::UID									_creatorUid,
				ak::UID									_createdUid
			);
			
			//! @brief Will cast and return the object to a QWidget
			QWidget * castToWidget(
				ak::UID									_objectUid
			);

			// ###############################################################################################################################################
			// Object storing

			std::map<ak::UID, ak::ui::core::aObject *>		my_mapObjects;
			typedef std::map<ak::UID,
				ak::ui::core::aObject *>::iterator			my_mapObjectsIterator;

			std::map<ak::UID,
				std::vector<ak::UID> *>						my_mapCreators;				//! Contains all creators
			typedef std::map<ak::UID,
				std::vector<ak::UID> *>::iterator			my_mapCreatorsIterator;		//! Iterator used to iterate trough the creators

			std::map<QString, ak::UID>						my_mapAliases;				//! Contains the UIDs for a specified alias
			typedef std::map<QString, ak::UID>::iterator	my_mapAliasesIterator;		//! Iterator used to iterate trough the aliases

			// ###############################################################################################################################################

			ak::uidManager *				my_uidManager;									//! The globally used uidManager
			ak::messenger *					my_messenger;									//! The globally used messaging system
			ak::ui::signalLinker *			my_signalLinker;								//! The internal signal linker
			ak::ui::colorStyle *			my_colorStyle;									//! The globally used color style
			ak::ui::iconManager *			my_iconManager;									//! The globally used icon manager
			ak::notifierObjectManager *		my_notifier;									//! The notifier used to catch the destroyed messages

		};
	} // namespace ui
} // namespace ak
