/*
 * ak_ui_widget_tree.cpp
 *
 *  Created on: March 10, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include "ak_ui_widget_tree.h"				// corresponding class
#include "ak_exception.h"					// error handling
#include "ak_messenger.h"					// messenger
#include "ak_ui_signalLinker.h"				// filter signal to messaging system connection
#include "ak_ui_treeSignalLinker.h"			// tree signal to messaging system connection
#include "ak_ui_qt_textedit.h"				// textEdit for the filter
#include "ak_ui_qt_tree.h"					// object this tree widget is managing
#include "ak_ui_qt_treeItem.h"				// treeItem
#include "ak_ui_colorStyle.h"				// colorStyle
#include "ak_ui_color.h"					// color
#include "ak_notifierTreeFilter.h"			// notify the tree that the filter has changed
#include "ak_ui_core.h"						// objectType, convet textAlignment
#include "ak_uidMangager.h"					// UID manager

ak::ui::widget::tree::tree(
	ak::messenger *			_messenger,
	ak::uidManager *		_uidManager,
	ak::ui::iconManager *	_iconManager,
	ak::ui::colorStyle *	_colorStyle
) : ak::ui::core::aWidgetManager(ak::ui::core::objectType::oTree, _iconManager,  _messenger, _uidManager, nullptr, _colorStyle),
	my_tree(nullptr), my_filter(nullptr), my_layout(nullptr), my_multiWidget(nullptr), my_treeSignalLinker(nullptr), my_filterSignalLinker(nullptr),
	my_notifierFilter(nullptr), my_filterCaseSensitive(false), my_filterRefreshOnChange(true), my_currentId(1), 
	my_internalMessenger(nullptr), my_internalUidManager(nullptr), my_selectAndDeselectChildren(false)
{
	try {
		// Set my UID
		my_uid = my_uidManager->getId();

		// Create tree
		my_tree = new ak::ui::qt::tree(my_colorStyle);
		if (my_tree == nullptr) { throw ak::Exception("Failed to create", "Create tree"); }
		my_tree->setUid(1);
		
		// Create filter
		my_filter = new ak::ui::qt::textEdit();
		if (my_filter == nullptr) { throw ak::Exception("Failed to create", "Create filter"); }
		my_filter->setMaximumSize(QSize(999, 24));
		my_filter->setVisible(false);

		// Create layout
		my_layout = new QVBoxLayout();
		if (my_layout == nullptr) { throw ak::Exception("Failed to create", "Create layout"); }
		my_layout->addWidget(my_filter);
		my_layout->addWidget(my_tree);

		// Create widget
		my_multiWidget = new QWidget();
		if (my_multiWidget == nullptr) { throw ak::Exception("Failed to create", "Create widget"); }
		my_multiWidget->setLayout(my_layout);

		// Create tree signal linker
		my_treeSignalLinker = new ak::ui::treeSignalLinker(this, my_tree);
		if (my_treeSignalLinker == nullptr) { throw ak::Exception("Failed to create", "Create tree signal linker"); }

		// Create internal UID manager
		my_internalUidManager = new ak::uidManager();
		if (my_internalUidManager == nullptr) { throw ak::Exception("Failed to create", "Create UID manager"); }

		// Create internal messenger
		my_internalMessenger = new ak::messenger();
		if (my_internalMessenger == nullptr) { throw ak::Exception("Failed to create", "Create messenger"); }

		// Create filter signal linker
		my_filterSignalLinker = new ak::ui::signalLinker(my_internalMessenger, my_internalUidManager);
		if (my_filterSignalLinker == nullptr) { throw ak::Exception("Failed to create", "Create filter singal linker"); }
		my_filterSignalLinker->addLink(my_filter);

		// Create notifier filter
		my_notifierFilter = new ak::notifierTreeFilter(this);
		if (my_notifierFilter == nullptr) { throw ak::Exception("Failed to create", "Create notifier filter"); }

		my_tree->setHeaderLabel(QString(""));
		
		my_tree->setMouseTracking(true);
		my_tree->setHeaderHidden(true);

		my_internalMessenger->registerReceiver(my_filter->uid(), ak::core::messageType::mEvent, my_notifierFilter);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::tree()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::tree()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::tree()"); }
}

ak::ui::widget::tree::~tree() { memFree(); }

QWidget * ak::ui::widget::tree::widget(void) { return my_multiWidget; }

void ak::ui::widget::tree::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check colorStyle"); }
		my_colorStyle = _colorStyle;
		my_tree->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTree));
		my_filter->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTextEdit));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::setColorStyle()"); }
}

// ###########################################################################################################################

// Data manipulation

ak::ID ak::ui::widget::tree::add(
	ak::ID							_parentId,
	const QString &					_text,
	ak::ui::core::textAlignment		_textAlignment,
	QIcon							_icon
) {
	try {
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
			if (parent == my_items.end()) { throw ak::Exception("Invalid ID", "Check parent ID"); }
			ak::ui::qt::treeItem * itm = parent->second->findChild(_text);
			if (itm == nullptr) {
				itm = createItem(_text, _textAlignment, _icon);
				parent->second->AddChild(itm);
				my_items.insert_or_assign(itm->id(), itm);
			}
			return itm->id();
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::add(parent)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::add(parent)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::add(parent)"); }
}

ak::ID ak::ui::widget::tree::add(
	const QString &					_cmd,
	char							_delimiter,
	ak::ui::core::textAlignment		_textAlignment,
	const QIcon  &					_icon
) {
	try {
		if (_cmd.length() == 0) { throw ak::Exception("Command is empty", "Check command"); }
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::add(command)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::add(command)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::add(command)"); }
}

//! @brief Will clear all tree items, receivers will get a destroyed message for each item
void ak::ui::widget::tree::clear(void) {
	assert(0); // Not implemented yet
}

void ak::ui::widget::tree::setItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		my_treeSignalLinker->disable();
		itm->second->setSelected(_selected);
		if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(_selected); }
		my_treeSignalLinker->enable();
		my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eSelectionChanged);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::select()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::select()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::select()"); }
}

void ak::ui::widget::tree::setSingleItemSelected(
	ak::ID							_itemId,
	bool							_selected
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		my_treeSignalLinker->disable();
		itm->second->setSelected(_selected);
		my_treeSignalLinker->enable();
		my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eSelectionChanged);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::select()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::select()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::select()"); }
}

void ak::ui::widget::tree::toggleItemSelection(
	ak::ID							_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		my_treeSignalLinker->disable();
		bool state = itm->second->isSelected();
		itm->second->setSelected(!state);
		if (my_selectAndDeselectChildren) { itm->second->setChildsSelected(!state); }
		my_treeSignalLinker->enable();
		my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eSelectionChanged);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::select()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::select()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::select()"); }
}

void ak::ui::widget::tree::deselectAllItems(void) {
	my_treeSignalLinker->disable();
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setSelected(false); }
	my_treeSignalLinker->enable();
	my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eSelectionChanged);
}

// ###########################################################################################################################################

// Filter

void ak::ui::widget::tree::setFilterVisible(
	bool							_visible
) { my_filter->setVisible(_visible); }

void ak::ui::widget::tree::applyCurrentFilter(void) {
	assert(0); // Not implemented yet
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

void ak::ui::widget::tree::setSelectAndDeselectChildrenEnabled(
	bool							_enabled
) { my_selectAndDeselectChildren = _enabled; }

void ak::ui::widget::tree::expandAllItems(void) {
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(true); }
}

void ak::ui::widget::tree::collapseAllItems(void) {
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setExpanded(false); }
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
	ak::ID									_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		return itm->second->getItemPath();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getItemPath()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getItemPath()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getItemPath()"); }
}

QString ak::ui::widget::tree::getItemPathString(
	ak::ID									_itemId,
	char									_delimiter
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		return itm->second->getItemPathString(_delimiter);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getItemPathString()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getItemPathString()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getItemPathString()"); }
}

// ###########################################################################################################################

// Events

void ak::ui::widget::tree::performFilterTextChanged(void) {
	try {
		if (my_filterRefreshOnChange) { performFilterEnterPressed(); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::performFilterTextChanged()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::performFilterTextChanged()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::performFilterTextChanged()"); }
}

void ak::ui::widget::tree::performFilterEnterPressed(void) {
	try {
		QString filter = my_filter->toPlainText();
		if (filter.length() == 0) {
			// Show all items
			for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) { itm->second->setHidden(false); }
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::performFilterEnterPressed()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::performFilterEnterPressed()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::performFilterEnterPressed()"); }
}

void ak::ui::widget::tree::raiseItemEvent(
	ak::ID							_itemId,
	ak::core::eventType				_eventType,
	int								_info2
) {
	try {
		switch (_eventType)
		{
		case ak::core::eventType::eDestroyed: my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eDestroyed, 0, 0); return;
		case ak::core::eventType::eActivated: break;
		case ak::core::eventType::eChanged: break;
		case ak::core::eventType::eClicked: break;
		case ak::core::eventType::eCollpased: break;
		case ak::core::eventType::eDoubleClicked: break;
		case ak::core::eventType::eFocused: break;
		case ak::core::eventType::eExpanded: break;
		default: throw ak::Exception("Invalid event type", "Check event type"); break;
		}

		// Check item id (id only not required for destroyed message)

		my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, _eventType, _itemId, _info2);
		return;
	}
	catch (ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::raiseItemEvent()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::raiseItemEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::raiseItemEvent()"); }
}

void ak::ui::widget::tree::selectionChangedEvent(void) {
	try {
		if (my_selectAndDeselectChildren) {
			my_treeSignalLinker->disable();
			// Get selected items
			QList<QTreeWidgetItem *> selected = my_tree->selectedItems();
			for (QTreeWidgetItem * itm : selected) {
				// Cast item
				ak::ui::qt::treeItem * i = nullptr;
				i = dynamic_cast<ak::ui::qt::treeItem *>(itm);
				assert(i != nullptr); // Cast failed
				bool state = i->isSelected();
				i->setChildsSelected(state);
			}
			my_treeSignalLinker->enable();
		}
		my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eSelectionChanged);
	}
	catch (ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::selectionChangedEvent()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::selectionChangedEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::selectionChangedEvent()"); }
}

// ###########################################################################################################################

ak::ui::qt::treeItem * ak::ui::widget::tree::createItem(
	const QString &					_text,
	ak::ui::core::textAlignment		_textAlignment,
	QIcon							_icon
) {
	try {
		// Create item
		ak::ui::qt::treeItem * itm = nullptr;
		itm = new ak::ui::qt::treeItem(my_currentId);
		if (itm == nullptr) { throw ak::Exception("Failed to create", "Create tree item"); }
		// Set params
		itm->setTextAlignment(0, ak::ui::core::toQtAlignment(_textAlignment));
		itm->setText(0, _text);
		itm->setIcon(0, _icon);
		if (my_colorStyle != nullptr) {
			itm->setTextColor(0, my_colorStyle->getControlsMainForecolor().toQColor());
			itm->setBackgroundColor(0, my_colorStyle->getControlsMainBackcolor().toQColor());
		}
		my_currentId++;
		return itm;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::createItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::createItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::createItem()"); }
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
	if (my_filterSignalLinker != nullptr) { delete my_filterSignalLinker; my_filterSignalLinker = nullptr; }
	if (my_filter != nullptr) { delete my_filter; my_filter = nullptr; }
	if (my_tree != nullptr) { delete my_tree; my_tree = nullptr; }
	if (my_multiWidget != nullptr) { delete my_multiWidget; my_multiWidget = nullptr; }
	if (my_layout != nullptr) { delete my_layout; my_layout = nullptr; }
	if (my_notifierFilter != nullptr) { delete my_notifierFilter; my_notifierFilter = nullptr; }
	if (my_internalMessenger != nullptr) { delete my_internalMessenger; my_internalMessenger = nullptr; }
	if (my_internalUidManager != nullptr) { delete my_internalUidManager; my_internalUidManager = nullptr; }
}

void ak::ui::widget::tree::clearItem(
	ak::ui::qt::treeItem *			_item
) {
	try {
		std::vector<ak::ui::qt::treeItem *> v = _item->childs();
		for (size_t i = 0; i < v.size(); i++) {
			ak::ui::qt::treeItem * itm = v.at(i);
			clearItem(itm);
			my_items.erase(itm->id());
			delete itm;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::clearItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::clearItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::clearItem()"); }
}

/*
// ###########################################################################################################################################


// ###########################################################################################################################################



void ak::ui::widget::tree::setText(
	ak::ID							_itemId,
	const QString &					_text
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		itm->second->setText(0, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::setText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::setText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::setText()"); }
}

void ak::ui::widget::tree::setTextColor(
	ak::ID							_itemId,
	const QColor &					_color
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		itm->second->setTextColor(0, _color);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::setTextColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::setTextColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::setTextColor()"); }
}

void ak::ui::widget::tree::setTextAlignment(
	ak::ID							_itemId,
	ak::ui::core::textAlignment		_textAlignment
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		switch (_textAlignment)
		{
		case ak::ui::core::textAlignment::alignLeft: itm->second->setTextAlignment(0, Qt::AlignLeft); break;
		case ak::ui::core::textAlignment::alignRight: itm->second->setTextAlignment(0, Qt::AlignRight); break;
		case ak::ui::core::textAlignment::alignCenter: itm->second->setTextAlignment(0, Qt::AlignCenter); break;
		default: itm->second->setTextAlignment(0, Qt::AlignLeft); break; 
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::setTextAlignment()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::setTextAlignment()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::setTextAlignment()"); }
}

void ak::ui::widget::tree::setBackColor(
	ak::ID							_itemId,
	const QColor &					_color
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		itm->second->setBackgroundColor(0, _color);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::setBackColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::setBackColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::setBackColor()"); }
}

void ak::ui::widget::tree::setEnabled(
	ak::ID							_itemId,
	bool							_enabled
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid UID", "Check item UID"); }
		itm->second->setDisabled(!_enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::setEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::setEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::setEnabled()"); }
}

void ak::ui::widget::tree::setIcon(
	ak::ID							_itemId,
	const QIcon &					_icon
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		itm->second->setIcon(0, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::setIcon()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::setIcon()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::setIcon()"); }
}

// ###########################################################################################################################################

void ak::ui::widget::tree::clear(void) {
	try {
		while (my_tree->topLevelItemCount() > 0) {
			// Take out the first top level item
			QTreeWidgetItem * itm = my_tree->takeTopLevelItem(0);
			// Remove the items childs
			removeChilds(itm);
			// Notify all receivers
			my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eDestroyed, getItemId(itm));
			delete itm;
		}
		my_items.clear();
		my_tree->clear();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree:clear()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree:clear()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree:clear()"); }
}

std::vector<ak::ID> ak::ui::widget::tree::selectedItems(void)
{
	std::vector<ak::ID> selected;

	QList <QTreeWidgetItem *> selItems = my_tree->selectedItems();

	for (auto item : selItems)
	{
		ak::ID itemID = getItemId(item);
		selected.push_back(itemID);
	}
	return selected;
}

void ak::ui::widget::tree::setFilterVisible(
	bool							_visible
) {
	try { my_filter->setVisible(_visible); }
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree:setFilterVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree:setFilterVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree:setFilterVisible()"); }
}

void ak::ui::widget::tree::applyCurrentFilter(void) {
	try {
		if (my_filter->toPlainText().length() == 0) { // no filter
			for (int i = 0; i < my_tree->topLevelItemCount(); i++) { showAll(my_tree->topLevelItem(i)); }
		}
		else { // filter
			for (int i = 0; i < my_tree->topLevelItemCount(); i++) { checkFilter(my_tree->topLevelItem(i)); }
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree:applyCurrentFilter()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree:applyCurrentFilter()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree:applyCurrentFilter()"); }
}

void ak::ui::widget::tree::setFilterCaseSensitive(
	bool							_caseSensitive,
	bool							_refresh
) {
	try {
		my_filterCaseSensitive = _caseSensitive;
		if (_refresh) { applyCurrentFilter(); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree:setFilterCaseSensitive()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree:setFilterCaseSensitive()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree:setFilterCaseSensitive()"); }
}

void ak::ui::widget::tree::setFilterRefreshOnChange(
	bool							_refreshOnChange
) { my_filterRefreshOnChange = _refreshOnChange; }

void ak::ui::widget::tree::performFilterTextChanged(void) {
	try {
		if (my_filterRefreshOnChange) { applyCurrentFilter(); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree:setFilterVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree:setFilterVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree:setFilterVisible()"); }
}

void ak::ui::widget::tree::performFilterEnterPressed(void) {
	try {
		applyCurrentFilter();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree:setFilterVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree:setFilterVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree:setFilterVisible()"); }
}

// #####################################################################################
// Getter

ak::ID ak::ui::widget::tree::getItemId(
	const QString &					_cmd,
	char							_delimiter,
	bool							_throwException
) {
	try {
		if (_cmd.length() == 0) { throw ak::Exception("Is empty", "Check command"); }
		QStringList items = _cmd.split(_delimiter);

		// Create a pointer to store the current item
		QTreeWidgetItem * currentItem = nullptr;

		// Search for the root element
		for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) {
			if (itm->second != nullptr) {
				if (itm->second->text(0) == items.at(0)) { currentItem = itm->second; break; }
			}
		}

		// Create new root item if doesnt exist
		if (currentItem == nullptr) {
			if (_throwException) { throw ak::Exception(QString(items.at(0) + " does not exist").toStdString().c_str(), "Check root"); }
			return 0;
		}

		// Check all child items
		for (int i = 1; i < items.count(); i++) {
			QTreeWidgetItem * child = nullptr;
			// Check all childs of the current item
			for (int c = 0; c < currentItem->childCount(); c++) {
				if (currentItem->child(c)->text(0) == items.at(i)) { child = currentItem->child(c); break; }
			}
			if (child == nullptr) {
				if (_throwException) { throw ak::Exception(QString(items.at(i) + " does not exist").toStdString().c_str(), "Check child"); }
				return 0;
			}
			currentItem = child;
		}

		// Find the very last items UID

		for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) {
			if (itm->second == currentItem) { return itm->first; }
		}

		throw ak::Exception("Fatal error", "Check item");
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree:getItemUid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree:getItemUid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree:getItemUid()"); }
}

QString ak::ui::widget::tree::getHeaderText() const { return my_headerText; }

bool ak::ui::widget::tree::getHeaderVisible() const { return (!my_tree->isHeaderHidden()); }

QString ak::ui::widget::tree::getText(
	ak::ID									_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		return itm->second->text(0);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getText()"); }
}

ak::ui::core::textAlignment ak::ui::widget::tree::getTextAlignment(
	ak::ID									_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		switch (itm->second->textAlignment(0))
		{
		case Qt::AlignLeft: return ak::ui::core::textAlignment::alignLeft;
		case Qt::AlignRight: return ak::ui::core::textAlignment::alignRight;
		case Qt::AlignCenter: return ak::ui::core::textAlignment::alignCenter;
		default: return ak::ui::core::textAlignment::alignLeft;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getTextAlignment()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getTextAlignment()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getTextAlignment()"); }
}

QColor ak::ui::widget::tree::getTextColor(
	ak::ID									_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		return itm->second->textColor(0);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getTextColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getTextColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getTextColor()"); }
}

QColor ak::ui::widget::tree::getBackColor(
	ak::ID									_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		return itm->second->backgroundColor(0);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getBackColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getBackColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getBackColor()"); }
}

bool ak::ui::widget::tree::getEnabled(
	ak::ID									_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		return (!itm->second->isDisabled());
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getEnabled()"); }
}

QIcon ak::ui::widget::tree::getIcon(
	ak::ID									_itemId
) {
	try {
		my_itemsIterator itm = my_items.find(_itemId);
		if (itm == my_items.end()) { throw ak::Exception("Invalid ID", "Check item ID"); }
		return itm->second->icon(0);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getText()"); }
}

bool ak::ui::widget::tree::filterCaseSensitive(void) { return my_filterCaseSensitive; }

bool ak::ui::widget::tree::filterRefreshOnChange(void) { return my_filterRefreshOnChange; }

ak::UID ak::ui::widget::tree::filterUid(void) { return my_filter->uid(); }

// #####################################################################################


void ak::ui::widget::tree::removeChildsFromItem(
	ak::ID						_parent
) {
		try {
		if (_parent == nullptr) { throw ak::Exception("Is nullptr", "Check parent"); }
		while (_parent->childCount() > 0) {
			// Take the items first child
			QTreeWidgetItem * child = _parent->takeChild(0);
			// Remove all childs of the child
			removeChilds(child);
			// Get the childs UID
			ak::ID childId = getItemId(child);
			// Remove the child from the items map
			my_items.erase(childId);
			// Notify all
			my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eDestroyed, childId);
			delete child;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::removeChilds()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::removeChilds()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::removeChilds()"); }
}

ak::ID ak::ui::widget::tree::getItemId(
	ak::ui::qt::treeItem *					_item
) {
	try {
		for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) {
			if (itm->second == _item) { return itm->first; }
		}
		throw ak::Exception("Item does not exist", "Find item");
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::getItemUid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::getItemUid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::getItemUid()"); }
}

bool ak::ui::widget::tree::checkFilter(
	ak::ui::qt::treeItem *						_item
) {
	try {
		if (_item == nullptr) { throw ak::Exception("Is nullptr", "Check item"); }
		bool vis = false;
		// Check item
		if (my_filterCaseSensitive) { if (_item->text(0).contains(my_filter->toPlainText())) { vis = true; } }
		else { if (_item->text(0).toLower().contains(my_filter->toPlainText().toLower())) { vis = true; } }
		// Check all items childs
		for (int i = 0; i < _item->childCount(); i++) {
			if (checkFilter(castTreeItem(_item->child(i)))) { vis = true; }
		}
		// Append data
		_item->setHidden(!vis);
		return vis;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::checkFilter()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::checkFilter()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::checkFilter()"); }
}

void ak::ui::widget::tree::showAll(
	ak::ui::qt::treeItem *						_item
) {
	try {
		if (_item == nullptr) { throw ak::Exception("Is nullptr", "Check item"); }		
		// Unhide
		_item->setHidden(false);
		// call all childs
		for (int i = 0; i < _item->childCount(); i++) {
			showAll(castTreeItem(_item->child(i))); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::showAll()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::showAll()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::showAll()"); }
}

ak::ui::qt::treeItem * ak::ui::widget::tree::castTreeItem(
	QTreeWidgetItem *							_item
) {
	try {
		if (_item == nullptr) { throw ak::Exception("Is nullptr", "Check item"); }
		ak::ui::qt::treeItem * itm = nullptr;
		itm = dynamic_cast<ak::ui::qt::treeItem *>(_item);
		if (itm == nullptr) { throw ak::Exception("Cast failed", "Cast treeItem"); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tree::castItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tree::castItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tree::castItem()"); }
}

*/
