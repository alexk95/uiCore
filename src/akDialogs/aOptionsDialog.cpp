/*
 *	File:		aOptionsDialog.cpp
 *	Package:	akDialogs
 *
 *  Created on: November 03, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // Corresponding header
#include <akDialogs/aOptionsDialog.h>

// AK Core header
#include <akCore/akCore.h>
#include <akCore/aMessenger.h>

// AK GUI header
#include <akGui/aColorStyle.h>
#include <akGui/aWindowEventHandler.h>

// AK Widgets header
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aTreeWidget.h>
#include <akWidgets/aPropertyGridWidget.h>

// QT header
#include <qwidget.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qevent.h>

ak::aOptionsDialog::aOptionsDialog(
	const QString &						_title
) :
	aPaintable(otOptionsDialog),
	m_mainLayout{ nullptr }, m_buttonsLayout{ nullptr },
	m_buttonsWidget{ nullptr }, m_centralWidget{ nullptr }, m_tree{ nullptr }, m_dummy{ nullptr },
	m_btnApply{ nullptr }, m_btnCancel{ nullptr }, m_btnOk{ nullptr }, m_groupIconsSet{ false },
	m_settingsChanged{ false }
{
	// Create layouts
	m_mainLayout = new QVBoxLayout{ this };
	m_buttonsWidget = new QWidget;
	m_buttonsLayout = new QHBoxLayout{ m_buttonsWidget };
	m_centralWidget = new QSplitter(Qt::Orientation::Horizontal);
	m_mainLayout->addWidget(m_centralWidget, 2);
	m_mainLayout->addWidget(m_buttonsWidget);

	// Create buttons
	m_btnApply = new aPushButtonWidget{ "Apply" };
	m_btnCancel = new aPushButtonWidget{ "Cancel" };
	m_btnOk = new aPushButtonWidget{ "Ok" };
	m_buttonsLayout->addStretch();
	m_buttonsLayout->addWidget(m_btnOk, 0, Qt::AlignRight);
	m_buttonsLayout->addWidget(m_btnApply, 0, Qt::AlignRight);
	m_buttonsLayout->addWidget(m_btnCancel, 0, Qt::AlignRight);
	m_btnApply->setEnabled(false);

	connect(m_btnApply, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
	connect(m_btnOk, SIGNAL(clicked()), this, SLOT(slotOk()));
	
	// Create central components
	m_tree = new aTreeWidget;
	m_tree->setMultiSelectionEnabled(false);
	m_tree->setSortingEnabled(true);
	if (m_colorStyle != nullptr) { m_tree->setColorStyle(m_colorStyle); }
	connect(m_tree, &aTreeWidget::selectionChanged, this, &aOptionsDialog::slotSelectionChanged);

	m_dummy = new aPropertyGridWidget;
	m_dummy->setUid(2);
	if (m_colorStyle != nullptr) { m_dummy->setColorStyle(m_colorStyle); }

	m_centralWidget->addWidget(m_tree->widget());
	m_centralWidget->addWidget(m_dummy->widget());
	
	// Set properties
	setMinimumSize(400, 300);
	setWindowTitle(_title);
	hideInfoButton();
}

ak::aOptionsDialog::~aOptionsDialog() {
	A_OBJECT_DESTROYING

	for (auto itm : m_categories) {
		delete itm.second;
	}
	delete m_dummy;
	delete m_btnApply;
	delete m_btnCancel;
	delete m_btnOk;
	delete m_tree;
	delete m_buttonsLayout;
	delete m_buttonsWidget;
	delete m_centralWidget;
	delete m_mainLayout;
}

void ak::aOptionsDialog::setColorStyle(
	aColorStyle *			_colorStyle
) {
	m_colorStyle = _colorStyle;
	m_tree->setColorStyle(_colorStyle);
	m_btnApply->setColorStyle(_colorStyle);
	m_btnCancel->setColorStyle(_colorStyle);
	m_btnOk->setColorStyle(_colorStyle);
	m_dummy->setColorStyle(_colorStyle);
	for (auto itm : m_categories) {
		itm.second->setColorStyle(_colorStyle);
	}
	if (m_colorStyle != nullptr) {
		QString sheet(m_colorStyle->toStyleSheet(cafBackgroundColorDialogWindow | cafForegroundColorDialogWindow, "QDialog {", "}"));
		setStyleSheet(sheet);
		sheet = m_colorStyle->toStyleSheet(cafBackgroundColorButton |
			cafForegroundColorButton, "QSplitter[orientation=\"1\"]::handle {", "}");
		m_centralWidget->setStyleSheet(sheet);
	}
	else {
		setStyleSheet("");
		m_centralWidget->setStyleSheet("");
	}
}

void ak::aOptionsDialog::closeEvent(QCloseEvent * _event) {
	if (m_settingsChanged) {
		for (auto handler : m_windowEventHandler) {
			if (!handler->closeEvent()) {
				_event->ignore();
				return;
			}
		}
	}
	_event->accept();
	QDialog::closeEvent(_event);
}

// ###################################################################################################################################################

ak::ID ak::aOptionsDialog::createCategory(
	ID									_parentID,
	const QString &						_text
) {
	ID id = m_tree->add(_parentID, _text);
	aOptionsDialogCategory * newCategory = new aOptionsDialogCategory{ this, id };
	m_categories.insert_or_assign(id, newCategory);
	if (m_groupIconsSet) {
		newCategory->getPropertyGrid()->setGroupStateIcons(m_iconGroupExpanded, m_iconGroupCollapsed);
	}
	if (m_colorStyle != nullptr) { newCategory->setColorStyle(m_colorStyle); }
	return id;
}

ak::ID ak::aOptionsDialog::createCategory(
	ID									_parentID,
	const QString &						_text,
	const QIcon &						_icon
) {
	ID id = m_tree->add(_parentID, _text);
	m_tree->setItemIcon(id, _icon);
	aOptionsDialogCategory * newCategory = new aOptionsDialogCategory{ this, id };
	m_categories.insert_or_assign(id, newCategory);
	if (m_groupIconsSet) {
		newCategory->getPropertyGrid()->setGroupStateIcons(m_iconGroupExpanded, m_iconGroupCollapsed);
	}
	if (m_colorStyle != nullptr) { newCategory->setColorStyle(m_colorStyle); }
	return id;
}

void ak::aOptionsDialog::createGroup(
	ID									_categoryID,
	const QString &						_name,
	const QColor &						_color
) {
	assert(_categoryID >= 0 && _categoryID < m_categories.size());
	auto actualCategory = m_categories.at(_categoryID);
	actualCategory->getPropertyGrid()->addGroup(_color, _name);
}

void ak::aOptionsDialog::setGroupStateIcons(
	const QIcon &						_groupExpanded,
	const QIcon &						_groupCollapsed
) {
	m_iconGroupExpanded = _groupExpanded;
	m_iconGroupCollapsed = _groupCollapsed;
	m_groupIconsSet = true;
	for (auto category : m_categories) {
		category.second->getPropertyGrid()->setGroupStateIcons(m_iconGroupExpanded, m_iconGroupCollapsed);
	}
}

// ###################################################################################################################################################

// Item creation

// Add item to default group

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aColor &								_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _possibleSelection, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

// Add item to specified group

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	bool											_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const aColor &									_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	double											_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	int												_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _possibleSelection, _value);
}

ak::ID ak::aOptionsDialog::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) {
	auto itm{ m_categories.find(_categoryID) };
	assert(itm != m_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

// ###################################################################################################################################################

// Data manipulation

void ak::aOptionsDialog::clear(void) {
	m_tree->clear();
	for (auto category : m_categories) {
		delete category.second;
	}
	m_categories.clear();
	m_settingsChanged = false;
}

// ###################################################################################################################################################

// Event handling

void ak::aOptionsDialog::slotSelectionChanged() {
	auto selection{ m_tree->selectedItems() };
	if (selection.size() == 0) {
		m_centralWidget->replaceWidget(1, m_dummy->widget());
	}
	else if (selection.size() == 1) {
		auto category{ m_categories.find(selection.at(0)) };
		assert(category != m_categories.end()); // That should not happen. Invalid category selected
		m_centralWidget->replaceWidget(1, category->second->widget());
	}
	else {
		assert(0);	// To many items selected
	}
}

void ak::aOptionsDialog::addWindowEventHandler(
	aWindowEventHandler *					_handler
) { m_windowEventHandler.push_back(_handler); }

void ak::aOptionsDialog::slotChanged(int _itemId) {
	m_settingsChanged = true;
	m_result = resultCancel;
}

void ak::aOptionsDialog::slotOk() {
	slotApply();
	close();
}

void ak::aOptionsDialog::slotApply() {
	m_result = resultYes;
	emit changed();
}

void ak::aOptionsDialog::slotCancel() {
	close();
}

// ###################################################################################################################################################

// ###################################################################################################################################################

// ###################################################################################################################################################

ak::aOptionsDialogCategory::aOptionsDialogCategory(
	aOptionsDialog *			_optionsDialog,
	ID					_treeItemId
) : aWidget{ otNone }, m_treeItemId{ _treeItemId }, m_propertyGrid{ nullptr }, m_optionsDialog{ _optionsDialog }
{
	m_propertyGrid = new aPropertyGridWidget;
	m_propertyGrid->setUid(2);
}

ak::aOptionsDialogCategory::~aOptionsDialogCategory() {
	delete m_propertyGrid;
}

// #######################################################################################################

// Base class functions

QWidget * ak::aOptionsDialogCategory::widget(void) { return m_propertyGrid->widget(); }

void ak::aOptionsDialogCategory::setColorStyle(
	ak::aColorStyle *					_colorStyle
) { m_propertyGrid->setColorStyle(_colorStyle); }
