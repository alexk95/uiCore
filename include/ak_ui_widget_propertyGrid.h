/*
 * ak_ui_widget_propertyGrid.h
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
#include <qobject.h>					// Base class
#include <qstring.h>					//
#include <qcolor.h>						// QColor
#include <qtablewidget.h>				// QTableWidgetItem

// AK header
#include <ak_ui_core_aWidgetManager.h>	// Base class
#include <ak_globalDataTypes.h>			// AK ID type
#include <ak_core.h>
#include <ak_ui_color.h>

// Forward declaration
class QWidget;

namespace ak {

	class messenger;
	class uidManager;

	namespace ui {
		
		class colorStyle;
		namespace qt { class table; class checkBox; class colorEditButton; class comboButton; }

		namespace widget {

			class propertyGridGroup;
			class propertyGridItem;

			class propertyGrid : public QObject, public ui::core::aWidgetManager {
				Q_OBJECT
			public:
				propertyGrid(
					messenger *				_messenger,
					uidManager *			_uidManager
				);

				virtual ~propertyGrid();

				// ##############################################################################################################

				// base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *						_colorStyle
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

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_settingName,
					bool											_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_settingName,
					const ui::color &								_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_settingName,
					double											_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_settingName,
					int												_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_settingName,
					const std::vector<QString> &					_possibleSelection,
					const QString &									_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_settingName,
					const QString &									_value
				);

				// Add item to specified group

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					bool											_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					const ui::color &									_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					double											_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					int												_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					const std::vector<QString> &					_possibleSelection,
					const QString &									_value
				);

				ak::ID addItem(
					bool											_isMultipleValues,
					const QString &									_groupName,
					const QString &									_settingName,
					const QString &									_value
				);

				// ##############################################################################################################

				// Item information gathering

				QString getItemGroup(
					ak::ID											_itemID
				);

				QString getItemName(
					ak::ID											_itemID
				);

				std::vector<QString> getItemPossibleSelection(
					ak::ID											_itemID
				);

				bool getItemValueBool(
					ak::ID											_itemID
				);

				ui::color getItemValueColor(
					ak::ID											_itemID
				);

				double getItemValueDouble(
					ak::ID											_itemID
				);

				int getItemValueInteger(
					ak::ID											_itemID
				);

				QString getItemValueSelection(
					ak::ID											_itemID
				);

				QString getItemValueString(
					ak::ID											_itemID
				);

				ak::core::valueType getItemValueType(
					ak::ID											_itemID
				);

			private slots:
				void slotItemChanged(void);

			private:
				qt::table *								my_table;

				std::map<QString, propertyGridGroup *>	my_groups;
				typedef std::map<QString,
					propertyGridGroup *>::iterator		my_groupsIterator;

				std::map<ak::ID, propertyGridItem *>	my_items;
				typedef std::map<ak::ID,
					propertyGridItem *>::iterator		my_itemsIterator;

				propertyGridGroup *						my_defaultGroup;

				ak::ID									my_currentID;

				QColor									my_groupHeaderForeColor;
				QColor									my_groupHeaderBackColor;
				QColor									my_itemDefaultBackgroundColor;
				QColor									my_itemTextColorError;
				QColor									my_itemTextColorNormal;

			};

			// ##############################################################################################################

			// ##############################################################################################################

			// ##############################################################################################################

			class propertyGridGroup : public QObject {
				Q_OBJECT
			public:
				propertyGridGroup(
					qt::table *							_propertyGridTable,
					const QString &						_groupName
				);

				virtual ~propertyGridGroup();

				QString name(void) const;

				void setName(
					const QString &						_groupName
				);

				void activate(void);

				void setHeaderColors(
					const QColor &								_foreColor,
					const QColor &								_backColor
				);

				void setItemsBackColor(
					const QColor &									_backgroundColor
				);

				void setItemsTextColors(
					const QColor &									_textColorNormal,
					const QColor &									_textColorError
				);

				propertyGridItem * addItem(
					ak::ID											_itemId,
					bool											_isMultipleValues,
					const QString &									_settingName,
					bool											_value
				);

				propertyGridItem * addItem(
					ak::ID											_itemId,
					bool											_isMultipleValues,
					const QString &									_settingName,
					const ui::color &								_value
				);

				propertyGridItem * addItem(
					ak::ID											_itemId,
					bool											_isMultipleValues,
					const QString &									_settingName,
					double											_value
				);

				propertyGridItem * addItem(
					ak::ID											_itemId,
					bool											_isMultipleValues,
					const QString &									_settingName,
					int												_value
				);

				propertyGridItem * addItem(
					ak::ID											_itemId,
					bool											_isMultipleValues,
					const QString &									_settingName,
					const std::vector<QString> &					_possibleSelection,
					const QString &									_value
				);

				propertyGridItem * addItem(
					ak::ID											_itemId,
					bool											_isMultipleValues,
					const QString &									_settingName,
					const QString &									_value
				);

			private slots:
				void slotDoubleClicked(QTableWidgetItem *);

			private:

				void checkVisibility(void);

				void repaint(void);

				std::list<propertyGridItem *>			my_items;
				qt::table *								my_propertyGridTable;
				QTableWidgetItem *						my_item;

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

				propertyGridGroup() = delete;
				propertyGridGroup(const propertyGridGroup &) = delete;

			};

			// ##############################################################################################################

			// ##############################################################################################################

			// ##############################################################################################################

			class propertyGridItem : public QObject {
				Q_OBJECT
			public:
				propertyGridItem(
					qt::table *							_propertyGridTable,
					const QString &						_group,
					int									_row,
					bool								_isMultipleValues,
					const QString &						_settingName,
					bool								_value
				);

				propertyGridItem(
					qt::table *							_propertyGridTable,
					const QString &						_group,
					int									_row,
					bool								_isMultipleValues,
					const QString &						_settingName,
					const ui::color &					_value
				);

				propertyGridItem(
					qt::table *							_propertyGridTable,
					const QString &						_group,
					int									_row,
					bool								_isMultipleValues,
					const QString &						_settingName,
					double								_value
				);

				propertyGridItem(
					qt::table *							_propertyGridTable,
					const QString &						_group,
					int									_row,
					bool								_isMultipleValues,
					const QString &						_settingName,
					int									_value
				);

				propertyGridItem(
					qt::table *							_propertyGridTable,
					const QString &						_group,
					int									_row,
					bool								_isMultipleValues,
					const QString &						_settingName,
					const std::vector<QString> &		_possibleSlection,
					const QString &						_value
				);

				propertyGridItem(
					qt::table *							_propertyGridTable,
					const QString &						_group,
					int									_row,
					bool								_isMultipleValues,
					const QString &						_settingName,
					const QString &						_value
				);

				virtual ~propertyGridItem();

				int row() const;

				void setId(
					ak::ID								_ID
				);

				ak::ID getId(void) const;

				bool getIsCurrentlyError() const;

				void setBackgroundColor(
					const QColor &						_backgroundColor
				);

				void setTextColors(
					const QColor &						_foregroundNormal,
					const QColor &						_foregroundError
				);


				// #################################################################################

				// Information gathering

				QString getGroup() const;

				QString getName() const;

				ak::core::valueType getValueType() const;

				std::vector<QString> getPossibleSelection() const;

				bool getValueBool() const;

				ui::color getValueColor() const;

				double getValueDouble() const;

				int getValueInteger() const;

				QString getValueSelection() const;

				QString getValueString() const;

				bool isMultipleValues(void) const;

			signals:
				void changed(void);

			private slots:
				void slotValueWidgetEvent(void);
				void slotTableCellChanged(QTableWidgetItem *);

			private:

				void ini(void);

				void repaint(void);

				ak::ID							my_id;
				bool							my_isMultipleValues;
				bool							my_ignoreCellEvent;
				bool							my_isCurrentlyError;

				qt::table *						my_propertyGridTable;
				QString							my_group;
				QString							my_name;

				QColor							my_colorErrorForeground;
				QColor							my_colorNormalForeground;
				ui::color						my_colorBackground;


				QTableWidgetItem *				my_cellSettingName;
				QTableWidgetItem *				my_cellValue;

				ak::core::valueType				my_valueType;
				bool							my_valueBool;
				ui::color						my_valueColor;
				double							my_valueDouble;
				int								my_valueInteger;
				QString							my_valueSelection;
				QString							my_valueString;

				std::vector<QString>			my_valuePossibleSelection;

				qt::checkBox *					my_widgetBool;
				qt::colorEditButton *			my_widgetColor;
				qt::comboButton *				my_widgetSelection;


				propertyGridItem() = delete;
				propertyGridItem(const propertyGridItem&) = delete;
			};
		}
	}
}