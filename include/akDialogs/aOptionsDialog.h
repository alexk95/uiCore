/*
 *	File:		aOptionsDialog.h
 *	Package:	akDialogs
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
#include <akCore/globalDataTypes.h>
#include <akCore/aNotifier.h>

#include <akGui/aPaintable.h>		// Base class: options
#include <akGui/aDialog.h>			// Base class: options
#include <akGui/aColor.h>			// Color

#include <akWidgets/aWidget.h>		// Base class: category

// Qt header
#include <qobject.h>
#include <qstring.h>
#include <qicon.h>
#include <qcolor.h>

#include <map>

class QVBoxLayout;
class QHBoxLayout;
class QSplitter;
class QCloseEvent;

namespace ak {

	class aColorStyle;
	class aWindowEventHandler;
	class aPushButtonWidget;
	class aPropertyGridWidget;
	class aTreeWidget;
	class aOptionsDialogCategory;

	//! The options dialog is used to manage any kind of settings.
	//! If the user selects ok or apply, a changed signal will be emitted
	class UICORE_API_EXPORT aOptionsDialog : public aDialog, public aPaintable {
		Q_OBJECT
	public:

		aOptionsDialog(
			const QString &				_title
		);

		virtual ~aOptionsDialog();

		// ###################################################################################################################################################

		//! @brief Will set the current color style for this dialog
		virtual void setColorStyle(
			aColorStyle *			_colorStyle
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

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_settingName,
			bool											_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aColor &								_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_settingName,
			double											_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_settingName,
			int												_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const std::vector<QString> &					_possibleSelection,
			const QString &									_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const QString &									_value
		);

		// Add item to specified group

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			bool											_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			const aColor &									_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			double											_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			int												_value
		);

		ID addItem(
			ID												_categoryID,
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			const std::vector<QString> &					_possibleSelection,
			const QString &									_value
		);

		ID addItem(
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
		aTreeWidget * getTree(void) const { return m_tree; }

		//! @brief Will return true if a least one setting has changed
		bool settingsChanged(void) const { return m_settingsChanged; }

		// ###################################################################################################################################################

		// Event handling

		//! @brief Will add the specified event handler to the options dialog
		void addWindowEventHandler(
			aWindowEventHandler *					_handler
		);

	signals:
		void changed();

	private slots:
		void slotSelectionChanged();
		void slotChanged(int);
		void slotOk();
		void slotApply();
		void slotCancel();

	private:

		QVBoxLayout *							m_mainLayout;
		QWidget *								m_buttonsWidget;
		QHBoxLayout *							m_buttonsLayout;
		QSplitter *								m_centralWidget;

		aPushButtonWidget *						m_btnOk;
		aPushButtonWidget *						m_btnApply;
		aPushButtonWidget *						m_btnCancel;

		aTreeWidget *							m_tree;
		aPropertyGridWidget *					m_dummy;
		std::map<ID, aOptionsDialogCategory *>	m_categories;
		bool									m_settingsChanged;
		ID										m_currentCategory;

		QIcon									m_iconGroupExpanded;
		QIcon									m_iconGroupCollapsed;
		bool									m_groupIconsSet;

		std::vector<aWindowEventHandler *>		m_windowEventHandler;

		aOptionsDialog() = delete;
		aOptionsDialog(const aOptionsDialog&) = delete;
		aOptionsDialog & operator = (const aOptionsDialog&) = delete;
	};

	// ###################################################################################################################################################

	// ###################################################################################################################################################

	// ###################################################################################################################################################

	class aOptionsDialogCategory : public aWidget {
	public:
		aOptionsDialogCategory(
			aOptionsDialog *	_optionsDialog,
			ID					_treeItemId
		);

		virtual ~aOptionsDialogCategory();

		// #######################################################################################################

		// Base class functions

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			aColorStyle *					_colorStyle
		) override;

		// #######################################################################################################

		// Getter

		//! @brief Will return the property grid of this category
		aPropertyGridWidget * getPropertyGrid(void) const { return m_propertyGrid; }

	private:
		ID						m_treeItemId;
		aPropertyGridWidget *	m_propertyGrid;
		aOptionsDialog *		m_optionsDialog;

		aOptionsDialogCategory() = delete;
		aOptionsDialogCategory(const aOptionsDialogCategory &) = delete;
		aOptionsDialogCategory & operator = (const aOptionsDialogCategory&) = delete;
	};
}