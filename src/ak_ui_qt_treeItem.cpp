/*
 * ak_ui_qt_treeItem.cpp
 *
 *  Created on: August 30, 2020
 *	Last modified on: August 31, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_qt_treeItem.h>			// corresponding header
#include <ak_exception.h>				// error handling
#include <ak_ui_qt_tree.h>				// tree
#include <ak_ui_core.h>					// object type

// Qt header
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
	while (my_childs.size() > 0) {
		for (auto itm : my_childs) {
			delete itm; break;
		}
	}
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
		my_childs.push_back(_child);
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
	for (auto itm : my_childs) {
		itm->setSelected(_selected);
		itm->setChildsSelected(_selected);
	}
}

void ak::ui::qt::treeItem::expandAllParents(
	bool							_expandThis
) {
	if (_expandThis) { setExpanded(true); }
	if (my_parent != nullptr) { my_parent->expandAllParents(true); }
}

void ak::ui::qt::treeItem::collapse(void) {
	if (isExpanded()) { setExpanded(false); }
	for (auto itm : my_childs) { itm->collapse(); }
}

// ##############################################################################################

// Getter

ak::ui::qt::treeItem * ak::ui::qt::treeItem::findChild(
	ak::ID							_id
) {
	for (auto itm : my_childs) { if (itm->id() == _id) { return itm; } }
	return nullptr;
}

ak::ui::qt::treeItem * ak::ui::qt::treeItem::findChild(
	const QString &					_text
) {
	for (auto itm : my_childs) { if (itm->text(0) == _text) { return itm; } }
	return nullptr;
}

ak::ID ak::ui::qt::treeItem::getItemID(
	const QStringList &						_itemPath,
	int										_currentIndex
) {
	try {
		assert(_currentIndex < _itemPath.count()); // Invalid index provided
		ak::ui::qt::treeItem * child = findChild(_itemPath.at(_currentIndex));
		if (child == nullptr) { return ak::invalidID; }
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
	std::list<treeItem *>::iterator it1 = my_allChilds.begin();
	std::list<ak::ID>::iterator it2 = my_allChildsIDs.begin();
	for (auto itm : my_allChilds) {
		if (itm->id() == _id) { my_allChilds.erase(it1); break; }
		it1++;
	}
	it1 = my_childs.begin();
	for (auto itm : my_childs) {
		if (itm->id() == _id) { my_childs.erase(it1); break; }
		it1++;
	}
	for (auto itm : my_allChildsIDs) {
		if (itm == _id) { my_allChildsIDs.erase(it2); break; }
		it2++;
	}
}


const std::list<ak::ui::qt::treeItem *> & ak::ui::qt::treeItem::childs(void) { return my_childs; }

const std::list<ak::ui::qt::treeItem *> & ak::ui::qt::treeItem::allChilds(void) {
	my_allChilds.clear();
	for (auto itm : my_childs) {
		my_allChilds.push_back(itm);
		const std::list<ak::ui::qt::treeItem *> & lst = itm->allChilds();
		for (auto cpy : lst) { my_allChilds.push_back(cpy); }
	}
	return my_allChilds;
}

const std::list<ak::ID> & ak::ui::qt::treeItem::allChildsIDs(void) {
	my_allChildsIDs.clear();
	for (auto itm : my_childs) {
		my_allChildsIDs.push_back(itm->id());
		const std::list<ak::ID> & lst = itm->allChildsIDs();
		for (auto cpy : lst) { my_allChildsIDs.push_back(cpy); }
	}
	return my_allChildsIDs;
}

int ak::ui::qt::treeItem::childCount(void) const { return my_childs.size(); }

ak::ID ak::ui::qt::treeItem::id(void) const { return my_id; }

ak::ID ak::ui::qt::treeItem::parentId(void) const {
	if (my_parent == nullptr) { return ak::invalidID; }
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

std::list<QString> ak::ui::qt::treeItem::getItemPath() {
	std::list<QString> ret;
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