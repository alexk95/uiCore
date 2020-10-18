/*
/ * ak_ui_qt_tree.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_qt_tree.h>			// corresponding header
#include <ak_exception.h>			// error handling
#include <ak_ui_qt_treeItem.h>		// treeItem
#include <ak_ui_core.h>				// objectType
#include <ak_ui_colorStyle.h>		// colorStyle

// Qt header
#include <qheaderview.h>

ak::ui::qt::tree::tree(
	const ak::ui::colorStyle *		_colorStyle
) : QTreeWidget(),
	ak::ui::core::aWidget(ak::ui::core::objectType::oTree, _colorStyle) {
	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }
}

ak::ui::qt::tree::~tree() {}

// #######################################################################################################
// Event handling

void ak::ui::qt::tree::keyPressEvent(QKeyEvent *_event)
{ QTreeWidget::keyPressEvent(_event); emit keyPressed(_event); }

void ak::ui::qt::tree::keyReleaseEvent(QKeyEvent * _event)
{ QTreeWidget::keyReleaseEvent(_event); emit keyReleased(_event); }

void ak::ui::qt::tree::mouseMoveEvent(QMouseEvent * _event)
{ QTreeWidget::mouseMoveEvent(_event); emit mouseMove(_event); }

void ak::ui::qt::tree::leaveEvent(QEvent *_event)
{ QTreeWidget::leaveEvent(_event); emit leave(_event); }

// #######################################################################################################

QWidget * ak::ui::qt::tree::widget(void) { return this; }

void ak::ui::qt::tree::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	try {
		assert(_colorStyle != nullptr); // nullptr provided
		my_colorStyle = _colorStyle;
		setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTree));
		header()->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sObjectHeader));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::tree::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::tree::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::tree::setColorStyle()"); }
}

// ####################################################################################################################################

void ak::ui::qt::tree::AddTopLevelItem(
	treeItem *						_item
) {
	try {
		if (_item == nullptr) { throw ak::Exception("Is nullptr", "Check treeItem"); }
		if (_item->id() == ak::invalidID) { throw ak::Exception("Is invalid ID", "Check item ID"); }
		if (topLevelItem(_item->text(0)) != nullptr) { throw ak::Exception("Item does already exist", "Check for duplicates"); }
		addTopLevelItem(_item);
		my_topLevelItems.insert_or_assign(_item->id(), _item);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::tree::AddTopLevelItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::tree::AddTopLevelItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::tree::AddTopLevelItem()"); }
}

ak::ui::qt::treeItem * ak::ui::qt::tree::topLevelItem(
	const QString &					_text
) {
	try {
		for (my_topLevelItemsIterator itm = my_topLevelItems.begin(); itm != my_topLevelItems.end(); itm++) {
			assert(itm->second != nullptr); // That should not happen..
			if (itm->second->text(0) == _text) { return itm->second; }
		}
		return nullptr;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::tree::topLevelItem(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::tree::topLevelItem(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::tree::topLevelItem(QString)"); }
}

ak::ui::qt::treeItem * ak::ui::qt::tree::topLevelItem(
	ak::ID							_id
) {
	try {
		for (my_topLevelItemsIterator itm = my_topLevelItems.begin(); itm != my_topLevelItems.end(); itm++) {
			if (itm->second->id() == _id) { return itm->second; }
		}
		return nullptr;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::tree::topLevelItem(ID)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::tree::topLevelItem(ID)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::tree::topLevelItem(ID)"); }
}

std::vector<QString> ak::ui::qt::tree::topLevelItemsText(void) {
	std::vector<QString> r;
	for (my_topLevelItemsIterator itm = my_topLevelItems.begin(); itm != my_topLevelItems.end(); itm++) {
		r.push_back(itm->second->text(0));
	}
	return r;
}

void ak::ui::qt::tree::Clear(void) {
	my_topLevelItems.clear();
	clear();
}

void ak::ui::qt::tree::removeTopLevelItem(
	ak::ID							_id
) { my_topLevelItems.erase(_id); }

// ####################################################################################################################################

ak::ID ak::ui::qt::tree::getItemId(
	QTreeWidgetItem *		_item
) {
	if (_item == nullptr) { return ak::invalidID; }
	treeItem * itm = nullptr;
	itm = dynamic_cast<treeItem *>(_item);
	assert(itm != nullptr); // Cast failed
	return itm->id();
}
