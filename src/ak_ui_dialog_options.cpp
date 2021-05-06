/*
 * ak_ui_objectManager.cpp
 *
 *  Created on: November 03, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_dialog_options.h>		// Corresponding header

#include <ak_ui_qt_tree.h>
#include <ak_ui_qt_propertyGrid.h>

#include <ak_ui_qt_pushButton.h>

#include <ak_ui_windowEventHandler.h>
#include <ak_ui_colorStyle.h>
#include <ak_messenger.h>

#include <qwidget.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qevent.h>

ak::ui::dialog::options::options(
	const QString &						_title
) :
	ui::core::aPaintable(ui::core::objectType::oOptionsDialog),
	my_mainLayout{ nullptr }, my_buttonsLayout{ nullptr },
	my_buttonsWidget{ nullptr }, my_centralWidget{ nullptr }, my_tree{ nullptr }, my_dummy{ nullptr },
	my_btnApply{ nullptr }, my_btnCancel{ nullptr }, my_btnOk{ nullptr }, my_groupIconsSet{ false },
	my_settingsChanged{ false }
{
	// Create layouts
	my_mainLayout = new QVBoxLayout{ this };
	my_buttonsWidget = new QWidget;
	my_buttonsLayout = new QHBoxLayout{ my_buttonsWidget };
	my_centralWidget = new QSplitter(Qt::Orientation::Horizontal);
	my_mainLayout->addWidget(my_centralWidget, 2);
	my_mainLayout->addWidget(my_buttonsWidget);

	// Create buttons
	my_btnApply = new qt::pushButton{ "Apply" };
	my_btnCancel = new qt::pushButton{ "Cancel" };
	my_btnOk = new qt::pushButton{ "Ok" };
	my_buttonsLayout->addStretch();
	my_buttonsLayout->addWidget(my_btnOk, 0, Qt::AlignRight);
	my_buttonsLayout->addWidget(my_btnApply, 0, Qt::AlignRight);
	my_buttonsLayout->addWidget(my_btnCancel, 0, Qt::AlignRight);
	my_btnApply->setEnabled(false);

	connect(my_btnApply, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(my_btnCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
	connect(my_btnOk, SIGNAL(clicked()), this, SLOT(slotOk()));
	
	// Create central components
	my_tree = new qt::tree;
	my_tree->setMultiSelectionEnabled(false);
	my_tree->setSortingEnabled(true);
	if (my_colorStyle != nullptr) { my_tree->setColorStyle(my_colorStyle); }
	connect(my_tree, &qt::tree::selectionChanged, this, &options::slotSelectionChanged);

	my_dummy = new qt::propertyGrid;
	my_dummy->setUid(2);
	if (my_colorStyle != nullptr) { my_dummy->setColorStyle(my_colorStyle); }

	my_centralWidget->addWidget(my_tree->widget());
	my_centralWidget->addWidget(my_dummy->widget());
	
	// Set properties
	setMinimumSize(400, 300);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowTitle(_title);
}

ak::ui::dialog::options::~options() {
	A_OBJECT_DESTROYING

	for (auto itm : my_categories) {
		delete itm.second;
	}
	delete my_dummy;
	delete my_btnApply;
	delete my_btnCancel;
	delete my_btnOk;
	delete my_tree;
	delete my_buttonsLayout;
	delete my_buttonsWidget;
	delete my_centralWidget;
	delete my_mainLayout;
}

void ak::ui::dialog::options::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	my_colorStyle = _colorStyle;
	my_tree->setColorStyle(_colorStyle);
	my_btnApply->setColorStyle(_colorStyle);
	my_btnCancel->setColorStyle(_colorStyle);
	my_btnOk->setColorStyle(_colorStyle);
	my_dummy->setColorStyle(_colorStyle);
	for (auto itm : my_categories) {
		itm.second->setColorStyle(_colorStyle);
	}
	if (my_colorStyle != nullptr) {
		QString sheet(my_colorStyle->toStyleSheet(core::colorAreaFlag::caBackgroundColorDialogWindow | core::colorAreaFlag::caForegroundColorDialogWindow, "QDialog {", "}"));
		setStyleSheet(sheet);
		sheet = my_colorStyle->toStyleSheet(core::colorAreaFlag::caBackgroundColorButton |
			core::colorAreaFlag::caForegroundColorButton, "QSplitter[orientation=\"1\"]::handle {", "}");
		my_centralWidget->setStyleSheet(sheet);
	}
	else {
		setStyleSheet("");
		my_centralWidget->setStyleSheet("");
	}
}

void ak::ui::dialog::options::closeEvent(QCloseEvent * _event) {
	if (my_settingsChanged) {
		for (auto handler : my_windowEventHandler) {
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

ak::ID ak::ui::dialog::options::createCategory(
	ID									_parentID,
	const QString &						_text
) {
	ID id = my_tree->add(_parentID, _text);
	optionsCategory * newCategory = new optionsCategory{ this, id };
	my_categories.insert_or_assign(id, newCategory);
	if (my_groupIconsSet) {
		newCategory->getPropertyGrid()->setGroupStateIcons(my_iconGroupExpanded, my_iconGroupCollapsed);
	}
	if (my_colorStyle != nullptr) { newCategory->setColorStyle(my_colorStyle); }
	return id;
}

ak::ID ak::ui::dialog::options::createCategory(
	ID									_parentID,
	const QString &						_text,
	const QIcon &						_icon
) {
	ID id = my_tree->add(_parentID, _text);
	my_tree->setItemIcon(id, _icon);
	optionsCategory * newCategory = new optionsCategory{ this, id };
	my_categories.insert_or_assign(id, newCategory);
	if (my_groupIconsSet) {
		newCategory->getPropertyGrid()->setGroupStateIcons(my_iconGroupExpanded, my_iconGroupCollapsed);
	}
	if (my_colorStyle != nullptr) { newCategory->setColorStyle(my_colorStyle); }
	return id;
}

void ak::ui::dialog::options::createGroup(
	ID									_categoryID,
	const QString &						_name,
	const QColor &						_color
) {
	assert(_categoryID >= 0 && _categoryID < my_categories.size());
	auto actualCategory = my_categories.at(_categoryID);
	actualCategory->getPropertyGrid()->addGroup(_color, _name);
}

void ak::ui::dialog::options::setGroupStateIcons(
	const QIcon &						_groupExpanded,
	const QIcon &						_groupCollapsed
) {
	my_iconGroupExpanded = _groupExpanded;
	my_iconGroupCollapsed = _groupCollapsed;
	my_groupIconsSet = true;
	for (auto category : my_categories) {
		category.second->getPropertyGrid()->setGroupStateIcons(my_iconGroupExpanded, my_iconGroupCollapsed);
	}
}

// ###################################################################################################################################################

// Item creation

// Add item to default group

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const ui::color &								_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _possibleSelection, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _settingName, _value);
}

// Add item to specified group

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	bool											_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const ui::color &									_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	double											_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	int												_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _possibleSelection, _value);
}

ak::ID ak::ui::dialog::options::addItem(
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) {
	auto itm{ my_categories.find(_categoryID) };
	assert(itm != my_categories.end());	// Invalid category ID
	return itm->second->getPropertyGrid()->addItem(_isMultipleValues, _groupName, _settingName, _value);
}

// ###################################################################################################################################################

// Data manipulation

void ak::ui::dialog::options::clear(void) {
	my_tree->clear();
	for (auto category : my_categories) {
		delete category.second;
	}
	my_categories.clear();
	my_settingsChanged = false;
}

// ###################################################################################################################################################

// Event handling

void ak::ui::dialog::options::slotSelectionChanged() {
	auto selection{ my_tree->selectedItems() };
	if (selection.size() == 0) {
		my_centralWidget->replaceWidget(1, my_dummy->widget());
	}
	else if (selection.size() == 1) {
		auto category{ my_categories.find(selection.at(0)) };
		assert(category != my_categories.end()); // That should not happen. Invalid category selected
		my_centralWidget->replaceWidget(1, category->second->widget());
	}
	else {
		assert(0);	// To many items selected
	}
}

void ak::ui::dialog::options::addWindowEventHandler(
	windowEventHandler *					_handler
) { my_windowEventHandler.push_back(_handler); }

void ak::ui::dialog::options::slotChanged(int _itemId) {
	my_settingsChanged = true;
	my_result = ui::core::resultCancel;
}

void ak::ui::dialog::options::slotOk() {
	slotApply();
	close();
}

void ak::ui::dialog::options::slotApply() {
	my_result = ui::core::resultYes;
	emit changed();
}

void ak::ui::dialog::options::slotCancel() {
	close();
}

// ###################################################################################################################################################

// ###################################################################################################################################################

// ###################################################################################################################################################

ak::ui::dialog::optionsCategory::optionsCategory(
	options *			_optionsDialog,
	ID					_treeItemId
) : core::aWidget{ core::oNone }, my_treeItemId{ _treeItemId }, my_propertyGrid{ nullptr }, my_optionsDialog{ _optionsDialog }
{
	my_propertyGrid = new qt::propertyGrid;
	my_propertyGrid->setUid(2);
}

ak::ui::dialog::optionsCategory::~optionsCategory() {
	delete my_propertyGrid;
}

// #######################################################################################################

// Base class functions

QWidget * ak::ui::dialog::optionsCategory::widget(void) { return my_propertyGrid->widget(); }

void ak::ui::dialog::optionsCategory::setColorStyle(
	const ak::ui::colorStyle *					_colorStyle
) { my_propertyGrid->setColorStyle(_colorStyle); }
