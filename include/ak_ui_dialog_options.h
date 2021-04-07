/*
 * ak_ui_objectManager.h
 *
 *  Created on: November 03, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aDialog.h>			// Base class: options
#include <ak_ui_core_aPaintable.h>		// Base class: options
#include <ak_globalDataTypes.h>			// AK ID type
#include <ak_ui_core_aWidget.h>			// Base class: category
#include <ak_notifier.h>
#include <ak_ui_color.h>

// Qt header
#include <qobject.h>
#include <qstring.h>
#include <qicon.h>
#include <qcolor.h>

#include <map>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QSplitter;
class QCloseEvent;

namespace ak {

	// Forward declaration
	class messenger;

	namespace ui {

		// Forward declaration
		class windowEventHandler;
		namespace qt { class pushButton; }
		namespace widget { class tree; class propertyGrid; }

		namespace dialog {

			class optionsCategory;
			class optionsNotifier;

			//! The options dialog is used to manage any kind of settings.
			//! If the user selects ok or apply, a changed signal will be emitted
			class UICORE_API_EXPORT options : public ui::core::aDialog, public ak::ui::core::aPaintable {
				Q_OBJECT
			public:

				options(
					const QString &						_title
				);

				virtual ~options();

				// ###################################################################################################################################################

				//! @brief Will set the current color style for this dialog
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				virtual void closeEvent(QCloseEvent * _event) override;

				// ###################################################################################################################################################

				// Object creation

				//! @brief Will create a new category at the options dialog
				//! Each category can hold individual settings
				//! @param _parentID The id of the parent category. If ak::invalidID is provided the category is a top level category
				//! @param _text The text of the category that will be displayed to the user
				ID createCategory(
					ID									_parentID,
					const QString &						_text
				);

				//! @brief Will create a new category at the options dialog
				//! Each category can hold individual settings
				//! @param _parentID The id of the parent category. If ak::invalidID is provided the category is a top level category
				//! @param _text The text of the category that will be displayed to the user
				//! @param _icon The icon that will be displayed to the user in the navigation
				ID createCategory(
					ID									_parentID,
					const QString &						_text,
					const QIcon &						_icon
				);

				//! @brief Will create a group at the specified category
				//! A group can be accessed via the group name and the category ID
				void createGroup(
					ID									_categoryID,
					const QString &						_name,
					const QColor &						_color
				);

				//! @brief Will set the state icons for the groups
				//! @param _groupExpanded The expanded icon to set
				//! @param _groupCollapsed The collapsed icon to set
				void setGroupStateIcons(
					const QIcon &						_groupExpanded,
					const QIcon &						_groupCollapsed
				);

				// ###################################################################################################################################################

				// Item creation

				// Add item to default group

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_settingName,
					bool											_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_settingName,
					const ui::color &								_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_settingName,
					double											_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_settingName,
					int												_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_settingName,
					const std::vector<QString> &					_possibleSelection,
					const QString &									_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_settingName,
					const QString &									_value
				);

				// Add item to specified group

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					bool											_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					const ui::color &									_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					double											_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					int												_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					const std::vector<QString> &					_possibleSelection,
					const QString &									_value
				);

				ak::ID addItem(
					ID												_categoryID,
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					const QString &									_value
				);

				// ###################################################################################################################################################

				// Data manipulation

				//! @brief Will clear the whole options dialog
				void clear(void);

				// ###################################################################################################################################################

				// Getter

				//! @brief Will return the tree used in this options dialog
				widget::tree * getTree(void) const { return my_tree; }

				//! @brief Will return true if a least one setting has changed
				bool settingsChanged(void) const { return my_settingsChanged; }

				// ###################################################################################################################################################

				// Event handling

				//! @brief Will notify the options dialog about the occuring central component event
				//! @param _senderId The sender ID the message was send from
				//! @param _event The event message
				//! @param _info1 Message addition 1
				//! @param _info2 Message addition 2
				void notify(
					ak::UID									_senderId,
					ak::core::eventType						_event,
					int										_info1,
					int										_info2
				);

				//! @brief Will add the specified event handler to the options dialog
				void addWindowEventHandler(
					windowEventHandler *					_handler
				);

			signals:
				void changed();

			private slots:
				void slotOk();
				void slotApply();
				void slotCancel();

			private:

				QVBoxLayout *							my_mainLayout;
				QWidget *								my_buttonsWidget;
				QHBoxLayout *							my_buttonsLayout;
				QSplitter *								my_centralWidget;

				qt::pushButton *						my_btnOk;
				qt::pushButton *						my_btnApply;
				qt::pushButton *						my_btnCancel;

				widget::tree *							my_tree;
				widget::propertyGrid *					my_dummy;
				std::map<ID, optionsCategory *>			my_categories;
				bool									my_settingsChanged;
				ID										my_currentCategory;

				QIcon									my_iconGroupExpanded;
				QIcon									my_iconGroupCollapsed;
				bool									my_groupIconsSet;

				std::vector<windowEventHandler *>		my_windowEventHandler;

				messenger *								my_messenger;
				optionsNotifier *						my_notifier;

				options() = delete;
				options(const options&) = delete;
				options & operator = (const options&) = delete;
			};

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			class optionsCategory : public ui::core::aWidget {
			public:
				optionsCategory(
					options *			_optionsDialog,
					ID					_treeItemId,
					messenger *			_messenger
				);

				virtual ~optionsCategory();

				// #######################################################################################################

				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *					_colorStyle
				) override;

				// #######################################################################################################

				// Getter

				//! @brief Will return the property grid of this category
				widget::propertyGrid * getPropertyGrid(void) const { return my_propertyGrid; }

			private:

				ID						my_treeItemId;
				widget::propertyGrid *	my_propertyGrid;
				messenger *				my_messenger;
				options *				my_optionsDialog;

				optionsCategory() = delete;
				optionsCategory(const optionsCategory &) = delete;
				optionsCategory & operator = (const optionsCategory&) = delete;
			};

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			class optionsNotifier : public notifier {
			public:
				optionsNotifier(
					options *			_options
				);

				virtual ~optionsNotifier() {}

				//! @brief Will call the callback function with the provided parameters
				//! @param _senderId The sender ID the message was send from
				//! @param _event The event message
				//! @param _info1 Message addition 1
				//! @param _info2 Message addition 2
				virtual void notify(
					ak::UID									_senderId,
					ak::core::eventType						_event,
					int										_info1,
					int										_info2
				) override;

			private:

				options *									my_options;

				optionsNotifier() = delete;
				optionsNotifier(const optionsNotifier &) = delete;
				optionsNotifier & operator = (const optionsNotifier &) = delete;
			};

		}
	}
}