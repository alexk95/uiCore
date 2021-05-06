/*
 * ak_ui_widget_tree.cpp
 *
 *  Created on: March 10, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_tree.h>					// corresponding class
#include <ak_ui_qt_lineEdit.h>				// textEdit for the filter
#include <ak_ui_colorStyle.h>				// colorStyle 
#include <ak_ui_color.h>					// color
#include <ak_ui_core.h>						// objectType, convet textAlignment

// Qt header
#include <qmessagebox.h>
#include <qmargins.h>
#include <qevent.h>

#include <qheaderview.h>

ak::ui::qt::tree::tree(
	const ak::ui::colorStyle *	_colorStyle
) : ak::ui::core::aWidget(ak::ui::core::objectType::oTree, _colorStyle),
	my_tree(nullptr), my_filter(nullptr), my_layout(nullptr),
	my_filterCaseSensitive(false), my_filterRefreshOnChange(true), my_currentId(0), my_itemsAreEditable(false),
	my_selectAndDeselectChildren(false), my_ignoreEvents(false), my_focusedItem(invalidID)
{
	// Create tree
	my_tree = new ak::ui::qt::treeBase(my_colorStyle);
	assert(my_tree != nullptr); // Failed to create

	// Create filter
	my_filter = new ak::ui::qt::lineEdit();
	assert(my_filter != nullptr); // Failed to create
	my_filter->setVisible(false);

	// Create widget
	my_widget = new QWidget;
	my_widget->setContentsMargins(0, 0, 0, 0);

	// Create layout
	my_layout = new QVBoxLayout(my_widget);
	assert(my_layout != nullptr); // Failed to create
	my_layout->setContentsMargins(0, 0, 0, 0);
	my_layout->addWidget(my_filter);
	my_layout->addWidget(my_tree);

	my_tree->setHeaderLabel(QString(""));

	my_tree->setMouseTracking(true);
	my_tree->setHeaderHidden(true);
	my_tree->header()->setSortIndicator(0, Qt::SortOrder::AscendingOrder);

	connect(my_filter, &lineEdit::keyPressed, this, &tree::slotFilterKeyPressed);
	connect(my_filter, &QLineEdit::textChanged, this, &tree::slotFilterTextChanged);

	connect(my_tree, &treeBase::keyPressed, this, &tree::slotTreeKeyPressed);
	connect(my_tree, &treeBase::keyReleased, this, &tree::slotTreeKeyReleased);
	connect(my_tree, &QTreeWidget::itemActivated, this, &tree::slotTreeItemActivated);
	connect(my_tree, &QTreeWidget::itemChanged, this, &tree::slotTreeItemChanged);
	connect(my_tree, &QTreeWidget::itemClicked, this, &tree::slotTreeItemClicked);
	connect(my_tree, &QTreeWidget::itemCollapsed, this, &tree::slotTreeItemCollapsed);
	connect(my_tree, &QTreeWidget::itemDoubleClicked, this, &tree::slotTreeItemDoubleClicked);
	connect(my_tree, &QTreeWidget::itemExpanded, this, &tree::slotTreeItemExpanded);
	connect(my_tree, &QTreeWidget::itemSelectionChanged, this, &tree::slotTreeSelectionChanged);
	connect(my_tree, &treeBase::mouseMove, this, &tree::slotTreeMouseMove);
	connect(my_tree, &treeBase::leave, this, &tree::slotTreeLeave);
}

ak::ui::qt::tree::~tree() {
	A_OBJECT_DESTROYING
	memFree();
}

QWidget * ak::ui::qt::tree::widget(void) { return my_widget; }

void ak::ui::qt::tree::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_tree->setColorStyle(my_colorStyle);
	my_filter->setColorStyle(my_colorStyle);
}

// ###########################################################################################################################

// Data manipulation

ak::ID ak::ui::qt::tree::add(
	ak::ID							_parentId,
	const QString &					_text,
	ak::ui::core::textAlignment		_textAlignment,
	QIcon							_icon
) {
	if (_parentId == -1) {
		// Check if top level item already exists
		ak::ui::qt::treeItem * itm = my_tree->topLevelItem(_text);
		if (itm == nullptr) {
			// Create and add new item
			itm = createItem(_text, _textAlignment, _icon);
			my_tree->AddTopLevelItem(itm);
			my_items.insert_or_assign(itm->id(), itm);
		}
		return itm->id();
	}
	else {
		// Find parent object
		my_itemsIterator parent = my_items.find(_parentId);
		assert(parent != my_items.end()); // Invalid ID provided
		ak::ui::qt::treeItem * itm = parent->second->findChild(_text);
		if (itm == nullptr) {
			itm = createItem(_text, _textAlignment, _icon);
			parent->second->AddChild(itm);
			if (parent->second->isSelected()) { itm->setSelected(true); }
			my_items.insert_or_assign(itm->id(), itm);
		}
		return itm->id();
	}
}

ak::ID ak::ui::qt::tree::add(
	const QString &					_cmd,
	char							_delimiter,
	ak::ui::core::textAlignment		_textAlignment,
	const QIcon  &					_icon
) {
	assert(_cmd.length() > 0); // Provided command is empty
	QStringList items = _cmd.split(_delimiter);

	assert(items.count() != 0); // Split failed

	// Create a pointer to store the current item
	ak::ui::qt::treeItem * currentItem = nullptr;

	// Search for the root element
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) {
		if (itm->second != nullptr) {
			if (itm->second->text(0) == items.at(0)) { currentItem = itm->second; break; }
		}
	}

	// Create a new root item
	if (currentItem == nullptr) {
		currentItem = createItem(items.at(0), _textAlignment);
		assert(currentItem != nullptr); // Failed to create item
		my_tree->AddTopLevelItem(currentItem);
		my_items.insert_or_assign(currentItem->id(), currentItem);
	}

	my_ignoreEvents = true;

	for (int i = 1; i < items.count(); i++) {
		ak::ui::qt::treeItem * nItm = currentItem->findChild(items.at(i));
		if (nItm == nullptr) {
			// Create new item
			if (i == items.count() - 1) { nItm = createItem(items.at(i), _textAlignment, _icon); }
			else { nItm = createItem(items.at(i), _textAlignment); }
			assert(nItm != nullptr); // Failed to create
			// Add item
			currentItem->AddChild(nItm);
			if (currentItem->isSelected()) { nItm->setSelected(true); }

			// Store data
			my_items.insert_or_assign(nItm->id(), nItm);
		}
		currentItem = nItm;
	}

	my_ignoreEvents = false;
	selectionChangedEvent(false);

	return currentItem->id();
}

void ak::ui::qt::tree::clear(bool _emitEvent) {
	my_tree->Clear();
	my_items.clear();
	my_currentId = 0;
	if (_emitEvent) { emit cleared(); }
}

void ak::ui::qt::tree::setItemEnabled(
	ak::ID							_itemId,
	bool							_enabled
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid ID provided
	my_ignoreEvents = true;
	itm->second->setDisabled(!_enabled);
	if (my_selectAndDeselectChildren) { itm->second->setChildsEnabled(_enabled); }
	my_ignoreEvents = false;
}

void ak::ui::qt::tree::setItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	itm->second->setSelected(_selected);
	if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(_selected); }
	my_ignoreEvents = false;
	selectionChangedEvent(true);
}

void ak::ui::qt::tree::setItemVisible(
	ak::ID							_itemId,
	bool							_visible
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	itm->second->setVisible(_visible);
	my_ignoreEvents = false;
	//if (my_selectAndDeselectChildren) { itm->second->setChildsVisible(_visible); }
}

void ak::ui::qt::tree::setItemText(
	ak::ID							_itemId,
	const QString &					_text
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	itm->second->setText(0, _text);
	my_ignoreEvents = false;
}

void ak::ui::qt::tree::setSingleItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	deselectAllItems(false);
	setItemSelected(_itemId, _selected);
}

void ak::ui::qt::tree::toggleItemSelection(
	ak::ID							_itemId
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	bool state = itm->second->isSelected();
	itm->second->setSelected(!state);
	if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(!state); }
	my_ignoreEvents = false;
	selectionChangedEvent(true);
}

void ak::ui::qt::tree::deselectAllItems(
	bool							_emitEvent
) {
	my_ignoreEvents = true;
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setSelected(false); }
	my_ignoreEvents = false;

	if (_emitEvent) { emit selectionChanged(); }
}

void ak::ui::qt::tree::setEnabled(
	bool							_enabled
) { my_tree->setEnabled(_enabled); }

void ak::ui::qt::tree::setVisible(
	bool							_visible
) { my_tree->setVisible(_visible); }

void ak::ui::qt::tree::setItemIcon(
	ak::ID							_itemId,
	const QIcon &					_icon
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->setIcon(0, _icon);
}

void ak::ui::qt::tree::setSortingEnabled(
	bool							_enabled
) { 
	my_tree->setSortingEnabled(_enabled);
	if (_enabled) {
		my_tree->sortByColumn(0);
	}
}

// ###########################################################################################################################################

// Filter

void ak::ui::qt::tree::setFilterVisible(
	bool							_visible
) { my_filter->setVisible(_visible); }

void ak::ui::qt::tree::applyCurrentFilter(void) {
	QString filter = my_filter->text();
	if (filter.length() == 0) {
		// Show all items
		my_ignoreEvents = true;
		for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setHidden(false); }
		collapseAllItems();
		selectionChangedEvent(false);
		my_ignoreEvents = false;
	}
	else {
		// Check filter
		Qt::CaseSensitivity sens = Qt::CaseSensitivity::CaseInsensitive;
		if (my_filterCaseSensitive) { sens = Qt::CaseSensitivity::CaseSensitive; }
		for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) {
			if (itm->second->text(0).toLower().contains(filter, sens)) { itm->second->setVisible(true); }
			else { itm->second->setHidden(true); }
		}
	}
}

void ak::ui::qt::tree::setFilterCaseSensitive(
	bool							_caseSensitive,
	bool							_refresh
) { my_filterCaseSensitive = _caseSensitive; if (_refresh) { applyCurrentFilter(); } }

void ak::ui::qt::tree::setFilterRefreshOnChange(
	bool							_refreshOnChange
) { my_filterRefreshOnChange = _refreshOnChange; }

void ak::ui::qt::tree::setMultiSelectionEnabled(
	bool							_multiSelection
) {
	if (_multiSelection) { my_tree->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection); }
	else { my_tree->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection); }
}

void ak::ui::qt::tree::setAutoSelectAndDeselectChildrenEnabled(
	bool							_enabled
) { my_selectAndDeselectChildren = _enabled; }

void ak::ui::qt::tree::expandAllItems(void) {
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(true); }
}

void ak::ui::qt::tree::collapseAllItems(void) {
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(false); }
}

void ak::ui::qt::tree::deleteItem(
	ak::ID												_itemID
) {
	my_ignoreEvents = true;
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end());		// Invalid item id
	qt::treeItem * item = itm->second;
	for (auto cId : item->allChildsIDs()) { my_items.erase(cId); }
	if (item->parentId() == ak::invalidID) {
		my_tree->removeTopLevelItem(item->id());
	}
	delete item;
	my_items.erase(_itemID);

	my_ignoreEvents = false;
	selectionChangedEvent(true);
}

void ak::ui::qt::tree::deleteItems(
	const std::vector<ak::ID> &							_itemIDs
) {
	my_ignoreEvents = true;

	for (auto id : _itemIDs) {
		my_itemsIterator itm = my_items.find(id);
		if (itm != my_items.end()) {
			qt::treeItem * item = itm->second;
			for (auto cId : item->allChildsIDs()) { my_items.erase(cId); }
			if (item->parentId() == ak::invalidID) {
				my_tree->removeTopLevelItem(item->id());
			}
			delete item;
			my_items.erase(id);
		}
	}
	my_ignoreEvents = false;
	selectionChangedEvent(true);
}

void ak::ui::qt::tree::setItemsAreEditable(
	bool												_editable,
	bool												_applyToAll
) {
	my_itemsAreEditable = _editable;
	if (_applyToAll) {
		for (auto itm : my_items) {
			auto f = itm.second->flags();
			f.setFlag(Qt::ItemIsEditable, my_itemsAreEditable);
			itm.second->setFlags(f);
		}
	}
}

void ak::ui::qt::tree::setItemIsEditable(
	ak::ID												_itemID,
	bool												_editable
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	auto f = itm->second->flags();
	f.setFlag(Qt::ItemIsEditable, _editable);
	itm->second->setFlags(f);
}

void ak::ui::qt::tree::setItemIsEditable(
	const std::vector<ak::ID> &							_itemIDs,
	bool												_editable
) {
	for (auto id : _itemIDs) { setItemIsEditable(id, _editable); }
}

// ###########################################################################################################################

// Information gathering

std::vector<ak::ID> ak::ui::qt::tree::selectedItems(void) {
	std::vector<ak::ID> r;
	for (QTreeWidgetItem * itm : my_tree->selectedItems()) {
		r.push_back(treeBase::getItemId(itm));
	}
	return r;
}

std::vector<QString> ak::ui::qt::tree::getItemPath(
	ak::ID							_itemId
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return toVector(itm->second->getItemPath());
}

QString ak::ui::qt::tree::getItemPathString(
	ak::ID							_itemId,
	char							_delimiter
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getItemPathString(_delimiter);
}

ak::ID ak::ui::qt::tree::getItemID(
	const QString &					_itemPath,
	char							_delimiter
) {
	assert(_itemPath.length() != 0); // No item path provided
	QStringList lst = _itemPath.split(_delimiter);
	assert(lst.count() > 0); // split error
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) {
		if (itm->second->text(0) == lst.at(0)) {
			if (lst.count() == 1) { return itm->second->id(); }
			return itm->second->getItemID(lst, 1);
		}
	}
	return ak::invalidID;
}

QString ak::ui::qt::tree::getItemText(
	ak::ID							_itemId
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->text(0);
}

bool ak::ui::qt::tree::enabled() const { return my_tree->isEnabled(); }

int ak::ui::qt::tree::itemCount(void) const { return my_items.size(); }

bool ak::ui::qt::tree::isSortingEnabled(void) const { return my_tree->isSortingEnabled(); }

// ###########################################################################################################################

// Events

void ak::ui::qt::tree::selectionChangedEvent(
	bool							_emitEvent
) {
	if (my_selectAndDeselectChildren) {
		bool ignoreBackup = my_ignoreEvents;
		my_ignoreEvents = true;
		// Get selected items
		QList<QTreeWidgetItem *> selected = my_tree->selectedItems();
		// Select childs of selected items
		for (QTreeWidgetItem * itm : selected) {
			// Cast item
			ak::ui::qt::treeItem * i = nullptr;
			i = dynamic_cast<ak::ui::qt::treeItem *>(itm);
			assert(i != nullptr); // Cast failed
			i->setChildsSelected(true);
			i->ensureTopLevelSelectionVisible();
		}
		my_ignoreEvents = ignoreBackup;
	}
	else {
		bool ignoreBackup = my_ignoreEvents;
		my_ignoreEvents = true;
		// Get selected items
		QList<QTreeWidgetItem *> selected = my_tree->selectedItems();
		for (QTreeWidgetItem * itm : selected) {
			// Cast item
			ak::ui::qt::treeItem * i = nullptr;
			i = dynamic_cast<ak::ui::qt::treeItem *>(itm);
			assert(i != nullptr); // Cast failed
			i->ensureTopLevelSelectionVisible();
		}
		my_ignoreEvents = ignoreBackup;
	}
	if (_emitEvent) { emit selectionChanged(); }
}

// ###########################################################################################################################

void ak::ui::qt::tree::slotTreeKeyPressed(QKeyEvent * _event) {
	if (my_ignoreEvents) { return; }
	emit keyPressed(_event);
}

void ak::ui::qt::tree::slotTreeKeyReleased(QKeyEvent * _event) {
	if (my_ignoreEvents) { return; }
	emit keyReleased(_event);
}

void ak::ui::qt::tree::slotTreeItemActivated(QTreeWidgetItem * _item, int _column) {
	if (my_ignoreEvents) { return; }
	emit itemActivated(_item, _column);
}

void ak::ui::qt::tree::slotTreeItemChanged(QTreeWidgetItem * _item, int _column) {
	qt::treeItem * itm = nullptr;
	itm = dynamic_cast<qt::treeItem *>(_item);
	assert(itm != nullptr);		// Cast failed
	if (itm->text(0) != itm->storedText()) {
		itm->setStoredText(itm->text(0));
		if (my_ignoreEvents) { return; }
		emit itemTextChanged(_item, _column);
	}
	else if (!my_ignoreEvents) {
		emit itemChanged(_item, _column);
	}
}

void ak::ui::qt::tree::slotTreeItemClicked(QTreeWidgetItem * _item, int _column) {
	if (my_ignoreEvents) { return; }
	emit itemClicked(_item, _column);
}

void ak::ui::qt::tree::slotTreeItemCollapsed(QTreeWidgetItem * _item) {
	treeItem * itm = nullptr;
	itm = dynamic_cast<treeItem *>(_item);
	assert(itm != nullptr); // Cast failed
	itm->collapse();
	if (my_ignoreEvents) { return; }
	emit itemCollapsed(_item);

}

void ak::ui::qt::tree::slotTreeItemDoubleClicked(QTreeWidgetItem * _item, int _column) {
	if (my_ignoreEvents) { return; }
	emit itemDoubleClicked(_item, _column);
}

void ak::ui::qt::tree::slotTreeItemExpanded(QTreeWidgetItem * _item) {
	if (my_ignoreEvents) { return; }
	emit itemExpanded(_item);
}

void ak::ui::qt::tree::slotTreeSelectionChanged() {
	if (my_ignoreEvents) { return; }
	selectionChangedEvent(true);
}

void ak::ui::qt::tree::slotTreeMouseMove(QMouseEvent * _event) {
	QTreeWidgetItem * item = my_tree->itemAt(_event->pos());
	if (item == nullptr) {
		if (my_focusedItem != invalidID) {
			my_focusedItem = invalidID;
			if (my_ignoreEvents) { return; }
			emit focusLost();
		}
	}
	else {
		treeItem * actualItem = nullptr;
		actualItem = dynamic_cast<treeItem *>(item);
		assert(actualItem != nullptr);
		if (actualItem->id() != my_focusedItem) {
			my_focusedItem = actualItem->id();
			if (my_ignoreEvents) { return; }
			emit itemFocused(item);
		}
	}
}

void ak::ui::qt::tree::slotTreeLeave(QEvent * _event) {
	if (my_focusedItem != invalidID) {
		my_focusedItem = invalidID;
		if (my_ignoreEvents) { return; }
		emit focusLost();
	}
}

void ak::ui::qt::tree::slotFilterTextChanged(void) {
	if (my_filterRefreshOnChange) { applyCurrentFilter(); }
}

void ak::ui::qt::tree::slotFilterKeyPressed(QKeyEvent * _event) {
	if (ak::ui::core::getKey(_event) == ui::core::keyType::key_Return) {
		applyCurrentFilter();
	}
}


// ###########################################################################################################################

ak::ui::qt::treeItem * ak::ui::qt::tree::createItem(
	const QString &					_text,
	ak::ui::core::textAlignment		_textAlignment,
	QIcon							_icon
) {
	// Create item
	ak::ui::qt::treeItem * itm = nullptr;
	itm = new ak::ui::qt::treeItem(my_currentId);
	// Set params
	itm->setTextAlignment(0, ak::ui::core::toQtAlignment(_textAlignment));
	itm->setText(0, _text);
	itm->setIcon(0, _icon);
	itm->setStoredText(_text);
	if (my_itemsAreEditable) {
		itm->setFlags(itm->flags() | Qt::ItemIsEditable);
	}
	//if (my_colorStyle != nullptr) {
		//itm->setTextColor(0, my_colorStyle->getControlsMainForecolor().toQColor());
		//itm->setBackgroundColor(0, my_colorStyle->getControlsMainBackcolor().toQColor());
	//}
	my_currentId++;
	return itm;
}

void ak::ui::qt::tree::memFree(void) {
	// Delete items
	while (my_items.begin() != my_items.end()) 
	{
		ak::ui::qt::treeItem * itm = my_items.begin()->second;
		my_items.erase(my_items.begin()->first);
		clearItem(itm);
		delete itm;
	}

	// Delete members
	if (my_filter != nullptr) { delete my_filter; my_filter = nullptr; }
	if (my_tree != nullptr) { delete my_tree; my_tree = nullptr; }
	if (my_layout != nullptr) { delete my_layout; my_layout = nullptr; }
	if (my_widget != nullptr) { delete my_widget; my_widget = nullptr; }
}

void ak::ui::qt::tree::clearItem(
	ak::ui::qt::treeItem *			_item
) {
	for (auto itm : _item->childs()) {
		clearItem(_item);
		my_items.erase(itm->id());
		delete itm;
	}
}

// ###########################################################################################################################################

// ###########################################################################################################################################

// ###########################################################################################################################################


ak::ui::qt::treeBase::treeBase(
	const colorStyle *			_colorStyle
) : QTreeWidget(),
ak::ui::core::aWidget(ak::ui::core::objectType::oTree, _colorStyle) {
	setStyleSheet("");
	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }
}

ak::ui::qt::treeBase::~treeBase() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Event handling

void ak::ui::qt::treeBase::keyPressEvent(QKeyEvent *_event)
{
	QTreeWidget::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::ui::qt::treeBase::keyReleaseEvent(QKeyEvent * _event)
{
	QTreeWidget::keyReleaseEvent(_event); emit keyReleased(_event);
}

void ak::ui::qt::treeBase::mouseMoveEvent(QMouseEvent * _event)
{
	QTreeWidget::mouseMoveEvent(_event); emit mouseMove(_event);
}

void ak::ui::qt::treeBase::leaveEvent(QEvent *_event)
{
	QTreeWidget::leaveEvent(_event); emit leave(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::treeBase::widget(void) { return this; }

void ak::ui::qt::treeBase::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(core::caForegroundColorControls |
		core::caBackgroundColorControls, "QTreeWidget{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorControls |
		core::caBackgroundColorControls, "QTreeWidget::item{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorFocus |
		core::caBackgroundColorFocus, "QTreeWidget::item:hover{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorSelected |
		core::caBackgroundColorSelected, "QTreeWidget::item:selected:!hover{", "}"));

	if (!sheet.isEmpty()) {
		sheet.append("QTreeWidget::branch:has-siblings:!adjoins-item{border-image: url(");
		sheet.append(my_colorStyle->getFilePath("Tree/Tree_Branch_End_Root.png"));
		sheet.append(") 0;}");

		sheet.append("QTreeWidget::branch:has-siblings:adjoins-item{border-image: url(");
		sheet.append(my_colorStyle->getFilePath("Tree/Tree_Branch_HasSiblings.png"));
		sheet.append(") 0;}");

		sheet.append("QTreeWidget::branch:!has-children:!has-siblings:adjoins-item{border-image: url(");
		sheet.append(my_colorStyle->getFilePath("Tree/Tree_Branch_End.png"));
		sheet.append(") 0;}");

		sheet.append("QTreeWidget::branch:has-children:!has-siblings:closed:!hover,"
			"QTreeWidget::branch:closed:has-children:has-siblings:!hover{border-image: none; image: url(");
		sheet.append(my_colorStyle->getFilePath("Tree/Tree_Branch_HasChildren.png"));
		sheet.append(");}");

		sheet.append("QTreeWidget::branch:has-children:!has-siblings:closed:hover,"
			"QTreeWidget::branch:closed:has-children:has-siblings:hover{border-image: none; image: url(");
		sheet.append(my_colorStyle->getFilePath("Tree/Tree_Branch_HasChildren_Focus.png"));
		sheet.append(");}");

		sheet.append("QTreeWidget::branch:open:has-children:!has-siblings:!hover,"
			"QTreeWidget::branch:open:has-children:has-siblings:!hover{border-image: none; image: url(");
		sheet.append(my_colorStyle->getFilePath("Tree/Tree_Branch_Open.png"));
		sheet.append(");}");

		sheet.append("QTreeWidget::branch:open:has-children:!has-siblings:hover,"
			"QTreeWidget::branch:open:has-children:has-siblings:hover{border-image: none; image: url(");
		sheet.append(my_colorStyle->getFilePath("Tree/Tree_Branch_Open_Focus.png"));
		sheet.append(");}");
	}
	this->setStyleSheet(sheet);
}

// ####################################################################################################################################

void ak::ui::qt::treeBase::AddTopLevelItem(
	treeItem *						_item
) {
	try {
		if (_item == nullptr) { throw ak::Exception("Is nullptr", "Check treeItem"); }
		if (_item->id() == ak::invalidID) { throw ak::Exception("Is invalid ID", "Check item ID"); }
		if (topLevelItem(_item->text(0)) != nullptr) { throw ak::Exception("Item does already exist", "Check for duplicates"); }
		addTopLevelItem(_item);
		my_topLevelItems.insert_or_assign(_item->id(), _item);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::treeBase::AddTopLevelItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::treeBase::AddTopLevelItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::treeBase::AddTopLevelItem()"); }
}

ak::ui::qt::treeItem * ak::ui::qt::treeBase::topLevelItem(
	const QString &					_text
) {
	try {
		for (my_topLevelItemsIterator itm = my_topLevelItems.begin(); itm != my_topLevelItems.end(); itm++) {
			assert(itm->second != nullptr); // That should not happen..
			if (itm->second->text(0) == _text) { return itm->second; }
		}
		return nullptr;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::treeBase::topLevelItem(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::treeBase::topLevelItem(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::treeBase::topLevelItem(QString)"); }
}

ak::ui::qt::treeItem * ak::ui::qt::treeBase::topLevelItem(
	ak::ID							_id
) {
	try {
		for (my_topLevelItemsIterator itm = my_topLevelItems.begin(); itm != my_topLevelItems.end(); itm++) {
			if (itm->second->id() == _id) { return itm->second; }
		}
		return nullptr;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::treeBase::topLevelItem(ID)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::treeBase::topLevelItem(ID)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::treeBase::topLevelItem(ID)"); }
}

std::vector<QString> ak::ui::qt::treeBase::topLevelItemsText(void) {
	std::vector<QString> r;
	for (my_topLevelItemsIterator itm = my_topLevelItems.begin(); itm != my_topLevelItems.end(); itm++) {
		r.push_back(itm->second->text(0));
	}
	return r;
}

void ak::ui::qt::treeBase::Clear(void) {
	my_topLevelItems.clear();
	clear();
}

void ak::ui::qt::treeBase::removeTopLevelItem(
	ak::ID							_id
) {
	my_topLevelItems.erase(_id);
}

// ####################################################################################################################################

ak::ID ak::ui::qt::treeBase::getItemId(
	QTreeWidgetItem *		_item
) {
	if (_item == nullptr) { return ak::invalidID; }
	treeItem * itm = nullptr;
	itm = dynamic_cast<treeItem *>(_item);
	assert(itm != nullptr); // Cast failed
	return itm->id();
}

// ###########################################################################################################################################

// ###########################################################################################################################################

// ###########################################################################################################################################

ak::ui::qt::treeItem::treeItem(
	ak::ID							_newId,
	treeItem *						_parent,
	int								_type
) : ak::ui::core::aObject(ak::ui::core::objectType::oTreeItem), QTreeWidgetItem(_type),
my_id(_newId), my_parent(_parent) {}

ak::ui::qt::treeItem::treeItem(
	ak::ui::qt::treeBase *			_view,
	ak::ID							_newId,
	treeItem *						_parent,
	int								_type
) : ak::ui::core::aObject(ak::ui::core::objectType::oTreeItem), QTreeWidgetItem(_view, _type),
my_id(_newId), my_parent(_parent) {}

ak::ui::qt::treeItem::~treeItem() {
	A_OBJECT_DESTROYING
		treeItem * dx = this;
	QString txt = dx->text(0);
	if (my_parent != nullptr) { my_parent->eraseChild(my_id); }

	// OLD
	/*
	while (my_childs.size() > 0) {
		for (auto itm : my_childs) {
			delete itm; break;
		}
	}
	*/

	// NEW
	// The child items themselves will be deleted by the corresponding functionality in the QTreeWidgetItem
	// However, they must no longer refer to this deleted item. Therefore we set their parent to nullptr.
	for (auto itm : my_childs) {
		itm->setParentItem(nullptr);
	}
	my_childs.clear();
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
) {
	my_parent = _parent;
}

void ak::ui::qt::treeItem::setChildsEnabled(
	bool							_enabled
) {
	for (auto itm : my_childs) {
		itm->setDisabled(!_enabled);
		itm->setChildsEnabled(_enabled);
	}
}

void ak::ui::qt::treeItem::setChildsSelected(
	bool							_selected
) {
	for (auto itm : my_childs) {
		itm->setSelected(_selected);
		itm->setChildsSelected(_selected);
	}
}

void ak::ui::qt::treeItem::setChildsVisible(
	bool							_visible
) {
	for (auto itm : my_childs) {
		itm->setVisible(_visible);
		itm->setChildsVisible(_visible);
	}
}

void ak::ui::qt::treeItem::ensureTopLevelSelectionVisible() {
	if (my_parent != nullptr) {
		if (my_parent->isSelected()) { my_parent->ensureTopLevelSelectionVisible(); }
		else if (isSelected()) {
			my_parent->expandAllParents(true);
		}
	}
	//else if (isSelected()) { setExpanded(true); }
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
	if (my_parent != nullptr) {
		QString ret(my_parent->getItemPathString(_delimiter));
		ret.append(_delimiter);
		ret.append(text(0));
		return ret;
	}
	else { return text(0); }
}