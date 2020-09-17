/*
 * ak_ui_widget_tree.h
 *
 *  Created on: March 10, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <map>

// Qt header
#include <qlayout.h>						// layout for the VBoxLayout
#include <qicon.h>							// QIcon
#include <qcolor.h>							// QColor
#include <qstring.h>						// QString

// AK header
#include "ak_ui_core_aWidgetManager.h"		// base class
#include "ak_globalDataTypes.h"				// UID and ID type
#include "ak_core.h"						// eventType

// Forward declaration
class QTreeWidgetItem;

namespace ak {

	// Forward declaration
	class notifierTreeFilter;
	class uidManager;

	namespace ui {

		// Forward declaration
		namespace qt { class tree; class treeItem; class textEdit; }
		class objectManager;
		class signalLinker;
		class treeSignalLinker;
		class colorStyle;

		namespace widget {

			class tree : public ak::ui::core::aWidgetManager {
			public:
				//! @brief Default constructor
				//! @param _messenger The globally used messenger
				//! @param _uidManager The globally used UID manager
				//! @param _dock The dock widget to display the tree at
				tree(
					ak::messenger *					_messenger,
					ak::uidManager *				_uidManager,
					ak::ui::iconManager *			_iconManager,
					ak::ui::colorStyle *			_colorStyle = nullptr
				);

				//! @brief Deconstructor
				virtual ~tree();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw sim::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

				// ###########################################################################################################################################

				// Data manipulation

				//! @brief Will add a new item to the tree widget and return its ID
				//! @param _parentUid The ID of the parent item. If Id is -1, the item will be added as top level item
				ak::ID add(
					ak::ID							_parentId = -1,
					const QString &					_text = "",
					ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::alignLeft,
					QIcon							_icon = QIcon()
				);

				//! @brief Will add new items to the tree according to the provided command
				//! The command consists of the root item and the childs.
				//! The command may look like this:
				//! root|child|child2
				//! In this case the delimiter will be the '|' and the tree will get a root item with the text "root" (if doesnt exist before)
				//! The root gets a child item with the text "child" and the child gets a child with the text "child2", so the tree looks like this:
				//! ->root
				//! ->->child
				//! ->->->child2
				//! The very last item will also get the alignment, colors and icon set.
				//! Will return the UID of the very last item.
				//! @param _cmd The tree command
				//! @param _delimiter The delimiter used in the command which seperates the items
				ak::ID add(
					const QString &					_cmd,
					char							_delimiter = '|',
					ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::alignLeft,
					const QIcon  &					_icon = QIcon()
				);

				//! @brief Will clear all tree items, receivers will get a destroyed message for each item
				void clear(void);

				//! @brief Will set the selected state of the provided item.
				//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
				//! @param _itemId The ID of the item
				//! @param _selected The selected state of the item
				//! @throw ak::Exception if the ID is invalid
				void setItemSelected(
					ak::ID							_itemId,
					bool							_selected
				);

				//! @brief Will set the selected state of the provided item.
				//! Will not change the selected state of the childs item even if the selectAndDeselectChilds option is true
				//! @param _itemId The ID of the item
				//! @param _selected The selected state of the item
				//! @throw ak::Exception if the ID is invalid
				void setSingleItemSelected(
					ak::ID							_itemId,
					bool							_selected
				);

				//! @brief Will toggle the selected state of the provided item.
				//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
				//! @param _itemId The ID of the item
				//! @param _selected The selected state of the item
				//! @throw ak::Exception if the ID is invalid
				void toggleItemSelection(
					ak::ID							_itemId
				);

				//! @brief Will deselect all items
				void deselectAllItems(void);

				// ###########################################################################################################################################

				// Filter

				//! @brief Will set the visible mode of the filter textedit
				//! @param _visible If true, the filter textedit will be visible
				void setFilterVisible(
					bool							_visible
				);

				//! @brief Will refresh the tree by means of the current filter
				//! If the filter is empty (length = 0) all items will be shown
				void applyCurrentFilter(void);

				//! @brief Will set the case sensitive mode for the filter
				//! @param _caseSensitive If true, the filter is case sensitive
				//! @param _refresh If true, the filter will be checked with the new setting
				void setFilterCaseSensitive(
					bool							_caseSensitive,
					bool							_refresh = true
				);

				//! @brief Will set the filter refresh on change mode
				//! @param _refreshOnChange If true, the filter will always be applied when he changes, othewise only on return pressed
				void setFilterRefreshOnChange(
					bool							_refreshOnChange
				);

				//! @brief Enables or disables the ability to select multiple tree items
				//! @param _multiSelection Specify whether multiple items can be selected
				void setMultiSelectionEnabled(
					bool							_multiSelection
				);

				//! @brief Enables or disables the ability to automatically select/deselect the childrens of an item
				//! @param _enabled if true, the childs of an item will be selected/deselected automatically
				void setSelectAndDeselectChildrenEnabled(
					bool							_enabled
				);

				//! @brief Will expand all items in this tree
				void expandAllItems(void);

				//! @brief Will collapse all items in this tree
				void collapseAllItems(void);

				// ###########################################################################################################################################

				// Information gathering

				//! @brief Returns a list of all selected items
				std::vector<ak::ID> selectedItems(void);

				//! @brief Will return all items from root to specified item as a vector where the first item is the root item
				//! @param _itemId The ID of the requested item
				//! @throw ak::Exception if the provided item ID is invalid
				std::vector<QString> getItemPath(
					ak::ID									_itemId
				);

				//! @brief Will return all items from root to specified item as a string seperated with the provided delimiter where the first item is the root item
				//! @param _itemId The ID of the requested item
				//! @param _delimiter The delimiter between the items
				//! @throw ak::Exception if the provided item ID is invalid
				QString getItemPathString(
					ak::ID									_itemId,
					char									_delimiter = '|'
				);

				// ###########################################################################################################################################

				// Events

				//! @brief Will perform actions on the filter text changed event
				void performFilterTextChanged(void);

				//! @brief Will perform actions on the filter enter pressed event
				void performFilterEnterPressed(void);

				//! @brief Will send a event message to the messaging system in the name of the provided tree widget item
				//! @param _item The sender
				//! @param _eventType The type of the event
				//! @param _info1 Additional information 1
				//! @param _info2 Additional information 2
				void raiseItemEvent(
					ak::ID							_itemId,
					ak::core::eventType				_eventType,
					int								_info2 = 0
				);

				//! @brief Will perform the selection changed event
				void selectionChangedEvent(void);

			private:

				//! @brief Create a new tree item
				//! @param _text The item to set
				//! @param _textAlignment The text alignment
				//! @param _foreColor The fore color to set
				//! @param _backColor The back color to set
				//! @param _icon The icon to set
				//! @throw ak::Exception If failed to create
				ak::ui::qt::treeItem * createItem(
					const QString &					_text = "",
					ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::alignLeft,
					QIcon							_icon = QIcon()
				);

				//! @brief Will clear the memory
				void memFree(void);

				//! @brief Will delete the childs of this item and remove them from the map
				//! @param _itm The item to clear the childs from
				void clearItem(
					ak::ui::qt::treeItem *			_itm
				);

				ak::ui::qt::tree *							my_tree;					//! The tree widget this class is managing
				ak::ui::qt::textEdit *						my_filter;					//! The filter used in this tree
				QVBoxLayout *								my_layout;					//! The layout of the tree
				QWidget *									my_multiWidget;				//! The widget used to apply the layout for the tree to
				bool										my_filterCaseSensitive;		//! If true, the filter is case sensitive
				bool										my_filterRefreshOnChange;	//! If true, the filter will always be applied when the text changes
				ak::ui::treeSignalLinker *					my_treeSignalLinker;		//! The signal linker used to connect the tree signals to the messaging system
				
				// Internal message system

				ak::messenger *								my_internalMessenger;		//! The internally used messenger
				ak::uidManager *							my_internalUidManager;		//! The internally used UID manager
				ak::ui::signalLinker *						my_filterSignalLinker;		//! The signal linker used to connect the filter signals
				ak::notifierTreeFilter *					my_notifierFilter;			//! The notifier used to catch the filter signals

				QString										my_headerText;				//! The header text of the tree

				ak::ID										my_currentId;				//! The current id of the items

				std::map<ak::ID, ak::ui::qt::treeItem *>	my_items;
				typedef std::map<ak::ID,
					ak::ui::qt::treeItem *>::iterator		my_itemsIterator;


				bool										my_selectAndDeselectChildren;

			};

			/*

			//! This class is used to manage a QTreeWidget and provides the functionality for a filter
			class tree : public ak::ui::core::aWidgetManager {
			public:

		
				// ###########################################################################################################################################
				
				// Data manipulation

				
				//void remove(std::list<ak::uidManager::UID> items);

				

				// ###########################################################################################################################################

				// ###########################################################################################################################################
			
				//! @brief Will set the text of the specified tree item
				//! @param _itemUid The items UID to change the text of
				//! @param _text The text to set at the specified item
				void setText(
					ak::ID									_itemId,
					const QString &							_text
				);

				//! @brief Will set the text color of the specified tree item
				//! @param _itemUid The items UID to change the text color of
				//! @param _color The text color to set at the specified item
				void setTextColor(
					ak::ID									_itemId,
					const QColor &							_color
				);

				//! @brief Will set the text alignment of the specified tree item
				//! @param _itemUid The items UID to change the text alignment of
				//! @param _textAlignment The text alignment to set at the specified item
				void setTextAlignment(
					ak::ID									_itemId,
					ak::ui::core::textAlignment				_textAlignment
				);

				//! @brief Will set the back color of the specified tree item
				//! @param _itemUid The items UID to change the back color of
				//! @param _color The back color to set at the specified item
				void setBackColor(
					ak::ID									_itemId,
					const QColor &							_color
				);

				//! @brief Will set the specified tree item endabled or disabled
				//! @param _itemUid The items UID to change the enabled state of
				//! @param _enabled If true, the specified item will be enabled
				void setEnabled(
					ak::ID									_itemId,
					bool									_enabled
				);

				//! @brief Will set the icon of the specified tree item
				//! @param _itemUid The items UID to change the icon of
				//! @param _icon The icon to set at the specified item
				void setIcon(
					ak::ID									_itemId,
					const QIcon &							_icon
				);

				// ###########################################################################################################################################

				

				// ###########################################################################################################################################
		
				// Getter

				//! @brief Returns the UID of the specified item
				//! Will return the UID of the very last item in the command
				//! The command describes all childs from root to the required ited. The command may look like this:
				//! root|child|child2
				//! In this case the delimiter is '|'.
				//! The requsted item, is the item with the text "child2".
				//! The items parent is the item with the text "child",
				//! and its parent is the item with the text "root".
				//! @param _cmd The requested items tree command
				//! @param _delimiter The character which is the delimiter between the items in the command,
				//! @param _throwException If true, an ak::Exception will be thrown in the case that the requested item doesn't exist
				ak::ID getItemId(
					const QString &					_cmd,
					char							_delimiter = '|',
					bool							_throwException = true
				);

				//! @brief Will return the tree header text
				QString getHeaderText() const;

				//! @brief Will return true if the tree header is visible
				bool getHeaderVisible() const;

				//! @brief Will return the text of the specified tree item
				//! @brief The UID of the tree item
				QString getText(
					ak::ID								_itemId
				);

				//! @brief Will return the text color of the specified tree item
				//! @brief The UID of the tree item
				QColor getTextColor(
					ak::ID								_itemId
				);

				//! @brief Will return the text alignment of the specified tree item
				//! @brief The UID of the tree item
				ak::ui::core::textAlignment getTextAlignment(
					ak::ID								_itemId
				);

				//! @brief Will return the back color of the specified tree item
				//! @brief The UID of the tree item
				QColor getBackColor(
					ak::ID								_itemId
				);

				//! @brief Will return true if the specified item is enabled
				//! @brief The UID of the tree item
				bool getEnabled(
					ak::ID								_itemId
				);

				//! @brief Will return the icon of the specified tree item
				//! @brief The UID of the tree item
				QIcon getIcon(
					ak::ID								_itemId
				);

				//! @brief Returns true if the filter is case sensitive
				bool filterCaseSensitive(void);

				//! @brief Returns the UID of the filter
				ak::UID filterUid(void);

				//! @brief Returns true if the filter will be applied when its text changes
				bool filterRefreshOnChange(void);

			private:
				

				std::map<ak::ID, ak::ui::qt::treeItem *>	my_items;					//! All items that are currently created in this tree
				typedef std::map<ak::ID,
					ak::ui::qt::treeItem *>::iterator		my_itemsIterator;			//! Iterator used to iterate trough the items


				// ###########################################################################################################################################

				//! @brief Will remove all childs from the item provided
				//! Will not delete the provided item, but all childs
				//! @param _parent The item to remove all childs from
				void removeChildsFromItem(
					ak::ID									_parent
				);

				//! @brief Will return the UID of the provided item, throws exception if doesnt exist
				//! @param _item The item to find the UID
				ak::ID getItemId(
					ak::ui::qt::treeItem *					_item
				);

				//! @brief Will check if one of the childs matches the filter
				//! Will also check all childs of the child aswell
				//! @param _item The child to check
				bool checkFilter(
					ak::ui::qt::treeItem *					_item
				);

				//! @brief Will show all childs of the tree widget item provided and iteself
				//! @param _item The item to show all its childs of
				void showAll(
					ak::ui::qt::treeItem *					_item
				);

				//! @brief Will cast the provided QTreeWidgetItem to a treeItem
				//! @param _item The item to cast
				//! @throw ak::Exception if the cast fails or provided item is nullptr
				static ak::ui::qt::treeItem * castTreeItem(QTreeWidgetItem * _item);

			}; // class tree

			*/

		} // namespace widget
	} // namespace ui
} // namespace ak