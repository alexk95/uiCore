/*
 *	File:		aPropertyGridWidget.h
 *	Package:	akWidgets
 *
 *  Created on: October 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

 // C++ header
#include <map>
#include <list>
#include <vector>

// Qt header
#include <qwidget.h>					// Base class
#include <qstring.h>					//
#include <qcolor.h>						// QColor
#include <qtablewidget.h>				// QTableWidgetItem
#include <qicon.h>

// AK header
#include <akCore/globalDataTypes.h>
#include <akCore/akCore.h>
#include <akWidgets/aWidget.h>
#include <akGui/aColor.h>

// Forward declaration
class QHBoxLayout;
class QVBoxLayout;

namespace ak {

	class aMessenger;
	class aUidManager;
	class aPropertyGridGroup;
	class aPropertyGridItem;
	class aTableWidget;
	class aCheckBoxWidget;
	class aColorEditButtonWidget;
	class aComboButtonWidget;
	class aTextEditWidget;

	class UICORE_API_EXPORT aPropertyGridWidget : public QWidget, public aWidget {
		Q_OBJECT
	public:
		aPropertyGridWidget();

		virtual ~aPropertyGridWidget();

		// ##############################################################################################################

		// base class functions

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *						_colorStyle
		) override;

		//! @brief Will set the alias for this object
		//! @param _alias The alias to set
		virtual void setAlias(
			const QString &							_alias
		) override;

		// ##############################################################################################################

		// Item creation and manipulation

		void addGroup(
			const QString &									_group
		);

		void addGroup(
			const QColor &									_color,
			const QString &									_group
		);

		// Add item to default group

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			bool											_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aColor &									_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			double											_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			int												_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			const std::vector<QString> &					_possibleSelection,
			const QString &									_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			const QString &									_value
		);

		// Add item to specified group

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			bool											_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			const aColor &									_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			double											_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			int												_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			const std::vector<QString> &					_possibleSelection,
			const QString &									_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			const QString &									_value
		);

		void setItemReadOnly(
			ID											_itemID,
			bool											_readOnly = true
		);

		bool itemIsReadOnly(
			ID											_itemID
		);

		void setGroupStateIcons(
			const QIcon &									_groupExpanded,
			const QIcon &									_groupCollapsed
		);

		//! @brief Will set the provided value to the specified item
		//! @param _itemID The ID of the item
		//! @param _valueToReset The value to set to the item
		void resetItemAsError(
			ID											_itemID,
			const QString &									_valueToReset
		);

		//! @brief Will set the provided value to the specified item
		//! @param _itemID The ID of the item
		//! @param _valueToReset The value to set to the item
		void resetItemAsError(
			ID											_itemID,
			int												_valueToReset
		);

		//! @brief Will set the provided value to the specified item
		//! @param _itemID The ID of the item
		//! @param _valueToReset The value to set to the item
		void resetItemAsError(
			ID											_itemID,
			double											_valueToReset
		);

		//! @brief Will show the specified item as an error
		//! @param _itemID The item to set as error
		void showItemAsError(
			ID											_itemID
		);

		//! @brief Will set the enabled state of the property grid
		//! @param _enabled The enabled state to set
		void setEnabled(
			bool											_enabled
		);

		// ##############################################################################################################

		// Clear items

		//! @brief Will clear the property grid
		//! @param _keepGroups If true, the groups will be cleared but not deleted
		void clear(
			bool											_keepGroups = false
		);

		// ##############################################################################################################

		// Item information gathering

		QString getItemGroup(
			ID											_itemID
		);

		bool getItemIsMultipleValues(
			ID											_itemID
		);

		QString getItemName(
			ID											_itemID
		);

		std::vector<QString> getItemPossibleSelection(
			ID											_itemID
		);

		bool getItemValueBool(
			ID											_itemID
		);

		aColor getItemValueColor(
			ID											_itemID
		);

		double getItemValueDouble(
			ID											_itemID
		);

		int getItemValueInteger(
			ID											_itemID
		);

		QString getItemValueSelection(
			ID											_itemID
		);

		QString getItemValueString(
			ID											_itemID
		);

		valueType getItemValueType(
			ID											_itemID
		);

		bool isEnabled(void) const { return my_isEnabled; }

	signals:
		void cleared(void);
		void itemChanged(int _itemId);

	private slots:
		void slotItemChanged(void);
		void slotCheckItemVisibility(void);
		void slotFocusLost(void);

	private:

		void itemCountChanged(void);
		ID newItemCreated(aPropertyGridItem * _item);

		QVBoxLayout *							my_layout;
		aTextEditWidget *						my_infoTextEdit;
		aTableWidget *							my_table;
		bool									my_checkItemVisibilityRequired;

		std::map<QString, aPropertyGridGroup *>	my_groups;
		std::map<ID, aPropertyGridItem *>		my_items;
		std::map<aPropertyGridItem *, bool>		my_itemStateMap;

		aPropertyGridGroup *					my_defaultGroup;

		ID										my_currentID;
		bool									my_isEnabled;

		QColor									my_groupHeaderForeColor;
		QColor									my_groupHeaderBackColor;
		QColor									my_itemDefaultBackgroundColor;
		QColor									my_itemTextColorError;
		QColor									my_itemTextColorNormal;

		QIcon									my_groupIconExpanded;
		QIcon									my_groupIconCollapsed;

		aPropertyGridWidget(aPropertyGridWidget &) = delete;
		aPropertyGridWidget& operator = (aPropertyGridWidget &) = delete;
	};

	// ##############################################################################################################

	// ##############################################################################################################

	// ##############################################################################################################

	class UICORE_API_EXPORT aPropertyGridGroup : public QObject {
		Q_OBJECT
	public:
		aPropertyGridGroup(
			aTableWidget *						_propertyGridTable,
			const QString &						_groupName
		);

		virtual ~aPropertyGridGroup();

		QString name(void) const;

		void setName(
			const QString &						_groupName
		);

		void activate(void);

		void setHeaderColors(
			const QColor &						_foreColor,
			const QColor &						_backColor
		);

		void setItemsBackColor(
			const QColor &						_backgroundColor
		);

		void setItemsTextColors(
			const QColor &						_textColorNormal,
			const QColor &						_textColorError
		);

		aPropertyGridItem * addItem(
			ID											_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			bool											_value
		);

		aPropertyGridItem * addItem(
			ID											_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aColor &								_value
		);

		aPropertyGridItem * addItem(
			ID											_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			double											_value
		);

		aPropertyGridItem * addItem(
			ID											_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			int												_value
		);

		aPropertyGridItem * addItem(
			ID											_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const std::vector<QString> &					_possibleSelection,
			const QString &									_value
		);

		aPropertyGridItem * addItem(
			ID											_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const QString &									_value
		);

		void setGroupHeaderVisible(
			bool											_isVisible
		);

		void clear(void);

		void deselect(void);

		void setStateIcons(
			QIcon *											_expanded,
			QIcon *											_collapsed
		);

	private slots:
		void slotDoubleClicked(QTableWidgetItem *);

	private:

		void checkVisibility(void);

		void repaint(void);

		void refreshIcon(void);

		std::list<aPropertyGridItem *>			my_items;
		aTableWidget *							my_propertyGridTable;
		QTableWidgetItem *						my_item;
		bool									my_headerIsVisible;

		QString									my_name;
		bool									my_isActivated;
		bool									my_isVisible;
		bool									my_isAlternateBackground;

		QColor									my_foreColor;
		QColor									my_backColor;
		QColor									my_colorTextError;
		QColor									my_colorTextNormal;
		QColor									my_colorItemBackground;
		QColor									my_colorItemBackgroundAlternate;

		QIcon *									my_iconExpanded;
		QIcon *									my_iconCollapsed;

		aPropertyGridGroup() = delete;
		aPropertyGridGroup(const aPropertyGridGroup &) = delete;
	};

	// ##############################################################################################################

	// ##############################################################################################################

	// ##############################################################################################################

	class UICORE_API_EXPORT aPropertyGridItem : public QObject {
		Q_OBJECT
	public:
		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			bool								_value
		);

		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			const aColor &						_value
		);

		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			double								_value
		);

		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			int									_value
		);

		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			const std::vector<QString> &		_possibleSlection,
			const QString &						_value
		);

		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			const QString &						_value
		);

		virtual ~aPropertyGridItem();

		int row() const;

		void setId(
			ID								_ID
		);

		ID getId(void) const;

		bool getIsCurrentlyError() const;

		void setBackgroundColor(
			const QColor &						_backgroundColor
		);

		void setTextColors(
			const QColor &						_foregroundNormal,
			const QColor &						_foregroundError
		);

		void deselect(void);

		// #################################################################################

		// Information gathering

		QString getGroup() const;

		bool getIsMultipleValues() const;

		QString getName() const;

		valueType getValueType() const;

		std::vector<QString> getPossibleSelection() const;

		bool getValueBool() const;

		aColor getValueColor() const;

		double getValueDouble() const;

		int getValueInteger() const;

		QString getValueSelection() const;

		QString getValueString() const;

		bool isMultipleValues(void) const;

		void setReadOnly(
			bool					_readOnly = true
		);

		bool isReadOnly();

		void setEnabled(
			bool					_enabled = true
		);

		bool isEnabled(void) const { return my_isEnabled; }

		void setColorStyle(
			const aColorStyle *	_style
		);

		//! @brief Will set the provided value to this item
		//! @param _valueToReset The value to set
		void resetAsError(
			const QString &									_valueToReset
		);

		//! @brief Will set the provided value to this item
		//! @param _valueToReset The value to set
		void resetAsError(
			int												_valueToReset
		);

		//! @brief Will set the provided value to this item
		//! @param _valueToReset The value to set
		void resetAsError(
			double											_valueToReset
		);

		//! @brief Will show the item as an error
		void showAsError(void);

	signals:
		void changed(void);

	private slots:
		void slotValueWidgetEvent(void);
		void slotTableCellChanged(QTableWidgetItem *);

	private:

		void ini(void);

		void repaint(void);

		ID								my_id;
		bool							my_isMultipleValues;
		bool							my_ignoreCellEvent;
		bool							my_isCurrentlyError;
		bool							my_isReadOnly;
		bool							my_isEnabled;

		aTableWidget *					my_propertyGridTable;
		QString							my_group;
		QString							my_name;

		aColor							my_colorErrorForeground;
		aColor							my_colorNormalForeground;
		aColor							my_colorBackground;

		const aColorStyle *				my_globalColorStyle;

		QTableWidgetItem *				my_cellSettingName;
		QTableWidgetItem *				my_cellValue;

		valueType						my_valueType;
		bool							my_valueBool;
		aColor							my_valueColor;
		double							my_valueDouble;
		int								my_valueInteger;
		QString							my_valueSelection;
		QString							my_valueString;

		std::vector<QString>			my_valuePossibleSelection;

		aCheckBoxWidget *				my_widgetBool;
		aColorEditButtonWidget *		my_widgetColor;
		aComboButtonWidget *			my_widgetSelection;


		aPropertyGridItem() = delete;
		aPropertyGridItem(const aPropertyGridItem&) = delete;
	};
}