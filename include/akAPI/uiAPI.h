/*
 *	File:		uiAPI.h
 *	Package:	akAPI
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
#include <akCore/akCore.h>
#include <akCore/globalDataTypes.h>

#include <akGui/aColor.h>
#include <akGui/aApplication.h>
#include <akGui/aObjectManager.h>

#include <akWidgets/aComboButtonWidgetItem.h>

// Forward declaration
class QSurfaceFormat;
class QWidget;
class QMovie;
class QTabBar;

namespace ak {

	// Forward declaration
	class aApplication;
	class aMessenger;
	class aNotifier;
	class aUidManager;
	class aFile;
	class aObjectManager;
	class aColorStyle;
	class aIconManager;
	class aWindowEventHandler;

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
				aMessenger *			_messenger = nullptr,
				aUidManager *			_uidManager = nullptr,
				aIconManager *			_iconManager = nullptr,
				aObjectManager *		_objectManager = nullptr
			);

			//! @brief Returns true if the API was initialized
			bool isInitialized(void) const;

			//! @brief Will call the execute function of the QApplication
			int exec(void);

			//! @brief Will return the default surface format
			QSurfaceFormat * getDefaultSurfaceFormat(void);

			//! @brief Will return the file with the provided UID
			//! @param _fileUid The UID of the file to get, if the provided UID is invalid a new one will be created
			ak::aFile * getFile(
				UID						_fileUid = ak::invalidUID
			);

			//! @brief Will return the file with the provided UID
			//! @param _fileUid The UID of the file to get
			ak::aFile * getExistingFile(
				UID						_fileUid
			);

			//! @brief Will delete the provided file
			//! @param _fileUid The UID of the file to delete
			void deleteFile(
				UID						_fileUid
			);

			//! @brief Will delete all files created
			void deleteAllFiles();

			aApplication * app();

			QDesktopWidget * desktop();

		protected:
			aApplication *				my_app;							//! The core application
			bool						my_appIsRunning;				//! True if the core application is already running
			QSurfaceFormat *			my_defaultSurfaceFormat;
			QDesktopWidget *			my_desktop;

			bool						my_objManagerIsExtern;			//! If true, then the object manager was created externally
			bool						my_messengerIsExtern;			//! If true, then the messenger was created externally
			bool						my_uidManagerIsExtern;			//! If true, then the UID manager was created externally
			bool						my_iconManagerIsExtern;			//! If true, then the icon manager was created externally
			bool						my_isInitialized;				//! If true, then the API was initialized

			aUidManager *				my_fileUidManager;				//! The UID manager used for files in this API
			std::map<UID, ak::aFile *>	my_mapFiles;					//! Map containing all files created in this API

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
			aMessenger *						_messenger = nullptr,
			aUidManager *						_uidManager = nullptr,
			ak::aIconManager *					_iconManager = nullptr,
			ak::aObjectManager *				_objectManager = nullptr
		);

		//! @brief Will destroy all objects created by this API
		UICORE_API_EXPORT void destroy(void);

		//! @brief Will enable the provided event types to be send
		//! @param _types The types to allow
		UICORE_API_EXPORT void enableEventTypes(
			eventType							_types
		);

		//! @brief Will disable the provided event types to be send
		//! @param _types The types to disable
		UICORE_API_EXPORT void disableEventTypes(
			eventType							_types
		);

		//! @brief Will set the enabled state of the messenger
		//! @param _enabled The enabled state to set. If false, no messages will be sent
		UICORE_API_EXPORT void setMessengerEnabled(
			bool								_enabled
		);

		//! @brief Will return the current enabled state for the messenger
		UICORE_API_EXPORT bool messengerIsEnabled(void);

		//! @brief Will return the enabled event types that will be send
		UICORE_API_EXPORT std::vector<eventType> enabledEventTypes(void);

		//! @brief Will return the disabled event types that will be send
		UICORE_API_EXPORT std::vector<eventType> disabledEventTypes(void);

		//! @brief Will create and return a JSON string representing the UI
		UICORE_API_EXPORT std::string saveStateWindow(
			const std::string &					_applicationVersion
		);

		//! @brief Will create and return a JSON string representing the currently set ColorStyle
		UICORE_API_EXPORT std::string saveStateColorStyle(
			const std::string &					_applicationVersion
		);

		//! @brief Will setup the UI with the settings provided in the settings JSON string
		//! @param _json The JSON string containing the settings
		UICORE_API_EXPORT settingsRestoreErrorCode restoreStateWindow(
			const std::string &					_json,
			const std::string &					_applicationVersion
		);

		//! @brief Will restore tthe color style from the settings JSON string
		//! @param _json The JSON string containing the settings
		UICORE_API_EXPORT settingsRestoreErrorCode restoreStateColorStyle(
			const std::string &					_json,
			const std::string &					_applicationVersion
		);

		//! @brief Will return the QWidget of the specifed object
		//! @param _objectUid The UID of the object
		UICORE_API_EXPORT QWidget * getWidget(
			UID									_objectUid
		);

		//! @brief Will return the creator UID of the specified object
		//! @param _objectUID The UID of the object to get its cretor from
		UICORE_API_EXPORT UID getObjectCreator(
			UID									_objectUID
		);

		//! @brief Will return the object manager used in this API
		UICORE_API_EXPORT aObjectManager * getObjectManager(void);

		// ###############################################################################################################################################
		
		// message functions

		//! @brief Will register the notifier at the messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _senderUid The sender UID for which to register the provided notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID registerUidNotifier(
			UID									_senderUid,
			ak::aNotifier *						_notifier
		);

		//! @brief Will register the notifier at the messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _event The event type for which to register the provided notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID registerEventTypeNotifier(
			eventType							_event,
			ak::aNotifier *						_notifier
		);

		//! @brief Will register the notifier for all messages send at the global messenger
		//! The provided notifier will get a new uid if not done so before.
		//! Returns the UID of the notifier
		//! @param _notifier The notifier which to register
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID registerAllMessagesNotifier(
			ak::aNotifier *						_notifier
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
			UID									_senderUid,
			eventType							_event,
			int									_info1 = 0,
			int									_info2 = 0
		);

		//! @brief Will set the samples count for the default surface formt
		//! @param _count The count of samples
		UICORE_API_EXPORT void setSurfaceFormatDefaultSamplesCount(
			int									_count
		);

		// ###############################################################################################################################################

		// object creation

		//! @brief Will create a new Action and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the action
		//! @param _iconSize The size of the icon to display in the action (requires also the icon name to be provided)
		//! @param _iconName The name of the icon to display in the action (requires also the icon size to be provided)
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID createAction(
			UID									_creatorUid,
			const QString &						_text,
			const QString &						_iconName,
			const QString &						_iconSize = QString("32")
		);

		//! @brief Will create a new Action and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The text of the action
		//! @param _icon The icon of the action
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID createAction(
			UID									_craetorUid,
			const QString &						_text,
			const QIcon &						_icon
		);

		//! @brief Will create a new CheckBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the CheckBox
		//! @param _checked The initial checked state of the checkbox
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID createCheckbox(
			UID									_creatorUid,
			const QString &						_text,
			bool								_checked = false
		);

		//! @brief Will create a new ColorEditButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _r The Red-Channel value of the ColorEditButton color
		//! @param _g The Green-Channel value of the ColorEditButton color
		//! @param _b The Blue-Channel value of the ColorEditButton color
		//! @param _a The Alpha-Channel value of the ColorEditButton color
		//! @param _textOverride If provided the RGB-text representation of the provided color will be overwritten by the text provided
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID createColorEditButton(
			UID									_creatorUid,
			int									_r,
			int									_g,
			int									_b,
			int									_a = 255,
			const QString &						_textOverride = QString("")
		);

		//! @brief Will create a new ColorEditButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _color The color of the ColorEditButton
		//! @param _textOverride If provided the RGB-text representation of the provided color will be overwritten by the text provided
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID createColorEditButton(
			UID									_creatorUid,
			const aColor &						_color,
			const QString &						_textOverride = QString("")
		);

		//! @brief Will create a new ComboBox and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID createComboBox(
			UID									_creatorUid
		);

		//! @brief Will create a new ComboButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the ComboButton
		//! @param _possibleSelection The items the ComboButton will display when showing the drop-down menu
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT UID createComboButton(
			UID									_creatorUid,
			const QString &						_text = QString(""),
			const std::vector<QString> &		_possibleSelection = std::vector<QString>()
		);

		/*
		//! @brief Will create a defaultWelcomeScreen and return its UID
		//! @param _creatorUid The UID of the creator
		UICORE_API_EXPORT UID createWelcomeScreen(
			UID												_creatorUid
		);
		*/

		//! @brief Will create a new Dock and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the Dock
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createDock(
			UID									_creatorUid,
			const QString &						_text = QString("")
		);

		//! @brief Will create a line edit and return its UID
		//! param _creatorUid The UID of the creator
		//! @param _initialText The initial text to display
		UICORE_API_EXPORT UID createLineEdit(
			UID									_creatorUid,
			const QString &						_initialText = QString("")
		);

		//! @brief Will create a log in dialog and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _showSavePassword if true the save password checkbox will be displayed
		//! @param _username The initial username
		//! @param _password The initial password
		UICORE_API_EXPORT UID createLogInDialog(
			UID									_creatorUid,
			bool								_showSavePassword,
			const QString &						_imageName,
			const QString &						_username = QString(),
			const QString &						_password = QString()
		);

		//! @brief Will create a nice line edit
		//! @param _initialText The initial text of the edit field
		//! @param _infoLabelText The initial text of the information label
		UICORE_API_EXPORT UID createNiceLineEdit(
			UID									_creatorUid,
			const QString &						_initialText,
			const QString &						_infoLabelText
		);

		//! @brief Will create a new PropertyGrid and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createPropertyGrid(
			UID									_creatorUid
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createPushButton(
			UID									_creatorUid,
			const QString &						_text = QString("")
		);

		//! @brief Will create a new PushButton and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _icon The icon of the PushButton
		//! @param _text The initial text of the PushButton
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createPushButton(
			UID									_creatorUid,
			const QIcon &						_icon,
			const QString &						_text = QString("")
		);

		//! @brief Will create a new options dialog and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		UICORE_API_EXPORT UID createOptionsDialog(
			UID									_creatorUid,
			const QString &						_title = QString("Options")
		);

		//! @brief Will create a new special TabBar and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		UICORE_API_EXPORT UID createSpecialTabBar(
			UID									_creatorUid
		);

		//! @brief Will create a new Table and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _rows The initial row count of the Table
		//! @param _columns The initial column count of the Table
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createTable(
			UID									_creatorUid,
			int									_rows,
			int									_columns
		);

		//! @brief Will create a new TabToolBar subcountainer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _parentUid The UID of the parent TabToolBar object (may be a uiManager, TabToolBarPage or TabToolBarGroup)
		//! @param _text The text of the SubContainer
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createTabToolBarSubContainer(
			UID									_creatorUid,
			UID									_parentUid,
			const QString &						_text = QString("")
		);

		//! @brief Will create a new TextEdit and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @param _initialText The initial text of the TextEdit
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createTextEdit(
			UID									_creatorUid,
			const QString &						_initialText = QString("")
		);

		//! @brief Will create a new timer and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		UICORE_API_EXPORT UID createTimer(
			UID									_creatorUid
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		UICORE_API_EXPORT UID createToolButton(
			UID									_creatorUid
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _text The initial text of the toolButton
		UICORE_API_EXPORT UID createToolButton(
			UID									_creatorUid,
			const QString &						_text
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _text The initial text of the toolButton
		//! @param _icon The initial icon of the toolButton
		UICORE_API_EXPORT UID createToolButton(
			UID									_creatorUid,
			const QString &						_text,
			const QIcon &						_icon
		);

		//! @brief Will create a toolButton and return its UID
		//! @param _creatorUid The UID of the creator
		//! @param _text The initial text of the toolButton
		//! @param _iconName The name of the initial icon for the toolButton
		//! @param _iconFolder The folder of the initial icon for the toolButton
		UICORE_API_EXPORT UID createToolButton(
			UID									_creatorUid,
			const QString &						_text,
			const QString &						_iconName,
			const QString &						_iconFolder
		);

		//! @brief Will create a new Tree and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createTree(
			UID									_creatorUid
		);

		//! @brief Will create a new tab view and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createTabView(
			UID									_creatorUid
		);

		//! @brief Will create a new UI manager and return its UID
		//! @param _creatorUid The UID of the creator who creates this object
		//! @throw ak::Exception if the provided UID is invalid or the API is not initialized
		UICORE_API_EXPORT UID createWindow(
			UID									_creatorUid
		);

		// Object creation

		// ###############################################################################################################################################

		// Action

		namespace action {

			//! @brief Will set the enabled state of the action
			//! @param _actionUID The UID of the action
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				UID								_actionUID,
				bool							_enabled
			);

			//! @brief Will set the text of the action
			//! @param _actionUID The UID of the action
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				UID								_actionUID,
				const QString &					_text
			);

			//! @brief Will set the icon of the action
			//! @param _actionUID The UID of the action
			//! @param _icon The icon to set
			UICORE_API_EXPORT void setIcon(
				UID								_actionUID,
				const QIcon &					_icon
			);

			//! @brief Will set the icon of the action
			//! @param _actionUID The UID of the action
			//! @param _iconName The name of the icon
			//! @param _iconFolder The folder the icon is located at
			UICORE_API_EXPORT void setIcon(
				UID								_actionUID,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			//! @brief Will return the text of the action
			//! @param _actionUID The UID of the action
			UICORE_API_EXPORT QString getText(
				UID								_actionUID
			);

		} // namespace action

		// ###############################################################################################################################################

		// CheckBox

		namespace checkBox {

			UICORE_API_EXPORT void setChecked(
				UID								_checkBoxUID,
				bool							_checked
			);

			UICORE_API_EXPORT void setText(
				UID								_checkBoxUID,
				const QString &					_text
			);

			UICORE_API_EXPORT void setEnabled(
				UID								_checkBoxUID,
				bool							_enabled
			);

			UICORE_API_EXPORT QString text(
				UID								_checkBoxUID
			);

			UICORE_API_EXPORT bool isChecked(
				UID								_checkBoxUID
			);

			UICORE_API_EXPORT bool isEnabled(
				UID								_checkBoxUID
			);

		}

		// ###############################################################################################################################################

		// Container

		namespace container {

			//! @brief Will add an object to the specified container
			//! @param _containerUID The UID of the container
			//! @param _objectUID The UID of the object to add
			UICORE_API_EXPORT void addObject(
				UID								_containerUID,
				UID								_objectUID
			);

			//! @brief Will set the enabled state of the specified container
			//! @param _containerUID The UID of the container
			//! @param _isEnabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				UID								_containerUID,
				bool							_isEnabled = true
			);

			//! @brief Will return the enabled state of the specified container
			//! @param _containerUID The UID of the container
			UICORE_API_EXPORT bool isEnabled(
				UID								_containerUID
			);
		}

		// ###############################################################################################################################################

		// Context menu

		namespace contextMenu {

			//! @brief Will add a context menu item and return its ID
			//! @param _textEditUID The UID of the text edit
			//! @param _text The text of the new item
			//! @param _role The role of the item, if none a signal will be emitted, otherwise the corresponding action will be performed
			UICORE_API_EXPORT ID addItem(
				UID								_textEditUID,
				const QString &					_text,
				contextMenuRole					_role = cmrNone
			);

			//! @brief Will add a context menu item and return its ID
			//! @param _textEditUID The UID of the text edit
			//! @param _icon The icon of the new item
			//! @param _text The text of the new item
			//! @param _role The role of the item, if none a signal will be emitted, otherwise the corresponding action will be performed
			UICORE_API_EXPORT ID addItem(
				UID								_textEditUID,
				const QIcon &					_icon,
				const QString &					_text,
				contextMenuRole					_role = cmrNone
			);

			//! @brief Will add a context menu item and return its ID
			//! @param _textEditUID The UID of the text edit
			//! @param _icon The icon of the new item
			//! @param _text The text of the new item
			//! @param _role The role of the item, if none a signal will be emitted, otherwise the corresponding action will be performed
			UICORE_API_EXPORT ID addItem(
				UID							_textEditUID,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconSize,
				contextMenuRole					_role = cmrNone
			);

			//! @brief Will add a sperator at the context menu of the textEdit
			//! @param _textEditUID The UID of the text edit
			UICORE_API_EXPORT void addSeparator(
				UID							_textEditUID
			);

			//! @brief Will remove all context menu items from the context menu
			//! @param _textEditUID The UID of the text edit
			UICORE_API_EXPORT void clear(
				UID							_textEditUID
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
				UID												_dockUID,
				UID												_widgetUID
			);

			//! @brief Will set the provided widget as central widget of the dock
			//! @param _dockUID The UID of the dock
			//! @param _widget The widget to set as central widget
			UICORE_API_EXPORT void setCentralWidget(
				UID												_dockUID,
				QWidget *											_widget
			);

			//! @brief Will set the visible state of the specified dock
			//! @param _dockUID The UID of the dock
			//! @param _visible The visible state to set
			UICORE_API_EXPORT void setVisible(
				UID												_dockUID,
				bool												_visible = true
			);

			//! @brief Will return the currently set visible state of the 
			UICORE_API_EXPORT bool isVisible(
				UID												_dockUID
			);

		}

		// ###############################################################################################################################################

		// File

		namespace file {

			// File setter

			//! @brief Will load informations from the specifiied file
			UICORE_API_EXPORT UID load(
				const QString &										_filePath
			);

			//! @brief Will load informations from the specified file
			//! @param _fileUid The UID of the file
			//! @param _filePath If provided this path will be set as current file path
			UICORE_API_EXPORT void load(
				UID												_fileUid,
				const QString &										_filePath = QString("")
			);

			//! @brief Will save the current set lines to the current set file path
			//! @param _fileUid The UID of the file
			//! @param _append If true, the file will be opened on append mode
			UICORE_API_EXPORT void save(
				UID												_fileUid,
				bool												_append = false
			);

			//! @brief Will save the current set lines to the provided file path
			//! @param _fileUid The UID of the file
			//! @param _filePath The fule path to set as current file
			//! @param _append If true, the file will be opened on append mode
			UICORE_API_EXPORT void save(
				UID												_fileUid,
				const QString &										_filePath,
				bool												_append = false
			);

			//! @brief Will set the current path for the file
			//! @param _fileUid The UID of the file
			//! @param _path The file path to set
			UICORE_API_EXPORT void setPath(
				UID												_fileUid,
				const QString &										_path
			);

			//! @brief Will set the current lines for the file
			//! @param _fileUid The UID of the file
			//! @param _lines The lines to set
			UICORE_API_EXPORT void setLines(
				UID												_fileUid,
				const QStringList &									_lines
			);

			//! @brief Will append the provided line to the current lines
			//! @param _fileUid The UID of the file
			//! @param _line The line to add
			UICORE_API_EXPORT void addLine(
				UID												_fileUid,
				const QString &										_line
			);

			//! @brief Will append the provided lines to the current lines
			//! @param _fileUid The UID of the file
			//! @param _lines The lines to add
			UICORE_API_EXPORT void addLine(
				UID												_fileUid,
				const QStringList &									_lines
			);

			// #######################################################################################################

			// File Getter

			//! @brief Will return the files name
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QString name(
				UID												_fileUid
			);

			//! @brief Will return the files path
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QString path(
				UID												_fileUid
			);

			//! @brief Will return the files extension
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QString extension(
				UID												_fileUid
			);

			//! @brief Will return the lines in this file
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT QStringList lines(
				UID												_fileUid
			);

			//! @brief Will return the count of the lines in this file
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT int linesCount(
				UID												_fileUid
			);

			//! @brief Will return true if the file has changed after it was loaded or saved the last time
			//! @param _fileUid The UID of the file
			UICORE_API_EXPORT bool hasChanged(
				UID												_fileUid
			);

		} // namespace file

		// ###############################################################################################################################################

		// Line edit

		namespace lineEdit {
			
			//! @brief Will set the line edit text
			//! @param _lineEditUid The UID of the line edit
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				UID													_lineEditUid,
				const QString &										_text
			);

			//! @brief Will set the nice line edit error state
			//! @param _lineEditUid The UID of the line edit
			//! @param _error The error state
			UICORE_API_EXPORT void setErrorState(
				UID													_lineEditUid,
				bool												_error
			);

			//! @brief Will set wather the error state will have an impact on the foreground or the background
			//! @param _lineEditUid The UID of the line edit
			//! @param _isForeground If true, the error state will change the text foreground color, otherwise back and foreground
			UICORE_API_EXPORT void setErrorStateIsForeground(
				UID													_lineEditUid,
				bool												_isForeground
			);

			//! @brief Will set the line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				UID													_lineEditUid,
				bool												_enabled
			);

			//! @brief Will return the line edit text
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT QString text(
				UID													_lineEditUid
			);

			//! @brief Will return the line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT bool isEnabled(
				UID													_lineEditUid
			);
		}

		// ###############################################################################################################################################

		// Log in dialog

		namespace logInDialog {

			UICORE_API_EXPORT dialogResult showDialog(
				UID												_dialogUID
			);

			UICORE_API_EXPORT void close(
				UID												_dialogUID,
				dialogResult									_result = resultNone
			);

			UICORE_API_EXPORT QString getUsername(
				UID												_dialogUID
			);

			UICORE_API_EXPORT QString getPassword(
				UID												_dialogUID
			);

			UICORE_API_EXPORT bool getSavePassword(
				UID												_dialogUID
			);

			//! @brief Will display a invalid log in popup message at this dialogs username input
			//! @param _text The text to set as a tooltip
			UICORE_API_EXPORT void showToolTipAtUsername(
				UID												_dialogUID,
				const QString &										_text
			);

			//! @brief Will display a invalid log in popup message at this dialogs password input
			//! @param _text The text to set as a tooltip
			UICORE_API_EXPORT void showToolTipAtPassword(
				UID												_dialogUID,
				const QString &										_text
			);

			UICORE_API_EXPORT void showToolTipAtCustomField(
				UID												_dialogUID,
				ID												_inputID,
				const QString &										_text
			);

			UICORE_API_EXPORT ID addCustomInputField(
				UID												_dialogUID,
				const QString &										_labelText,
				const QString &										_inputFieldInitialText = QString("")
			);

			UICORE_API_EXPORT QString getCustomFieldText(
				UID												_dialogUID,
				ID												_fieldID
			);

		} // namespace logInDialog

		// ###############################################################################################################################################

		// Nice line edit

		namespace niceLineEdit {

			//! @brief Will set the nice line edit text
			//! @param _lineEditUid The UID of the line edit
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				UID												_lineEditUid,
				const QString &										_text
			);

			//! @brief Will set the nice line edit error state
			//! @param _lineEditUid The UID of the line edit
			//! @param _error The error state
			UICORE_API_EXPORT void setErrorState(
				UID												_lineEditUid,
				bool												_error
			);

			//! @brief Will set wather the error state will have an impact on the foreground or the background
			//! @param _lineEditUid The UID of the line edit
			//! @param _isForeground If true, the error state will change the text foreground color, otherwise back and foreground
			UICORE_API_EXPORT void setErrorStateIsForeground(
				UID													_lineEditUid,
				bool												_isForeground
			);

			//! @brief Will set the nice line edit info label text
			//! @param _lineEditUid The UID of the line edit
			//! @param _text The text to set
			UICORE_API_EXPORT void setInfoLabelText(
				UID												_lineEditUid,
				const QString &										_text
			);

			//! @brief Will set the nice line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				UID												_lineEditUid,
				bool												_enabled
			);

			//! @brief Will return the nice line edit text
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT QString text(
				UID												_lineEditUid
			);

			//! @brief Will return the nice line edit info label text
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT QString infoLabelText(
				UID												_lineEditUid
			);

			//! @brief Will return the nice line edit enabled state
			//! @param _lineEditUid The UID of the line edit
			UICORE_API_EXPORT bool isEnabled(
				UID												_lineEditUid
			);
		}

		// ###############################################################################################################################################

		// Object

		namespace object {

			//! @brief Will destroy the specifed object
			//! @param _objectUID The UID of the object to destroy
			//! @param _ignoreIfObjectHasChildObjects If true, will not destroy the specified object if it still has child objects
			UICORE_API_EXPORT void destroy(
				UID												_objectUID,
				bool												_ignoreIfObjectHasChildObjects = false
			);

			UICORE_API_EXPORT void setAlias(
				UID												_objectUID,
				const QString &										_alias
			);

			//! @brief Will set an object name to the specified object so it can be accessed later.
			//! The name must be unique!
			//! @param _objectUID The UID of the object to set the name at
			//! @param _name The name to set
			UICORE_API_EXPORT void setObjectUniqueName(
				UID												_objectUID,
				const QString &										_name
			);

			//! @brief Will return the unique name set to the object
			//! @param _objectUID The UID of the object
			UICORE_API_EXPORT QString getObjectUniqueName(
				UID												_objectUID
			);

			//! @brief Will return the UID of the object with the specified name
			//! Will return ak::invalidUID if there is no object with the specified name
			//! @param _name The name of the object
			UICORE_API_EXPORT UID getUidFromObjectUniqueName(
				const QString &										_name
			);

			//! @brief Will set the objects ToolTip
			//! For the list of suitable objects refer to the documentation
			//! @param _objectUID The UID of the object
			//! @param _toolTip The toop tip to set
			UICORE_API_EXPORT void setToolTip(
				UID													_objectUID,
				const QString &										_toolTip
			);

			//! @brief Will set the objects enabeld state
			//! For the list of suitable objects refer to the documentation
			//! @param _objectUID The UID of the object
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				UID													_objectUID,
				bool												_enabled = true
			);

			//! @brief Will return the objects enabeld state
			//! For the list of suitable objects refer to the documentation
			//! @param _objectUID The UID of the object
			UICORE_API_EXPORT bool getIsEnabled(
				UID													_objectUID
			);

			//! @brief Will return true if the specified object exists
			//! @param _objectUID The UID of the object
			UICORE_API_EXPORT bool exists(
				UID													_objectUID
			);

			//! @brief Will return the object type of the specified object
			//! @param _objectUID The UID of the object
			UICORE_API_EXPORT objectType type(
				UID													_objectUID
			);

			//! @brief Will cast and return the object with the specified id
			//! @param _objectUID The UID of the object
			template<class T> T * get(
				UID													_objectUID
			) {
				T * obj = nullptr;
				obj = dynamic_cast<T *>(getObjectManager()->object(_objectUID));
				assert(obj != nullptr); // Invalid object type
				return obj;
			}

		}

		// ###############################################################################################################################################

		// Options dialog

		namespace optionsDialog {

			//! @brief Will show the options dialog
			//! @param _dialogUID The UID of the options dialog
			UICORE_API_EXPORT dialogResult show(
				UID												_dialogUID
			);

			//! @brief Will create a new category at the options dialog
			//! Each category can hold individual settings
			//! @param _parentID The id of the parent category. If ak::invalidID is provided the category is a top level category
			//! @param _text The text of the category that will be displayed to the user
			UICORE_API_EXPORT ID createCategory(
				UID												_dialogUID,
				ID												_parentID,
				const QString &									_text
			);

			//! @brief Will create a new category at the options dialog
			//! Each category can hold individual settings
			//! @param _parentID The id of the parent category. If ak::invalidID is provided the category is a top level category
			//! @param _text The text of the category that will be displayed to the user
			//! @param _icon The icon that will be displayed to the user in the navigation
			UICORE_API_EXPORT ID createCategory(
				UID												_dialogUID,
				ID												_parentID,
				const QString &									_text,
				const QIcon &									_icon
			);

			//! @brief Will create a new category at the options dialog
			//! Each category can hold individual settings
			//! @param _parentID The id of the parent category. If ak::invalidID is provided the category is a top level category
			//! @param _text The text of the category that will be displayed to the user
			//! @param _iconName The name of the icon. Icon will be imported via the icon manager
			//! @param _iconPath The path of the icon. Icon will be imported via the icon manager
			UICORE_API_EXPORT ID createCategory(
				UID												_dialogUID,
				ID												_parentID,
				const QString &									_text,
				const QString &									_iconName,
				const QString &									_iconPath
			);

			//! @brief Will create a group at the specified category
			//! A group can be accessed via the group name and the category ID
			UICORE_API_EXPORT void createGroup(
				UID												_dialogUID,
				ID												_categoryID,
				const QString &									_name,
				const aColor &								_color
			);

			//! @brief Will set the state icons for the groups
			//! @param _groupExpanded The expanded icon to set
			//! @param _groupCollapsed The collapsed icon to set
			UICORE_API_EXPORT void setGroupStateIcons(
				UID												_dialogUID,
				const QIcon &									_groupExpanded,
				const QIcon &									_groupCollapsed
			);

			//! @brief Will set the state icons for the groups
			//! @param _groupExpanded The expanded icon to set
			//! @param _groupCollapsed The collapsed icon to set
			UICORE_API_EXPORT void setGroupStateIcons(
				UID												_dialogUID,
				const QString &									_groupExpandedIconName,
				const QString &									_groupExpandedIconPath,
				const QString &									_groupCollapsedIconName,
				const QString &									_groupCollapsedIconPath
			);

			// Add item to default group

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_settingName,
				bool											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_settingName,
				const aColor &								_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_settingName,
				double											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_settingName,
				int												_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_settingName,
				const std::vector<QString> &					_possibleSelection,
				const QString &									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_settingName,
				const QString &									_value
			);

			// Add item to specified group

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_settingName,
				bool											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_settingName,
				const aColor &									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_settingName,
				double											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_settingName,
				int												_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_settingName,
				const std::vector<QString> &					_possibleSelection,
				const QString &									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_dialogUID,
				ID												_categoryID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_settingName,
				const QString &									_value
			);
			
			//! @brief Will clear the whole options dialog
			UICORE_API_EXPORT void clear(
				UID												_dialogUID
			);

			//! @brief Will return true if a least one setting has changed
			UICORE_API_EXPORT bool settingsChanged(
				UID												_dialogUID
			);

			//! @brief Will add the specified event handler to the options dialog
			UICORE_API_EXPORT void addWindowEventHandler(
				UID												_dialogUID,
				aWindowEventHandler *							_handler
			);

		} // namespace optionsDialog

		// ###############################################################################################################################################

		// Prompt

		namespace promptDialog {

			UICORE_API_EXPORT dialogResult show(
				const QString &				_message,
				const QString &				_title,
				promptType					_type,
				const QString &				_iconName,
				const QString &				_iconPath
			);

			UICORE_API_EXPORT dialogResult show(
				const QString &				_message,
				const QString &				_title,
				promptType					_type,
				const QIcon &				_icon
			);

			UICORE_API_EXPORT dialogResult show(
				const QString &				_message,
				const QString &				_title,
				promptType					_type
			);

			UICORE_API_EXPORT dialogResult show(
				const QString &				_message,
				const QString &				_title
			);

		} // namespace prompt

		// ###############################################################################################################################################

		// PropertyGrid

		namespace propertyGrid {

			UICORE_API_EXPORT void addGroup(
				UID												_propertyGridUID,
				const QString &									_groupName
			);

			UICORE_API_EXPORT void addGroup(
				UID												_propertyGridUID,
				const QColor &									_color,
				const QString &									_groupName
			);

			UICORE_API_EXPORT ID addItem(
				UID												_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				bool											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const aColor &									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				double											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				int												_value
			);

			UICORE_API_EXPORT ID addItem(
				UID												_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const std::vector<QString> &					_possibleSelection,
				const QString &									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const char *									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_itemName,
				const QString &									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				bool											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const aColor &								_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				double											_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				int												_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const std::vector<QString> &					_possibleSelection,
				const QString &									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const char *									_value
			);

			UICORE_API_EXPORT ID addItem(
				UID											_propertyGridUID,
				bool											_isMultipleValues,
				const QString &									_groupName,
				const QString &									_itemName,
				const QString &									_value
			);

			//! @brief Will clear the property grid
			//! @param _keepGroups The groups will be cleared but not deleted
			UICORE_API_EXPORT void clear(
				UID											_propertyGridUID,
				bool											_keepGroups = false
			);

			UICORE_API_EXPORT bool getItemIsReadOnly(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT QString getItemGroup(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT bool getItemIsMultipleValues(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT QString getItemName(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT std::vector<QString> getItemPossibleSelection(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT bool getItemValueBool(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT aColor getItemValueColor(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT double getItemValueDouble(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT int getItemValueInteger(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT QString getItemValueSelection(
				UID											_propertyGridUID,
				ID											_itemID
			);
			
			UICORE_API_EXPORT QString getItemValueString(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT valueType getItemValueType(
				UID											_propertyGridUID,
				ID											_itemID
			);

			UICORE_API_EXPORT void setItemIsReadOnly(
				UID											_propertyGridUID,
				ID											_itemID,
				bool											_readOnly = true
			);

			UICORE_API_EXPORT void setGroupStateIcons(
				UID											_propertyGridUID,
				const QIcon &									_groupExpanded,
				const QIcon &									_groupCollapsed
			);

			UICORE_API_EXPORT void setGroupStateIcons(
				UID											_propertyGridUID,
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
				UID											_propertyGridUID,
				ID											_itemID,
				const QString &									_valueToReset
			);

			//! @brief Will set the provided value to the specified item
			//! @param _propertyGridUID The UID of the property grid
			//! @param _itemID The ID of the item
			//! @param _valueToReset The value to set to the item
			UICORE_API_EXPORT void resetItemAsError(
				UID											_propertyGridUID,
				ID											_itemID,
				int												_valueToReset
			);

			//! @brief Will set the provided value to the specified item
			//! @param _propertyGridUID The UID of the property grid
			//! @param _itemID The ID of the item
			//! @param _valueToReset The value to set to the item
			UICORE_API_EXPORT void resetItemAsError(
				UID											_propertyGridUID,
				ID											_itemID,
				double											_valueToReset
			);

			//! @brief Will show the specified item as an error
			//! @param _propertyGridUID The UID of the property grid
			//! @param _itemID The item to set as error
			UICORE_API_EXPORT void showItemAsError(
				UID											_propertyGridUID,
				ID											_itemID
			);

			//! @brief Will set the property grids enabeld state
			//! @param _propertyGridUID The UID of the property grid
			//! @param _enabled The enabled state to set
			UICORE_API_EXPORT void setEnabled(
				UID											_propertyGridUID,
				bool											_enabled
			);

			//! @brief Will return the property grids enabeld state
			//! @param _propertyGridUID The UID of the property grid
			UICORE_API_EXPORT bool isEnabled(
				UID											_propertyGridUID
			);

		}

		// ###############################################################################################################################################

		// Special tab bar

		namespace specialTabBar {

			UICORE_API_EXPORT void clearColors(
				UID			_specialTabBarUID,
				bool			_repaint = true
			);

			UICORE_API_EXPORT void clearColor(
				UID			_specialTabBarUID,
				int				_index,
				bool			_repaint = true
			);

			UICORE_API_EXPORT void addColor(
				UID			_specialTabBarUID,
				int				_index,
				aColor		_color,
				bool			_repaint = true
			);

			UICORE_API_EXPORT void setRepaintBlocked(
				UID			_specialTabBarUID,
				bool			_blocked = true
			);

		} // namespace specialTabBar

		// ###############################################################################################################################################

		// TabView

		namespace tabWidget {

			UICORE_API_EXPORT ID addTab(
				UID				_tabWidgetUID,
				UID				_widgetUID,
				const QString &		_title
			);

			UICORE_API_EXPORT ID addTab(
				UID				_tabWidgetUID,
				UID				_widgetUID,
				const QString &		_title,
				const QString &		_iconName,
				const QString &		_iconFolder
			);

			UICORE_API_EXPORT ID addTab(
				UID				_tabWidgetUID,
				UID				_widgetUID,
				const QString &		_title,
				const QIcon &		_icon
			);

			UICORE_API_EXPORT ID addTab(
				UID				_tabWidgetUID,
				QWidget *			_widget,
				const QString &		_title
			);

			UICORE_API_EXPORT ID addTab(
				UID				_tabWidgetUID,
				QWidget *			_widget,
				const QString &		_title,
				const QString &		_iconName,
				const QString &		_iconFolder
			);

			UICORE_API_EXPORT ID addTab(
				UID				_tabWidgetUID,
				QWidget *			_widget,
				const QString &		_title,
				const QIcon &		_icon
			);

			//! @brief Will close all tabs of the specified tabWidget
			UICORE_API_EXPORT void closeAllTabs(
				UID				_tabWidgetUID
			);

			UICORE_API_EXPORT void closeTab(
				UID				_tabWidgetUID,
				ID				_tabID
			);

			UICORE_API_EXPORT ID getFocusedTab(
				UID				_tabWidgetUID
			);

			UICORE_API_EXPORT bool getTabsClosable(
				UID				_tabWidgetUID
			);

			UICORE_API_EXPORT QString getTabText(
				UID				_tabWidgetUID,
				ID				_tabID
			);

			UICORE_API_EXPORT void setEnabled(
				UID				_tabWidgetUID,
				bool				_enabled
			);

			UICORE_API_EXPORT void setTabbarLocation(
				UID								_tabWidgetUID,
				tabLocation						_location
			);

			UICORE_API_EXPORT void setTabFocused(
				UID				_tabWidgetUID,
				ID				_tabID
			);

			//! @brief Will set the closeabled state for the tabs in this tabWidget
			//! @param _closeable If true the tabs can be closed by the user
			UICORE_API_EXPORT void setTabsClosable(
				UID								_tabWidgetUID,
				bool								_closeable
			);

			//! @brief Will set the text of the specified tab
			//! @param _tabWidgetUID The UID of the tab view
			//! @param _tab The tab to set the text at
			//! @param _text The text to set
			UICORE_API_EXPORT void setTabText(
				UID								_tabWidgetUID,
				ID								_tab,
				const QString &						_text
			);

			//! @brief Will set the provided tab bar to the tab view
			//! @param _tabWidgetUID The UID of the tab view to set the tab bar at
			//! @param _specialTabBarUID The UID of the special tab bar to set
			UICORE_API_EXPORT void setSpecialTabBar(
				UID								_tabWidgetUID,
				UID								_specialTabBarUID
			);

			//! @brief Will set the provided tab bar to the tab view
			//! @param _tabWidgetUID The UID of the tab view to set the tab bar at
			//! @param _specialTabBar The special tab bar to set
			UICORE_API_EXPORT void setSpecialTabBar(
				UID								_tabWidgetUID,
				QTabBar *							_specialTabBar
			);

			UICORE_API_EXPORT void setVisible(
				UID				_tabWidgetUID,
				bool				_visible
			);

			UICORE_API_EXPORT void setObjectName(
				UID							_tabWidgetUID,
				const QString &					_name
			);

			//! @brief Will return true if a tab with the provided text exists
			//! @param _tabText The text of the tab to look for
			UICORE_API_EXPORT bool hasTab(
				UID							_tabWidgetUID,
				const QString & _tabText
			);

			//! @brief Will return the ID of the first tab with the specified name
			UICORE_API_EXPORT ID getTabIDByText(
				UID							_tabWidgetUID,
				const QString & _tabText
			);

		}

		// ###############################################################################################################################################

		// TextEdit

		namespace textEdit {

			UICORE_API_EXPORT void appendText(
				UID				_textEditUID,
				const QString &		_text
			);

			UICORE_API_EXPORT void clear(
				UID				_textEditUID
			);

			UICORE_API_EXPORT void setAutoScrollToBottomEnabled(
				UID				_textEditUID,
				bool				_enabled = true
			);

			UICORE_API_EXPORT void setReadOnly(
				UID				_textEditUID,
				bool				_readOnly = true
			);

			UICORE_API_EXPORT void setText(
				UID				_textEditUID,
				const QString &		_text
			);

		}

		// ###############################################################################################################################################

		// Timer

		namespace timer {

			UICORE_API_EXPORT int getInterval(
				UID							_timerUID
			);

			UICORE_API_EXPORT void setInterval(
				UID							_timerUID,
				int								_interval
			);

			UICORE_API_EXPORT void shoot(
				UID							_timerUID
			);

			UICORE_API_EXPORT void shoot(
				UID							_timerUID,
				int								_interval
			);

			UICORE_API_EXPORT void start(
				UID							_timerUID
			);

			UICORE_API_EXPORT void start(
				UID							_timerUID,
				int								_interval
			);

			UICORE_API_EXPORT void stop(
				UID							_timerUID
			);

		} // namespace timer

		// ###############################################################################################################################################

		// ToolButton

		namespace toolButton {

			//! @brief Will return the current enabled state of the toolButton
			UICORE_API_EXPORT bool getIsEnabled(
				UID							_toolButtonUID
			);

			//! @brief Will get the text of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT QString getText(
				UID							_toolButtonUID
			);

			//! @brief Will get the current ToolTip of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT QString getToolTip(
				UID							_toolButtonUID
			);

			//! @brief Will set the enabled state of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _enabled If true, the toolButton will be enabled
			UICORE_API_EXPORT void setEnabled(
				UID							_toolButtonUID,
				bool							_enabled = true
			);

			//! @brief Will set the text of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text to set
			UICORE_API_EXPORT void setText(
				UID							_toolButtonUID,
				const QString &					_text
			);

			//! @brief Will set the ToolTip of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text to set
			UICORE_API_EXPORT void setToolTip(
				UID							_toolButtonUID,
				const QString &					_text
			);

			//! @brief Will set the icon of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _icon The icon to set
			UICORE_API_EXPORT void setIcon(
				UID							_toolButtonUID,
				const QIcon &					_icon
			);

			//! @brief Will set the icon of the toolButton
			//! @param _toolButtonUID The UID of the tool button
			//! @param _iconName The name of the icon to set
			//! @param _iconFolder The folder of the icon to set
			UICORE_API_EXPORT void setIcon(
				UID							_toolButtonUID,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			//! @brief Will add a new menu item to the menu
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text of the new item
			UICORE_API_EXPORT ID addMenuItem(
				UID							_toolButtonUID,
				const QString &					_text
			);

			//! @brief Will add a new menu item to the menu
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text of the new item
			UICORE_API_EXPORT ID addMenuItem(
				UID							_toolButtonUID,
				const QIcon &					_icon,
				const QString &					_text
			);

			//! @brief Will add a new menu item to the menu
			//! @param _toolButtonUID The UID of the tool button
			//! @param _text The text of the new item
			UICORE_API_EXPORT ID addMenuItem(
				UID							_toolButtonUID,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			//! @brief Will add a menu seperator to the menu
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT void addMenuSeperator(
				UID							_toolButtonUID
			);

			//! @brief Will clear the menu
			//! @param _toolButtonUID The UID of the tool button
			UICORE_API_EXPORT void clearMenu(
				UID							_toolButtonUID
			);

			//! @brief Will set the checked state of the specified menu item
			//! @param _toolButtonUID The UID of the tool button
			//! @param _itemID The ID of the item
			//! @param _checked The checked state to set
			UICORE_API_EXPORT void setMenuItemChecked(
				UID								_toolButtonUID,
				ID								_itemID,
				bool							_checked = true
			);

			//! @brief Will disable the ability to check and uncheck the item (can be reenabled with setChecked)
			//! @param _toolButtonUID The UID of the tool button
			//! @param _itemID The ID of the item
			UICORE_API_EXPORT void setMenuItemNotCheckable(
				UID								_toolButtonUID,
				ID								_itemID
			);

			//! @brief Will return the text of the specified menu item
			//! @param _itemID The ID of the menu item
			UICORE_API_EXPORT QString getMenuItemText(
				UID								_toolButtonUID,
				ID								_itemID
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
			UICORE_API_EXPORT ID addItem(
				UID								_treeUID,
				const QString &					_text,
				ID								_parentId = -1,
				textAlignment					_textAlignment = alignLeft				
			);

			//! @brief Will add a new item to the tree widget and return its ID
			//! @param _treeUID The UID of the tree
			//! @param _text The text of the item
			//! @param _parentUid The ID of the parent item. If ID is -1, the item will be added as top level item
			//! @param _textAlignment The text alignment
			//! @param _icon The icon for the item
			UICORE_API_EXPORT ID addItem(
				UID								_treeUID,
				const QString &					_text,
				const QString &					_iconName,
				const QString &					_iconSize,
				ID								_parentId = -1,
				textAlignment					_textAlignment = alignLeft
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
			UICORE_API_EXPORT ID addItem(
				UID								_treeUID,
				const QString &					_itemPath,
				char							_delimiter,
				textAlignment					_textAlignment = alignLeft
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
			UICORE_API_EXPORT ID addItem(
				UID								_treeUID,
				const QString &					_itemPath,
				char							_delimiter,
				const QString &					_iconName,
				const QString &					_iconSize,
				textAlignment					_textAlignment = alignLeft
			);

			UICORE_API_EXPORT void clear(
				UID							_treeUID
			);

			UICORE_API_EXPORT void collapseAllItems(
				UID							_treeUID
			);

			UICORE_API_EXPORT void deleteItem(
				UID							_treeUID,
				ID							_itemID
			);

			UICORE_API_EXPORT void deleteItems(
				UID							_treeUID,
				const std::vector<ID> &		_itemIDs
			);

			//! @brief Will set the items are editable flag
			//! @param _editable If true, the items can be modified by the user
			//! @param _applyToAll If true, then the new state will be applied to all existing items, otherwise this chane will only affect items created after this point
			UICORE_API_EXPORT void setItemsAreEditable(
				UID							_treeUID,
				bool							_editable = true,
				bool							_applyToAll = true
			);

			//! @brief Will set the editable flag of one item
			//! @param _item The item id
			//! @param _editable If true, the item can be modified by the user
			UICORE_API_EXPORT void setItemIsEditable(
				UID							_treeUID,
				ID							_itemID,
				bool							_editable = true
			);

			//! @brief Will set the read only state of the tree
			//! If read only, the items can not be edited
			//! @param _treeUID The tree UID
			//! @param _readOnly The read only state to set
			UICORE_API_EXPORT void setIsReadOnly(
				UID							_treeUID,
				bool							_readOnly = true
			);

			//! @brief Will set the editable flag of the provided items item
			//! @param _item The item id
			//! @param _editable If true, the item can be modified by the user
			UICORE_API_EXPORT void setItemIsEditable(
				UID							_treeUID,
				const std::vector<ID> &		_itemIDs,
				bool							_editable = true
			);

			UICORE_API_EXPORT void deselectAllItems(
				UID							_treeUID,
				bool							_sendSelectionChangedEvent = true
			);

			UICORE_API_EXPORT void expandAllItems(
				UID							_treeUID
			);

			UICORE_API_EXPORT ID getItemID(
				UID							_treeUID,
				const QString &					_itemPath,
				char							_delimiter
			);

			UICORE_API_EXPORT std::vector<QString> getItemPath(
				UID							_treeUID,
				ID							_itemID
			);

			UICORE_API_EXPORT QString getItemPathString(
				UID							_treeUID,
				ID							_itemID,
				char							_delimiter = '|'
			);

			UICORE_API_EXPORT QString getItemText(
				UID							_treeUID,
				ID							_itemID
			);

			UICORE_API_EXPORT bool isEnabled(
				UID							_treeUID
			);

			UICORE_API_EXPORT int itemCount(
				UID							_treeUID
			);

			UICORE_API_EXPORT std::vector<ID> selectedItems(
				UID							_treeUID
			);

			UICORE_API_EXPORT void setAutoSelectAndDeselectChildrenEnabled(
				UID							_treeUID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT bool getAutoSelectAndDeselectChildrenEnabled(
				UID							_treeUID
			);

			UICORE_API_EXPORT void setEnabled(
				UID							_treeUID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT void setFilterCaseSensitiveEnabled(
				UID							_treeUID,
				bool							_caseSensitiveEnabled = true
			);

			UICORE_API_EXPORT void setFilterRefreshOnChangeEnabled(
				UID							_treeUID,
				bool							_refreshOnChangeEnabled
			);

			UICORE_API_EXPORT void setFilterVisible(
				UID							_treeUID,
				bool							_visible = true
			);
		
			UICORE_API_EXPORT void setItemIcon(
				UID							_treeUID,
				ID							_itemID,
				const QIcon &					_icon
			);

			UICORE_API_EXPORT void setItemIcon(
				UID							_treeUID,
				ID							_itemID,
				const QString &					_iconName,
				const QString &					_iconFolder
			);

			UICORE_API_EXPORT void setItemEnabled(
				UID							_treeUID,
				ID							_itemID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT void setItemSelected(
				UID							_treeUID,
				ID							_itemID,
				bool							_selected = true,
				bool							_singleSelection = false
			);

			UICORE_API_EXPORT void setItemVisible(
				UID							_treeUID,
				ID							_itemID,
				bool							_visible = true
			);

			//! @brief Will set the text of the provided item
			//! @param _itemId The ID of the item
			//! @param _text The text to set
			UICORE_API_EXPORT void setItemText(
				UID							_treeUID,
				ID							_itemId,
				const QString &					_text
			);

			UICORE_API_EXPORT void setMultiSelectionEnabled(
				UID							_treeUID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT void setVisible(
				UID							_treeUID,
				bool							_visible = true
			);

			UICORE_API_EXPORT void toggleItemSelection(
				UID							_treeUID,
				ID							_itemID
			);

			UICORE_API_EXPORT void setSortingEnabled(
				UID							_treeUID,
				bool							_enabled = true
			);

			UICORE_API_EXPORT bool isSortingEnabled(
				UID							_treeUID
			);

			UICORE_API_EXPORT bool isReadOnly(
				UID							_treeUID
			);

		} // namespace tree

		// ###############################################################################################################################################

		// Window

		namespace window {

			//! @brief Will add the provided event handler to the window
			//! @param _windowUID The UID of the window
			//! @param _eventHandler The event handler to add
			UICORE_API_EXPORT void addEventHandler(
				UID												_windowUID,
				aWindowEventHandler *								_eventHandler
			);

			//! @brief Will close the provided window
			//! @param _windowUID The UID of the uiManager to close
			UICORE_API_EXPORT void close(
				UID												_windowUID
			);

			//! @brief Will remove the provided event handler from the window
			//! @param _windowUID The UID of the window
			//! @param _eventHandler The event handler to remove
			UICORE_API_EXPORT void removeEventHandler(
				UID												_windowUID,
				aWindowEventHandler *								_eventHandler
			);

			//! @brief Will set the visible state of the status label of the provided window.
			//! @param _windowUID The UID of the window
			//! @param _visible If true the status label will be visible
			UICORE_API_EXPORT void setStatusLabelVisible(
				UID												_windowUID,
				bool												_visible = true,
				bool												_delayed = true
			);

			//! @brief Will set the visible state of the status progress bar of the provided window.
			//! @param _windowUID The UID of the window
			//! @param _visible If true the status progress bar will be visible
			UICORE_API_EXPORT void setStatusProgressVisible(
				UID												_windowUID,
				bool												_visible = true,
				bool												_delayed = true
			);

			//! @brief Will set the status label text at the provided window
			//! @param _windowUID The UID of the window
			//! @param _text The text to set
			UICORE_API_EXPORT void setStatusLabelText(
				UID												_windowUID,
				const QString &										_text
			);

			//! @brief Will set the value of the status progress bar at the provided window
			//! @param _windowUID The UID of the window
			//! @param _value The value to set (0 - 100)
			UICORE_API_EXPORT void setStatusProgressValue(
				UID												_windowUID,
				int													_value
			);

			//! @brief Will set the continuous option of the status progress bar at the provided window
			//! @param _windowUID The UID of the window
			//! @param _continuous If true the status progress bar will be displayed as a continuous bar
			UICORE_API_EXPORT void setStatusProgressContinuous(
				UID												_windowUID,
				bool												_continuous = true
			);

			//! @brief Will set the visible state of the tabToolbar in the specified window
			//! @param _windowUID The UID of the window
			//! @param _visible If true the tabToolbar will be visible
			UICORE_API_EXPORT void setTabToolBarVisible(
				UID												_windowUID,
				bool												_visible = true
			);

			//! @brief Will return the current tab of the tab tool bar at the specified window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT ID getCurrentTabToolBarTab(
				UID												_windowUID
			);

			//! @brief Will return the tab count of the tab tool bar at the specified window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT int getTabToolBarTabCount(
				UID												_windowUID
			);

			//! @brief Will set the specified tab toolbar tab as current at the specified window
			//! @param _windowUID The UID of the window
			//! @param _tabID The tab to set as current
			UICORE_API_EXPORT void setCurrentTabToolBarTab(
				UID												_windowUID,
				ID												_tabID
			);

			UICORE_API_EXPORT void setCurrentTabToolBarTab(
				UID												_windowUID,
				const std::string								   &_tabName
			);

			//! @brief Will set the title at the specified window
			//! @param _windowUID The UID of the window
			//! @param _title The title to set
			UICORE_API_EXPORT void setTitle(
				UID												_windowUID,
				const QString &										_title
			);

			//! @brief Will return true if the status label is visible at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT bool getStatusLabelVisible(
				UID												_windowUID
			);

			//! @brief Will return true if the status progress bar is visible at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT bool getStatusProgressVisible(
				UID												_windowUID
			);

			//! @brief Will return the status label text at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT QString getStatusLabelText(
				UID												_windowUID
			);

			//! @brief Will return the status progress bar value at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT int getStatusProgressValue(
				UID												_windowUID
			);

			//! @brief Will return the continuous option of the status progress bar at the provided window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT bool getStatusProgressContinuous(
				UID												_windowUID
			);

			//! @brief Will add a dock at the specified location to the window
			//! @param _windowUID The UID of the window
			//! @param _dockUid The UID of the dock to add
			//! @param _dockLocation The dock location to add the dock at
			UICORE_API_EXPORT void addDock(
				UID												_windowUID,
				UID												_dockUid,
				dockLocation									_dockLocation
			);

			//! @brief Will tabify two docks
			//! The parent dock will be raised
			//! @param _windowUID The UID of the window
			//! @param _parentDockUid The UID of the parent dock
			//! @param _dockUid The UID of the dock to add as a tabbed dock
			UICORE_API_EXPORT void tabifyDock(
				UID												_windowUID,
				UID												_parentDockUid,
				UID												_dockUid
			);

			//! @brief Will set the dock priority in the bottom left corner
			//! @param _windowUID The UID of the window
			//! @param _dockLocation The dock location that will have the higher priority
			UICORE_API_EXPORT void setDockBottomLeftPriority(
				UID												_windowUID,
				dockLocation									_dockLocation
			);

			//! @brief Will set the dock priority in the bottom right corner
			//! @param _windowUID The UID of the window
			//! @param _dockLocation The dock location that will have the higher priority
			UICORE_API_EXPORT void setDockBottomRightPriority(
				UID												_windowUID,
				dockLocation									_dockLocation
			);

			//! @brief Will set the windows central widget (The widget will be added to a container, the actual central widget differs)
			//! @param _windowUID The UID of the window
			//! @param _widgetUID The widget UID of the widget to set as central widget
			UICORE_API_EXPORT void setCentralWidget(
				UID												_windowUID,
				UID												_widgetUID
			);

			//! @brief Will set the windows central widget (The widget will be added to a container, the actual central widget differs)
			//! @param _windowUID The UID of the window
			//! @param _widget The widget to set as central widget
			UICORE_API_EXPORT void setCentralWidget(
				UID												_windowUID,
				QWidget *											_widget
			);

			//! @brief Will show the window as maximized
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT void showMaximized(
				UID												_windowUID
			);

			//! @brief Will show the window as minimized
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT void showMinimized(
				UID												_windowUID
			);

			//! @brief Will return the size of the window
			//! @param _windowUID The UID of the window
			UICORE_API_EXPORT QSize size(
				UID												_windowUID
			);

			//! @brief Will set the visible state of the waiting animation
			//! @param _windowUID The UID of the window
			//! @param _visible The visible state to set
			UICORE_API_EXPORT void setWaitingAnimationVisible(
				UID												_windowUID,
				bool												_visible = true
			);

			//! @brief Will set the movie as waiting animation in the window
			//! @param _windowUID The UID of the window
			//! @param _movie The movie to set
			UICORE_API_EXPORT void setWaitingAnimation(
				UID												_windowUID,
				QMovie *											_movie
			);

			//! @brief Will set the animation as waiting animation in the window
			//! @param _windowUID The UID of the window
			//! @param _animationName The name of the animation
			UICORE_API_EXPORT void setWaitingAnimation(
				UID												_windowUID,
				const QString &										_animationName
			);

			//! @brief Will set the central widget minimum size
			//! @param _windowUID The UID of the window
			//! @param _width The width to set
			//! @param _height The height to set
			UICORE_API_EXPORT void setCentralWidgetMinimumSize(
				UID												_windowUID,
				int													_width,
				int													_height
			);

			//! @brief Will return the device pixel ratio of the currently used device
			UICORE_API_EXPORT int devicePixelRatio(void);

		} // namespace window

		// ###############################################################################################################################################

		// Crypto

		namespace crypto {

			UICORE_API_EXPORT QString hash(
				const QString &			_orig,
				HashAlgorithm			_algorithm
			);

		}

		// ###############################################################################################################################################

		//! @brief Will destroy all objects created by this creator
		//! @param _creatorUid The UID of the creator
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT void creatorDestroyed(
			UID									_creatorUid
		);

		//! @brief Will add the color style to the API
		//! @param _colorStyle The color style to add
		//! @param _activate If true, the provided color style will be set as current color style
		UICORE_API_EXPORT void addColorStyle(
			ak::aColorStyle *					_colorStyle,
			bool									_activate
		);

		//! @brief Will set the color style with the specified name as current color style
		UICORE_API_EXPORT void setColorStyle(
			const QString &				_colorStyleName
		);

		//! @brief Will return the current color style
		UICORE_API_EXPORT const ak::aColorStyle * getCurrentColorStyle(void);

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
		UICORE_API_EXPORT UID createUid(void);

		//! @brief Will return the messenger
		//! @throw ak::Exception if the API is not initialized
		UICORE_API_EXPORT aMessenger * getMessenger(void);

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
				eventType								_type
			);

			//! @brief Will return a string representation of the provided valueType
			//! @param _type The value type that should be represented
			UICORE_API_EXPORT QString toString(
				valueType								_type
			);

			//! @brief Will return a string representation of the provided valtextAlignmentueType
			//! @param _type The text alignment that should be represented
			UICORE_API_EXPORT QString toString(
				textAlignment							_type
			);

			//! @brief Will return a string representation of the provided dockLocation
			//! @param _type The dock location that should be represented
			UICORE_API_EXPORT QString toString(
				dockLocation							_dockLocation
			);

			//! @brief Will return a string representation of the provided tab location
			//! @param _type The tab location that should be represented
			UICORE_API_EXPORT QString toString(
				tabLocation								_tabLocation
			);

			//! @brief Create a string representation of the provided dialog result
			//! @param _dialogResult The dialog result to be converted
			UICORE_API_EXPORT QString toString(
				dialogResult							_dialogResult
			);

			//! @brief Will return a string representation of the provided keyType
			//! @param _type The key type that should be represented
			UICORE_API_EXPORT QString toString(
				keyType									_type
			);

			//! @brief Will return a string representation of the provided objectType
			//! @param _type The object type that should be represented
			UICORE_API_EXPORT QString toString(
				objectType								_type
			);

			//! @brief Will return a string representation of the provided settingsRestoreErrorCode
			//! @param _type The error code that should be represented
			UICORE_API_EXPORT QString toString(
				settingsRestoreErrorCode				_settingsRestoreErrorCode
			);

			UICORE_API_EXPORT QString toString(
				const QDate &							_date,
				dateFormat								_format,
				const QString &							_delimiter
			);

			UICORE_API_EXPORT QString toString(
				const QTime &							_time,
				timeFormat								_format,
				const QString &							_delimiter,
				const QString &							_millisecondDelimiter = "."
			);

			//! @brief Will create a JSON type representation of the event
			UICORE_API_EXPORT QString toEventText(
				UID										_senderUID,
				eventType								_eventType,
				int										_info1,
				int										_info2
			);

		}

	} // namespace uiAPI
} // namespace ak