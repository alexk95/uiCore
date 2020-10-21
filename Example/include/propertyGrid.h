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
#include <vector>

// Qt header
#include <qobject.h>		// Base class
#include <qstring.h>
#include <qtablewidget.h>
#include <qcolor.h>

// AK header
#include <ak_ui_core_aWidget.h>	// Base class
#include <ak_globalDataTypes.h>	// AK ID type
#include <ak_ui_color.h>

// Forward declaration
class QWidget;

class propertyGridGroup;
class propertyGridItem;

namespace ak { namespace ui { class colorStyle; } }

class propertyGrid : public QObject, public ak::ui::core::aWidget {
	Q_OBJECT
public:
	propertyGrid();
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

	ak::ID addItem(
		const QString &									_settingName,
		const QString &									_value
	);

	ak::ID addItem(
		const QString &									_groupName,
		const QString &									_settingName,
		const QString &									_value
	);

private:
	QTableWidget *							my_table;

	std::map<QString, propertyGridGroup *>	my_groups;
	typedef std::map<QString,
		propertyGridGroup *>::iterator		my_groupsIterator;
	propertyGridGroup *						my_defaultGroup;

	ak::ID									my_currentID;
	QColor									my_alternateForeColor;
	QColor									my_alternateBackColor;
	bool									my_isAlternateGroup;
};

// ##############################################################################################################

// ##############################################################################################################

// ##############################################################################################################

class propertyGridGroup : public QObject {
	Q_OBJECT
public:
	propertyGridGroup(
		const QString &						_groupName,
		QTableWidget *						_propertyGridTable
	);

	virtual ~propertyGridGroup();

	QString name(void) const;

	void setName(
		const QString &						_groupName
	);

	void activate(void);

	void setColors(
		const QColor &								_foreColor,
		const QColor &								_backColor
	);

	void addItem(
		const QString &									_settingName,
		const QString &									_value,
		ak::ID											_itemId
	);



private slots:
	void slotDoubleClicked(QTableWidgetItem *);

private:

	void checkVisibility(void);

	std::vector<propertyGridItem *>			my_items;
	QTableWidget *							my_propertyGridTable;
	QTableWidgetItem *						my_item;

	QString									my_name;
	bool									my_isActivated;
	bool									my_isVisible;

	QColor									my_foreColor;
	QColor									my_backColor;
	bool									my_colorWasSet;

	propertyGridGroup() = delete;
	propertyGridGroup(const propertyGridGroup &) = delete;

};

// ##############################################################################################################

// ##############################################################################################################

// ##############################################################################################################

class propertyGridItem {
public:
	propertyGridItem(
		QTableWidget *						_propertyGridTable,
		int									_row,
		const QString &						_settingName,
		const QString &						_value
	);

	virtual ~propertyGridItem();

	int row() const;

	void setTextColor(
		const QColor &						_color
	);

	void setBackColor(
		const QColor &						_color
	);

private:
	QTableWidget *							my_propertyGridTable;

	QTableWidgetItem *				my_settingName;
	QTableWidgetItem *				my_value;

	propertyGridItem() = delete;
	propertyGridItem(const propertyGridItem&) = delete;
};
