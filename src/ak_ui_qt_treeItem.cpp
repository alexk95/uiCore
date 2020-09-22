/*
 * ak_ui_qt_treeItem.cpp
 *
 *  Created on: August 30, 2020
 *	Last modified on: August 31, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_qt_treeItem.h"			// corresponding header
#include "ak_exception.h"				// error handling
#include "ak_ui_qt_tree.h"				// tree
#include "ak_ui_core.h"					// object type

#include <qstring.h>
#include <qstringlist.h>

ak::ui::qt::treeItem::treeItem(
	ak::ID							_newId,
	treeItem *						_parent,
	int								_type
) : ak::ui::core::aObject(ak::ui::core::objectType::oTreeItem), QTreeWidgetItem(_type),
	my_id(_newId), my_parent(_parent) {}

ak::ui::qt::treeItem::treeItem(
	ak::ui::qt::tree *				_view,
	ak::ID							_newId,
	treeItem *						_parent,
	int								_type
) : ak::ui::core::aObject(ak::ui::core::objectType::oTreeItem), QTreeWidgetItem(_view, _type),
	my_id(_newId), my_parent(_parent) {}

ak::ui::qt::treeItem::~treeItem() {
	treeItem * dx = this;
	QString txt = dx->text(0);
	if (my_parent != nullptr) { my_parent->eraseChild(my_id); }
	const std::vector<ak::ui::qt::treeItem *> & v = my_childs.toVector();
	for (auto itm : v) { delete itm; }
}

// ##############################################################################################

// Setter

void ak::ui::qt::treeItem::AddChild(
	treeItem *						_child
) {
	try {
		if (findChild(_child->text(0)) != nullptr) { throw ak::Exception("Item does already exist", "Check duplicate"); }
		if (_child == nullptr) { throw ak::Exception("Is nullptr", "Check child"); }
		_child->setParentItem(this);
		my_childs.add(_child);
		addChild(_child);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::treeItem::AddChild(treeItem)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::treeItem::AddChild(treeItem)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::treeItem::AddChild(treeItem)"); }
}

void ak::ui::qt::treeItem::setParentItem(
	treeItem *						_parent
) { my_parent = _parent; }

void ak::ui::qt::treeItem::setChildsSelected(
	bool							_selected
) {
	const std::vector<treeItem *> & childs = my_childs.toVector();
	for (size_t i = 0; i < childs.size(); i++) {
		ak::ui::qt::treeItem * itm = childs.at(i);
		itm->setSelected(_selected);
		itm->setChildsSelected(_selected);
	}
}

// ##############################################################################################

// Getter

ak::ui::qt::treeItem * ak::ui::qt::treeItem::findChild(
	ak::ID							_id
) {
	const std::vector<treeItem *> & childs = my_childs.toVector();
	for (treeItem * itm : childs) { if (itm->id() == _id) { return itm; } }
	return nullptr;
}

ak::ui::qt::treeItem * ak::ui::qt::treeItem::findChild(
	const QString &					_text
) {
	const std::vector<treeItem *> & childs = my_childs.toVector();
	for (treeItem * itm : childs) { if (itm->text(0) == _text) { return itm; } }
	return nullptr;
}

ak::ID ak::ui::qt::treeItem::getItemID(
	const QStringList &						_itemPath,
	int										_currentIndex
) {
	try {
		assert(_currentIndex < _itemPath.count()); // Invalid index provided
		ak::ui::qt::treeItem * child = findChild(_itemPath.at(_currentIndex));
		if (child == nullptr) { throw ak::Exception("Invalid item path provided", "Check item path"); }
		if (_currentIndex == _itemPath.count() - 1) { return child->id(); }
		return child->getItemID(_itemPath, _currentIndex + 1);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::treeItem::getItemID()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::treeItem::getItemID()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::treeItem::getItemID()"); }
}

void ak::ui::qt::treeItem::eraseChild(
	ak::ID							_id
) {	
	treeItem * itm = findChild(_id);
	my_allChildsIDs.erase(_id);
	my_allChilds.erase(itm);
	my_childs.erase(itm);
}


const std::vector<ak::ui::qt::treeItem *> & ak::ui::qt::treeItem::childs(void) { return my_childs.toVector(); }

const std::vector<ak::ui::qt::treeItem *> & ak::ui::qt::treeItem::allChilds(void) {
	my_allChilds.clear();
	const std::vector<ak::ui::qt::treeItem *> & v = my_childs.toVector();
	for (int i = 0; i < v.size(); i++) {
		ak::ui::qt::treeItem * itm = v.at(i);
		my_allChilds.add(itm);
		const std::vector<ak::ui::qt::treeItem *> & v2 = itm->allChilds();
		for (int c = 0; c < v2.size(); c++) { my_allChilds.add(v2.at(c)); }
	}
	return my_allChilds.toVector();
}

const std::vector<ak::ID> & ak::ui::qt::treeItem::allChildsIDs(void) {
	my_allChildsIDs.clear();
	const std::vector<ak::ui::qt::treeItem *> & v = my_childs.toVector();
	for (int i = 0; i < v.size(); i++) {
		ak::ui::qt::treeItem * itm = v.at(i);
		my_allChildsIDs.add(itm->id());
		const std::vector<ak::ui::qt::treeItem *> & v2 = itm->allChilds();
		for (int c = 0; c < v2.size(); c++) { my_allChildsIDs.add(v2.at(c)->id()); }
	}
	return my_allChildsIDs.toVector();
}

int ak::ui::qt::treeItem::childCount(void) const { return my_childs.count(); }

ak::ID ak::ui::qt::treeItem::id(void) const { return my_id; }

ak::ID ak::ui::qt::treeItem::parentId(void) const {
	if (my_parent == nullptr) { return -1; }
	else { return my_parent->id(); }
}

void ak::ui::qt::treeItem::setVisible(
	bool							_expandParents
) {
	setHidden(false);
	if (_expandParents && my_parent != nullptr) {
		my_parent->setExpanded(true);
		my_parent->setVisible(true);
	}
}

std::vector<QString> ak::ui::qt::treeItem::getItemPath() {
	std::vector<QString> ret;
	if (my_parent != nullptr) { ret = my_parent->getItemPath(); }
	ret.push_back(text(0));
	return ret;
}

QString ak::ui::qt::treeItem::getItemPathString(
	char									_delimiter
) {
	if (my_parent == nullptr) {
		QString ret(my_parent->getItemPathString(_delimiter));
		ret.append(_delimiter);
		ret.append(text(0));
		return ret;
	}
	else { return text(0); }
}