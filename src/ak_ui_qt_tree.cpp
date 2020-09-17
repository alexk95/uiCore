/*
/ * ak_ui_qt_tree.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include <ak_ui_qt_tree.h>			// corresponding header
#include <ak_exception.h>			// error handling
#include <ak_ui_qt_treeItem.h>		// treeItem
#include <ak_ui_core.h>				// objectType
#include <ak_ui_colorStyle.h>		// colorStyle

#include <qheaderview.h>

ak::ui::qt::tree::tree(
	ak::ui::colorStyle *			_colorStyle
) : QTreeWidget(),
	ak::ui::core::aWidget(ak::ui::core::objectType::oTree, _colorStyle) {
	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }
}

ak::ui::qt::tree::~tree() {}

QWidget * ak::ui::qt::tree::widget(void) { return this; }

void ak::ui::qt::tree::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check colorStyle"); }
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

// ####################################################################################################################################

ak::ID ak::ui::qt::tree::getItemId(
	QTreeWidgetItem *		_item
) {
	treeItem * itm = nullptr;
	itm = dynamic_cast<treeItem *>(_item);
	assert(itm != nullptr); // Cast failed
	return itm->id();
}


/*

// AK header
#include "ak_ui_qt_tree.h"			// corresponding class
#include "ak_ui_colorStyle.h"		// colorStyle
#include "ak_ui_widget_tree.h"		// widget::tree
#include "ak_exception.h"			// error handling
#include "ak_ui_core.h"				// objectType
#include "ak_ui_qt_treeItem.h"

ak::ui::qt::tree::tree(
	ak::ui::widget::tree *				_creator,
	ak::ui::colorStyle *				_colorStyle
)	: ak::ui::core::aWidget(ak::ui::core::objectType::oTree, _colorStyle)
{
	try {
		if (_creator == nullptr) { throw ak::Exception("Is nullptr", "Check creator", ak::Exception::exceptionType::Nullptr); }
		my_tree = _creator;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::tree::tree()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::tree::tree()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::tree::tree()"); }
}

ak::ui::qt::tree::~tree() {}

QWidget * ak::ui::qt::tree::widget(void) { return this; }

void ak::ui::qt::tree::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style", ak::Exception::exceptionType::Nullptr); }
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTextEdit));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::tree::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::tree::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::tree::setColorStyle()"); }
}

void ak::ui::qt::tree::mouseMoveEvent(QMouseEvent *event)
{
	ak::uidManager::UID currentItemID = 0;

	QPoint mousePoint = event->pos();
	QTreeWidgetItem *item = itemAt(mousePoint);
	my_tree->mouseHoverItem(item);
};

void ak::ui::qt::tree::leaveEvent(QEvent *event)
{
	my_tree->mouseHoverItem(nullptr);
	QTreeWidget::leaveEvent(event);
}

void ak::ui::qt::tree::selectItem(QModelIndex item)
{
	QTreeWidgetItem *treeItem = itemFromIndex(item);
	if (treeItem != nullptr)
	{
		treeItem->setSelected(true);
	}
}

void ak::ui::qt::tree::deselectItem(QModelIndex item)
{
	QTreeWidgetItem *treeItem = itemFromIndex(item);
	if (treeItem != nullptr)
	{
		treeItem->setSelected(false);
	}
}

void ak::ui::qt::tree::selectChildren(QModelIndex item)
{
	QTreeWidgetItem *treeItm = itemFromIndex(item);
	if (treeItm != nullptr)
	{
		treeItm->setSelected(true);
		selectChildren(treeItm);
	}
}

void ak::ui::qt::tree::addItemToList(QModelIndex item, std::list<treeItem *> &itemList)
{
	QTreeWidgetItem *treeItem = itemFromIndex(item);
	itemList.push_back(treeItem);
}

std::string ak::ui::qt::tree::getItemName(QModelIndex item)
{
	QTreeWidgetItem *treeItem = itemFromIndex(item);

	return (treeItem->text(0).toStdString());
}

void ak::ui::qt::tree::deselectChildren(QModelIndex item)
{
	QTreeWidgetItem *treeItm = itemFromIndex(item);
	if (treeItm != nullptr)
	{
		treeItm->setSelected(false);
		deselectChildren(treeItm);
	}
}

void ak::ui::qt::tree::deselectParentFolders(QModelIndex item)
{
	QTreeWidgetItem *treeItm = itemFromIndex(item);
	if (treeItm != nullptr)
	{ deselectParentFolders(treeItm); }
}

void ak::ui::qt::tree::selectChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++)
	{
		QTreeWidgetItem *child = item->child(i);

		child->setSelected(true);

		selectChildren(child);
	}
}

void ak::ui::qt::tree::deselectChildren(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); i++)
	{
		QTreeWidgetItem *child = item->child(i);

		child->setSelected(false);

		deselectChildren(child);
	}
}

void ak::ui::qt::tree::deselectParentFolders(QTreeWidgetItem *item)
{
	QTreeWidgetItem *parent = item->parent();
	if (parent != nullptr)
	{
		parent->setSelected(false);
		deselectParentFolders(parent);
	}
}

void ak::ui::qt::tree::keyPressEvent(QKeyEvent *event)
{
	bool oldValue = getSendSelectionChangedNotification();
	setSendSelectionChangedNotification(false);

	QTreeWidgetItem *previousItem = currentItem();

	QTreeWidget::keyPressEvent(event);

	QTreeWidgetItem *item = currentItem();

	if (item != nullptr && previousItem != item) selectChildren(item);

	setSendSelectionChangedNotification(oldValue);

	if (previousItem != item)
	{
		my_tree->selectionChangedEvent();
	}
}

void ak::ui::qt::tree::setSendSelectionChangedNotification(bool f) { my_sendSelectionChangedNotification = f; }

bool ak::ui::qt::tree::getSendSelectionChangedNotification(void) { return my_sendSelectionChangedNotification; }

*/