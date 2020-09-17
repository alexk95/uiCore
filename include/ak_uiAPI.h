/*
 * ak_uiAPI.h
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <string>						// C++ string
#include <vector>						// vector<>

// Qt header
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon

// AK header
#include "ak_core.h"					// message type
#include "ak_ui_core.h"					// object type
#include "ak_ui_color.h"				// color

#include "ak_globalDataTypes.h"			// UID and ID type
#include "ak_ui_qt_comboButtonItem.h"	// comboButtonItem

namespace ak {

	// Forward declaration
	class messenger;
	class notifier;
	class uidManager;
	namespace ui {
		class objectManager;
		class colorStyle;
		class iconManager;
	}

	namespace uiAPI {

		//! The API manager is used to manage the global objects required for this API to work
		class apiManager {
		public:
			//! @brief Constructor
			apiManager();
			
			//! @brief Deconstructor
			virtual ~apiManager();
			
			//! @brief Will initialize the API
			//! @param _messenger The external messenger. If nullptr a new one will be created
			//! @param _uidManager The external UID manager. If nullptr a new one will be created
			//! @param _iconManager The external icon manager. If nullptr a new one will be created
			//! @param _objectManager The external object manager. If nullptr a new one will be created
			void ini(
				bool						_createQApplication,
				int							_argc = 0,
				char **						_argv = nullptr,
				ak::messenger *				_messenger = nullptr,
				ak::uidManager *			_uidManager = nullptr,
				ak::ui::iconManager *		_iconManager = nullptr,
				ak::ui::objectManager *		_objectManager = nullptr
			);

			//! @brief Returns true if the API was initialized
			bool isInitialized(void) const;

			//! @brief Will return the messenger
			ak::messenger * messenger(void);

			//! @brief Will return the UID manager
			ak::uidManager * uidManager(void);

			//! @brief Will return the icon manager
			ak::ui::iconManager * iconManager(void);

			//! @brief Will return the object manager
			ak::ui::objectManager * objectManager(void);

			//! @brief Will set the color style
			//! @param _colorStyle The colorStyle to set
			//! @throw ak::Exception on any error
			void setColorStyle(
				ak::ui::colorStyle *		_colorStyle
			);

			//! @brief Will set the default dark colorStyle
			//! @throw ak::Exception on any error
			void setDarkColorStyle(void);

			//! @brief Will set the default dark colorStyle
			//! @throw ak::Exception on any error
			void setBrightColorStyle(void);

			//! @brief Will call the execute function of the QApplication
			int exec(void);

		protected:
			QApplication *				my_app;							//! The core application
			bool						my_appIsRunning;				//! True if the core application is already running

			ak::ui::objectManager *		my_objManager;					//! The object manager used in this API
			bool						my_objManagerIsExtern;			//! If true, then the object manager was created externally
			ak::messenger *				my_messenger;					//! The messenger used in this API
			bool						my_messengerIsExtern;			//! If true, then the messenger was created externally
			ak::uidManager *			my_uidManager;					//! The UID manager used in this API
			bool						my_uidManagerIsExtern;			//! If true, then the UID manager was created externally
			ak::ui::iconManager *		my_iconManager;					//! The icon manager used in this API
			bool						my_iconManagerIsExtern;			//! If true, then the icon manager was created externally
			ak::ui::colorStyle *		my_colorStyle;					//! The color style used in this API
			bool						my_colorStyleIsExtern;			//! If true, then the color style was created externally
			bool						my_isInitialized;				//! If true, then the API was initialized
		};

		// ###################################################################################################################################################

		//! @brief Will initialize the application
		//! @param _messenger The messenger that will be used in the aplication, if nullptr a new one will be created
		//! @param _uidManager The UID manager that will be used in the aplication, if nullptr a new one will be created
		//! @param _iconManager The icon manager that will be used in the aplication, if nullptr a new one will be created
		//! @param _objectManager The object manager that will be used in the aplication, if nullptr a new one will be created
		//! @throw ak::Exception if failed to create a object
		//! @throw ak::Exception if the API was already initialized
		__declspec(dllexport) void ini(
			bool													_createQApplication,
			int														_argc = 0,
			char **													_argv = nullptr,
			ak::messenger *											_messenger = nullptr,
			ak::uidManager *										_uidManager = nullptr,
			ak::ui::iconManager *									_iconManager = nullptr,
			ak::ui::objectManager *									_objectManager = nullptr,
			ak::ui::colorStyle *									_colorStyle = nullptr
		);

		//! @brief Will destroy all objects created by this API
		__declspec(dllexport) void destroy(void);

		//! @brief Will destroy the provided object
		__declspec(dllexport) void destroyObject(
			ak::UID													_objectUid
		);

		//! @brief Will enable the provided event types to be send
		//! @param _types The types to allow
		__declspec(dllexport) void enableEventTypes(
			ak::core::eventType										_types
		);

		//! @brief Will disable the provided event types to be send
		//! @param _types The types to disable
		__declspec(dllexport) void disableEventTypes(
			ak::core::eventType										_types
		);

		//! @brief Will return the enabled event types that will be send
		__declspec(dllexport) std::vector<ak::core::eventType> enabledEventTypes(void);

		//! @brief Will return the disabled event types that will be send
		__declspec(dllexport) std::vector<ak::core::eventType> disabledEventTypes(void);

		// ###############################################################################
		
		// message functions

		//! @brief Will register the notifier in the messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _senderUid For messages of which sender this notifier should be registered
		//! @param _messagType For which message type this notifier should be registered
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID registerNotifier(
			ak::UID													_senderUid,
			ak::notifier *											_notifier,
			ak::core::messageType									_messageType = ak::core::messageType::mEvent
		);

		//! @brief Will send a message to the messanger
		//! @param _senderUid Who sends this message
		//! @param _messageType What type of message is send
		//! @param _message The message itself
		//! @param _info1 Additional information 1
		//! @param _info2 Additional information 2
		//! @throw ak::Exception if the API is not initialized
		//! @throw ak::Exception At any kind of error that may occur in any subroutine call when calling the receivers
		__declspec(dllexport) void sendMessage(
			ak::UID													_senderUid,
			ak::core::messageType									_messageType,
			int														_message,
			int														_info1 = 0,
			int														_info2 = 0
		);

		// ###############################################################################

		// object creation

		//! @brief Will create a new Action and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the action
		//! @param _iconSize The size of the icon to display in the action (requires also the icon name to be provided)
		//! @param _iconName The name of the icon to display in the action (requires also the icon size to be provided)
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createAction(
			ak::UID													_creatorUid,
			const char *											_text,
			const char *											_iconName = nullptr,
			const char *											_iconSize = "32"
		);

		//! @brief Will create a new Action and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the action
		//! @param _iconSize The size of the icon to display in the action (requires also the icon name to be provided)
		//! @param _iconName The name of the icon to display in the action (requires also the icon size to be provided)
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createAction(
			ak::UID													_creatorUid,
			const QString &											_text,
			const QString &											_iconName = QString(""),
			const QString &											_iconSize = QString("32")
		);

		//! @brief Will create a new Action and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the action
		//! @param _icon The icon of the action
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createAction(
			ak::UID										_craetorUid,
			const QString &											_text,
			const QIcon &											_icon
		);

		//! @brief Will create a new CheckBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the CheckBox
		//! @param _checked The initial checked state of the checkbox
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createCheckbox(
			ak::UID										_creatorUid,
			const char *											_text,
			bool													_checked = false
		);

		//! @brief Will create a new CheckBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the CheckBox
		//! @param _checked The initial checked state of the checkbox
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createCheckbox(
			ak::UID										_creatorUid,
			const QString &											_text,
			bool													_checked = false
		);

		//! @brief Will create a new ColorEditButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _r The Red-Channel value of the ColorEditButton color
		//! @param _g The Green-Channel value of the ColorEditButton color
		//! @param _b The Blue-Channel value of the ColorEditButton color
		//! @param _a The Alpha-Channel value of the ColorEditButton color
		//! @param _textOverride If provided the RGB-text representation of the provided color will be overwritten by the text provided
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createColorEditButton(
			ak::UID										_creatorUid,
			int														_r,
			int														_g,
			int														_b,
			int														_a,
			const char *											_textOverride
		);

		//! @brief Will create a new ColorEditButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _r The Red-Channel value of the ColorEditButton color
		//! @param _g The Green-Channel value of the ColorEditButton color
		//! @param _b The Blue-Channel value of the ColorEditButton color
		//! @param _a The Alpha-Channel value of the ColorEditButton color
		//! @param _textOverride If provided the RGB-text representation of the provided color will be overwritten by the text provided
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createColorEditButton(
			ak::UID										_creatorUid,
			int														_r,
			int														_g,
			int														_b,
			int														_a = 255,
			const QString &											_textOverride = QString("")
		);

		//! @brief Will create a new ColorEditButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _color The color of the ColorEditButton
		//! @param _textOverride If provided the RGB-text representation of the provided color will be overwritten by the text provided
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createColorEditButton(
			ak::UID										_creatorUid,
			const ak::ui::color &									_color,
			const QString &											_textOverride = QString("")
		);

		//! @brief Will create a new ComboBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createComboBox(
			ak::UID										_creatorUid
		);

		//! @brief Will create a new ComboButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the ComboButton
		//! @param _possibleSelection The items the ComboButton will display when showing the drop-down menu
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createComboButton(
			ak::UID										_creatorUid,
			const char *											_text,
			const std::vector<ak::ui::qt::comboButtonItem> &		_possibleSelection = std::vector<ak::ui::qt::comboButtonItem>()
		);

		//! @brief Will create a new ComboButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the ComboButton
		//! @param _possibleSelection The items the ComboButton will display when showing the drop-down menu
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createComboButton(
			ak::UID										_creatorUid,
			const QString &											_text = QString(""),
			const std::vector<ak::ui::qt::comboButtonItem> &		_possibleSelection = std::vector<ak::ui::qt::comboButtonItem>()
		);

		//! @brief Will create a new ComboButtonItem and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the ComboButtonItem
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createComboButtonItem(
			ak::UID										_creatorUid,
			const char *											_text
		);

		//! @brief Will create a new ComboButtonItem and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the ComboButtonItem
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createComboButtonItem(
			ak::UID										_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new ComboButtonItem and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _icon The icon of the ComboButtonItem
		//! @param _text The text of the ComboButtonItem
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createComboButtonItem(
			ak::UID										_creatorUid,
			const QIcon &											_icon,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new Dock and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the Dock
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createDock(
			ak::UID										_creatorUid,
			const char *											_text
		);

		//! @brief Will create a new Dock and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the Dock
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createDock(
			ak::UID										_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new PropertyGrid and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createPropertyGrid(
			ak::UID										_creatorUid
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createPushButton(
			ak::UID										_creatorUid,
			const char *											_text
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createPushButton(
			ak::UID										_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _icon The icon of the PushButton
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createPushButton(
			ak::UID										_creatorUid,
			const QIcon &											_icon,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new Table and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _rows The initial row count of the Table
		//! @param _columns The initial column count of the Table
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createTable(
			ak::UID										_creatorUid,
			int														_rows,
			int														_columns
		);

		//! @brief Will create a new TabToolBar subcountainer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _parentUid The UID of the parent TabToolBar object (may be a uiManager, TabToolBarPage or TabToolBarGroup)
		//! @param _text The text of the SubContainer
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createTabToolBarSubContainer(
			ak::UID										_creatorUid,
			ak::UID										_parentUid,
			const char *											_text
		);

		//! @brief Will create a new TabToolBar subcountainer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _parentUid The UID of the parent TabToolBar object (may be a uiManager, TabToolBarPage or TabToolBarGroup)
		//! @param _text The text of the SubContainer
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createTabToolBarSubContainer(
			ak::UID										_creatorUid,
			ak::UID										_parentUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new TextEdit and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _initialText The initial text of the TextEdit
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createTextEdit(
			ak::UID										_creatorUid,
			const char *											_initialText
		);

		//! @brief Will create a new TextEdit and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _initialText The initial text of the TextEdit
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createTextEdit(
			ak::UID										_creatorUid,
			const QString &											_initialText = QString("")
		);

		//! @brief Will create a new Tree and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createTree(
			ak::UID										_creatorUid
		);

		//! @brief Will create a new UI manager and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createUiManager(
			ak::UID										_creatorUid
		);

		// ###############################################################################
		
		// object manipulation

		//! @brief Will add the specified object to the specified container
		//! @param _parentUid The container UID to add the object to
		//! @param _objectUid The object UID to add to the container
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void addObjectToContainer(
			ak::UID									_parentUid,
			ak::UID									_objectUid
		);

		//! @brief Will set the specified object as the central widget of the parent object
		//! @param _parentUid The parent object to set the object as central widget
		//! @param _objectUid The UID of the object to set as central widget
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setCentralWidget(
			ak::UID									_parentUid,
			ak::UID									_objectUid
		);

		//! @brief Will set the specified widget as the central widget of the parent object
		//! @param _parentUid The parent object to set the object as central widget
		//! @param _widget The widget to set as central widget
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setCentralWidget(
			ak::UID									_parentUid,
			QWidget *								_widget
		);

		//! @brief Will set the text of the object
		//! @param _objectUid The object to set the text at
		//! @param _text The text to set
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setObjectText(
			ak::UID									_objectUid,
			const char *										_text
		);

		//! @brief Will set the text of the object
		//! @param _objectUid The object to set the text at
		//! @param _text The text to set
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setObjectText(
			ak::UID									_objectUid,
			const QString &										_text
		);

		//! @brief Will set the checked state of the object
		//! @param _objectUid The object to set the checked state at
		//! @param _checked The checked state to set
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setObjectChecked(
			ak::UID									_objectUid,
			bool												_checked
		);

		//! @brief Will set the tristate of the object
		//! @param _objectUid The object to set the tristate at
		//! @param _isTristate The tristate to set
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setObjectTristate(
			ak::UID									_objectUid,
			bool												_isTristate
		);

		//! @brief Will set the filter visible state of the specified object
		//! @param _objectUid The UID of the object the filters visible state should be set
		//! @param _vis If true, then the filter of the object will be visible
		//! @throw ak::Exception if the object UID is invalid
		__declspec(dllexport) void setFilterVisible(
			ak::UID									_objectUid,
			bool												_vis = true
		);

		//! @brief Will set the filter case sensitive state of the specified object
		//! @param _objectUid The UID of the object the filters visible state should be set
		//! @param _vis If true, then the filter of the object will be visible
		//! @throw ak::Exception if the object UID is invalid
		__declspec(dllexport) void setFilterCaseSensitive(
			ak::UID												_objectUid,
			bool												_caseSensitive,
			bool												_refresh = true
		);

		//! @brief Will set the filter refresh on change state of the specified object
		//! @param _objectUid The UID of the object the filters visible state should be set
		//! @param _vis If true, then the filter of the object will be visible
		//! @throw ak::Exception if the object UID is invalid
		__declspec(dllexport) void setFilterRefreshOnChange(
			ak::UID												_objectUid,
			bool												_refreshOnChange
		);

		//! @brief Will set the comboButtonItems to the object
		//! @param _objectUid The object to set the items at
		//! @param _items The items to set
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setObjectItems(
			ak::UID									_objectUid,
			const std::vector<ak::ui::qt::comboButtonItem> &	_items
		);

		//! @brief Will set the color to the object
		//! @param _objectUid The object to set to color at
		//! @param _r The Red-Channel value of the color to set
		//! @param _g The Green-Channel value of the color to set
		//! @param _b The Blue-Channel value of the color to set
		//! @param _a The Alpha-Channel value of the color to set
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setObjectColor(
			ak::UID									_objectUid,
			int													_r,
			int													_g,
			int													_b,
			int													_a = 255
		);

		//! @brief Will set the color to the object
		//! @param _objectUid The object to set to color at
		//! @param _color The color to set
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void setObjectColor(
			ak::UID									_objectUid,
			const ak::ui::color &								_color
		);

		__declspec(dllexport) void setObjectIcon(
			ak::UID									_objectUid,
			const char *										_iconName,
			const char *										_iconSize
		);

		__declspec(dllexport) void setObjectIcon(
			ak::UID									_objectUid,
			const QString &										_iconName,
			const QString &										_iconSize
		);

		__declspec(dllexport) void setObjectIcon(
			ak::UID									_objectUid,
			const QIcon &										_icon
		);

		//! @brief Will set the visible status of the tabToolbar
		//! @param _uiManagerUid The UID of the UI manager the tab toolbar is located at
		//! @param _vis If true, the tab toolBar will be visible
		__declspec(dllexport) void setTabToolBarVisible(
			ak::UID									_uiManagerUid,
			bool												_vis = true
		);

		//! @brief Will append the provided text to the provided object
		//! @param _objectUid The UID of the object
		//! @param _text The text to append
		__declspec(dllexport) void appendText(
			ak::UID									_objectUid,
			const char *										_text
		);

		//! @brief Will append the provided text to the provided object
		//! @param _objectUid The UID of the object
		//! @param _text The text to append
		__declspec(dllexport) void appendText(
			ak::UID									_objectUid,
			const QString &										_text
		);

		//! @brief Will set the auto scroll option for the provided object
			//! @param _objectUid The UID of the requested object
			//! @param _enabled If true, the auto scroll option for the object will be enabled
			//! @throw ak::Exception if the provided object UID is invalid
		__declspec(dllexport) void setAutoScrollToBottomEnabled(
			ak::UID									_objectUid,
			bool									_enabled = true
		);

		//! @brief Will add a new object with the value type Boolean to the property Grid
		//! @param _itemName The name of the property
		//! @param _value The value of the property
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const char *									_itemName,
			bool											_value,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new object with the value type Boolean to the property Grid
		//! @param _itemName The name of the property
		//! @param _value The value of the property
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const QString &									_itemName,
			bool											_value,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new object with the value type Integer to the property Grid
		//! @param _itemName The name of the property
		//! @param _value The value of the property
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const char *									_itemName,
			int												_value,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new object with the value type Integer to the property Grid
		//! @param _itemName The name of the property
		//! @param _value The value of the property
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
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
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const char *									_itemName,
			double											_value,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new object with the value type Double to the property Grid
		//! @param _itemName The name of the property
		//! @param _value The value of the property
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
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
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const char *									_itemName,
			const char *									_value,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new object with the value type String to the property Grid
		//! @param _itemName The name of the property
		//! @param _value The value of the property
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
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
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const char *									_itemName,
			const ak::ui::color &								_value,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new object with the value type Color to the property Grid
		//! @param _itemName The name of the property
		//! @param _value The value of the property
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
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
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const char *									_itemName,
			const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
			const QString &									_selectedValue,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new object with the value type Selection to the property Grid
		//! @param _objectUid The UID of the object to add the property at
		//! @param _itemName The name of the property
		//! @param _selection The possible selection values
		//! @param _selectedValues The currently selected value
		//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
		//! @throw ak::Exception if a object creation failed
		__declspec(dllexport) void addProperty(
			ak::UID								_objectUid,
			const QString &									_itemName,
			const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
			const QString &									_selectedValue,
			bool											_isMultipleValues = false
		);

		//! @brief Will add a new item to the tree widget
		//! @param _objectUid The UID of the object
		//! @param _parentUid The UID of the parent object
		//! @param _text The text of the object to add
		//! @param _textAlignment The text alignment of the very last child
		//! @param _foreColor The foreground color of the very last child
		//! @param _backColor The background color of the very last child
		//! @param _icon The icon to add to the very last child
		__declspec(dllexport) ak::UID addObjectToTree(
			ak::UID								_objectUid,
			ak::UID								_parentUid,
			const QString &									_text = QString(""),
			ak::ui::core::textAlignment						_textAlignment = ak::ui::core::textAlignment::alignLeft,
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
		__declspec(dllexport) ak::UID addObjectToTree(
			ak::UID								_objectUid,
			const QString &									_cmd,
			char											_delimiter = '|',
			ak::ui::core::textAlignment						_textAlignment = ak::ui::core::textAlignment::alignLeft,
			const QIcon  &									_icon = QIcon()
		);

		//! @brief Will set the selected state of the provided item.
		//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
		//! @param _objectUid The UID of the object
		//! @param _itemId The ID of the item
		//! @param _selected The selected state of the item
		//! @throw ak::Exception if the ID is invalid
		__declspec(dllexport) void setItemSelected(
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
		__declspec(dllexport) void setSingleItemSelected(
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
		__declspec(dllexport) void toggleItemSelection(
			ak::UID											_objectUid,
			ak::ID											_itemId
		);

		//! @brief Will deselect all items
		//! @param _objectUid The UID of the object
		__declspec(dllexport) void deselectAllItems(
			ak::UID											_objectUid
		);

		//! @brief Enables or disables the ability to select multiple items
			//! @param _objectUid The UID of the object
			//! @param _multiSelection Specify whether multiple items can be selected
		__declspec(dllexport) void setMultiSelectionEnabled(
			ak::UID											_objectUid,
			bool											_multiSelection = true
		);

		//! @brief Enables or disables the ability to automatically select/deselect the childrens of an item
		//! @param _objectUid The UID of the object
		//! @param _enabled if true, the childs of an item will be selected/deselected automatically
		__declspec(dllexport) void setSelectAndDeselectChildrenEnabled(
			ak::UID											_objectUid,
			bool											_enabled = true
		);

		// ###############################################################################################################################################

		//! @brief Will return the object text of the specified object
		//! @param _objectUid The UID of the object
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) QString getObjectText(
			ak::UID									_objectUid
		);

		//! @brief Will return the object tristate of the specified object
		//! @param _objectUid The UID of the object
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) bool getObjectTristate(
			ak::UID									_objectUid
		);

		//! @brief Will return the object checked state of the specified object
		//! @param _objectUid The UID of the object
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) bool getObjectChecked(
			ak::UID									_objectUid
		);

		//! @brief Will return all selected objects of the specified object
		//! @param _objectUid The object to return the slection from
		__declspec(dllexport) std::vector<ak::ID> getSelectedItems(
			ak::UID									_objectUid
		);

		//! @brief Will return the auto scroll option for the provided object
		//! @param _objectUid The UID of the requested object
		//! @throw ak::Exception if the provided object UID is invalid
		__declspec(dllexport) bool getAutoScrollToBottomEnabled(
			ak::UID									_objectUid
		);

		//! @brief Will return all items from root to specified item as a vector where the first item is the root item
		//! @param _objectUid The UID of the requested tree object
		//! @param _itemId The ID of the requested item
		//! @throw ak::Exception if the provided object UID is invalid
		//! @throw ak::Exception if the provided item ID is invalid
		__declspec(dllexport) std::vector<QString> getTreeItemPath(
			ak::UID									_objectUid,
			ak::ID									_itemId
		);

		//! @brief Will return all items from root to specified item as a string seperated with the provided delimiter where the first item is the root item
		//! @param _objectUid The UID of the requested tree object
		//! @param _itemId The ID of the requested item
		//! @param _delimiter The delimiter between the items
		//! @throw ak::Exception if the provided object UID is invalid
		//! @throw ak::Exception if the provided item ID is invalid
		__declspec(dllexport) QString getTreeItemPathString(
			ak::UID									_objectUid,
			ak::ID									_itemId,
			char									_delimiter = '|'
		);


		// ###############################################################################################################################################

		__declspec(dllexport) std::string toString(
			ak::core::eventType									_eventType
		);

		// ###############################################################################################################################################

		//! @brief Will destroy all objects created by this creator
		//! @param _creatorUid The UID of the creator
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void creatorDestroyed(
			ak::UID									_creatorUid
		);

		//! @brief Will show a message box with the provided main window as parent
		//! @param _uiManagerUid The UI manager that will be the parent for the message box
		//! @param _messega The message to display
		//! @param _title The title of the message box
		__declspec(dllexport) void showMessageBox(
			ak::UID									_uiManagerUid,
			const char *										_message,
			const char *										_title
		);

		//! @brief Will show a message box with the provided main window as parent
		//! @param _uiManagerUid The UI manager that will be the parent for the message box
		//! @param _messega The message to display
		//! @param _title The title of the message box
		__declspec(dllexport) void showMessageBox(
			ak::UID									_uiManagerUid,
			const QString &										_message,
			const QString &										_title
		);

		// ###############################################################################################################################################

		// dock

		//! @brief Will add the dock to the uiManager at the specified dock location
			//! @param _uiManagerUid The UID of the UI manager to add the dock at
			//! @param _dockUid The UID of the existing dock to add to the UI manager
			//! @param _dockLocation The location where to put the dock at
		__declspec(dllexport) void addDock(
			ak::UID									_uiManagerUid,
			ak::UID									_dockUid,
			ak::ui::core::dockLocation							_dockLocation
		);

		//! @brief Will add the dock as a tab to the parent dock
		//! @param _parentUid The UID of the parent dock
		//! @param _dockUid The UID of the dock to tabify
		__declspec(dllexport) void tabifyDock(
			ak::UID									_uiManagerUid,
			ak::UID									_parentUid,
			ak::UID									_dockUid
		);

		//! @brief Will set the current color style used in this API
			//! @param _colorStyle The color style to set
		__declspec(dllexport) void setColorStyle(
			ak::ui::colorStyle *		_colorStyle
		);

		//! @brief Will set the current color style to the default dark color style
		__declspec(dllexport) void setDefaultDarkColorStyle(void);

		//! @brief Will set the current color style to the default bright color style
		__declspec(dllexport) void setDefaultBrightColorStyle(void);

		// ###############################################################################################################################################

		// parameters

		//! @brief Will add the provided search path to the icon manager
		//! @param _path The search path to add
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void addIconSearchPath(
			const char *											_path
		);

		//! @brief Will add the provided search path to the icon manager
		//! @param _path The search path to add
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void addIconSearchPath(
			const QString &											_path
		);

		//! @brief Will remove the provided search path from the icon manager
		//! @param _path The existing search path to remove
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void removeIconSearchPath(
			const char *											_path
		);

		//! @brief Will remove the provided search path from the icon manager
		//! @param _path The existing search path to remove
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) void removeIconSearchPath(
			const QString &											_path
		);

		//! @brief Will generate a new UID and return it
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::UID createUid(void);

		//! @brief Will return the messenger
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::messenger * getMessenger(void);

		//! @brief WIll return the icon manager
		//! @throw ak::Exception if the API is not initialized
		__declspec(dllexport) ak::ui::iconManager * getIconManager(void);

		// ###############################################################################################################################################

		// special

		//! @brief Will run a QApplication to start the event callback routine
		//! The return value will be returned as soon as the application is closing
		__declspec(dllexport) int exec(void);

	} // namespace uiAPI
} // namespace ak