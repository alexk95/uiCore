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
#include <akCore/aDate.h>
#include <akCore/aTime.h>
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
	class aCustomizableColorStyle;
	class aColorEditButtonWidget;
	class aComboButtonWidget;
	class aTextEditWidget;
	class aDatePickWidget;
	class aTimePickWidget;

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
			aColorStyle *						_colorStyle
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

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aDate &									_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aTime &									_value
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

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			const aDate &									_value
		);

		ID addItem(
			bool											_isMultipleValues,
			const QString &									_groupName,
			const QString &									_settingName,
			const aTime &									_value
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

		aDate getItemValueDate(
			ID											_itemID
		);

		aTime getItemValueTime(
			ID											_itemID
		);

		valueType getItemValueType(
			ID											_itemID
		);

		bool isEnabled(void) const { return m_isEnabled; }

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

		QVBoxLayout *							m_layout;
		aTextEditWidget *						m_infoTextEdit;
		aTableWidget *							m_table;
		bool									m_checkItemVisibilityRequired;

		std::map<QString, aPropertyGridGroup *>	m_groups;
		std::map<ID, aPropertyGridItem *>		m_items;
		std::map<aPropertyGridItem *, bool>		m_itemStateMap;

		aPropertyGridGroup *					m_defaultGroup;

		ID										m_currentID;
		bool									m_isEnabled;

		QColor									m_groupHeaderForeColor;
		QColor									m_groupHeaderBackColor;
		QColor									m_itemDefaultBackgroundColor;
		QColor									m_itemTextColorError;
		QColor									m_itemTextColorNormal;

		QIcon									m_groupIconExpanded;
		QIcon									m_groupIconCollapsed;

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
			const QColor &						_backgroundColor,
			bool								_repaint
		);

		void setItemsTextColors(
			const QColor &						_textColorNormal,
			const QColor &						_textColorError,
			bool								_repaint
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			bool											_value
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aColor &									_value
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			double											_value
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			int												_value
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const std::vector<QString> &					_possibleSelection,
			const QString &									_value
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const QString &									_value
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aDate &									_value
		);

		aPropertyGridItem * addItem(
			ID												_itemId,
			bool											_isMultipleValues,
			const QString &									_settingName,
			const aTime &									_value
		);

		void setGroupHeaderVisible(
			bool											_isVisible
		);

		void setColorStyle(
			aColorStyle *									_colorStyle,
			bool											_repaint
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

		void itemCreated(aPropertyGridItem * _item);

		void checkVisibility(void);

		void repaint(void);

		void rebuildStyleSheets(void);

		void refreshIcon(void);

		std::list<aPropertyGridItem *>			m_items;
		aTableWidget *							m_propertyGridTable;
		QTableWidgetItem *						m_item;
		bool									m_headerIsVisible;

		QString									m_name;
		bool									m_isActivated;
		bool									m_isVisible;
		bool									m_isAlternateBackground;

		QColor									m_foreColor;
		QColor									m_backColor;
		QColor									m_colorTextError;
		QColor									m_colorTextNormal;
		QColor									m_colorItemBackground;
		QColor									m_colorItemBackgroundAlternate;

		QIcon *									m_iconExpanded;
		QIcon *									m_iconCollapsed;

		aColorStyle *							m_externColorStyle;
		aCustomizableColorStyle *				m_colorStyle;
		aCustomizableColorStyle *				m_colorStyleAlt;

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

		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			const aDate &						_value
		);

		aPropertyGridItem(
			aTableWidget *						_propertyGridTable,
			const QString &						_group,
			int									_row,
			bool								_isMultipleValues,
			const QString &						_settingName,
			const aTime &						_value
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

		QString getGroup(void) const;

		bool getIsMultipleValues(void) const;

		QString getName(void) const;

		valueType getValueType(void) const;

		std::vector<QString> getPossibleSelection(void) const;

		bool getValueBool(void) const;

		aColor getValueColor(void) const;

		double getValueDouble(void) const;

		int getValueInteger(void) const;

		QString getValueSelection(void) const;

		QString getValueString(void) const;

		aDate getValueDate(void) const;

		aTime getValueTime(void) const;

		bool isMultipleValues(void) const;

		void setReadOnly(
			bool					_readOnly = true
		);

		bool isReadOnly();

		void setEnabled(
			bool					_enabled = true
		);

		bool isEnabled(void) const { return m_isEnabled; }

		void setColorStyle(
			aColorStyle *	_style
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

		ID								m_id;
		bool							m_isMultipleValues;
		bool							m_ignoreCellEvent;
		bool							m_isCurrentlyError;
		bool							m_isReadOnly;
		bool							m_isEnabled;

		aTableWidget *					m_propertyGridTable;
		QString							m_group;
		QString							m_name;

		aColor							m_colorErrorForeground;
		aColor							m_colorNormalForeground;
		aColor							m_colorBackground;

		aColorStyle *					m_globalColorStyle;

		QTableWidgetItem *				m_cellSettingName;
		QTableWidgetItem *				m_cellValue;

		valueType						m_valueType;
		bool							m_valueBool;
		aColor							m_valueColor;
		double							m_valueDouble;
		int								m_valueInteger;
		QString							m_valueSelection;
		QString							m_valueString;
		aDate							m_valueDate;
		aTime							m_valueTime;

		std::vector<QString>			m_valuePossibleSelection;

		aCheckBoxWidget *				m_widgetBool;
		aColorEditButtonWidget *		m_widgetColor;
		aComboButtonWidget *			m_widgetSelection;
		aDatePickWidget *				m_widgetDate;
		aTimePickWidget *				m_widgetTime;

		aPropertyGridItem() = delete;
		aPropertyGridItem(const aPropertyGridItem&) = delete;
	};
}