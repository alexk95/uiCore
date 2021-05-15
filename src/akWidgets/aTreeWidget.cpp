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
#include <akCore/aException.h>
#include <akGui/aColor.h>
#include <akGui/aColorStyle.h>

#include <akWidgets/aTreeWidget.h>
#include <akWidgets/aLineEditWidget.h>

// Qt header
#include <qmessagebox.h>
#include <qmargins.h>
#include <qevent.h>

#include <qheaderview.h>

ak::aTreeWidget::aTreeWidget(
	const aColorStyle *	_colorStyle
) : ak::aWidget(otTree, _colorStyle),
	my_tree(nullptr), my_filter(nullptr), my_layout(nullptr),
	my_filterCaseSensitive(false), my_filterRefreshOnChange(true), my_currentId(0), my_itemsAreEditable(false),
	my_selectAndDeselectChildren(false), my_ignoreEvents(false), my_focusedItem(invalidID), my_isReadOnly(false)
{
	// Create tree
	my_tree = new ak::aTreeWidgetBase(my_colorStyle);
	assert(my_tree != nullptr); // Failed to create

	// Create filter
	my_filter = new aLineEditWidget();
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

	connect(my_filter, &aLineEditWidget::keyPressed, this, &aTreeWidget::slotFilterKeyPressed);
	connect(my_filter, &QLineEdit::textChanged, this, &aTreeWidget::slotFilterTextChanged);

	connect(my_tree, &aTreeWidgetBase::keyPressed, this, &aTreeWidget::slotTreeKeyPressed);
	connect(my_tree, &aTreeWidgetBase::keyReleased, this, &aTreeWidget::slotTreeKeyReleased);
	connect(my_tree, &QTreeWidget::itemActivated, this, &aTreeWidget::slotTreeItemActivated);
	connect(my_tree, &QTreeWidget::itemChanged, this, &aTreeWidget::slotTreeItemChanged);
	connect(my_tree, &QTreeWidget::itemClicked, this, &aTreeWidget::slotTreeItemClicked);
	connect(my_tree, &QTreeWidget::itemCollapsed, this, &aTreeWidget::slotTreeItemCollapsed);
	connect(my_tree, &QTreeWidget::itemDoubleClicked, this, &aTreeWidget::slotTreeItemDoubleClicked);
	connect(my_tree, &QTreeWidget::itemExpanded, this, &aTreeWidget::slotTreeItemExpanded);
	connect(my_tree, &QTreeWidget::itemSelectionChanged, this, &aTreeWidget::slotTreeSelectionChanged);
	connect(my_tree, &aTreeWidgetBase::mouseMove, this, &aTreeWidget::slotTreeMouseMove);
	connect(my_tree, &aTreeWidgetBase::leave, this, &aTreeWidget::slotTreeLeave);
}

ak::aTreeWidget::~aTreeWidget() {
	A_OBJECT_DESTROYING
	memFree();
}

QWidget * ak::aTreeWidget::widget(void) { return my_widget; }

void ak::aTreeWidget::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_tree->setColorStyle(my_colorStyle);
	my_filter->setColorStyle(my_colorStyle);
}

// ###########################################################################################################################

// Data manipulation

ak::ID ak::aTreeWidget::add(
	ak::ID							_parentId,
	const QString &					_text,
	ak::textAlignment		_textAlignment,
	QIcon							_icon
) {
	if (_parentId == -1) {
		// Check if top level item already exists
		ak::aTreeWidgetItem * itm = my_tree->topLevelItem(_text);
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
		auto parent = my_items.find(_parentId);
		assert(parent != my_items.end()); // Invalid ID provided
		ak::aTreeWidgetItem * itm = parent->second->findChild(_text);
		if (itm == nullptr) {
			itm = createItem(_text, _textAlignment, _icon);
			parent->second->AddChild(itm);
			if (parent->second->isSelected()) { itm->setSelected(true); }
			my_items.insert_or_assign(itm->id(), itm);
		}
		return itm->id();
	}
}

ak::ID ak::aTreeWidget::add(
	const QString &					_cmd,
	char							_delimiter,
	ak::textAlignment		_textAlignment,
	const QIcon  &					_icon
) {
	assert(_cmd.length() > 0); // Provided command is empty
	QStringList items = _cmd.split(_delimiter);

	assert(items.count() != 0); // Split failed

	// Create a pointer to store the current item
	ak::aTreeWidgetItem * currentItem = nullptr;

	// Search for the root element
	for (auto itm = my_items.begin(); itm != my_items.end(); itm++) {
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
		ak::aTreeWidgetItem * nItm = currentItem->findChild(items.at(i));
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

void ak::aTreeWidget::clear(bool _emitEvent) {
	my_tree->Clear();
	my_items.clear();
	my_currentId = 0;
	if (_emitEvent) { emit cleared(); }
}

void ak::aTreeWidget::setItemEnabled(
	ak::ID							_itemId,
	bool							_enabled
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid ID provided
	my_ignoreEvents = true;
	itm->second->setDisabled(!_enabled);
	if (my_selectAndDeselectChildren) { itm->second->setChildsEnabled(_enabled); }
	my_ignoreEvents = false;
}

void ak::aTreeWidget::setIsReadOnly(
	bool							_readOnly
) {
	my_isReadOnly = _readOnly;
	for (auto itm : my_items) {
		itm.second->setLocked(my_isReadOnly);
	}
}

void ak::aTreeWidget::setItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	itm->second->setSelected(_selected);
	if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(_selected); }
	my_ignoreEvents = false;
	selectionChangedEvent(true);
}

void ak::aTreeWidget::setItemVisible(
	ak::ID							_itemId,
	bool							_visible
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	itm->second->setVisible(_visible);
	my_ignoreEvents = false;
	//if (my_selectAndDeselectChildren) { itm->second->setChildsVisible(_visible); }
}

void ak::aTreeWidget::setItemText(
	ak::ID							_itemId,
	const QString &					_text
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	itm->second->setText(0, _text);
	my_ignoreEvents = false;
}

void ak::aTreeWidget::setSingleItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	deselectAllItems(false);
	setItemSelected(_itemId, _selected);
}

void ak::aTreeWidget::toggleItemSelection(
	ak::ID							_itemId
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_ignoreEvents = true;
	bool state = itm->second->isSelected();
	itm->second->setSelected(!state);
	if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(!state); }
	my_ignoreEvents = false;
	selectionChangedEvent(true);
}

void ak::aTreeWidget::deselectAllItems(
	bool							_emitEvent
) {
	my_ignoreEvents = true;
	for (auto itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setSelected(false); }
	my_ignoreEvents = false;

	if (_emitEvent) { emit selectionChanged(); }
}

void ak::aTreeWidget::setEnabled(
	bool							_enabled
) { my_tree->setEnabled(_enabled); }

void ak::aTreeWidget::setVisible(
	bool							_visible
) { my_tree->setVisible(_visible); }

void ak::aTreeWidget::setItemIcon(
	ak::ID							_itemId,
	const QIcon &					_icon
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->setIcon(0, _icon);
}

void ak::aTreeWidget::setSortingEnabled(
	bool							_enabled
) { 
	my_tree->setSortingEnabled(_enabled);
	if (_enabled) {
		my_tree->sortByColumn(0);
	}
}

// ###########################################################################################################################################

// Filter

void ak::aTreeWidget::setFilterVisible(
	bool							_visible
) { my_filter->setVisible(_visible); }

void ak::aTreeWidget::applyCurrentFilter(void) {
	QString filter = my_filter->text();
	if (filter.length() == 0) {
		// Show all items
		my_ignoreEvents = true;
		for (auto itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setHidden(false); }
		collapseAllItems();
		selectionChangedEvent(false);
		my_ignoreEvents = false;
	}
	else {
		// Check filter
		Qt::CaseSensitivity sens = Qt::CaseSensitivity::CaseInsensitive;
		if (my_filterCaseSensitive) { sens = Qt::CaseSensitivity::CaseSensitive; }
		for (auto itm = my_items.begin(); itm != my_items.end(); itm++) {
			if (itm->second->text(0).toLower().contains(filter, sens)) { itm->second->setVisible(true); }
			else { itm->second->setHidden(true); }
		}
	}
}

void ak::aTreeWidget::setFilterCaseSensitive(
	bool							_caseSensitive,
	bool							_refresh
) { my_filterCaseSensitive = _caseSensitive; if (_refresh) { applyCurrentFilter(); } }

void ak::aTreeWidget::setFilterRefreshOnChange(
	bool							_refreshOnChange
) { my_filterRefreshOnChange = _refreshOnChange; }

void ak::aTreeWidget::setMultiSelectionEnabled(
	bool							_multiSelection
) {
	if (_multiSelection) { my_tree->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection); }
	else { my_tree->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection); }
}

void ak::aTreeWidget::setAutoSelectAndDeselectChildrenEnabled(
	bool							_enabled
) { my_selectAndDeselectChildren = _enabled; }

void ak::aTreeWidget::expandAllItems(void) {
	for (auto itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(true); }
}

void ak::aTreeWidget::collapseAllItems(void) {
	for (auto itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(false); }
}

void ak::aTreeWidget::deleteItem(
	ak::ID												_itemID
) {
	my_ignoreEvents = true;
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end());		// Invalid item id
	aTreeWidgetItem * item = itm->second;
	for (auto cId : item->allChildsIDs()) { my_items.erase(cId); }
	if (item->parentId() == ak::invalidID) {
		my_tree->removeTopLevelItem(item->id());
	}
	delete item;
	my_items.erase(_itemID);

	my_ignoreEvents = false;
	selectionChangedEvent(true);
}

void ak::aTreeWidget::deleteItems(
	const std::vector<ak::ID> &							_itemIDs
) {
	my_ignoreEvents = true;

	for (auto id : _itemIDs) {
		auto itm = my_items.find(id);
		if (itm != my_items.end()) {
			aTreeWidgetItem * item = itm->second;
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

void ak::aTreeWidget::setItemsAreEditable(
	bool												_editable,
	bool												_applyToAll
) {
	my_itemsAreEditable = _editable;
	if (_applyToAll) {
		for (auto itm : my_items) {
			itm.second->setEditable(_editable);
		}
	}
}

void ak::aTreeWidget::setItemIsEditable(
	ak::ID												_itemID,
	bool												_editable
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->setEditable(_editable);
}

void ak::aTreeWidget::setItemIsEditable(
	const std::vector<ak::ID> &							_itemIDs,
	bool												_editable
) {
	for (auto id : _itemIDs) { setItemIsEditable(id, _editable); }
}

// ###########################################################################################################################

// Information gathering

std::vector<ak::ID> ak::aTreeWidget::selectedItems(void) {
	std::vector<ak::ID> r;
	for (QTreeWidgetItem * itm : my_tree->selectedItems()) {
		r.push_back(aTreeWidgetBase::getItemId(itm));
	}
	return r;
}

std::vector<QString> ak::aTreeWidget::getItemPath(
	ak::ID							_itemId
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return toVector(itm->second->getItemPath());
}

QString ak::aTreeWidget::getItemPathString(
	ak::ID							_itemId,
	char							_delimiter
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getItemPathString(_delimiter);
}

ak::ID ak::aTreeWidget::getItemID(
	const QString &					_itemPath,
	char							_delimiter
) {
	assert(_itemPath.length() != 0); // No item path provided
	QStringList lst = _itemPath.split(_delimiter);
	assert(lst.count() > 0); // split error
	for (auto itm = my_items.begin(); itm != my_items.end(); itm++) {
		if (itm->second->text(0) == lst.at(0)) {
			if (lst.count() == 1) { return itm->second->id(); }
			return itm->second->getItemID(lst, 1);
		}
	}
	return ak::invalidID;
}

QString ak::aTreeWidget::getItemText(
	ak::ID							_itemId
) {
	auto itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->text(0);
}

bool ak::aTreeWidget::enabled() const { return my_tree->isEnabled(); }

int ak::aTreeWidget::itemCount(void) const { return my_items.size(); }

bool ak::aTreeWidget::isSortingEnabled(void) const { return my_tree->isSortingEnabled(); }

// ###########################################################################################################################

// Events

void ak::aTreeWidget::selectionChangedEvent(
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
			ak::aTreeWidgetItem * i = nullptr;
			i = dynamic_cast<ak::aTreeWidgetItem *>(itm);
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
			ak::aTreeWidgetItem * i = nullptr;
			i = dynamic_cast<ak::aTreeWidgetItem *>(itm);
			assert(i != nullptr); // Cast failed
			i->ensureTopLevelSelectionVisible();
		}
		my_ignoreEvents = ignoreBackup;
	}
	if (_emitEvent) { emit selectionChanged(); }
}

// ###########################################################################################################################

void ak::aTreeWidget::slotTreeKeyPressed(QKeyEvent * _event) {
	if (my_ignoreEvents) { return; }
	emit keyPressed(_event);
}

void ak::aTreeWidget::slotTreeKeyReleased(QKeyEvent * _event) {
	if (my_ignoreEvents) { return; }
	emit keyReleased(_event);
}

void ak::aTreeWidget::slotTreeItemActivated(QTreeWidgetItem * _item, int _column) {
	if (my_ignoreEvents) { return; }
	emit itemActivated(_item, _column);
}

void ak::aTreeWidget::slotTreeItemChanged(QTreeWidgetItem * _item, int _column) {
	aTreeWidgetItem * itm = nullptr;
	itm = dynamic_cast<aTreeWidgetItem *>(_item);
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

void ak::aTreeWidget::slotTreeItemClicked(QTreeWidgetItem * _item, int _column) {
	if (my_ignoreEvents) { return; }
	emit itemClicked(_item, _column);
}

void ak::aTreeWidget::slotTreeItemCollapsed(QTreeWidgetItem * _item) {
	aTreeWidgetItem * itm = nullptr;
	itm = dynamic_cast<aTreeWidgetItem *>(_item);
	assert(itm != nullptr); // Cast failed
	itm->collapse();
	if (my_ignoreEvents) { return; }
	emit itemCollapsed(_item);

}

void ak::aTreeWidget::slotTreeItemDoubleClicked(QTreeWidgetItem * _item, int _column) {
	if (my_ignoreEvents) { return; }
	emit itemDoubleClicked(_item, _column);
}

void ak::aTreeWidget::slotTreeItemExpanded(QTreeWidgetItem * _item) {
	if (my_ignoreEvents) { return; }
	emit itemExpanded(_item);
}

void ak::aTreeWidget::slotTreeSelectionChanged() {
	if (my_ignoreEvents) { return; }
	selectionChangedEvent(true);
}

void ak::aTreeWidget::slotTreeMouseMove(QMouseEvent * _event) {
	QTreeWidgetItem * item = my_tree->itemAt(_event->pos());
	if (item == nullptr) {
		if (my_focusedItem != invalidID) {
			my_focusedItem = invalidID;
			if (my_ignoreEvents) { return; }
			emit focusLost();
		}
	}
	else {
		aTreeWidgetItem * actualItem = nullptr;
		actualItem = dynamic_cast<aTreeWidgetItem *>(item);
		assert(actualItem != nullptr);
		if (actualItem->id() != my_focusedItem) {
			my_focusedItem = actualItem->id();
			if (my_ignoreEvents) { return; }
			emit itemFocused(item);
		}
	}
}

void ak::aTreeWidget::slotTreeLeave(QEvent * _event) {
	if (my_focusedItem != invalidID) {
		my_focusedItem = invalidID;
		if (my_ignoreEvents) { return; }
		emit focusLost();
	}
}

void ak::aTreeWidget::slotFilterTextChanged(void) {
	if (my_filterRefreshOnChange) { applyCurrentFilter(); }
}

void ak::aTreeWidget::slotFilterKeyPressed(QKeyEvent * _event) {
	if (toKeyType(_event) == keyReturn) {
		applyCurrentFilter();
	}
}


// ###########################################################################################################################

ak::aTreeWidgetItem * ak::aTreeWidget::createItem(
	const QString &					_text,
	ak::textAlignment		_textAlignment,
	QIcon							_icon
) {
	// Create item
	ak::aTreeWidgetItem * itm = nullptr;
	itm = new ak::aTreeWidgetItem(my_currentId);
	// Set params
	itm->setTextAlignment(0, ak::toQtAlignmentFlag(_textAlignment));
	itm->setText(0, _text);
	itm->setIcon(0, _icon);
	itm->setStoredText(_text);
	itm->setEditable(my_itemsAreEditable);
	itm->setLocked(my_isReadOnly);

	//if (my_colorStyle != nullptr) {
		//itm->setTextColor(0, my_colorStyle->getControlsMainForecolor().toQColor());
		//itm->setBackgroundColor(0, my_colorStyle->getControlsMainBackcolor().toQColor());
	//}
	my_currentId++;
	return itm;
}

void ak::aTreeWidget::memFree(void) {
	// Delete items
	while (my_items.begin() != my_items.end()) 
	{
		ak::aTreeWidgetItem * itm = my_items.begin()->second;
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

void ak::aTreeWidget::clearItem(
	ak::aTreeWidgetItem *			_item
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


ak::aTreeWidgetBase::aTreeWidgetBase(
	const aColorStyle *			_colorStyle
) : QTreeWidget(),
ak::aWidget(otTree, _colorStyle) {
	setStyleSheet("");
	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }
}

ak::aTreeWidgetBase::~aTreeWidgetBase() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Event handling

void ak::aTreeWidgetBase::keyPressEvent(QKeyEvent *_event)
{
	QTreeWidget::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::aTreeWidgetBase::keyReleaseEvent(QKeyEvent * _event)
{
	QTreeWidget::keyReleaseEvent(_event); emit keyReleased(_event);
}

void ak::aTreeWidgetBase::mouseMoveEvent(QMouseEvent * _event)
{
	QTreeWidget::mouseMoveEvent(_event); emit mouseMove(_event);
}

void ak::aTreeWidgetBase::leaveEvent(QEvent *_event)
{
	QTreeWidget::leaveEvent(_event); emit leave(_event);
}

// #######################################################################################################

QWidget * ak::aTreeWidgetBase::widget(void) { return this; }

void ak::aTreeWidgetBase::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls, "QTreeWidget{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls, "QTreeWidget::item{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus |
		cafBackgroundColorFocus, "QTreeWidget::item:hover{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected |
		cafBackgroundColorSelected, "QTreeWidget::item:selected:!hover{", "}"));

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

void ak::aTreeWidgetBase::AddTopLevelItem(
	aTreeWidgetItem *						_item
) {
	try {
		if (_item == nullptr) { throw aException("Is nullptr", "Check aTreeWidgetItem"); }
		if (_item->id() == ak::invalidID) { throw aException("Is invalid ID", "Check item ID"); }
		if (topLevelItem(_item->text(0)) != nullptr) { throw aException("Item does already exist", "Check for duplicates"); }
		addTopLevelItem(_item);
		my_topLevelItems.insert_or_assign(_item->id(), _item);
	}
	catch (const aException & e) { throw aException(e, "ak::aTreeWidgetBase::AddTopLevelItem()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aTreeWidgetBase::AddTopLevelItem()"); }
	catch (...) { throw aException("Unknown error", "ak::aTreeWidgetBase::AddTopLevelItem()"); }
}

ak::aTreeWidgetItem * ak::aTreeWidgetBase::topLevelItem(
	const QString &					_text
) {
	try {
		for (auto itm{ my_topLevelItems.begin() }; itm != my_topLevelItems.end(); itm++) {
			assert(itm->second != nullptr); // That should not happen..
			if (itm->second->text(0) == _text) { return itm->second; }
		}
		return nullptr;
	}
	catch (const aException & e) { throw aException(e, "ak::aTreeWidgetBase::topLevelItem(QString)"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aTreeWidgetBase::topLevelItem(QString)"); }
	catch (...) { throw aException("Unknown error", "ak::aTreeWidgetBase::topLevelItem(QString)"); }
}

ak::aTreeWidgetItem * ak::aTreeWidgetBase::topLevelItem(
	ak::ID							_id
) {
	try {
		for (auto itm{ my_topLevelItems.begin() }; itm != my_topLevelItems.end(); itm++) {
			if (itm->second->id() == _id) { return itm->second; }
		}
		return nullptr;
	}
	catch (const aException & e) { throw aException(e, "ak::aTreeWidgetBase::topLevelItem(ID)"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aTreeWidgetBase::topLevelItem(ID)"); }
	catch (...) { throw aException("Unknown error", "ak::aTreeWidgetBase::topLevelItem(ID)"); }
}

std::vector<QString> ak::aTreeWidgetBase::topLevelItemsText(void) {
	std::vector<QString> r;
	for (auto itm{ my_topLevelItems.begin() }; itm != my_topLevelItems.end(); itm++) {
		r.push_back(itm->second->text(0));
	}
	return r;
}

void ak::aTreeWidgetBase::Clear(void) {
	my_topLevelItems.clear();
	clear();
}

void ak::aTreeWidgetBase::removeTopLevelItem(
	ak::ID							_id
) {
	my_topLevelItems.erase(_id);
}

// ####################################################################################################################################

ak::ID ak::aTreeWidgetBase::getItemId(
	QTreeWidgetItem *		_item
) {
	if (_item == nullptr) { return ak::invalidID; }
	aTreeWidgetItem * itm = nullptr;
	itm = dynamic_cast<aTreeWidgetItem *>(_item);
	assert(itm != nullptr); // Cast failed
	return itm->id();
}

// ###########################################################################################################################################

// ###########################################################################################################################################

// ###########################################################################################################################################

ak::aTreeWidgetItem::aTreeWidgetItem(
	ak::ID							_newId,
	aTreeWidgetItem *						_parent,
	int								_type
) : ak::aObject(otTreeItem), QTreeWidgetItem(_type),
my_id(_newId), my_parent(_parent), my_isEditable(false), my_isLockedForEdit(false) {}

ak::aTreeWidgetItem::aTreeWidgetItem(
	ak::aTreeWidgetBase *			_view,
	ak::ID							_newId,
	aTreeWidgetItem *						_parent,
	int								_type
) : ak::aObject(otTreeItem), QTreeWidgetItem(_view, _type),
my_id(_newId), my_parent(_parent), my_isEditable(false), my_isLockedForEdit(false) {}

ak::aTreeWidgetItem::~aTreeWidgetItem() {
	A_OBJECT_DESTROYING
		aTreeWidgetItem * dx = this;
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

void ak::aTreeWidgetItem::AddChild(
	aTreeWidgetItem *						_child
) {
	try {
		if (findChild(_child->text(0)) != nullptr) { throw aException("Item does already exist", "Check duplicate"); }
		if (_child == nullptr) { throw aException("Is nullptr", "Check child"); }
		_child->setParentItem(this);
		my_childs.push_back(_child);
		addChild(_child);
	}
	catch (const aException & e) { throw aException(e, "ak::aTreeWidgetItem::AddChild(aTreeWidgetItem)"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aTreeWidgetItem::AddChild(aTreeWidgetItem)"); }
	catch (...) { throw aException("Unknown error", "ak::aTreeWidgetItem::AddChild(aTreeWidgetItem)"); }
}

void ak::aTreeWidgetItem::setParentItem(
	aTreeWidgetItem *						_parent
) {
	my_parent = _parent;
}

void ak::aTreeWidgetItem::setChildsEnabled(
	bool							_enabled
) {
	for (auto itm : my_childs) {
		itm->setDisabled(!_enabled);
		itm->setChildsEnabled(_enabled);
	}
}

void ak::aTreeWidgetItem::setChildsSelected(
	bool							_selected
) {
	for (auto itm : my_childs) {
		itm->setSelected(_selected);
		itm->setChildsSelected(_selected);
	}
}

void ak::aTreeWidgetItem::setChildsVisible(
	bool							_visible
) {
	for (auto itm : my_childs) {
		itm->setVisible(_visible);
		itm->setChildsVisible(_visible);
	}
}

void ak::aTreeWidgetItem::ensureTopLevelSelectionVisible() {
	if (my_parent != nullptr) {
		if (my_parent->isSelected()) { my_parent->ensureTopLevelSelectionVisible(); }
		else if (isSelected()) {
			my_parent->expandAllParents(true);
		}
	}
	//else if (isSelected()) { setExpanded(true); }
}

void ak::aTreeWidgetItem::expandAllParents(
	bool							_expandThis
) {
	if (_expandThis) { setExpanded(true); }
	if (my_parent != nullptr) { my_parent->expandAllParents(true); }
}

void ak::aTreeWidgetItem::collapse(void) {
	if (isExpanded()) { setExpanded(false); }
	for (auto itm : my_childs) { itm->collapse(); }
}

void ak::aTreeWidgetItem::setEditable(bool _editable) {
	my_isEditable = _editable;
	refreshEditableState();
}

void ak::aTreeWidgetItem::setLocked(bool _locked) {
	my_isLockedForEdit = _locked;
	refreshEditableState();
}


// ##############################################################################################

// Getter

ak::aTreeWidgetItem * ak::aTreeWidgetItem::findChild(
	ak::ID							_id
) {
	for (auto itm : my_childs) { if (itm->id() == _id) { return itm; } }
	return nullptr;
}

ak::aTreeWidgetItem * ak::aTreeWidgetItem::findChild(
	const QString &					_text
) {
	for (auto itm : my_childs) { if (itm->text(0) == _text) { return itm; } }
	return nullptr;
}

ak::ID ak::aTreeWidgetItem::getItemID(
	const QStringList &						_itemPath,
	int										_currentIndex
) {
	try {
		assert(_currentIndex < _itemPath.count()); // Invalid index provided
		ak::aTreeWidgetItem * child = findChild(_itemPath.at(_currentIndex));
		if (child == nullptr) { return ak::invalidID; }
		if (_currentIndex == _itemPath.count() - 1) { return child->id(); }
		return child->getItemID(_itemPath, _currentIndex + 1);
	}
	catch (const aException & e) { throw aException(e, "ak::aTreeWidgetItem::getItemID()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aTreeWidgetItem::getItemID()"); }
	catch (...) { throw aException("Unknown error", "ak::aTreeWidgetItem::getItemID()"); }
}

void ak::aTreeWidgetItem::eraseChild(
	ak::ID							_id
) {
	std::list<aTreeWidgetItem *>::iterator it1 = my_allChilds.begin();
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


const std::list<ak::aTreeWidgetItem *> & ak::aTreeWidgetItem::childs(void) { return my_childs; }

const std::list<ak::aTreeWidgetItem *> & ak::aTreeWidgetItem::allChilds(void) {
	my_allChilds.clear();
	for (auto itm : my_childs) {
		my_allChilds.push_back(itm);
		const std::list<ak::aTreeWidgetItem *> & lst = itm->allChilds();
		for (auto cpy : lst) { my_allChilds.push_back(cpy); }
	}
	return my_allChilds;
}

const std::list<ak::ID> & ak::aTreeWidgetItem::allChildsIDs(void) {
	my_allChildsIDs.clear();
	for (auto itm : my_childs) {
		my_allChildsIDs.push_back(itm->id());
		const std::list<ak::ID> & lst = itm->allChildsIDs();
		for (auto cpy : lst) { my_allChildsIDs.push_back(cpy); }
	}
	return my_allChildsIDs;
}

int ak::aTreeWidgetItem::childCount(void) const { return my_childs.size(); }

ak::ID ak::aTreeWidgetItem::id(void) const { return my_id; }

ak::ID ak::aTreeWidgetItem::parentId(void) const {
	if (my_parent == nullptr) { return ak::invalidID; }
	else { return my_parent->id(); }
}

void ak::aTreeWidgetItem::setVisible(
	bool							_expandParents
) {
	setHidden(false);
	if (_expandParents && my_parent != nullptr) {
		my_parent->setExpanded(true);
		my_parent->setVisible(true);
	}
}

std::list<QString> ak::aTreeWidgetItem::getItemPath() {
	std::list<QString> ret;
	if (my_parent != nullptr) { ret = my_parent->getItemPath(); }
	ret.push_back(text(0));
	return ret;
}

QString ak::aTreeWidgetItem::getItemPathString(
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

// ##############################################################################################

// Private functions

void ak::aTreeWidgetItem::refreshEditableState(void) {
	auto f = flags();
	f.setFlag(Qt::ItemIsEditable, my_isEditable && !my_isLockedForEdit);
	setFlags(f);
}