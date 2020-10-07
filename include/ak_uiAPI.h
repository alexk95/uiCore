/*
 * ak_uiAPI.h
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// C++ header
#include <vector>						// vector<>
#include <map>							// map<>

// Qt header
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon
#include <qstringlist.h>				// QStringList

// AK header
#include <ak_core.h>					// message type
#include <ak_ui_core.h>					// object type
#include <ak_ui_color.h>				// color

#include <ak_globalDataTypes.h>			// UID and ID type
#include <ak_ui_qt_comboButtonItem.h>	// comboButtonItem

// Forward declaration
class QSurfaceFormat;

namespace ak {

	// Forward declaration
	class messenger;
	class notifier;
	class uidManager;
	class file;
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
				bool												_createQApplication,
				int													_argc = 0,
				char **												_argv = nullptr,
				ak::messenger *										_messenger = nullptr,
				ak::uidManager *									_uidManager = nullptr,
				ak::ui::iconManager *								_iconManager = nullptr,
				ak::ui::objectManager *								_objectManager = nullptr
			);

			//! @brief Returns true if the API was initialized
			bool isInitialized(void) const;

			//! @brief Will return the messenger
			ak::messenger * messenger(void) const;

			//! @brief Will return the UID manager
			ak::uidManager * uidManager(void) const;

			//! @brief Will return the icon manager
			ak::ui::iconManager * iconManager(void) const;

			//! @brief Will return the object manager
			ak::ui::objectManager * objectManager(void) const;

			//! @brief Will return the color style
			ak::ui::colorStyle * colorStyle(void) const;

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

			//! @breif Will return the default surface format
			QSurfaceFormat * getDefaultSurfaceFormat(void);

			//! @brief Will return the file with the provided UID
			//! @param _fileUid The UID of the file to get, if the provided UID is invalid a new one will be created
			ak::file * getFile(
				ak::UID						_fileUid = ak::invalidUID
			);

			//! @brief Will return the file with the provided UID
			//! @param _fileUid The UID of the file to get
			ak::file * getExistingFile(
				ak::UID						_fileUid
			);

			//! @brief Will delete the provided file
			//! @param _fileUid The UID of the file to delete
			void deleteFile(
				ak::UID						_fileUid
			);

			//! @brief Will delete all files created
			void deleteAllFiles();

		protected:
			QApplication *				my_app;							//! The core application
			bool						my_appIsRunning;				//! True if the core application is already running
			QSurfaceFormat *			my_defaultSurfaceFormat;

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

			ak::uidManager *			my_fileUidManager;				//! The UID manager used for files in this API
			std::map<ak::UID,
				ak::file *>				my_mapFiles;					//! Map containing all files created in this API
			typedef std::map<ak::UID,
				ak::file *>::iterator	my_mapFilesIterator;			//! Iterator used to iterate trough the files

		private:
			apiManager(const apiManager &) = delete;
			apiManager & operator = (const apiManager &) = delete;
		};

		// ###############################################################################################################################################

		//! @brief Will initialize the application
		//! @param _messenger The messenger that will be used in the aplication, if nullptr a new one will be created
		//! @param _uidManager The UID manager that will be used in the aplication, if nullptr a new one will be created
		//! @param _iconManager The icon manager that will be used in the aplication, if nullptr a new one will be created
		//! @param _objectManager The object manager that will be used in the aplication, if nullptr a new one will be created
		//! @throw ak::Exception if failed to create a object
		//! @throw ak::Exception if the API was already initialized
		UICORE_API void ini(
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
		UICORE_API void destroy(void);

		//! @brief Will enable the provided event types to be send
		//! @param _types The types to allow
		UICORE_API void enableEventTypes(
			ak::core::eventType										_types
		);

		//! @brief Will disable the provided event types to be send
		//! @param _types The types to disable
		UICORE_API void disableEventTypes(
			ak::core::eventType										_types
		);

		//! @brief Will return the enabled event types that will be send
		UICORE_API std::vector<ak::core::eventType> enabledEventTypes(void);

		//! @brief Will return the disabled event types that will be send
		UICORE_API std::vector<ak::core::eventType> disabledEventTypes(void);

		//! @brief Will create and return a JSON string representing the UI
		UICORE_API std::string getSettingsJSON(void);

		//! @brief Will setup the UI with the settings provided in the settings JSON string
		//! @param _json The JSON string containing the settings
		//! @throw ak::Exception on syntax mismatch
		UICORE_API void setupSettings(
			const char *											_json
		);

		// ###############################################################################################################################################
		
		// message functions

		//! @brief Will register the notifier at the messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _senderUid The sender UID for which to register the provided notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID registerUidNotifier(
			ak::UID													_senderUid,
			ak::notifier *											_notifier
		);

		//! @brief Will register the notifier at the messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _event The event type for which to register the provided notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID registerEventTypeNotifier(
			ak::core::eventType										_event,
			ak::notifier *											_notifier
		);

		//! @brief Will register the notifier for all messages send at the global messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID registerAllMessagesNotifier(
			ak::notifier *											_notifier
		);

		//! @brief Will send a message to the messanger
		//! @param _senderUid Who sends this message
		//! @param _messageType What type of message is send
		//! @param _message The message itself
		//! @param _info1 Additional information 1
		//! @param _info2 Additional information 2
		//! @throw ak::Exception if the API is not initialized
		//! @throw ak::Exception At any kind of error that may occur in any subroutine call when calling the receivers
		UICORE_API void sendMessage(
			ak::UID													_senderUid,
			ak::core::eventType										_event,
			int														_info1 = 0,
			int														_info2 = 0
		);

		//! @brief Will set the samples count for the default surface formt
		//! @param _count The count of samples
		UICORE_API void setSurfaceFormatDefaultSamplesCount(
			int														_count
		);

		// ###############################################################################################################################################

		// object creation

		//! @brief Will create a new Action and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the action
		//! @param _iconSize The size of the icon to display in the action (requires also the icon name to be provided)
		//! @param _iconName The name of the icon to display in the action (requires also the icon size to be provided)
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createAction(
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
		UICORE_API ak::UID createAction(
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
		UICORE_API ak::UID createAction(
			ak::UID													_craetorUid,
			const QString &											_text,
			const QIcon &											_icon
		);

		//! @brief Will create a new CheckBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the CheckBox
		//! @param _checked The initial checked state of the checkbox
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createCheckbox(
			ak::UID													_creatorUid,
			const char *											_text,
			bool													_checked = false
		);

		//! @brief Will create a new CheckBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the CheckBox
		//! @param _checked The initial checked state of the checkbox
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createCheckbox(
			ak::UID													_creatorUid,
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
		UICORE_API ak::UID createColorEditButton(
			ak::UID													_creatorUid,
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
		UICORE_API ak::UID createColorEditButton(
			ak::UID													_creatorUid,
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
		UICORE_API ak::UID createColorEditButton(
			ak::UID													_creatorUid,
			const ak::ui::color &									_color,
			const QString &											_textOverride = QString("")
		);

		//! @brief Will create a new ComboBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createComboBox(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new ComboButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the ComboButton
		//! @param _possibleSelection The items the ComboButton will display when showing the drop-down menu
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createComboButton(
			ak::UID													_creatorUid,
			const char *											_text,
			const std::vector<ak::ui::qt::comboButtonItem> &		_possibleSelection = std::vector<ak::ui::qt::comboButtonItem>()
		);

		//! @brief Will create a new ComboButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the ComboButton
		//! @param _possibleSelection The items the ComboButton will display when showing the drop-down menu
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createComboButton(
			ak::UID													_creatorUid,
			const QString &											_text = QString(""),
			const std::vector<ak::ui::qt::comboButtonItem> &		_possibleSelection = std::vector<ak::ui::qt::comboButtonItem>()
		);

		//! @brief Will create a new ComboButtonItem and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the ComboButtonItem
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createComboButtonItem(
			ak::UID													_creatorUid,
			const char *											_text
		);

		//! @brief Will create a new ComboButtonItem and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the ComboButtonItem
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createComboButtonItem(
			ak::UID													_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new ComboButtonItem and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _icon The icon of the ComboButtonItem
		//! @param _text The text of the ComboButtonItem
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createComboButtonItem(
			ak::UID													_creatorUid,
			const QIcon &											_icon,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new Dock and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the Dock
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createDock(
			ak::UID													_creatorUid,
			const char *											_text
		);

		//! @brief Will create a new Dock and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the Dock
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createDock(
			ak::UID													_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a log in dialog and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _showSavePassword if true the save password checkbox will be displayed
		//! @param _username The initial username
		//! @param _password The initial password
		UICORE_API ak::UID createLogInDialog(
			ak::UID													_creatorUid,
			bool													_showSavePassword,
			const QString &											_username = QString(),
			const QString &											_password = QString()
		);

		//! @brief Will create a new PropertyGrid and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createPropertyGrid(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createPushButton(
			ak::UID													_creatorUid,
			const char *											_text
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createPushButton(
			ak::UID													_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _icon The icon of the PushButton
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createPushButton(
			ak::UID													_creatorUid,
			const QIcon &											_icon,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new Table and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _rows The initial row count of the Table
		//! @param _columns The initial column count of the Table
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createTable(
			ak::UID													_creatorUid,
			int														_rows,
			int														_columns
		);

		//! @brief Will create a new TabToolBar subcountainer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _parentUid The UID of the parent TabToolBar object (may be a uiManager, TabToolBarPage or TabToolBarGroup)
		//! @param _text The text of the SubContainer
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createTabToolBarSubContainer(
			ak::UID													_creatorUid,
			ak::UID													_parentUid,
			const char *											_text
		);

		//! @brief Will create a new TabToolBar subcountainer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _parentUid The UID of the parent TabToolBar object (may be a uiManager, TabToolBarPage or TabToolBarGroup)
		//! @param _text The text of the SubContainer
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createTabToolBarSubContainer(
			ak::UID													_creatorUid,
			ak::UID													_parentUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new TextEdit and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _initialText The initial text of the TextEdit
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createTextEdit(
			ak::UID													_creatorUid,
			const char *											_initialText
		);

		//! @brief Will create a new TextEdit and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _initialText The initial text of the TextEdit
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createTextEdit(
			ak::UID													_creatorUid,
			const QString &											_initialText = QString("")
		);

		//! @brief Will create a new timer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		UICORE_API ak::UID createTimer(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new Tree and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createTree(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new UI manager and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createUiManager(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new tab view and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API ak::UID createTabView(
			ak::UID													_creatorUid
		);

		// ###############################################################################################################################################
		
		// object setter

		namespace obj {

			//! @brief Will set the alias for the specified object
			//! If a empty alias will be provided the old one will be erased (if exists)
			//! @param _objectUid The UID of the object
			//! @param _alias The alias to set
			//! @throw ak::Exception if the API is not initialized or the object UID is invalid
			UICORE_API void setAlias(
				ak::UID												_objectUid,
				const QString &										_alias
			);

			//! @brief Will add the specified object to the specified container
			//! @param _parentUid The container UID to add the object to
			//! @param _objectUid The object UID to add to the container
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void addObjectToContainer(
				ak::UID												_parentUid,
				ak::UID												_objectUid
			);

			//! @brief Will set the specified object as the central widget of the parent object
			//! @param _parentUid The parent object to set the object as central widget
			//! @param _objectUid The UID of the object to set as central widget
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setCentralWidget(
				ak::UID												_parentUid,
				ak::UID												_objectUid
			);

			//! @brief Will set the specified widget as the central widget of the parent object
			//! @param _parentUid The parent object to set the object as central widget
			//! @param _widget The widget to set as central widget
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setCentralWidget(
				ak::UID												_parentUid,
				QWidget *											_widget
			);

			//! @brief Will set the text of the object
			//! @param _objectUid The object to set the text at
			//! @param _text The text to set
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setText(
				ak::UID												_objectUid,
				const char *										_text
			);

			//! @brief Will set the text of the object
			//! @param _objectUid The object to set the text at
			//! @param _text The text to set
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setText(
				ak::UID												_objectUid,
				const QString &										_text
			);

			//! @brief Will set the checked state of the object
			//! @param _objectUid The object to set the checked state at
			//! @param _checked The checked state to set
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setChecked(
				ak::UID												_objectUid,
				bool												_checked
			);

			//! @brief Will set the tristate of the object
			//! @param _objectUid The object to set the tristate at
			//! @param _isTristate The tristate to set
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setTristate(
				ak::UID												_objectUid,
				bool												_isTristate
			);

			//! @brief Will set the filter visible state of the specified object
			//! @param _objectUid The UID of the object the filters visible state should be set
			//! @param _vis If true, then the filter of the object will be visible
			//! @throw ak::Exception if the object UID is invalid
			UICORE_API void setFilterVisible(
				ak::UID												_objectUid,
				bool												_vis = true
			);

			//! @brief Will set the filter case sensitive state of the specified object
			//! @param _objectUid The UID of the object the filters visible state should be set
			//! @param _vis If true, then the filter of the object will be visible
			//! @throw ak::Exception if the object UID is invalid
			UICORE_API void setFilterCaseSensitive(
				ak::UID												_objectUid,
				bool												_caseSensitive,
				bool												_refresh = true
			);

			//! @brief Will set the filter refresh on change state of the specified object
			//! @param _objectUid The UID of the object the filters visible state should be set
			//! @param _vis If true, then the filter of the object will be visible
			//! @throw ak::Exception if the object UID is invalid
			UICORE_API void setFilterRefreshOnChange(
				ak::UID												_objectUid,
				bool												_refreshOnChange
			);

			//! @brief Will set the comboButtonItems to the object
			//! @param _objectUid The object to set the items at
			//! @param _items The items to set
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setItems(
				ak::UID												_objectUid,
				const std::vector<ak::ui::qt::comboButtonItem> &	_items
			);

			//! @brief Will set the color to the object
			//! @param _objectUid The object to set to color at
			//! @param _r The Red-Channel value of the color to set
			//! @param _g The Green-Channel value of the color to set
			//! @param _b The Blue-Channel value of the color to set
			//! @param _a The Alpha-Channel value of the color to set
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setColor(
				ak::UID												_objectUid,
				int													_r,
				int													_g,
				int													_b,
				int													_a = 255
			);

			//! @brief Will set the object color
			//! @param _objectUid The object to set to color at
			//! @param _color The color to set
			//! @throw ak::Exception if the API is not initialized
			UICORE_API void setColor(
				ak::UID												_objectUid,
				const ak::ui::color &								_color
			);

			//! @brief Will set the object icon
			//! The icon will be imported vias the iconManager
			//! @param _objectUid The UID of the object to set the icon an
			//! @param _iconName The name of the icon to set
			//! @param _iconSize The size of the icon to set
			//! @throw ak::Exception if the object UID is invalid
			//! @throw ak::Exception if the icon does not exist
			UICORE_API void setIcon(
				ak::UID												_objectUid,
				const char *										_iconName,
				const char *										_iconSize
			);

			//! @brief Will set the object icon
			//! The icon will be imported vias the iconManager
			//! @param _objectUid The UID of the object to set the icon an
			//! @param _iconName The name of the icon to set
			//! @param _iconSize The size of the icon to set
			//! @throw ak::Exception if the object UID is invalid
			//! @throw ak::Exception if the icon does not exist
			UICORE_API void setIcon(
				ak::UID												_objectUid,
				const QString &										_iconName,
				const QString &										_iconSize
			);

			//! @brief Will set the object icon
			//! @param _objectUid The UID of the object to set the icon an
			//! @param _icon The icon to set
			//! @throw ak::Exception if the object UID is invalid
			UICORE_API void setIcon(
				ak::UID												_objectUid,
				const QIcon &										_icon
			);

			//! @brief Will set the read only option for the provided object
			//! @param _objectUid The UID of the object to set the option at
			//! @param _readOnly If true, the object will be read only
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void setReadOnly(
				ak::UID												_objectUid,
				bool												_readOnly = true
			);

			//! @brief Will set the visible status of the tabToolbar
			//! @param _uiManagerUid The UID of the UI manager the tab toolbar is located at
			//! @param _vis If true, the tab toolBar will be visible
			//! @throw ak::Exception if the uiManager UID is invalid
			UICORE_API void setTabToolBarVisible(
				ak::UID												_uiManagerUid,
				bool												_vis = true
			);

			//! @brief Will set the tab location of the provided object
			//! @param _location The tab location to set
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void setTabLocation(
				ak::UID											_objectUid,
				ak::ui::core::tabLocation						_location
			);

			//! @brief Will focus the tab at the specified object
			//! @param _objectUid The UID of the object to focus the tab at
			//! @param _tab The tab to focus
			//! @throw ak::Exception if the object UID is invalid or the tab is invalid
			UICORE_API void setTabFocused(
				ak::UID											_objectUid,
				ak::ID											_tab
			);

			//! @brief Will append the provided text to the provided object
			//! @param _objectUid The UID of the object to append the text at
			//! @param _text The text to append
			//! @throw ak::Exception if the object UID is invalid
			UICORE_API void appendText(
				ak::UID												_objectUid,
				const char *										_text
			);

			//! @brief Will append the provided text to the provided object
			//! @param _objectUid The UID of the object to append the text at
			//! @param _text The text to append
			//! @throw ak::Exception if the object UID is invalid
			UICORE_API void appendText(
				ak::UID												_objectUid,
				const QString &										_text
			);

			//! @brief Will set the auto scroll option for the provided object
			//! @param _objectUid The UID of the requested object
			//! @param _enabled If true, the auto scroll option for the object will be enabled
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void setAutoScrollToBottomEnabled(
				ak::UID												_objectUid,
				bool												_enabled = true
			);

			//! @brief Will add a new object with the value type Boolean to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const char *										_itemName,
				bool												_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Boolean to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const QString &										_itemName,
				bool												_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Integer to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const char *										_itemName,
				int													_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Integer to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const QString &										_itemName,
				int													_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Double to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const char *										_itemName,
				double												_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Double to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const QString &										_itemName,
				double												_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type String to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const char *										_itemName,
				const char *										_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type String to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const QString &										_itemName,
				const QString &										_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Color to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const char *										_itemName,
				const ak::ui::color &								_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Color to the property Grid
			//! @param _itemName The name of the property
			//! @param _value The value of the property
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const QString &										_itemName,
				const ak::ui::color &								_value,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Selection to the property Grid
			//! @param _objectUid The UID of the object to add the property at
			//! @param _itemName The name of the property
			//! @param _selection The possible selection values
			//! @param _selectedValues The currently selected value
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const char *										_itemName,
				const std::vector<QString> &						_selection,
				const QString &										_selectedValue,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new object with the value type Selection to the property Grid
			//! @param _objectUid The UID of the object to add the property at
			//! @param _itemName The name of the property
			//! @param _selection The possible selection values
			//! @param _selectedValues The currently selected value
			//! @param _isMultipleValues If true, the setting will be displayed as a multiple values entry for the corresponding type
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void addProperty(
				ak::UID												_objectUid,
				const QString &										_itemName,
				const std::vector<QString> &						_selection,
				const QString &										_selectedValue,
				bool												_isMultipleValues = false
			);

			//! @brief Will add a new item to the tree widget
			//! @param _objectUid The UID of the object
			//! @param _parentId The ID of the parent item, if -1 is provided the item will be created as a root item
			//! @param _text The text of the object to add
			//! @param _textAlignment The text alignment of the very last child
			//! @param _icon The icon to add to the very last child
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API ak::ID createItem(
				ak::UID												_objectUid,
				ak::ID												_parentItemId,
				const QString &										_text = QString(""),
				ak::ui::core::textAlignment							_textAlignment = ak::ui::core::align_alignLeft,
				QIcon												_icon = QIcon()
			);

			//! @brief Will add a new item to the tree widget
			//! @param _objectUid The UID of the object
			//! @param _parentId The ID of the parent item, if -1 is provided the item will be created as a root item
			//! @param _iconName The name of the icon to add to the very last child
			//! @param _iconSize The size of the icon to add to the very last child
			//! @param _text The text of the object to add
			//! @param _textAlignment The text alignment of the very last child
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API ak::ID createItem(
				ak::UID												_objectUid,
				ak::ID												_parentItemId,
				const QString &										_iconName,
				const QString &										_iconSize,
				const QString &										_text = QString(""),
				ak::ui::core::textAlignment							_textAlignment = ak::ui::core::align_alignLeft
			);

			//! @brief Will add new items to the tree according to the provided command
			//! The command consists of the root item and the childs.
			//! The command may look like this:
			//! root|child|child2
			//! In this case the delimiter will be the '|' and the tree will get a root item with the text "root" (if doesnt exist before)
			//! The root gets a child item with the text "child" and the child gets a child with the text "child2", so the tree looks like this:
			//!   root
			//!   |>child
			//!     |->child2
			//! The very last item will also get the alignment, colors and icon set.
			//! Will return the ID of the very last item.
			//! @param _objectUid The UID of the object
			//! @param _cmd The tree command
			//! @param _delimiter The delimiter used in the command which seperates the items
			//! @param _textAlignment The text alignment of the very last child
			//! @param _icon The icon to add to the very last child
			UICORE_API ak::ID createItem(
				ak::UID											_objectUid,
				const QString &									_cmd,
				char											_delimiter = '|',
				ak::ui::core::textAlignment						_textAlignment = ak::ui::core::align_alignLeft,
				const QIcon  &									_icon = QIcon()
			);

			//! @brief Will add new items to the tree according to the provided command
			//! The command consists of the root item and the childs.
			//! The command may look like this:
			//! root|child|child2
			//! In this case the delimiter will be the '|' and the tree will get a root item with the text "root" (if doesnt exist before)
			//! The root gets a child item with the text "child" and the child gets a child with the text "child2", so the tree looks like this:
			//!   root
			//!   |>child
			//!     |->child2
			//! The very last item will also get the alignment, colors and icon set.
			//! Will return the ID of the very last item.
			//! @param _objectUid The UID of the object
			//! @param _cmd The tree command
			//! @param _iconName The name of the icon to add to the very last child
			//! @param _iconSize The size of the icon to add to the very last child
			//! @param _delimiter The delimiter used in the command which seperates the items
			//! @param _textAlignment The text alignment of the very last child
			UICORE_API ak::ID createItem(
				ak::UID											_objectUid,
				const QString &									_cmd,
				const QString &									_iconName,
				const QString &									_iconSize,
				char											_delimiter = '|',
				ak::ui::core::textAlignment						_textAlignment = ak::ui::core::textAlignment::align_alignLeft
			);

			//! @brief Will deselect all items
			//! @param _objectUid The UID of the object
			UICORE_API void deselectAllItems(
				ak::UID											_objectUid
			);

			//! @brief Will create a new tab at the provided object
			//! @param _objectUid The UID of the object to add the tab at
			//! @param _widgetUid The UID of the widget to set as a central widget
			//! @param _title The title of the tab
			//! @param _icon The icon of the tab
			UICORE_API ak::ID addTab(
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
			UICORE_API ak::ID addTab(
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
			UICORE_API ak::ID addTab(
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
			UICORE_API ak::ID addTab(
				ak::UID											_objectUid,
				QWidget *										_widget,
				const QString &									_title,
				const QString &									_iconName,
				const QString &									_iconSize
			);

			//! @brief Enables or disables the ability to select multiple items
				//! @param _objectUid The UID of the object
				//! @param _multiSelection Specify whether multiple items can be selected
			UICORE_API void setMultiSelectionEnabled(
				ak::UID											_objectUid,
				bool											_multiSelection = true
			);

			//! @brief Enables or disables the ability to automatically select/deselect the childrens of an item
			//! @param _objectUid The UID of the object
			//! @param _enabled if true, the childs of an item will be selected/deselected automatically
			UICORE_API void setAutoSelectAndDeselectChildrenEnabled(
				ak::UID											_objectUid,
				bool											_enabled = true
			);

			//! @brief Will set the auto expand selected items option for the specified object
			//! @param _objectUid The UID of the object
			//! @param _enabled If true the option is enabled
			//! @throw ak::Exception if the specified object is invalid
			UICORE_API void setAutoExpandSelectedItems(
				ak::UID											_objectUid,
				bool											_enabled = true
			);

			//! @brief Enables or disables the provided object
			//! @param _objectUid The UID of the object
			//! @param _enabled if true, the object will be enabled
			UICORE_API void setEnabled(
				ak::UID											_objectUid,
				bool											_enabled = true
			);

			//! @brief Shows or hides the provided object
			//! @param _objectUid The UID of the object
			//! @param _visible if true, the object will be visible
			UICORE_API void setVisible(
				ak::UID											_objectUid,
				bool											_visible = true
			);

			//! @brief Will clear the items of the provided object
			//! @param _objectUid The UID of the object to clear its items
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void clear(
				ak::UID											_objectUid
			);

			//! @brief Will delete all provided items from the provided object
			//! @param _objectUid The UID of the object to delete the items from
			//! @param _items The items to delete
			//! @throw ak::Exception if the provided object UID or any item ID is invalid
			UICORE_API void deleteItems(
				ak::UID											_objectUid,
				const std::vector<ak::ID> &						_items
			);

			//! @brief Will call the start function of the provided object with the given value
			//! @param _objectUid The UID of the object
			//! @param _value The value to set to the object
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void start(
				ak::UID												_objectUid,
				int													_value
			);

			//! @brief Will call the stop function of the provided object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void stop(
				ak::UID												_objectUid
			);

			//! @brief Will call the shoot function of the provided object
			//! @param _objectUid The UID of the object
			//! @param _value The value to set to the object
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API void shoot(
				ak::UID												_objectUid,
				int													_value
			);

			//! @brief Will destroy the provided object
			UICORE_API void destroy(
				ak::UID												_objectUid
			);

			// ###############################################################################################################################################

			// Object getter

			//! @brief Will return the alias of the specified object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the API is not initialized or the object UID is invalid
			UICORE_API QString getAlias(
				ak::UID									_objectUid
			);

			//! @brief Will return the object text of the specified object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the API is not initialized
			UICORE_API QString getText(
				ak::UID									_objectUid
			);

			//! @brief Will return the object tristate of the specified object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the API is not initialized
			UICORE_API bool getTristate(
				ak::UID									_objectUid
			);

			//! @brief Will return the object checked state of the specified object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the API is not initialized
			UICORE_API bool getChecked(
				ak::UID									_objectUid
			);

			//! @brief Will return all selected objects of the specified object
			//! @param _objectUid The object to return the slection from
			UICORE_API std::vector<ak::ID> getSelectedItems(
				ak::UID									_objectUid
			);

			//! @brief Will return the auto scroll option for the provided object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided object UID is invalid
			UICORE_API bool getAutoScrollToBottomEnabled(
				ak::UID									_objectUid
			);

			//! @brief Will return the ID of the item at this object with the specified text
			//! Returns invalidID if the item does not exist
			UICORE_API ak::ID getItem(
				ak::UID									_objectUid,
				const QString &							_text
			);

			//! @brief Will return the count of items at the specified object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided UID is invalid or the object type is invalid
			UICORE_API int getItemCount(
				ak::UID									_objectUid
			);

			//! @brief Will return the currently focused tab at the specified object
			//! @param _objectUid The UID of the requested object
			//! @throw ak::Exception if the provided UID is invalid
			UICORE_API int getFocusedTab(
				ak::UID									_objectUid
			);

			//! @brief Will get the auto expand selected items option for the specified object
			//! @param _objectUid The UID of the object
			//! @throw ak::Exception if the specified object is invalid
			UICORE_API bool getAutoExpandSelectedItems(
				ak::UID									_objectUid
			);

		} // namespace obj

		// ###############################################################################################################################################

		namespace itm {

			// Item setter

			//! @brief Will set the selected state of the provided item.
			//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item
			//! @param _selected The selected state of the item
			//! @throw ak::Exception if the ID is invalid
			UICORE_API void setSelected(
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
			UICORE_API void setSingleSelected(
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
			UICORE_API void toggleSelection(
				ak::UID											_objectUid,
				ak::ID											_itemId
			);

			//! @brief Will set the text of the specified tab
			//! @param _tab The tab to set the text at
			//! @param _text The text to set
			//! @throw ak::Exception if the specified tab is invalid
			UICORE_API void setText(
				ak::UID											_objectUid,
				ak::ID											_itemId,
				const QString &									_text
			);

			//! @brief Will set the enabled state of the specified item at the specified object
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item
			//! @param _enabled The enabld state
			//! @throw ak::Exception if the specified UID or ID is invalid
			UICORE_API void setEnabled(
				ak::UID												_objectUid,
				ak::ID												_itemId,
				bool												_enabled = true
			);

			//! @brief Will set the enabled state of the specified item at the specified object
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item
			//! @param _visible The enabld state
			//! @throw ak::Exception if the specified UID or ID is invalid
			UICORE_API void setVisible(
				ak::UID												_objectUid,
				ak::ID												_itemId,
				bool												_visible = true
			);

			// ###############################################################################################################################################

			// Item getter

			//! @brief Will return all items from root to specified item as a vector where the first item is the root item
			//! @param _objectUid The UID of the requested tree object
			//! @param _itemId The ID of the requested item
			//! @throw ak::Exception if the provided object UID is invalid
			//! @throw ak::Exception if the provided item ID is invalid
			UICORE_API std::vector<QString> getPath(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return all items from root to specified item as a string seperated with the provided delimiter where the first item is the root item
			//! @param _objectUid The UID of the requested tree object
			//! @param _itemId The ID of the requested item
			//! @param _delimiter The delimiter between the items
			//! @throw ak::Exception if the provided object UID is invalid
			//! @throw ak::Exception if the provided item ID is invalid
			UICORE_API QString getPathString(
				ak::UID									_objectUid,
				ak::ID									_itemId,
				char									_delimiter = '|'
			);

			//! @brief Will return the text of the specified item at the specified object
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item to get the text of
			UICORE_API QString getText(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the value type of the specified item at the specified object
			//! @param _objectUid The UID of the object
			//! @param _itemId The ID of the item to get the text of
			UICORE_API ak::core::valueType getValueType(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the ID of the specified item in the specified object
			//! @param _objectUid The UID of the requested object
			//! @param _itemPath The path of the requested item
			//! @param _delimiter The delimiter of the path string
			UICORE_API ak::ID getID(
				ak::UID									_objectUid,
				const QString &							_itemPath,
				char									_delimiter = '|'
			);

			//! @brief Will return the boolean value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API bool getValueBoolean(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the color value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API ui::color getValueColor(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the double value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API double getValueDouble(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the integer value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API int getValueInteger(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the boolean value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API std::vector<QString> getValuePossibleSelection(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the selected value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API QString getValueSelection(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the string value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API QString getValueString(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

			//! @brief Will return the boolean value of the specified item
			//! @param _objectUid The UID of the requested object
			//! @param _itemId The ID of the item to get its value from
			//! @throw ak::Exception if the API is not initialized, the UID is invalid or the ID is invalid
			UICORE_API bool getValueIsMultivalued(
				ak::UID									_objectUid,
				ak::ID									_itemId
			);

		} // namespace itm

		// ###############################################################################################################################################

		namespace special {

			//! @brief Will show a message box with the provided main window as parent
			//! @param _uiManagerUid The UI manager that will be the parent for the message box
			//! @param _messega The message to display
			//! @param _title The title of the message box
			UICORE_API void showMessageBox(
				ak::UID									_uiManagerUid,
				const char *										_message,
				const char *										_title
			);

			//! @brief Will show a message box with the provided main window as parent
			//! @param _uiManagerUid The UI manager that will be the parent for the message box
			//! @param _messega The message to display
			//! @param _title The title of the message box
			UICORE_API void showMessageBox(
				ak::UID									_uiManagerUid,
				const QString &										_message,
				const QString &										_title
			);

			//! @brief Will show an open file dialog and return the selected file
			//! @param _uiManagerUid The UID of the parent uiManager that the dialog will be displayed on
			//! @param _caption The caption
			//! @param _initialDir The initial search directory
			//! @param _filter The filter of the open file dialog
			//! @param _selectedFilter The selected filter
			UICORE_API QString openFileDialog(
				ak::UID												_uiManagerUid,
				const QString &										_caption,
				const QString &										_initialDir = QString(""),
				const QString &										_filter = QString(""),
				QString *											_selectedFilter = nullptr
			);

			//! @brief Will show an open file dialog and return the selected file
			//! @param _uiManagerUid The UID of the parent uiManager that the dialog will be displayed on
			//! @param _caption The caption
			//! @param _initialDir The initial search directory
			//! @param _filter The filter of the open file dialog
			//! @param _selectedFilter The selected filter
			UICORE_API QString saveFileDialog(
				ak::UID												_uiManagerUid,
				const QString &										_caption,
				const QString &										_initialDir = QString(""),
				const QString &										_filter = QString(""),
				QString *											_selectedFilter = nullptr
			);

			//! @brief Will create a text representation of the event send
			UICORE_API QString createEventText(
				ak::UID												_sender,
				ak::core::eventType									_event,
				int													_info1,
				int													_info2
			);

		}

		// ###############################################################################################################################################

		// File

		namespace file {

			// File setter

			//! @brief Will load informations from the specifiied file
			UICORE_API ak::UID load(
				const QString &										_filePath
			);

			//! @brief Will load informations from the specified file
			//! @param _fileUid The UID of the file
			//! @param _filePath If provided this path will be set as current file path
			UICORE_API void load(
				ak::UID												_fileUid,
				const QString &										_filePath = QString("")
			);

			//! @brief Will save the current set lines to the current set file path
			//! @param _fileUid The UID of the file
			//! @param _append If true, the file will be opened on append mode
			UICORE_API void save(
				ak::UID												_fileUid,
				bool												_append = false
			);

			//! @brief Will save the current set lines to the provided file path
			//! @param _fileUid The UID of the file
			//! @param _filePath The fule path to set as current file
			//! @param _append If true, the file will be opened on append mode
			UICORE_API void save(
				ak::UID												_fileUid,
				const QString &										_filePath,
				bool												_append = false
			);

			//! @brief Will set the current path for the file
			//! @param _fileUid The UID of the file
			//! @param _path The file path to set
			UICORE_API void setPath(
				ak::UID												_fileUid,
				const QString &										_path
			);

			//! @brief Will set the current lines for the file
			//! @param _fileUid The UID of the file
			//! @param _lines The lines to set
			UICORE_API void setLines(
				ak::UID												_fileUid,
				const QStringList &									_lines
			);

			//! @brief Will append the provided line to the current lines
			//! @param _fileUid The UID of the file
			//! @param _line The line to add
			UICORE_API void addLine(
				ak::UID												_fileUid,
				const QString &										_line
			);

			//! @brief Will append the provided lines to the current lines
			//! @param _fileUid The UID of the file
			//! @param _lines The lines to add
			UICORE_API void addLine(
				ak::UID												_fileUid,
				const QStringList &									_lines
			);

			// #######################################################################################################

			// File Getter

			//! @brief Will return the files UID
			//! @param _fileUid The UID of the file
			UICORE_API ak::UID uid(
				ak::UID												_fileUid
			);

			//! @brief Will return the files name
			//! @param _fileUid The UID of the file
			UICORE_API QString name(
				ak::UID												_fileUid
			);

			//! @brief Will return the files path
			//! @param _fileUid The UID of the file
			UICORE_API QString path(
				ak::UID												_fileUid
			);

			//! @brief Will return the files extension
			//! @param _fileUid The UID of the file
			UICORE_API QString extension(
				ak::UID												_fileUid
			);

			//! @brief Will return the lines in this file
			//! @param _fileUid The UID of the file
			UICORE_API QStringList lines(
				ak::UID												_fileUid
			);

			//! @brief Will return the count of the lines in this file
			//! @param _fileUid The UID of the file
			UICORE_API int linesCount(
				ak::UID												_fileUid
			);

			//! @brief Will return true if the file has changed after it was loaded or saved the last time
			//! @param _fileUid The UID of the file
			UICORE_API bool hasChanged(
				ak::UID												_fileUid
			);

		} // namespace file

		// ###############################################################################################################################################

		// Dialogs

		namespace dialog {

			//! @brief Will show the dialog with the provided UID
			//! @param _dialogUid The UID of the dialog
			//! @throw ak::Exception if the provided UID is invalid
			UICORE_API ak::ui::core::dialogResult show(
				ak::UID												_dialogUid
			);

			//! @brief Will return the result of the specified dialog
			//! @param _dialogUid The UID of the dialog to show
			//! @throw ak::Exception if the provided UID is invalid
			UICORE_API ak::ui::core::dialogResult result(
				ak::UID												_dialogUid
			);

			//! @brief Will return the username of the specified dialog
			//! Allowerd dialogs: logIn
			//! @param _dialogUid The UID of the dialog to return the username from
			//! @throw ak::Exception if the provided UID is invalid
			UICORE_API QString username(
				ak::UID												_dialogUid
			);

			//! @brief Will return the password of the specified dialog
			//! Allowerd dialogs: logIn
			//! @param _dialogUid The UID of the dialog to return the password from
			//! @throw ak::Exception if the provided UID is invalid
			UICORE_API QString password(
				ak::UID												_dialogUid
			);

			//! @brief Will return true if the save password option is checked at the specified dialog
			//! Allowed dialogs: logIn
			//! @param _dialogUid The UID of the dialog to return the save password option from
			//! @throw ak::Exception if the provided UID is invalid
			UICORE_API bool savePassword(
				ak::UID												_dialogUid
			);

			//! @brief Will close the specified dialog
			//! @param _dialogUid The UID of the dialog
			//! @param _result The dialog result to set to the dialog before closing
			//! @throw ak::Exception if the provided UID is invalid
			UICORE_API void close(
				ak::UID												_dialogUid,
				ak::ui::core::dialogResult							_result = ui::core::resultNone
			);

		} // namespace dialog

		// ###############################################################################################################################################

		//! @brief Will return a string representation of the provided eventType
		//! @param _type The event type that should be represented
		UICORE_API QString toString(
			ak::core::eventType									_type
		);

		//! @brief Will return a string representation of the provided valueType
		//! @param _type The value type that should be represented
		UICORE_API QString toString(
			ak::core::valueType									_type
		);

		//! @brief Will return a string representation of the provided valtextAlignmentueType
		//! @param _type The text alignment that should be represented
		UICORE_API QString toString(
			ak::ui::core::textAlignment							_type
		);

		//! @brief Will return a string representation of the provided dockLocation
		//! @param _type The dock location that should be represented
		UICORE_API QString toString(
			ak::ui::core::dockLocation							_dockLocation
		);

		//! @brief Will return a string representation of the provided tab location
		//! @param _type The tab location that should be represented
		UICORE_API QString toString(
			ak::ui::core::tabLocation							_tabLocation
		);

		//! @brief Create a string representation of the provided dialog result
		//! @param _dialogResult The dialog result to be converted
		UICORE_API QString toQString(
			ak::ui::core::dialogResult							_dialogResult
		);

		//! @brief Will return a string representation of the provided keyType
		//! @param _type The key type that should be represented
		UICORE_API QString toString(
			ak::ui::core::keyType								_type
		);

		//! @brief Will return a string representation of the provided objectType
		//! @param _type The object type that should be represented
		UICORE_API QString toString(
			ak::ui::core::objectType							_type
		);

		// ###############################################################################################################################################

		//! @brief Will destroy all objects created by this creator
		//! @param _creatorUid The UID of the creator
		//! @throw ak::Exception if the API is not initialized
		UICORE_API void creatorDestroyed(
			ak::UID									_creatorUid
		);

		//! @brief Will set the current color style used in this API
		//! @param _colorStyle The color style to set
		UICORE_API void setColorStyle(
			ak::ui::colorStyle *		_colorStyle
		);

		//! @brief Will return the current color style
		UICORE_API ak::ui::colorStyle * getColorStyle(void);

		//! @brief Will set the current color style to the default dark color style
		UICORE_API void setDefaultDarkColorStyle(void);

		//! @brief Will set the current color style to the default bright color style
		UICORE_API void setDefaultBrightColorStyle(void);

		//! @brief Will close the provided uiManager
		//! @param _uiManagerUid The UID of the uiManager to close
		//! @throw ak::Exception if the provided UID is invalid
		UICORE_API void closeWindow(
			ak::UID												_uiManagerUid
		);

		//! @brief Will set the visible state of the status label of the provided uiManager.
		//! @param _uiManagerUid The UID of the UI manager
		//! @param _visible If true the status label will be visible
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API void setStatusLabelVisible(
			ak::UID									_uiManagerUid,
			bool									_visible = true
		);

		//! @brief Will set the visible state of the status progress bar of the provided uiManager.
		//! @param _uiManagerUid The UID of the UI manager
		//! @param _visible If true the status progress bar will be visible
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API void setStatusProgressVisible(
			ak::UID									_uiManagerUid,
			bool									_visible = true
		);

		//! @brief Will set the status label text at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @param _text The text to set
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API void setStatusLabelText(
			ak::UID									_uiManagerUid,
			const QString &							_text
		);

		//! @brief Will set the value of the status progress bar at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @param _value The value to set (0 - 100)
		//! @throw ak::Exception if the provided UI manager UID is invalid or the provided value is out of range
		UICORE_API void setStatusProgressValue(
			ak::UID									_uiManagerUid,
			int										_value
		);

		//! @brief Will set the continuous option of the status progress bar at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @param _continuous If true the status progress bar will be displayed as a continuous bar
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API void setStatusProgressContinuous(
			ak::UID									_uiManagerUid,
			bool									_continuous = true
		);

		//! @brief Will return true if the status label is visible at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API bool getStatusLabelVisible(
			ak::UID									_uiManagerUid
		);

		//! @brief Will return true if the status progress bar is visible at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API bool getStatusProgressVisible(
			ak::UID									_uiManagerUid
		);

		//! @brief Will return the status label text at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API QString getStatusLabelText(
			ak::UID									_uiManagerUid
		);

		//! @brief Will return the status progress bar value at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API int getStatusProgressValue(
			ak::UID									_uiManagerUid
		);

		//! @brief Will return the continuous option of the status progress bar at the provided uiManager
		//! @param _uiManagerUid The UID of the UI manager
		//! @throw ak::Exception if the provided UI manager UID is invalid
		UICORE_API bool getStatusProgressContinuous(
			ak::UID									_uiManagerUid
		);

		// ###############################################################################################################################################

		// dock

		//! @brief Will add the dock to the uiManager at the specified dock location
			//! @param _uiManagerUid The UID of the UI manager to add the dock at
			//! @param _dockUid The UID of the existing dock to add to the UI manager
			//! @param _dockLocation The location where to put the dock at
		UICORE_API void addDock(
			ak::UID									_uiManagerUid,
			ak::UID									_dockUid,
			ak::ui::core::dockLocation							_dockLocation
		);

		//! @brief Will add the dock as a tab to the parent dock
		//! @param _parentUid The UID of the parent dock
		//! @param _dockUid The UID of the dock to tabify
		UICORE_API void tabifyDock(
			ak::UID									_uiManagerUid,
			ak::UID									_parentUid,
			ak::UID									_dockUid
		);

		//! @brief Will set the dock priority on the bottom left corner.
			//! The dock with the priority will expand into the corner.
			//! @param _uiManagerUid The UID of the UI manager
			//! @param _dockLocation The dock location to set the priority for (allowed dockBottom and dockLeft)
			//! @throw ak::Exception if the uiManager UID or the dock location is invalid
		UICORE_API void setDockBottomLeftPriority(
			ak::UID									_uiManagerUid,
			ak::ui::core::dockLocation				_dockLocation
		);

		//! @brief Will set the dock priority on the bottom left corner.
		//! The dock with the priority will expand into the corner.
		//! @param _uiManagerUid The UID of the UI manager
		//! @param _dockLocation The dock location to set the priority for (allowed dockBottom and dockRight)
		//! @throw ak::Exception if the uiManager UID or the dock location is invalid
		UICORE_API void setDockBottomRightPriority(
			ak::UID									_uiManagerUid,
			ak::ui::core::dockLocation				_dockLocation
		);

		// ###############################################################################################################################################

		// parameters

		//! @brief Will add the provided search path to the icon manager
		//! @param _path The search path to add
		//! @throw ak::Exception if the API is not initialized
		UICORE_API void addIconSearchPath(
			const char *											_path
		);

		//! @brief Will add the provided search path to the icon manager
		//! @param _path The search path to add
		//! @throw ak::Exception if the API is not initialized
		UICORE_API void addIconSearchPath(
			const QString &											_path
		);

		//! @brief Will remove the provided search path from the icon manager
		//! @param _path The existing search path to remove
		//! @throw ak::Exception if the API is not initialized
		UICORE_API void removeIconSearchPath(
			const char *											_path
		);

		//! @brief Will remove the provided search path from the icon manager
		//! @param _path The existing search path to remove
		//! @throw ak::Exception if the API is not initialized
		UICORE_API void removeIconSearchPath(
			const QString &											_path
		);

		//! @brief Will generate a new UID and return it
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::UID createUid(void);

		//! @brief Will return the messenger
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::messenger * getMessenger(void);

		//! @brief WIll return the icon manager
		//! @throw ak::Exception if the API is not initialized
		UICORE_API ak::ui::iconManager * getIconManager(void);

		// ###############################################################################################################################################

		// special

		//! @brief Will run a QApplication to start the event callback routine
		//! The return value will be returned as soon as the application is closing
		UICORE_API int exec(void);

		std::vector<ak::ui::qt::comboButtonItem> toComboButtonItem(
			const std::vector<QString> &							_items
		);

		//! @brief Used only for debug purporse (no functionallity)
		UICORE_API void testCall(void);

	} // namespace uiAPI
} // namespace ak