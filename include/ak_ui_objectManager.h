/*
 * ak_ui_objectManager.h
 *
 *  Created on: July 14, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <map>							// map

// Qt header
#include <qstring.h>
#include <qicon.h>
#include <qtoolbutton.h>				// QToolButton popup mode
#include <qpixmap.h>

// AK header
#include <ak_globalDataTypes.h>			// UID and ID type
#include <ak_ui_color.h>				// color

namespace ak {

	class notifierObjectManager;
	class messenger;
	class uidManager;

	namespace ui {

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
				ak::uidManager *									_uidManager
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
				const QString &										_text = QString(""),
				bool												_checked = false
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
				const std::vector<QString> &						_possibleSelection = std::vector<QString>()
			);

			//! @brief Will create a combo button item and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _icon The icon of the combo button item
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
				const QString &										_text = QString()
			);

			//! @brief Will create a log in dialog and return its UID
			//! @param _creatorUid The UID of the creator
			//! @param _showSavePassword if true the save password checkbox will be displayed
			//! @param _username The initial username
			//! @param _password The initial password
			ak::UID createLogInDialog(
				ak::UID												_creatorUid,
				bool												_showSavePassword,
				const QPixmap &										_backgroundImage,
				const QString &										_username = QString(),
				const QString &										_password = QString()
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

			//! @brief Will create a defaultWelcomeScreen and return its UID
			//! @param _creatorUid The UID of the creator
			ak::UID createWelcomeScreen(
				ak::UID												_creatorUid
			);

			//! @brief Will create a window and return its UID
			//! @param _creatorUid The UID of the creator
			ak::UID createWindow(
				ak::UID												_creatorUid
			);

			// ###############################################################################################################################################

			// Object manipulation

			//! @brief Will return the object with the specified object UID
			//! @param _objectUid The UID of the requested object
			ui::core::aObject * object(
				ak::UID												_objectUid
			);

			// ###############################################################################################################################################

			// Special events

			//! @brief Will destroy all objects created by this creator
			//! @param _creatorUid The UID of the creator
			void creatorDestroyed(
				ak::UID												_creatorUid
			);

			void destroy(
				ak::UID												_objectUID
			);

			void setIconSearchDirectories(
				const std::vector<QString>							_paths
			);

			void addColorStyle(
				ak::ui::colorStyle *								_colorStyle,
				bool												_activate
			);

			void setColorStyle(
				const QString &										_colorStyleName
			);

			void setDefaultColorStyle(void);

			void setDefaultDarkColorStyle(void);

			ui::colorStyle * getCurrentColorStyle(void) const;

			QString getCurrentColorStyleName(void) const;

			//! @brief Will destroy all objects
			void destroyAll(void);

			//! @brief Will return the JSON settings string of all objects that have an alias set
			std::string saveState(void);

			//! @brief Will setup the UI with the settings provided in the settings JSON string
			//! @param _json The JSON string containing the settings
			//! @throw ak::Exception on syntax mismatch
			void restoreState(
				const char *										_json
			);

			void addAlias(
				const QString &										_alias,
				ak::UID												_UID
			);

			void removeAlias(
				const QString &										_alias
			);

		private:

			void setColorStyle(
				ui::colorStyle *									_colorStyle
			);

			objectManager() = delete;
			objectManager(const ak::ui::objectManager &) = delete;

			// ###############################################################################################################################################

			//! @brief Will register the created UID for the creator
			//! @param _creatorUid The UID of the creator
			//! @param _createdUid THe UID of the created object
			void addCreatedUid(
				ak::UID												_creatorUid,
				ak::UID												_createdUid
			);
			
			//! @brief Will cast and return the object to a QWidget
			QWidget * castToWidget(
				ak::UID												_objectUid
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

			std::vector<ui::colorStyle *>					my_colorStyles;

			std::vector<QString>							my_iconSearchPaths;

			// ###############################################################################################################################################

			ak::uidManager *				my_uidManager;									//! The globally used uidManager
			ak::messenger *					my_messenger;									//! The globally used messaging system
			ak::ui::signalLinker *			my_signalLinker;								//! The internal signal linker
			ak::ui::colorStyle *			my_currentColorStyle;							//! The globally used color style
			ak::notifierObjectManager *		my_notifier;									//! The notifier used to catch the destroyed messages

		};
	} // namespace ui
} // namespace ak
