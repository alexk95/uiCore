/*
 * ak_uiAPI.h
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
#include <vector>						// vector<>
#include <map>							// map<>

// Qt header
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon
#include <qstringlist.h>				// QStringList
#include <qsize.h>

// AK header
#include <ak_core.h>					// message type
#include <ak_ui_core.h>					// object type
#include <ak_ui_color.h>				// color
#include <ak_ui_application.h>

#include <ak_globalDataTypes.h>			// UID and ID type
#include <ak_ui_qt_comboButtonItem.h>	// comboButtonItem

// Forward declaration
class QSurfaceFormat;
class QWidget;
class QMovie;

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
		class windowEventHandler;
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
				ak::messenger *										_messenger = nullptr,
				ak::uidManager *									_uidManager = nullptr,
				ak::ui::iconManager *								_iconManager = nullptr,
				ak::ui::objectManager *								_objectManager = nullptr
			);

			//! @brief Returns true if the API was initialized
			bool isInitialized(void) const;

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

			ui::application * app();

			QDesktopWidget * desktop();

		protected:
			ui::application *			my_app;							//! The core application
			bool						my_appIsRunning;				//! True if the core application is already running
			QSurfaceFormat *			my_defaultSurfaceFormat;
			QDesktopWidget *			my_desktop;

			bool						my_objManagerIsExtern;			//! If true, then the object manager was created externally
			bool						my_messengerIsExtern;			//! If true, then the messenger was created externally
			bool						my_uidManagerIsExtern;			//! If true, then the UID manager was created externally
			bool						my_iconManagerIsExtern;			//! If true, then the icon manager was created externally
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
		UICORE_API_EXPORT void ini(
			ak::messenger *											_messenger = nullptr,
			ak::uidManager *										_uidManager = nullptr,
			ak::ui::iconManager *									_iconManager = nullptr,
			ak::ui::objectManager *									_objectManager = nullptr
		);

		//! @brief Will destroy all objects created by this API
		UICORE_API_EXPORT void destroy(void);

		//! @brief Will enable the provided event types to be send
		//! @param _types The types to allow
		UICORE_API_EXPORT void enableEventTypes(
			ak::core::eventType										_types
		);

		//! @brief Will disable the provided event types to be send
		//! @param _types The types to disable
		UICORE_API_EXPORT void disableEventTypes(
			ak::core::eventType										_types
		);

		//! @brief Will set the enabled state of the messenger
		//! @param _enabled The enabled state to set. If false, no messages will be sent
		UICORE_API_EXPORT void setMessengerEnabled(
			bool													_enabled
		);

		//! @brief Will return the current enabled state for the messenger
		UICORE_API_EXPORT bool messengerIsEnabled(void);

		//! @brief Will return the enabled event types that will be send
		UICORE_API_EXPORT std::vector<ak::core::eventType> enabledEventTypes(void);

		//! @brief Will return the disabled event types that will be send
		UICORE_API_EXPORT std::vector<ak::core::eventType> disabledEventTypes(void);

		//! @brief Will create and return a JSON string representing the UI
		UICORE_API_EXPORT std::string saveStateWindow(
			const std::string &										_applicationVersion
		);

		//! @brief Will create and return a JSON string representing the currently set ColorStyle
		UICORE_API_EXPORT std::string saveStateColorStyle(
			const std::string &										_applicationVersion
		);

		//! @brief Will setup the UI with the settings provided in the settings JSON string
		//! @param _json The JSON string containing the settings
		UICORE_API_EXPORT ak::core::settingsRestoreErrorCode restoreStateWindow(
			const std::string &										_json,
			const std::string &										_applicationVersion
		);

		//! @brief Will restore tthe color style from the settings JSON string
		//! @param _json The JSON string containing the settings
		UICORE_API_EXPORT ak::core::settingsRestoreErrorCode restoreStateColorStyle(
			const std::string &										_json,
			const std::string &										_applicationVersion
		);

		//! @brief Will return the QWidget of the specifed object
		//! @param _objectUid The UID of the object
		UICORE_API_EXPORT QWidget * getWidget(
			ak::UID													_objectUid
		);

		//! @brief Will return the creator UID of the specified object
		//! @param _objectUID The UID of the object to get its cretor from
		UICORE_API_EXPORT ak::UID getObjectCreator(
			ak::UID												_objectUID
		);

		// ###############################################################################################################################################
		
		// message functions

		//! @brief Will register the notifier at the messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _senderUid The sender UID for which to register the provided notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID registerUidNotifier(
			ak::UID													_senderUid,
			ak::notifier *											_notifier
		);

		//! @brief Will register the notifier at the messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _event The event type for which to register the provided notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID registerEventTypeNotifier(
			ak::core::eventType										_event,
			ak::notifier *											_notifier
		);

		//! @brief Will register the notifier for all messages send at the global messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID registerAllMessagesNotifier(
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
		UICORE_API_EXPORT void sendMessage(
			ak::UID													_senderUid,
			ak::core::eventType										_event,
			int														_info1 = 0,
			int														_info2 = 0
		);

		//! @brief Will set the samples count for the default surface formt
		//! @param _count The count of samples
		UICORE_API_EXPORT void setSurfaceFormatDefaultSamplesCount(
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
		UICORE_API_EXPORT ak::UID createAction(
			ak::UID													_creatorUid,
			const QString &											_text,
			const QString &											_iconName,
			const QString &											_iconSize = QString("32")
		);

		//! @brief Will create a new Action and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the action
		//! @param _icon The icon of the action
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID createAction(
			ak::UID													_craetorUid,
			const QString &											_text,
			const QIcon &											_icon
		);

		//! @brief Will create a new CheckBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the CheckBox
		//! @param _checked The initial checked state of the checkbox
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID createCheckbox(
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
		UICORE_API_EXPORT ak::UID createColorEditButton(
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
		UICORE_API_EXPORT ak::UID createColorEditButton(
			ak::UID													_creatorUid,
			const ak::ui::color &									_color,
			const QString &											_textOverride = QString("")
		);

		//! @brief Will create a new ComboBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID createComboBox(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new ComboButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the ComboButton
		//! @param _possibleSelection The items the ComboButton will display when showing the drop-down menu
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID createComboButton(
			ak::UID													_creatorUid,
			const QString &											_text = QString(""),
			const std::vector<QString> &							_possibleSelection = std::vector<QString>()
		);

		//! @brief Will create a defaultWelcomeScreen and return its UID
		//! @param _creatorUid The UID of the creator
		UICORE_API_EXPORT ak::UID createWelcomeScreen(
			ak::UID												_creatorUid
		);

		//! @brief Will create a new Dock and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the Dock
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createDock(
			ak::UID													_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a line edit and return its UID
		//! param _creatorUid The UID of the creator
		//! @param _initialText The initial text to display
		UICORE_API_EXPORT ak::UID createLineEdit(
			ak::UID													_creatorUid,
			const QString &											_initialText = QString("")
		);

		//! @brief Will create a log in dialog and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _showSavePassword if true the save password checkbox will be displayed
		//! @param _username The initial username
		//! @param _password The initial password
		UICORE_API_EXPORT ak::UID createLogInDialog(
			ak::UID													_creatorUid,
			bool													_showSavePassword,
			const QString &											_imageName,
			const QString &											_username = QString(),
			const QString &											_password = QString()
		);

		//! @brief Will create a nice line edit
		//! @param _initialText The initial text of the edit field
		//! @param _infoLabelText The initial text of the information label
		UICORE_API_EXPORT ak::UID createNiceLineEdit(
			ak::UID												_creatorUid,
			const QString &										_initialText,
			const QString &										_infoLabelText
		);

		//! @brief Will create a new PropertyGrid and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createPropertyGrid(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createPushButton(
			ak::UID													_creatorUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _icon The icon of the PushButton
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createPushButton(
			ak::UID													_creatorUid,
			const QIcon &											_icon,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new options dialog and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		UICORE_API_EXPORT ak::UID createOptionsDialog(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new Table and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _rows The initial row count of the Table
		//! @param _columns The initial column count of the Table
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createTable(
			ak::UID													_creatorUid,
			int														_rows,
			int														_columns
		);

		//! @brief Will create a new TabToolBar subcountainer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _parentUid The UID of the parent TabToolBar object (may be a uiManager, TabToolBarPage or TabToolBarGroup)
		//! @param _text The text of the SubContainer
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createTabToolBarSubContainer(
			ak::UID													_creatorUid,
			ak::UID													_parentUid,
			const QString &											_text = QString("")
		);

		//! @brief Will create a new TextEdit and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _initialText The initial text of the TextEdit
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createTextEdit(
			ak::UID													_creatorUid,
			const QString &											_initialText = QString("")
		);

		//! @brief Will create a new timer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		UICORE_API_EXPORT ak::UID createTimer(
			ak::UID													_creatorUid
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		UICORE_API_EXPORT ak::UID createToolButton(
			ak::UID													_creatorUid
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _text The initial text of the toolButton
		UICORE_API_EXPORT ak::UID createToolButton(
			ak::UID													_creatorUid,
			const QString &											_text
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _text The initial text of the toolButton
		//! @param _icon The initial icon of the toolButton
		UICORE_API_EXPORT ak::UID createToolButton(
			ak::UID													_creatorUid,
			const QString &											_text,
			const QIcon &											_icon
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _text The initial text of the toolButton
		//! @param _iconName The name of the initial icon for the toolButton
		//! @param _iconFolder The folder of the initial icon for the toolButton
		UICORE_API_EXPORT ak::UID createToolButton(
			ak::UID													_creatorUid,
			const QString &											_text,
			const QString &											_iconName,
			const QString &											_iconFolder
		);

		//! @brief Will create a new Tree and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createTree(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new tab view and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createTabView(
			ak::UID													_creatorUid
		);

		//! @brief Will create a new UI manager and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT ak::UID createWindow(
			ak::UID													_creatorUid
		);

		// Object creation

		// ###############################################################################################################################################

		// Action

		namespace action {

			//! @brief Will set the enabled state of the action
			//! @param _actionUID The UID of the action
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				ak::UID												_actionUID,
				bool												_enabled
			);

			//! @brief Will set the text of the action
			//! @param _actionUID The UID of the action
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				ak::UID												_actionUID,
				const QString &										_text
			);

			//! @brief Will set the icon of the action
			//! @param _actionUID The UID of the action
			//! @param _icon The icon to set
			UICORE_API_EXPORT void setIcon(
				ak::UID												_actionUID,
				const QIcon &										_icon
			);

			//! @brief Will set the icon of the action
			//! @param _actionUID The UID of the action
			//! @param _iconName The name of the icon
			//! @param _iconFolder The folder the icon is located at
			UICORE_API_EXPORT void setIcon(
				ak::UID												_actionUID,
				const QString &										_iconName,
				const QString &										_iconFolder
			);

			//! @brief Will return the text of the action
			//! @param _actionUID The UID of the action
			UICORE_API_EXPORT QString getText(
				ak::UID												_actionUID
			);

		} // namespace action

		// ###############################################################################################################################################

		// Container

		namespace container {

			//! @brief Will add an object to the specified container
			//! @param _containerUID The UID of the container
			//! @param _objectUID The UID of the object to add
			UICORE_API_EXPORT void addObject(
				ak::UID												_containerUID,
				ak::UID												_objectUID
			);

			//! @brief Will set the enabled state of the specified container
			//! @param _containerUID The UID of the container
			//! @param _isEnabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				ak::UID												_containerUID,
				bool												_isEnabled = true
			);

			//! @brief Will return the enabled state of the specified container
			//! @param _containerUID The UID of the container
			UICORE_API_EXPORT bool isEnabled(
				ak::UID												_containerUID
			);
		}

		// ###############################################################################################################################################

		// Context menu

		namespace contextMenu {

			//! @brief Will add a context menu item and return its ID
			//! @param _textEditUID The UID of the text edit
			//! @param _text The text of the new item
			//! @param _role The role of the item, if none a signal will be emitted, otherwise the corresponding action will be performed
			UICORE_API_EXPORT ak::ID addItem(
				ak::UID							_textEditUID,
				const QString &					_text,
				ui::core::contextMenuRole		_role = ui::core::contextMenuRole::crNone
			);

			//! @brief Will add a context menu item and return its ID
			//! @param _textEditUID The UID of the text edit
			//! @param _icon The icon of the new item
			//! @param _text The text of the new item
			//! @param _role The role of the item, if none a signal will be emitted, otherwise the corresponding action will be performed
			UICORE_API_EXPORT ak::ID addItem(
				ak::UID							_textEditUID,
				const QIcon &					_icon,
				const QString &					_text,
				ui::core::contextMenuRole		_role = ui::core::contextMenuRole::crNone
			);

			//! @brief Will add a context menu item and return its ID
			//! @param _textEditUID The UID of the text edit
			//! @param _icon The icon of the new item
			//! @param _text The text of the new item
			//! @param _role The role of the item, if none a signal will be emitted, otherwise the corresponding action will be performed
			UICORE_API_EXPORT ak::ID addItem(
				ak::UID							_textEditUID,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconSize,
				ui::core::contextMenuRole		_role = ui::core::contextMenuRole::crNone
			);

			//! @brief Will add a sperator at the context menu of the textEdit
			//! @param _textEditUID The UID of the text edit
			UICORE_API_EXPORT void addSeparator(
				ak::UID							_textEditUID
			);

			//! @brief Will remove all context menu items from the context menu
			//! @param _textEditUID The UID of the text edit
			UICORE_API_EXPORT void clear(
				ak::UID							_textEditUID
			);

		}

		// ###############################################################################################################################################

		// Dialog

		namespace dialog {

			//! @brief Will show an open directory dialog and return the selected directory
			UICORE_API_EXPORT QString openDirectory(const QString & _title, const QString & _initialDir);

		}

		// ###############################################################################################################################################

		// Dock

		namespace dock {

			//! @brief Will set the widget as central widget of the dock
			//! @param _dockUID The UID of the dock
			//! @param _widgetUID The UID of the widget to set as central widget
			UICORE_API_EXPORT void setCentralWidget(
				ak::UID												_dockUID,
				ak::UID												_widgetUID
			);

			//! @brief Will set the provided widget as central widget of the dock
			//! @param _dockUID The UID of the dock
			//! @param _widget The widget to set as central widget
			UICORE_API_EXPORT void setCentralWidget(
				ak::UID												_dockUID,
				QWidget *											_widget
			);

			//! @brief Will set the visible state of the specified dock
			//! @param _dockUID The UID of the dock
			//! @param _visible The visible state to set
			UICORE_API_EXPORT void setVisible(
				ak::UID												_dockUID,
				bool												_visible = true
			);

			//! @brief Will return the currently set visible state of the 
			UICORE_API_EXPORT bool isVisible(
				ak::UID												_dockUID
			);

		}

		// ###############################################################################################################################################

		// File

		namespace file {

			// File setter

			//! @brief Will load informations from the specifiied file
			UICORE_API_EXPORT ak::UID load(
				const QString &										_filePath
			);

			//! @brief Will load informations from the specified file
			//! @param _fileUid The UID of the file
			//! @param _filePath If provided this path will be set as current file path
			UICORE_API_EXPORT void load(
				ak::UID												_fileUid,
				const QString &										_filePath = QString("")
			);

			//! @brief Will save the current set lines to the current set file path
			//! @param _fileUid The UID of the file
			//! @param _append If true, the file will be opened on append mode
			UICORE_API_EXPORT void save(
				ak::UID												_fileUid,
				bool												_append = false
			);

			//! @brief Will save the current set lines to the provided file path
			//! @param _fileUid The UID of the file
			//! @param _filePath The fule path to set as current file
			//! @param _append If true, the file will be opened on append mode
			UICORE_API_EXPORT void save(
				ak::UID												_fileUid,
				const QString &										_filePath,
				bool												_append = false
			);

			//! @brief Will set the current path for the file
			//! @param _fileUid The UID of the file
			//! @param _path The file path to set
			UICORE_API_EXPORT void setPath(
				ak::UID												_fileUid,
				const QString &										_path
			);

			//! @brief Will set the current lines for the file
			//! @param _fileUid The UID of the file
			//! @param _lines The lines to set
			UICORE_API_EXPORT void setLines(
				ak::UID												_fileUid,
				const QStringList &									_lines
			);

			//! @brief Will append the provided line to the current lines
			//! @param _fileUid The UID of the file
			//! @param _line The line to add
			UICORE_API_EXPORT void addLine(
				ak::UID												_fileUid,
				const QString &										_line
			);

			//! @brief Will append the provided lines to the current lines
			//! @param _fileUid The UID of the file
			//! @param _lines The lines to add
			UICORE_API_EXPORT void addLine(
				ak::UID												_fileUid,
				const QStringList &									_lines
			);

			// #######################################################################################################

			// File Getter

			//! @brief Will return the files name
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QString name(
				ak::UID												_fileUid
			);

			//! @brief Will return the files path
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QString path(
				ak::UID												_fileUid
			);

			//! @brief Will return the files extension
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QString extension(
				ak::UID												_fileUid
			);

			//! @brief Will return the lines in this file
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QStringList lines(
				ak::UID												_fileUid
			);

			//! @brief Will return the count of the lines in this file
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT int linesCount(
				ak::UID												_fileUid
			);

			//! @brief Will return true if the file has changed after it was loaded or saved the last time
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT bool hasChanged(
				ak::UID												_fileUid
			);

		} // namespace file

		// ###############################################################################################################################################

		// Line edit

		namespace lineEdit {
			
			//! @brief Will set the line edit text
			//! @param _lineEditUid The UID of the line edit
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				ak::UID												_lineEditUid,
				const QString &										_text
			);

			//! @brief Will set the line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				ak::UID												_lineEditUid,
				bool												_enabled
			);

			//! @brief Will return the line edit text
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT QString text(
				ak::UID												_lineEditUid
			);

			//! @brief Will return the line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT bool isEnabled(
				ak::UID												_lineEditUid
			);
		}

		// ###############################################################################################################################################

		// Log in dialog

		namespace logInDialog {

			UICORE_API_EXPORT ui::core::dialogResult showDialog(
				ak::UID												_dialogUID
			);

			UICORE_API_EXPORT void close(
				ak::UID												_dialogUID,
				ui::core::dialogResult								_result = ui::core::dialogResult::resultNone
			);

			UICORE_API_EXPORT QString getUsername(
				ak::UID												_dialogUID
			);

			UICORE_API_EXPORT QString getPassword(
				ak::UID												_dialogUID
			);

			UICORE_API_EXPORT bool getSavePassword(
				ak::UID												_dialogUID
			);

			//! @brief Will display a invalid log in popup message at this dialogs username input
			//! @param _text The text to set as a tooltip
			UICORE_API_EXPORT void showToolTipAtUsername(
				ak::UID												_dialogUID,
				const QString &										_text
			);

			//! @brief Will display a invalid log in popup message at this dialogs password input
			//! @param _text The text to set as a tooltip
			UICORE_API_EXPORT void showToolTipAtPassword(
				ak::UID												_dialogUID,
				const QString &										_text
			);

			UICORE_API_EXPORT void showToolTipAtCustomField(
				ak::UID												_dialogUID,
				ak::ID												_inputID,
				const QString &										_text
			);

			UICORE_API_EXPORT ak::ID addCustomInputField(
				ak::UID												_dialogUID,
				const QString &										_labelText,
				const QString &										_inputFieldInitialText = QString("")
			);

			UICORE_API_EXPORT QString getCustomFieldText(
				ak::UID												_dialogUID,
				ak::ID												_fieldID
			);

		} // namespace logInDialog

		// ###############################################################################################################################################

		// Nice line edit

		namespace niceLineEdit {

			//! @brief Will set the nice line edit text
			//! @param _lineEditUid The UID of the line edit
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				ak::UID												_lineEditUid,
				const QString &										_text
			);

			//! @brief Will set the nice line edit info label text
			//! @param _lineEditUid The UID of the line edit
			//! @param _text The text to set
			UICORE_API_EXPORT void setInfoLabelText(
				ak::UID												_lineEditUid,
				const QString &										_text
			);

			//! @brief Will set the nice line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				ak::UID												_lineEditUid,
				bool												_enabled
			);

			//! @brief Will return the nice line edit text
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT QString text(
				ak::UID												_lineEditUid
			);

			//! @brief Will return the nice line edit info label text
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT QString infoLabelText(
				ak::UID												_lineEditUid
			);

			//! @brief Will return the nice line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT bool isEnabled(
				ak::UID												_lineEditUid
			);
		}

		// ###############################################################################################################################################

		// Object

		namespace object {

			//! @brief Will destroy the specifed object
			//! @param _objectUID The UID of the object to destroy
			//! @param _ignoreIfObjectHasChildObjects If true, will not destroy the specified object if it still has child objects
			UICORE_API_EXPORT void destroy(
				ak::UID												_objectUID,
				bool												_ignoreIfObjectHasChildObjects = false
			);

			UICORE_API_EXPORT void setAlias(
				ak::UID												_objectUID,
				const QString &										_alias
			);

			//! @brief Will set an object name to the specified object so it can be accessed later.
			//! The name must be unique!
			//! @param _objectUID The UID of the object to set the name at
			//! @param _name The name to set
			UICORE_API_EXPORT void setObjectUniqueName(
				ak::UID												_objectUID,
				const QString &										_name
			);

			//! @brief Will return the UID of the object with the specified name
			//! Will return ak::invalidUID if there is no object with the specified name
			//! @param _name The name of the object
			UICORE_API_EXPORT ak::UID getUidFromObjectUniqueName(
				const QString &										_name
			);

		}

		// ###############################################################################################################################################

		// Prompt

		namespace promptDialog {

			UICORE_API_EXPORT ak::ui::core::dialogResult show(
				const QString &				_message,
				const QString &				_title,
				ak::ui::core::promptType	_type
			);

			UICORE_API_EXPORT ak::ui::core::dialogResult show(
				const QString &				_message,
				const QString &				_title
			);

		} // namespace prompt

		// ###############################################################################################################################################

		// PropertyGrid

		namespace propertyGrid {

			UICORE_API_EXPORT void addGroup(
				ak::UID											_propertyGridUID,
				const QString &									_groupName
			);

			UICORE_API_EXPORT void addGroup(
				ak::UID											_propertyGridUID,
				const QColor &									_color,
				const QString &									_groupName
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				bool											_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const ui::color &								_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				double											_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				int												_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const std::vector<QString> &					_possibleSelection,
				const QString &									_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const char *									_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const QString &									_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				bool											_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const ui::color &								_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				double											_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				int												_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const std::vector<QString> &					_possibleSelection,
				const QString &									_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const char *									_value
			);

			UICORE_API_EXPORT ak::ID addItem(
				ak::UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const QString &									_value
			);

			//! @brief Will clear the property grid
			//! @param _keepGroups The groups will be cleared but not deleted
			UICORE_API_EXPORT void clear(
				ak::UID											_propertyGridUID,
				bool											_keepGroups = false
			);

			UICORE_API_EXPORT bool getItemIsReadOnly(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT QString getItemGroup(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT bool getItemIsMultipleValues(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT QString getItemName(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT std::vector<QString> getItemPossibleSelection(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT bool getItemValueBool(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT ui::color getItemValueColor(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT double getItemValueDouble(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT int getItemValueInteger(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT QString getItemValueSelection(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);
			
			UICORE_API_EXPORT QString getItemValueString(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT ak::core::valueType getItemValueType(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			UICORE_API_EXPORT void setItemIsReadOnly(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID,
				bool											_readOnly = true
			);

			UICORE_API_EXPORT void setGroupStateIcons(
				ak::UID											_propertyGridUID,
				const QIcon &									_groupExpanded,
				const QIcon &									_groupCollapsed
			);

			UICORE_API_EXPORT void setGroupStateIcons(
				ak::UID											_propertyGridUID,
				const QString &									_groupExpandedIconName,
				const QString &									_groupExpandedIconSize,
				const QString &									_groupCollapsedIconName,
				const QString &									_groupCollapsedIconSize
			);

			//! @brief Will set the provided value to the specified item
			//! @param _propertyGridUID The UID of the property grid
			//! @param _itemID The ID of the item
			//! @param _valueToReset The value to set to the item
			UICORE_API_EXPORT void resetItemAsError(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID,
				const QString &									_valueToReset
			);

			//! @brief Will set the provided value to the specified item
			//! @param _propertyGridUID The UID of the property grid
			//! @param _itemID The ID of the item
			//! @param _valueToReset The value to set to the item
			UICORE_API_EXPORT void resetItemAsError(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID,
				int												_valueToReset
			);

			//! @brief Will set the provided value to the specified item
			//! @param _propertyGridUID The UID of the property grid
			//! @param _itemID The ID of the item
			//! @param _valueToReset The value to set to the item
			UICORE_API_EXPORT void resetItemAsError(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID,
				double											_valueToReset
			);

			//! @brief Will show the specified item as an error
			//! @param _propertyGridUID The UID of the property grid
			//! @param _itemID The item to set as error
			UICORE_API_EXPORT void showItemAsError(
				ak::UID											_propertyGridUID,
				ak::ID											_itemID
			);

			//! @brief Will set the property grids enabeld state
			//! @param _propertyGridUID The UID of the property grid
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				ak::UID											_propertyGridUID,
				bool											_enabled
			);

			//! @brief Will return the property grids enabeld state
			//! @param _propertyGridUID The UID of the property grid
			UICORE_API_EXPORT bool isEnabled(
				ak::UID											_propertyGridUID
			);

		}

		// ###############################################################################################################################################

		// TabView

		namespace tabView {

			UICORE_API_EXPORT ak::ID addTab(
				ak::UID				_tabViewUID,
				ak::UID				_widgetUID,
				const QString &		_title
			);

			UICORE_API_EXPORT ak::ID addTab(
				ak::UID				_tabViewUID,
				ak::UID				_widgetUID,
				const QString &		_title,
				const QString &		_iconName,
				const QString &		_iconFolder
			);

			UICORE_API_EXPORT ak::ID addTab(
				ak::UID				_tabViewUID,
				ak::UID				_widgetUID,
				const QString &		_title,
				const QIcon &		_icon
			);

			UICORE_API_EXPORT ak::ID addTab(
				ak::UID				_tabViewUID,
				QWidget *			_widget,
				const QString &		_title
			);

			UICORE_API_EXPORT ak::ID addTab(
				ak::UID				_tabViewUID,
				QWidget *			_widget,
				const QString &		_title,
				const QString &		_iconName,
				const QString &		_iconFolder
			);

			UICORE_API_EXPORT ak::ID addTab(
				ak::UID				_tabViewUID,
				QWidget *			_widget,
				const QString &		_title,
				const QIcon &		_icon
			);

			//! @brief Will close all tabs of the specified tabView
			UICORE_API_EXPORT void closeAllTabs(
				ak::UID				_tabViewUID
			);

			UICORE_API_EXPORT void closeTab(
				ak::UID				_tabViewUID,
				ak::ID				_tabID
			);

			UICORE_API_EXPORT ak::ID getFocusedTab(
				ak::UID				_tabViewUID
			);

			UICORE_API_EXPORT bool getTabsCloseable(
				ak::UID				_tabViewUID
			);

			UICORE_API_EXPORT QString getTabText(
				ak::UID				_tabViewUID,
				ak::ID				_tabID
			);

			UICORE_API_EXPORT void setEnabled(
				ak::UID				_tabViewUID,
				bool				_enabled
			);

			UICORE_API_EXPORT void setTabbarLocation(
				ak::UID								_tabViewUID,
				ak::ui::core::tabLocation			_location
			);

			UICORE_API_EXPORT void setTabFocused(
				ak::UID				_tabViewUID,
				ak::ID				_tabID
			);

			//! @brief Will set the closeabled state for the tabs in this tabView
			//! @param _closeable If true the tabs can be closed by the user
			UICORE_API_EXPORT void setTabsCloseable(
				ak::UID								_tabViewUID,
				bool								_closeable
			);

			//! @brief Will set the text of the specified tab
			//! @param _tabViewUID The UID of the tab view
			//! @param _tab The tab to set the text at
			//! @param _text The text to set
			UICORE_API_EXPORT void setTabText(
				ak::UID								_tabViewUID,
				ak::ID								_tab,
				const QString &						_text
			);

			UICORE_API_EXPORT void setVisible(
				ak::UID				_tabViewUID,
				bool				_visible
			);

			UICORE_API_EXPORT void setObjectName(
				ak::UID							_tabViewUID,
				const QString &					_name
			);

		}

		// ###############################################################################################################################################

		// TextEdit

		namespace textEdit {

			UICORE_API_EXPORT void appendText(
				ak::UID				_textEditUID,
				const QString &		_text
			);

			UICORE_API_EXPORT void clear(
				ak::UID				_textEditUID
			);

			UICORE_API_EXPORT void setAutoScrollToBottomEnabled(
				ak::UID				_textEditUID,
				bool				_enabled = true
			);

			UICORE_API_EXPORT void setReadOnly(
				ak::UID				_textEditUID,
				bool				_readOnly = true
			);

			UICORE_API_EXPORT void setText(
				ak::UID				_textEditUID,
				const QString &		_text
			);

		}

		// ###############################################################################################################################################

		// Timer

		namespace timer {

			UICORE_API_EXPORT int getInterval(
				ak::UID							_timerUID
			);

			UICORE_API_EXPORT void setInterval(
				ak::UID							_timerUID,
				int								_interval
			);

			UICORE_API_EXPORT void shoot(
				ak::UID							_timerUID
			);

			UICORE_API_EXPORT void shoot(
				ak::UID							_timerUID,
				int								_interval
			);

			UICORE_API_EXPORT void start(
				ak::UID							_timerUID
			);

			UICORE_API_EXPORT void start(
				ak::UID							_timerUID,
				int								_interval
			);

			UICORE_API_EXPORT void stop(
				ak::UID							_timerUID
			);

		} // namespace timer

		// ###############################################################################################################################################

		// ToolButton

		namespace toolButton {

			//! @brief Will return the current enabled state of the toolButton
			UICORE_API_EXPORT bool getIsEnabled(
				ak::UID							_toolButtonUID
			);

			//! @brief Will get the text of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT QString getText(
				ak::UID							_toolButtonUID
			);

			//! @brief Will get the current ToolTip of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT QString getToolTip(
				ak::UID							_toolButtonUID
			);

			//! @brief Will set the enabled state of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _enabled If true, the toolButton will be enabled
			UICORE_API_EXPORT void setEnabled(
				ak::UID							_toolButtonUID,
				bool							_enabled = true
			);

			//! @brief Will set the text of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				ak::UID							_toolButtonUID,
				const QString &					_text
			);

			//! @brief Will set the ToolTip of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text to set
			UICORE_API_EXPORT void setToolTip(
				ak::UID							_toolButtonUID,
				const QString &					_text
			);

			//! @brief Will set the icon of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _icon The icon to set
			UICORE_API_EXPORT void setIcon(
				ak::UID							_toolButtonUID,
				const QIcon &					_icon
			);

			//! @brief Will set the icon of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _iconName The name of the icon to set
			//! @param _iconFolder The folder of the icon to set
			UICORE_API_EXPORT void setIcon(
				ak::UID							_toolButtonUID,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			//! @brief Will add a new menu item to the menu
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text of the new item
			UICORE_API_EXPORT ak::ID addMenuItem(
				ak::UID							_toolButtonUID,
				const QString &					_text
			);

			//! @brief Will add a new menu item to the menu
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text of the new item
			UICORE_API_EXPORT ak::ID addMenuItem(
				ak::UID							_toolButtonUID,
				const QIcon &					_icon,
				const QString &					_text
			);

			//! @brief Will add a new menu item to the menu
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text of the new item
			UICORE_API_EXPORT ak::ID addMenuItem(
				ak::UID							_toolButtonUID,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			//! @brief Will add a menu seperator to the menu
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT void addMenuSeperator(
				ak::UID							_toolButtonUID
			);

			//! @brief Will clear the menu
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT void clearMenu(
				ak::UID							_toolButtonUID
			);

			//! @brief Will set the checked state of the specified menu item
			//! @param _toolButtonUID The UID of the tool button
			//! @param _itemID The ID of the item
			//! @param _checked The checked state to set
			UICORE_API_EXPORT void setMenuItemChecked(
				ak::UID							_toolButtonUID,
				ak::ID							_itemID,
				bool							_checked = true
			);

			//! @brief Will disable the ability to check and uncheck the item (can be reenabled with setChecked)
			//! @param _toolButtonUID The UID of the tool button
			//! @param _itemID The ID of the item
			UICORE_API_EXPORT void setMenuItemNotCheckable(
				ak::UID							_toolButtonUID,
				ak::ID							_itemID
			);

			//! @brief Will return the text of the specified menu item
			//! @param _itemID The ID of the menu item
			UICORE_API_EXPORT QString getMenuItemText(
				ak::UID							_toolButtonUID,
				ak::ID							_itemID
			);

		} // namespace toolButton

		// ###############################################################################################################################################

		// Tree

		namespace tree {

			//! @brief Will add a new item to the specified tree widget and return its ID
			//! @param _treeUID The UID of the tree
			//! @param _text The text of the item
			//! @param _parentUid The ID of the parent item. If ID is -1, the item will be added as top level item
			//! @param _textAlignment The text alignment
			UICORE_API_EXPORT ak::ID addItem(
				ak::UID							_treeUID,
				const QString &					_text,
				ak::ID							_parentId = -1,
				ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::align_alignLeft				
			);

			//! @brief Will add a new item to the tree widget and return its ID
			//! @param _treeUID The UID of the tree
			//! @param _text The text of the item
			//! @param _parentUid The ID of the parent item. If ID is -1, the item will be added as top level item
			//! @param _textAlignment The text alignment
			//! @param _icon The icon for the item
			UICORE_API_EXPORT ak::ID addItem(
				ak::UID							_treeUID,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconSize,
				ak::ID							_parentId = -1,
				ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::align_alignLeft
			);

			//! @brief Will add new items to the tree according to the provided item path
			//! The item path consists of the root item and the childs.
			//! The item path may look like this:
			//! root|child|child2
			//! In this case the delimiter will be the '|' and the tree will get a root item with the text "root" (if doesnt exist before)
			//! The root gets a child item with the text "child" and the child gets a child with the text "child2", so the tree looks like this:
			//! ->root
			//! ->->child
			//! ->->->child2
			//! The very last item will also get the alignment, colors and icon set.
			//! Will return the ID of the very last item.
			//! @param _treeUID The UID of the tree
			//! @param _itemPath The item path
			//! @param _delimiter The delimiter used in the command which seperates the items
			//! @param _textAlignment The text alignment
			UICORE_API_EXPORT ak::ID addItem(
				ak::UID							_treeUID,
				const QString &					_itemPath,
				char							_delimiter,
				ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::align_alignLeft
			);

			//! @brief Will add new items to the tree according to the provided item path
			//! The item path consists of the root item and the childs.
			//! The item path may look like this:
			//! root|child|child2
			//! In this case the delimiter will be the '|' and the tree will get a root item with the text "root" (if doesnt exist before)
			//! The root gets a child item with the text "child" and the child gets a child with the text "child2", so the tree looks like this:
			//! ->root
			//! ->->child
			//! ->->->child2
			//! The very last item will also get the alignment, colors and icon set.
			//! Will return the ID of the very last item.
			//! @param _treeUID The UID of the tree
			//! @param _cmd The item path
			//! @param _delimiter The delimiter used in the command which seperates the items
			//! @param _textAlignment The text alignment
			UICORE_API_EXPORT ak::ID addItem(
				ak::UID							_treeUID,
				const QString &					_itemPath,
				char							_delimiter,
				const QString &					_iconName,
				const QString &					_iconSize,
				ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::align_alignLeft
			);

			UICORE_API_EXPORT void clear(
				ak::UID							_treeUID
			);

			UICORE_API_EXPORT void collapseAllItems(
				ak::UID							_treeUID
			);

			UICORE_API_EXPORT void deleteItem(
				ak::UID							_treeUID,
				ak::ID							_itemID
			);

			UICORE_API_EXPORT void deleteItems(
				ak::UID							_treeUID,
				const std::vector<ak::ID> &		_itemIDs
			);

			//! @brief Will set the items are editable flag
			//! @param _editable If true, the items can be modified by the user
			//! @param _applyToAll If true, then the new state will be applied to all existing items, otherwise this chane will only affect items created after this point
			UICORE_API_EXPORT void setItemsAreEditable(
				ak::UID							_treeUID,
				bool							_editable = true,
				bool							_applyToAll = true
			);

			//! @brief Will set the editable flag of one item
			//! @param _item The item id
			//! @param _editable If true, the item can be modified by the user
			UICORE_API_EXPORT void setItemIsEditable(
				ak::UID							_treeUID,
				ak::ID							_itemID,
				bool							_editable = true
			);

			//! @brief Will set the editable flag of the provided items item
			//! @param _item The item id
			//! @param _editable If true, the item can be modified by the user
			UICORE_API_EXPORT void setItemIsEditable(
				ak::UID							_treeUID,
				const std::vector<ak::ID> &		_itemIDs,
				bool							_editable = true
			);

			UICORE_API_EXPORT void deselectAllItems(
				ak::UID							_treeUID,
				bool							_sendSelectionChangedEvent = true
			);

			UICORE_API_EXPORT void expandAllItems(
				ak::UID							_treeUID
			);

			UICORE_API_EXPORT ak::ID getItemID(
				ak::UID							_treeUID,
				const QString &					_itemPath,
				char							_delimiter
			);

			UICORE_API_EXPORT std::vector<QString> getItemPath(
				ak::UID							_treeUID,
				ak::ID							_itemID
			);

			UICORE_API_EXPORT QString getItemPathString(
				ak::UID							_treeUID,
				ak::ID							_itemID,
				char							_delimiter = '|'
			);

			UICORE_API_EXPORT QString getItemText(
				ak::UID							_treeUID,
				ak::ID							_itemID
			);

			UICORE_API_EXPORT bool isEnabled(
				ak::UID							_treeUID
			);

			UICORE_API_EXPORT int itemCount(
				ak::UID							_treeUID
			);

			UICORE_API_EXPORT std::vector<ak::ID> selectedItems(
				ak::UID							_treeUID
			);

			UICORE_API_EXPORT void setAutoSelectAndDeselectChildrenEnabled(
				ak::UID							_treeUID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT void setEnabled(
				ak::UID							_treeUID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT void setFilterCaseSensitiveEnabled(
				ak::UID							_treeUID,
				bool							_caseSensitiveEnabled = true
			);

			UICORE_API_EXPORT void setFilterRefreshOnChangeEnabled(
				ak::UID							_treeUID,
				bool							_refreshOnChangeEnabled
			);

			UICORE_API_EXPORT void setFilterVisible(
				ak::UID							_treeUID,
				bool							_visible = true
			);
		
			UICORE_API_EXPORT void setItemIcon(
				ak::UID							_treeUID,
				ak::ID							_itemID,
				const QIcon &					_icon
			);

			UICORE_API_EXPORT void setItemIcon(
				ak::UID							_treeUID,
				ak::ID							_itemID,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			UICORE_API_EXPORT void setItemEnabled(
				ak::UID							_treeUID,
				ak::ID							_itemID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT void setItemSelected(
				ak::UID							_treeUID,
				ak::ID							_itemID,
				bool							_selected = true,
				bool							_singleSelection = false
			);

			UICORE_API_EXPORT void setItemVisible(
				ak::UID							_treeUID,
				ak::ID							_itemID,
				bool							_visible = true
			);

			//! @brief Will set the text of the provided item
			//! @param _itemId The ID of the item
			//! @param _text The text to set
			UICORE_API_EXPORT void setItemText(
				ak::UID							_treeUID,
				ak::ID							_itemId,
				const QString &					_text
			);

			UICORE_API_EXPORT void setMultiSelectionEnabled(
				ak::UID							_treeUID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT void setVisible(
				ak::UID							_treeUID,
				bool							_visible = true
			);

			UICORE_API_EXPORT void toggleItemSelection(
				ak::UID							_treeUID,
				ak::ID							_itemID
			);

		} // namespace tree

		// ###############################################################################################################################################

		// welcomeScreen

		namespace welcomeScreen {

			UICORE_API_EXPORT void addItem(
				ak::UID							_welcomeScreen,
				ak::ID							_group,
				const QString &					_text
			);

			UICORE_API_EXPORT void addItem(
				ak::UID							_welcomeScreen,
				ak::ID							_group,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			UICORE_API_EXPORT void addItem(
				ak::UID							_welcomeScreen,
				ak::ID							_group,
				const QString &					_text,
				const QIcon &					_icon
			);

			UICORE_API_EXPORT void addItemAtReecent(
				ak::UID							_welcomeScreen,
				const QString &					_text
			);

			UICORE_API_EXPORT void addItemAtReecent(
				ak::UID							_welcomeScreen,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			UICORE_API_EXPORT void addItemAtReecent(
				ak::UID							_welcomeScreen,
				const QString &					_text,
				const QIcon &					_icon
			);

			UICORE_API_EXPORT void addItemAtOpen(
				ak::UID							_welcomeScreen,
				const QString &					_text
			);

			UICORE_API_EXPORT void addItemAtOpen(
				ak::UID							_welcomeScreen,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			UICORE_API_EXPORT void addItemAtOpen(
				ak::UID							_welcomeScreen,
				const QString &					_text,
				const QIcon &					_icon
			);

			UICORE_API_EXPORT void addItemAtNew(
				ak::UID							_welcomeScreen,
				const QString &					_text
			);

			UICORE_API_EXPORT void addItemAtNew(
				ak::UID							_welcomeScreen,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			UICORE_API_EXPORT void addItemAtNew(
				ak::UID							_welcomeScreen,
				const QString &					_text,
				const QIcon &					_icon
			);

			UICORE_API_EXPORT QString getItemText(
				ak::UID							_welcomeScreenUID,
				ak::ID							_group,
				ak::ID							_itemID
			);

			UICORE_API_EXPORT QString getItemTextAtRecent(
				ak::UID							_welcomeScreenUID,
				ak::ID							_itemID
			);

			UICORE_API_EXPORT QString getItemTextAtOpen(
				ak::UID							_welcomeScreenUID,
				ak::ID							_itemID
			);

			UICORE_API_EXPORT QString getItemTextAtNew(
				ak::UID							_welcomeScreenUID,
				ak::ID							_itemID
			);

			UICORE_API_EXPORT QString getGroupName(
				ak::UID							_welcomeScreenUID,
				ak::ID							_group
			);

			UICORE_API_EXPORT void setObjectName(
				ak::UID							_welcomeScreenUID,
				const QString &					_name
			);

		} // namespace welcomeScreen

		// ###############################################################################################################################################

		// Window

		namespace window {

			//! @brief Will add the provided event handler to the window
			//! @param _windowUID The UID of the window
			//! @param _eventHandler The event handler to add
			UICORE_API_EXPORT void addEventHandler(
				ak::UID												_windowUID,
				ui::windowEventHandler *							_eventHandler
			);

			//! @brief Will close the provided window
			//! @param _windowUID The UID of the uiManager to close
			UICORE_API_EXPORT void close(
				ak::UID												_windowUID
			);

			//! @brief Will remove the provided event handler from the window
			//! @param _windowUID The UID of the window
			//! @param _eventHandler The event handler to remove
			UICORE_API_EXPORT void removeEventHandler(
				ak::UID												_windowUID,
				ui::windowEventHandler *							_eventHandler
			);

			//! @brief Will set the visible state of the status label of the provided window.
			//! @param _windowUID The UID of the window
			//! @param _visible If true the status label will be visible
			UICORE_API_EXPORT void setStatusLabelVisible(
				ak::UID												_windowUID,
				bool												_visible = true,
				bool												_delayed = true
			);

			//! @brief Will set the visible state of the status progress bar of the provided window.
			//! @param _windowUID The UID of the window
			//! @param _visible If true the status progress bar will be visible
			UICORE_API_EXPORT void setStatusProgressVisible(
				ak::UID												_windowUID,
				bool												_visible = true,
				bool												_delayed = true
			);

			//! @brief Will set the status label text at the provided window
			//! @param _windowUID The UID of the window
			//! @param _text The text to set
			UICORE_API_EXPORT void setStatusLabelText(
				ak::UID												_windowUID,
				const QString &										_text
			);

			//! @brief Will set the value of the status progress bar at the provided window
			//! @param _windowUID The UID of the window
			//! @param _value The value to set (0 - 100)
			UICORE_API_EXPORT void setStatusProgressValue(
				ak::UID												_windowUID,
				int													_value
			);

			//! @brief Will set the continuous option of the status progress bar at the provided window
			//! @param _windowUID The UID of the window
			//! @param _continuous If true the status progress bar will be displayed as a continuous bar
			UICORE_API_EXPORT void setStatusProgressContinuous(
				ak::UID												_windowUID,
				bool												_continuous = true
			);

			//! @brief Will set the visible state of the tabToolbar in the specified window
			//! @param _windowUID The UID of the window
			//! @param _visible If true the tabToolbar will be visible
			UICORE_API_EXPORT void setTabToolBarVisible(
				ak::UID												_windowUID,
				bool												_visible = true
			);

			//! @brief Will return the current tab of the tab tool bar at the specified window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT ak::ID getCurrentTabToolBarTab(
				ak::UID												_windowUID
			);

			//! @brief Will return the tab count of the tab tool bar at the specified window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT int getTabToolBarTabCount(
				ak::UID												_windowUID
			);

			//! @brief Will set the specified tab toolbar tab as current at the specified window
			//! @param _windowUID The UID of the window
			//! @param _tabID The tab to set as current
			UICORE_API_EXPORT void setCurrentTabToolBarTab(
				ak::UID												_windowUID,
				ak::ID												_tabID
			);

			//! @brief Will set the title at the specified window
			//! @param _windowUID The UID of the window
			//! @param _title The title to set
			UICORE_API_EXPORT void setTitle(
				ak::UID												_windowUID,
				const QString &										_title
			);

			//! @brief Will return true if the status label is visible at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT bool getStatusLabelVisible(
				ak::UID												_windowUID
			);

			//! @brief Will return true if the status progress bar is visible at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT bool getStatusProgressVisible(
				ak::UID												_windowUID
			);

			//! @brief Will return the status label text at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT QString getStatusLabelText(
				ak::UID												_windowUID
			);

			//! @brief Will return the status progress bar value at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT int getStatusProgressValue(
				ak::UID												_windowUID
			);

			//! @brief Will return the continuous option of the status progress bar at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT bool getStatusProgressContinuous(
				ak::UID												_windowUID
			);

			//! @brief Will add a dock at the specified location to the window
			//! @param _windowUID The UID of the window
			//! @param _dockUid The UID of the dock to add
			//! @param _dockLocation The dock location to add the dock at
			UICORE_API_EXPORT void addDock(
				ak::UID												_windowUID,
				ak::UID												_dockUid,
				ak::ui::core::dockLocation							_dockLocation
			);

			//! @brief Will tabify two docks
			//! The parent dock will be raised
			//! @param _windowUID The UID of the window
			//! @param _parentDockUid The UID of the parent dock
			//! @param _dockUid The UID of the dock to add as a tabbed dock
			UICORE_API_EXPORT void tabifyDock(
				ak::UID												_windowUID,
				ak::UID												_parentDockUid,
				ak::UID												_dockUid
			);

			//! @brief Will set the dock priority in the bottom left corner
			//! @param _windowUID The UID of the window
			//! @param _dockLocation The dock location that will have the higher priority
			UICORE_API_EXPORT void setDockBottomLeftPriority(
				ak::UID												_windowUID,
				ak::ui::core::dockLocation							_dockLocation
			);

			//! @brief Will set the dock priority in the bottom right corner
			//! @param _windowUID The UID of the window
			//! @param _dockLocation The dock location that will have the higher priority
			UICORE_API_EXPORT void setDockBottomRightPriority(
				ak::UID												_windowUID,
				ak::ui::core::dockLocation							_dockLocation
			);

			//! @brief Will set the windows central widget (The widget will be added to a container, the actual central widget differs)
			//! @param _windowUID The UID of the window
			//! @param _widgetUID The widget UID of the widget to set as central widget
			UICORE_API_EXPORT void setCentralWidget(
				ak::UID												_windowUID,
				ak::UID												_widgetUID
			);

			//! @brief Will set the windows central widget (The widget will be added to a container, the actual central widget differs)
			//! @param _windowUID The UID of the window
			//! @param _widget The widget to set as central widget
			UICORE_API_EXPORT void setCentralWidget(
				ak::UID												_windowUID,
				QWidget *											_widget
			);

			//! @brief Will show the window as maximized
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT void showMaximized(
				ak::UID												_windowUID
			);

			//! @brief Will show the window as minimized
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT void showMinimized(
				ak::UID												_windowUID
			);

			//! @brief Will return the size of the window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT QSize size(
				ak::UID												_windowUID
			);

			//! @brief Will set the visible state of the waiting animation
			//! @param _windowUID The UID of the window
			//! @param _visible The visible state to set
			UICORE_API_EXPORT void setWaitingAnimationVisible(
				ak::UID												_windowUID,
				bool												_visible = true
			);

			//! @brief Will set the movie as waiting animation in the window
			//! @param _windowUID The UID of the window
			//! @param _movie The movie to set
			UICORE_API_EXPORT void setWaitingAnimation(
				ak::UID												_windowUID,
				QMovie *											_movie
			);

			//! @brief Will set the animation as waiting animation in the window
			//! @param _windowUID The UID of the window
			//! @param _animationName The name of the animation
			UICORE_API_EXPORT void setWaitingAnimation(
				ak::UID												_windowUID,
				const QString &										_animationName
			);

			//! @brief Will set the central widget minimum size
			//! @param _windowUID The UID of the window
			//! @param _width The width to set
			//! @param _height The height to set
			UICORE_API_EXPORT void setCentralWidgetMinimumSize(
				ak::UID												_windowUID,
				int													_width,
				int													_height
			);

			//! @brief Will return the device pixel ratio of the currently used device
			UICORE_API_EXPORT int devicePixelRatio(void);

		} // namespace window

		// ###############################################################################################################################################

		//! @brief Will destroy all objects created by this creator
		//! @param _creatorUid The UID of the creator
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT void creatorDestroyed(
			ak::UID									_creatorUid
		);

		//! @brief Will add the color style to the API
		//! @param _colorStyle The color style to add
		//! @param _activate If true, the provided color style will be set as current color style
		UICORE_API_EXPORT void addColorStyle(
			ak::ui::colorStyle *					_colorStyle,
			bool									_activate
		);

		//! @brief Will set the color style with the specified name as current color style
		UICORE_API_EXPORT void setColorStyle(
			const QString &				_colorStyleName
		);

		//! @brief Will return the current color style
		UICORE_API_EXPORT const ak::ui::colorStyle * getCurrentColorStyle(void);

		UICORE_API_EXPORT QString getCurrentColorStyleName(void);

		//! @brief Will set the current color style to the default dark color style
		UICORE_API_EXPORT void setDefaultDarkColorStyle(void);

		//! @brief Will set the current color style to the default bright color style
		UICORE_API_EXPORT void setDefaultColorStyle(void);

		// ###############################################################################################################################################

		// parameters

		//! @brief Will add the provided search path to the icon manager
		//! @param _path The search path to add
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT void addIconSearchPath(
			const QString &											_path
		);

		//! @brief Will remove the provided search path from the icon manager
		//! @param _path The existing search path to remove
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT void removeIconSearchPath(
			const QString &											_path
		);

		//! @brief Will generate a new UID and return it
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::UID createUid(void);

		//! @brief Will return the messenger
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT ak::messenger * getMessenger(void);

		//! @brief WIll return the icon with the specified name and path
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT const QIcon & getIcon(
			const QString &											_name,
			const QString &											_size
		);

		// ###############################################################################################################################################

		// special

		//! @brief Will run a QApplication to start the event callback routine
		//! The return value will be returned as soon as the application is closing
		UICORE_API_EXPORT int exec(void);

		namespace special {

			//! @brief Will return a string representation of the provided eventType
		//! @param _type The event type that should be represented
			UICORE_API_EXPORT QString toString(
				ak::core::eventType									_type
			);

			//! @brief Will return a string representation of the provided valueType
			//! @param _type The value type that should be represented
			UICORE_API_EXPORT QString toString(
				ak::core::valueType									_type
			);

			//! @brief Will return a string representation of the provided valtextAlignmentueType
			//! @param _type The text alignment that should be represented
			UICORE_API_EXPORT QString toString(
				ui::core::textAlignment								_type
			);

			//! @brief Will return a string representation of the provided dockLocation
			//! @param _type The dock location that should be represented
			UICORE_API_EXPORT QString toString(
				ui::core::dockLocation								_dockLocation
			);

			//! @brief Will return a string representation of the provided tab location
			//! @param _type The tab location that should be represented
			UICORE_API_EXPORT QString toString(
				ui::core::tabLocation								_tabLocation
			);

			//! @brief Create a string representation of the provided dialog result
			//! @param _dialogResult The dialog result to be converted
			UICORE_API_EXPORT QString toQString(
				ui::core::dialogResult								_dialogResult
			);

			//! @brief Will return a string representation of the provided keyType
			//! @param _type The key type that should be represented
			UICORE_API_EXPORT QString toString(
				ui::core::keyType									_type
			);

			//! @brief Will return a string representation of the provided objectType
			//! @param _type The object type that should be represented
			UICORE_API_EXPORT QString toString(
				ui::core::objectType								_type
			);

			//! @brief Will return a string representation of the provided settingsRestoreErrorCode
			//! @param _type The error code that should be represented
			UICORE_API_EXPORT QString toString(
				ak::core::settingsRestoreErrorCode					_settingsRestoreErrorCode
			);

			//! @brief Will create a JSON type representation of the event
			UICORE_API_EXPORT QString toEventText(
				ak::UID												_senderUID,
				ak::core::eventType									_eventType,
				int													_info1,
				int													_info2
			);

		}

	} // namespace uiAPI
} // namespace ak