/*
 * ak_ui_treeSignalLinker.cpp
 *
 *  Created on: March 10, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

 // AK header
#include "ak_ui_treeSignalLinker.h"		// corresponding class
#include "ak_ui_qt_tree.h"				// qt tree
#include "ak_ui_qt_treeItem.h"			// treeItem
#include "ak_ui_widget_tree.h"			// widget tree
#include "ak_exception.h"				// error handling
#include "ak_core.h"					// eventType

// Qt header
#include <qtreewidget.h>				// QTreeWidget
//#include <QStandardItem>
//#include <QApplication>

ak::ui::treeSignalLinker::treeSignalLinker(
	ak::ui::widget::tree *				_treeManager,
	ak::ui::qt::tree *					_tree
) : my_enabled(true), my_tree(nullptr), my_treeManager(nullptr)
{
	try {
		if (_tree == nullptr) { throw ak::Exception("Is nullptr", "Check tree"); }
		my_tree = _tree;
		if (_treeManager == nullptr) { throw ak::Exception("Is nullptr", "Check tree manager"); }
		my_treeManager = _treeManager;

		connect(my_tree, SIGNAL(destroyed()), this, SLOT(treeDestroyed()));
		connect(my_tree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(treeItemActivated(QTreeWidgetItem *, int)));
		connect(my_tree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(treeItemChanged(QTreeWidgetItem *, int)));
		connect(my_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(treeItemClicked(QTreeWidgetItem *, int)));
		connect(my_tree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(treeItemCollapsed(QTreeWidgetItem *)));
		connect(my_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(treeItemDoubleClicked(QTreeWidgetItem *, int)));
		connect(my_tree, SIGNAL(itemEntered(QTreeWidgetItem *, int)), this, SLOT(treeItemEntered(QTreeWidgetItem *, int)));
		connect(my_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(treeItemExpanded(QTreeWidgetItem *)));
		connect(my_tree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(treeItemPressed(QTreeWidgetItem *, int)));
		connect(my_tree, SIGNAL(itemSelectionChanged()), this, SLOT(treeItemSelectionChanged()));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::treeSignalLinker::treeSignalLinker()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::treeSignalLinker::treeSignalLinker()");}
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::treeSignalLinker::treeSignalLinker()"); }
}

ak::ui::treeSignalLinker::~treeSignalLinker() {
	disconnect(my_tree, SIGNAL(destroyed()), this, SLOT(treeDestroyed()));
	disconnect(my_tree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(treeItemActivated(QTreeWidgetItem *, int)));
	disconnect(my_tree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(treeItemChanged(QTreeWidgetItem *, int)));
	disconnect(my_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(treeItemClicked(QTreeWidgetItem *, int)));
	disconnect(my_tree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(treeItemCollapsed(QTreeWidgetItem *)));
	disconnect(my_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(treeItemDoubleClicked(QTreeWidgetItem *, int)));
	disconnect(my_tree, SIGNAL(itemEntered(QTreeWidgetItem *, int)), this, SLOT(treeItemEntered(QTreeWidgetItem *, int)));
	disconnect(my_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(treeItemExpanded(QTreeWidgetItem *)));
	disconnect(my_tree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(treeItemPressed(QTreeWidgetItem *, int)));
	disconnect(my_tree, SIGNAL(itemSelectionChanged()), this, SLOT(treeItemSelectionChanged()));
}

void ak::ui::treeSignalLinker::enable(void) { my_enabled = true; }

void ak::ui::treeSignalLinker::disable(void) { my_enabled = false; }

bool ak::ui::treeSignalLinker::enabled(void) const { return my_enabled; }

// ######################################################################################################################################

// Slots

void ak::ui::treeSignalLinker::treeDestroyed() {
	if (my_enabled) { my_treeManager->raiseItemEvent(-1, ak::core::eventType::eDestroyed, 0); }
}

void ak::ui::treeSignalLinker::treeItemActivated(QTreeWidgetItem *item, int column) {
	if (my_enabled) { my_treeManager->raiseItemEvent(ak::ui::qt::tree::getItemId(item), ak::core::eventType::eActivated, column); }
}

void ak::ui::treeSignalLinker::treeItemChanged(QTreeWidgetItem *item, int column) {
	if (my_enabled) { my_treeManager->raiseItemEvent(ak::ui::qt::tree::getItemId(item), ak::core::eventType::eChanged, column); }
}

void ak::ui::treeSignalLinker::treeItemClicked(QTreeWidgetItem *item, int column) {
	if (my_enabled) { my_treeManager->raiseItemEvent(ak::ui::qt::tree::getItemId(item), ak::core::eventType::eClicked, column); }
}

void ak::ui::treeSignalLinker::treeItemCollapsed(QTreeWidgetItem *item) {
	if (my_enabled) { my_treeManager->raiseItemEvent(ak::ui::qt::tree::getItemId(item), ak::core::eventType::eCollpased, 0); }
}

void ak::ui::treeSignalLinker::treeItemDoubleClicked(QTreeWidgetItem *item, int column) {
	if (my_enabled) { my_treeManager->raiseItemEvent(ak::ui::qt::tree::getItemId(item), ak::core::eventType::eDoubleClicked, column); }
}

void ak::ui::treeSignalLinker::treeItemEntered(QTreeWidgetItem *item, int column) {
	if (my_enabled) { my_treeManager->raiseItemEvent(ak::ui::qt::tree::getItemId(item), ak::core::eventType::eFocused, column); }
}

void ak::ui::treeSignalLinker::treeItemExpanded(QTreeWidgetItem *item) {
	if (my_enabled) { my_treeManager->raiseItemEvent(ak::ui::qt::tree::getItemId(item), ak::core::eventType::eExpanded, 0); }
}

void ak::ui::treeSignalLinker::treeItemSelectionChanged() {
	if (my_enabled) { my_treeManager->selectionChangedEvent(); }
}
