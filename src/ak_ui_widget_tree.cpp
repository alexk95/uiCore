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
#include <ak_ui_widget_tree.h>				// corresponding class
#include <ak_messenger.h>					// messenger
#include <ak_ui_treeSignalLinker.h>			// tree signal to messaging system connection
#include <ak_ui_qt_lineEdit.h>				// textEdit for the filter
#include <ak_ui_qt_tree.h>					// object this tree widget is managing
#include <ak_ui_qt_treeItem.h>				// treeItem
#include <ak_ui_colorStyle.h>				// colorStyle 
#include <ak_ui_color.h>					// color
#include <ak_ui_core.h>						// objectType, convet textAlignment
#include <ak_uidMangager.h>					// UID manager

// Qt header
#include <qmessagebox.h>
#include <qmargins.h>
#include <qevent.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::widget::tree::tree(
	ak::messenger *			_messenger,
	ak::uidManager *		_uidManager,
	const ak::ui::colorStyle *	_colorStyle
) : ak::ui::core::aWidgetManager(ak::ui::core::objectType::oTree, _messenger, _uidManager, _colorStyle),
	my_tree(nullptr), my_filter(nullptr), my_layout(nullptr), my_multiWidget(nullptr), my_treeSignalLinker(nullptr), 
	my_filterCaseSensitive(false), my_filterRefreshOnChange(true), my_currentId(0), my_itemsAreEditable(false),
	my_internalMessenger(nullptr), my_internalUidManager(nullptr), my_selectAndDeselectChildren(false)
{
	assert(_messenger != nullptr); // nullptr provided

	// Set my UID
	my_uid = my_uidManager->getId();

	// Create tree
	my_tree = new ak::ui::qt::tree(my_colorStyle);
	assert(my_tree != nullptr); // Failed to create
	my_tree->setUid(1);

	// Create filter
	my_filter = new ak::ui::qt::lineEdit();
	assert(my_filter != nullptr); // Failed to create
	my_filter->setVisible(false);
	connect(my_filter, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotFilterKeyPressed(QKeyEvent *)));
	connect(my_filter, SIGNAL(textChanged(const QString &)), this, SLOT(slotFilterTextChanged()));

	// Create widget
	my_multiWidget = new QWidget();
	assert(my_multiWidget != nullptr); // Failed to create
	my_multiWidget->setContentsMargins(0, 0, 0, 0);

	// Create layout
	my_layout = new QVBoxLayout(my_multiWidget);
	assert(my_layout != nullptr); // Failed to create
	my_layout->setContentsMargins(0, 0, 0, 0);
	my_layout->addWidget(my_filter);
	my_layout->addWidget(my_tree);

	// Create tree signal linker
	my_treeSignalLinker = new ak::ui::treeSignalLinker(this, my_tree);
	assert(my_treeSignalLinker != nullptr); // Failed to create

	// Create internal UID manager
	my_internalUidManager = new ak::uidManager();
	assert(my_internalUidManager != nullptr); // Failed to create

	// Create internal messenger
	my_internalMessenger = new ak::messenger();
	assert(my_internalMessenger != nullptr); // Failed to create

	my_tree->setHeaderLabel(QString(""));

	my_tree->setMouseTracking(true);
	my_tree->setHeaderHidden(true);

}

ak::ui::widget::tree::~tree() {
	A_OBJECT_DESTROYING
	memFree();
}

QWidget * ak::ui::widget::tree::widget(void) { return my_multiWidget; }

void ak::ui::widget::tree::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_tree->setColorStyle(my_colorStyle);
	my_filter->setColorStyle(my_colorStyle);
}

// ###########################################################################################################################

// Data manipulation

ak::ID ak::ui::widget::tree::add(
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
			my_items.insert_or_assign(itm->id(), itm);
		}
		return itm->id();
	}
}

ak::ID ak::ui::widget::tree::add(
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

	for (int i = 1; i < items.count(); i++) {
		ak::ui::qt::treeItem * nItm = currentItem->findChild(items.at(i));
		if (nItm == nullptr) {
			// Create new item
			if (i == items.count() - 1) { nItm = createItem(items.at(i), _textAlignment, _icon); }
			else { nItm = createItem(items.at(i), _textAlignment); }
			assert(nItm != nullptr); // Failed to create
			// Add item
			currentItem->AddChild(nItm);
			// Store data
			my_items.insert_or_assign(nItm->id(), nItm);
		}
		currentItem = nItm;
	}

	return currentItem->id();
}

void ak::ui::widget::tree::clear(void) {
	if (my_treeSignalLinker != nullptr) { delete my_treeSignalLinker; my_treeSignalLinker = nullptr; }
	my_treeSignalLinker = new ak::ui::treeSignalLinker(this, my_tree);

	my_tree->Clear();
	my_items.clear();
	my_currentId = 0;
	my_messenger->sendMessage(my_uid, ak::core::eventType::eCleared);
	//Note, end cleared message
}

void ak::ui::widget::tree::setItemEnabled(
	ak::ID							_itemId,
	bool							_enabled
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid ID provided
	my_treeSignalLinker->disable();
	itm->second->setDisabled(!_enabled);
	if (my_selectAndDeselectChildren) { itm->second->setChildsEnabled(_enabled); }
	my_treeSignalLinker->enable();
}

void ak::ui::widget::tree::setItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_treeSignalLinker->disable();
	itm->second->setSelected(_selected);
	if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(_selected); }
	my_treeSignalLinker->enable();
	selectionChangedEvent(true);
}

void ak::ui::widget::tree::setItemVisible(
	ak::ID							_itemId,
	bool							_visible
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_treeSignalLinker->disable();
	itm->second->setVisible(_visible);
	//if (my_selectAndDeselectChildren) { itm->second->setChildsVisible(_visible); }
}

void ak::ui::widget::tree::setItemText(
	ak::ID							_itemId,
	const QString &					_text
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_treeSignalLinker->disable();
	itm->second->setText(0, _text);
	my_treeSignalLinker->enable();
}

void ak::ui::widget::tree::setSingleItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	deselectAllItems(false);
	setItemSelected(_itemId, _selected);
}

void ak::ui::widget::tree::toggleItemSelection(
	ak::ID							_itemId
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	my_treeSignalLinker->disable();
	bool state = itm->second->isSelected();
	itm->second->setSelected(!state);
	if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(!state); }
	my_treeSignalLinker->enable();
	selectionChangedEvent(true);
}

void ak::ui::widget::tree::deselectAllItems(
	bool							_sendMessage
) {
	my_treeSignalLinker->disable();
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setSelected(false); }
	my_treeSignalLinker->enable();
	if (_sendMessage) { my_messenger->sendMessage(my_uid, ak::core::eventType::eSelectionChanged); }
}

void ak::ui::widget::tree::setEnabled(
	bool							_enabled
) { my_tree->setEnabled(_enabled); }

void ak::ui::widget::tree::setVisible(
	bool							_visible
) { my_tree->setVisible(_visible); }

void  ak::ui::widget::tree::setItemIcon(
	ak::ID							_itemId,
	const QIcon &					_icon
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->setIcon(0, _icon);
}

// ###########################################################################################################################################

// Filter

void ak::ui::widget::tree::setFilterVisible(
	bool							_visible
) { my_filter->setVisible(_visible); }

void ak::ui::widget::tree::applyCurrentFilter(void) {
	QString filter = my_filter->text();
	if (filter.length() == 0) {
		// Show all items
		my_treeSignalLinker->disable();
		for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setHidden(false); }
		collapseAllItems();
		selectionChangedEvent(false);
		my_treeSignalLinker->enable();
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

void ak::ui::widget::tree::setFilterCaseSensitive(
	bool							_caseSensitive,
	bool							_refresh
) { my_filterCaseSensitive = _caseSensitive; if (_refresh) { applyCurrentFilter(); } }

void ak::ui::widget::tree::setFilterRefreshOnChange(
	bool							_refreshOnChange
) { my_filterRefreshOnChange = _refreshOnChange; }

void ak::ui::widget::tree::setMultiSelectionEnabled(
	bool							_multiSelection
) {
	if (_multiSelection) { my_tree->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection); }
	else { my_tree->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection); }
}

void ak::ui::widget::tree::setAutoSelectAndDeselectChildrenEnabled(
	bool							_enabled
) { my_selectAndDeselectChildren = _enabled; }

void ak::ui::widget::tree::expandAllItems(void) {
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(true); }
}

void ak::ui::widget::tree::collapseAllItems(void) {
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(false); }
}

void ak::ui::widget::tree::deleteItem(
	ak::ID												_itemID
) {
	my_treeSignalLinker->disable();
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end());		// Invalid item id
	qt::treeItem * item = itm->second;
	for (auto cId : item->allChildsIDs()) { my_items.erase(cId); }
	if (item->parentId() == ak::invalidID) {
		my_tree->removeTopLevelItem(item->id());
	}
	delete item;
	my_items.erase(_itemID);

	my_treeSignalLinker->enable();
	selectionChangedEvent(true);
}

void ak::ui::widget::tree::deleteItems(
	const std::vector<ak::ID> &				_itemIDs
) {
	my_treeSignalLinker->disable();
	for (auto id : _itemIDs) {
		my_itemsIterator itm = my_items.find(id);
		assert(itm != my_items.end());		// Invalid item id
		qt::treeItem * item = itm->second;
		for (auto cId : item->allChildsIDs()) { my_items.erase(cId); }
		if (item->parentId() == ak::invalidID) {
			my_tree->removeTopLevelItem(item->id());
		}
		delete item;
		my_items.erase(id);
	}
	my_treeSignalLinker->enable();
	selectionChangedEvent(true);
}

void ak::ui::widget::tree::setItemsAreEditable(
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

void ak::ui::widget::tree::setItemIsEditable(
	ak::ID												_itemID,
	bool												_editable
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	auto f = itm->second->flags();
	f.setFlag(Qt::ItemIsEditable, _editable);
	itm->second->setFlags(f);
}

void ak::ui::widget::tree::setItemIsEditable(
	const std::vector<ak::ID> &							_itemIDs,
	bool												_editable
) {
	for (auto id : _itemIDs) { setItemIsEditable(id, _editable); }
}

// ###########################################################################################################################

// Information gathering

std::vector<ak::ID> ak::ui::widget::tree::selectedItems(void) {
	std::vector<ak::ID> r;
	for (QTreeWidgetItem * itm : my_tree->selectedItems()) {
		r.push_back(ak::ui::qt::tree::getItemId(itm));
	}
	return r;
}

std::vector<QString> ak::ui::widget::tree::getItemPath(
	ak::ID							_itemId
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return toVector(itm->second->getItemPath());
}

QString ak::ui::widget::tree::getItemPathString(
	ak::ID							_itemId,
	char							_delimiter
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getItemPathString(_delimiter);
}

ak::ID ak::ui::widget::tree::getItemID(
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

QString ak::ui::widget::tree::getItemText(
	ak::ID							_itemId
) {
	my_itemsIterator itm = my_items.find(_itemId);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->text(0);
}

bool ak::ui::widget::tree::enabled() const { return my_tree->isEnabled(); }

int ak::ui::widget::tree::itemCount(void) const { return my_items.size(); }

// ###########################################################################################################################

// Events

void ak::ui::widget::tree::raiseKeyEvent(
	ui::core::keyType				_key,
	bool							_keyDown
) {
	if (_keyDown) { my_messenger->sendMessage(my_uid, ak::core::eventType::eKeyPressed, 0, _key); }
	else { my_messenger->sendMessage(my_uid, ak::core::eventType::eKeyReleased, 0, _key); }
}

void ak::ui::widget::tree::raiseItemEvent(
	ak::ID							_itemId,
	ak::core::eventType				_eventType,
	int								_info2
) {
	switch (_eventType)
	{
	case ak::core::eventType::eDestroyed: my_messenger->sendMessage(my_uid, ak::core::eventType::eDestroyed, 0, 0); return;
	case ak::core::eventType::eActivated: break;
	case ak::core::eventType::eChanged: break;
	case ak::core::eventType::eClicked: break;
	case ak::core::eventType::eCollpased: break;
	case ak::core::eventType::eDoubleClicked: break;
	case ak::core::eventType::eFocused: break;
	case ak::core::eventType::eExpanded: break;
	case ak::core::eventType::eItemTextChanged: break;
	case ak::core::eventType::eLocationChanged: break;
	default:
		assert(0); // Invalid event type
		break;
	}

	// Check item id (id only not required for destroyed message)

	if (_eventType == ak::core::eCollpased) {
		my_itemsIterator itm = my_items.find(_itemId);
		assert(itm != my_items.end());
		itm->second->collapse();
	}

	my_messenger->sendMessage(my_uid, _eventType, _itemId, _info2);
	return;
}

void ak::ui::widget::tree::raiseLeaveEvent(void) {
	my_messenger->sendMessage(my_uid, ak::core::eFocusLeft, 0, 0);
}

void ak::ui::widget::tree::selectionChangedEvent(
	bool							_sendMessage
) {
	if (my_selectAndDeselectChildren) {
		my_treeSignalLinker->disable();
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
		my_treeSignalLinker->enable();
	}
	else {
		my_treeSignalLinker->disable();
		// Get selected items
		QList<QTreeWidgetItem *> selected = my_tree->selectedItems();
		for (QTreeWidgetItem * itm : selected) {
			// Cast item
			ak::ui::qt::treeItem * i = nullptr;
			i = dynamic_cast<ak::ui::qt::treeItem *>(itm);
			assert(i != nullptr); // Cast failed
			i->ensureTopLevelSelectionVisible();
		}
		my_treeSignalLinker->enable();
	}
	if (_sendMessage) { my_messenger->sendMessage(my_uid, ak::core::eventType::eSelectionChanged); }
}

// ###########################################################################################################################

void ak::ui::widget::tree::slotFilterTextChanged(void) {
	if (my_filterRefreshOnChange) { applyCurrentFilter(); }
}

void ak::ui::widget::tree::slotFilterKeyPressed(QKeyEvent * _event) {
	if (ak::ui::core::getKey(_event) == ui::core::keyType::key_Return) {
		applyCurrentFilter();
	}
}


// ###########################################################################################################################

ak::ui::qt::treeItem * ak::ui::widget::tree::createItem(
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

void ak::ui::widget::tree::memFree(void) {
	// Delete the signal linker first so all signals will be disconnected propertly
	if (my_treeSignalLinker != nullptr) { delete my_treeSignalLinker; my_treeSignalLinker = nullptr; }

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
	if (my_multiWidget != nullptr) { delete my_multiWidget; my_multiWidget = nullptr; }
	if (my_layout != nullptr) { delete my_layout; my_layout = nullptr; }
	if (my_internalMessenger != nullptr) { delete my_internalMessenger; my_internalMessenger = nullptr; }
	if (my_internalUidManager != nullptr) { delete my_internalUidManager; my_internalUidManager = nullptr; }
}

void ak::ui::widget::tree::clearItem(
	ak::ui::qt::treeItem *			_item
) {
	for (auto itm : _item->childs()) {
		clearItem(_item);
		my_items.erase(itm->id());
		delete itm;
	}
}

// ###########################################################################################################################################
