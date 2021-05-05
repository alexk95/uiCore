/*
 * ak_ui_widget_tree.h
 *
 *  Created on: March 10, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <map>
#include <vector>
#include <list>

// Qt header
#include <qobject.h>						// Base class
#include <qlayout.h>						// layout for the VBoxLayout
#include <qicon.h>							// QIcon
#include <qcolor.h>							// QColor
#include <qstring.h>						// QString
#include <qtreewidget.h>					// base class

// AK header
#include <ak_ui_core_aWidget.h>				// base class
#include <ak_globalDataTypes.h>				// UID and ID type
#include <ak_core.h>						// eventType
#include <ak_exception.h>					// Error handling

// Forward declaration
class QTreeWidgetItem;
class QKeyEvent;
class QWidget;
class QMouseEvent;

namespace ak {

	// Forward declaration
	class uidManager;

	namespace ui {

		// Forward declaration
		class objectManager;
		class signalLinker;
		class treeSignalLinker;
		class colorStyle;

		namespace qt {

			class treeBase; class treeItem; class lineEdit;

			class UICORE_API_EXPORT tree : public QObject, public ak::ui::core::aWidget {
				Q_OBJECT
			public:
				//! @brief Default constructor
				//! @param _messenger The globally used messenger
				//! @param _uidManager The globally used UID manager
				//! @param _dock The dock widget to display the tree at
				tree(
					const ak::ui::colorStyle *		_colorStyle = nullptr
				);

				//! @brief Deconstructor
				virtual ~tree();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				virtual void setColorStyle(
					const ak::ui::colorStyle *		_colorStyle
				) override;

				// ###########################################################################################################################################

				// Data manipulation

				//! @brief Will add a new item to the tree widget and return its ID
				//! @param _parentUid The ID of the parent item. If Id is -1, the item will be added as top level item
				ak::ID add(
					ak::ID							_parentId = -1,
					const QString &					_text = "",
					ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::align_alignLeft,
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
					ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::align_alignLeft,
					const QIcon  &					_icon = QIcon()
				);

				//! @brief Will clear all tree items, receivers will get a destroyed message for each item
				void clear(bool _emitEvent = true);

				//! @brief Will set enabled state of the provided item
				//! @param _itemId The ID of the item
				//! @param _enabled The enabled state of the item
				void setItemEnabled(
					ak::ID							_itemId,
					bool							_enabled
				);

				//! @brief Will set the selected state of the provided item.
				//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
				//! @param _itemId The ID of the item
				//! @param _selected The selected state of the item
				void setItemSelected(
					ak::ID							_itemId,
					bool							_selected
				);

				//! @brief Will set enabled state of the provided item
				//! @param _itemId The ID of the item
				//! @param _enabled The enabled state of the item
				void setItemVisible(
					ak::ID							_itemId,
					bool							_visible
				);

				//! @brief Will set the text of the provided item
				//! @param _itemId The ID of the item
				//! @param _text The text to set
				void setItemText(
					ak::ID							_itemId,
					const QString &					_text
				);

				//! @brief Will set the selected state of the provided item.
				//! Will not change the selected state of the childs item even if the selectAndDeselectChilds option is true
				//! @param _itemId The ID of the item
				//! @param _selected The selected state of the item
				void setSingleItemSelected(
					ak::ID							_itemId,
					bool							_selected
				);

				//! @brief Will toggle the selected state of the provided item.
				//! Will also set the selected state of the items childs if the selectAndDeselectChilds option is true
				//! @param _itemId The ID of the item
				//! @param _selected The selected state of the item
				void toggleItemSelection(
					ak::ID							_itemId
				);

				//! @brief Will deselect all items
				//! @param _emitEvent If true a selection changed signal will be emitted
				void deselectAllItems(
					bool							_emitEvent
				);

				//! @brief Will set the enabled state of this tree
				void setEnabled(
					bool							_enabled = true
				);

				//! @brief Will set the visible state of this tree
				void setVisible(
					bool							_visible = true
				);

				//! @brief Will set the item icon of the specified item
				//! @param _itemId The ID of the item to set the icon at
				//! @param _icon The icon to set
				void setItemIcon(
					ak::ID							_itemId,
					const QIcon &					_icon
				);

				//! @brief Will set the sorting enabled flag for this tree
				//! @param _enabled The enabled state to set
				void setSortingEnabled(
					bool							_enabled
				);

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
				void setAutoSelectAndDeselectChildrenEnabled(
					bool							_enabled
				);

				//! @brief Will expand all items in this tree
				void expandAllItems(void);

				//! @brief Will collapse all items in this tree
				void collapseAllItems(void);

				//! @brief Will delete the provided item from this tree
				//! @param _item The item to delete
				void deleteItem(
					ak::ID												_itemID
				);

				//! @brief Will delete the provided items from this tree
				//! @param _items The items to delete
				void deleteItems(
					const std::vector<ak::ID> &							_itemIDs
				);

				//! @brief Will set the items are editable flag
				//! @param _editable If true, the items can be modified by the user
				//! @param _applyToAll If true, then the new state will be applied to all existing items, otherwise this chane will only affect items created after this point
				void setItemsAreEditable(
					bool												_editable = true,
					bool												_applyToAll = true
				);

				//! @brief Will set the editable flag of one item
				//! @param _item The item id
				//! @param _editable If true, the item can be modified by the user
				void setItemIsEditable(
					ak::ID												_itemID,
					bool												_editable
				);

				//! @brief Will set the editable flag of the provided items item
				//! @param _item The item id
				//! @param _editable If true, the item can be modified by the user
				void setItemIsEditable(
					const std::vector<ak::ID> &							_itemIDs,
					bool												_editable
				);

				// ###########################################################################################################################################

				// Information gathering

				//! @brief Returns a list of all selected items
				std::vector<ak::ID> selectedItems(void);

				//! @brief Will return all items from root to specified item as a vector where the first item is the root item
				//! @param _itemId The ID of the requested item
				std::vector<QString> getItemPath(
					ak::ID									_itemId
				);

				//! @brief Will return all items from root to specified item as a string seperated with the provided delimiter where the first item is the root item
				//! @param _itemId The ID of the requested item
				//! @param _delimiter The delimiter between the items
				QString getItemPathString(
					ak::ID									_itemId,
					char									_delimiter = '|'
				);

				//! @brief Will return the ID of the specified item
				//! @param _itemPath The path of the requested item
				//! @param _delimiter The delimiter of the item path
				ak::ID getItemID(
					const QString &							_itemPath,
					char									_delimiter = '|'
				);

				//! @brief Will return the text of the specified item
				//! @param _itemId The ID of the item to get the text of
				QString getItemText(
					ak::ID									_itemId
				);

				//! @brief Will return the enabled state of this tree
				bool enabled() const;

				//! @brief Will return the count of items in this tree
				int itemCount(void) const;

				//! @brief Will return the sorting enabled state
				bool isSortingEnabled(void) const;

				// ###########################################################################################################################################

				// Events

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
				//! @param _emitEvent If true a selection changed signal will be emitted
				void selectionChangedEvent(
					bool							_emitEvent = true
				);

			signals:
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);
				void cleared(void);
				void focusLost(void);
				void selectionChanged(void);
				void itemActivated(QTreeWidgetItem *, int);
				void itemChanged(QTreeWidgetItem *, int);
				void itemClicked(QTreeWidgetItem *, int);
				void itemCollapsed(QTreeWidgetItem *);
				void itemDoubleClicked(QTreeWidgetItem *, int);
				void itemFocused(QTreeWidgetItem *);
				void itemExpanded(QTreeWidgetItem *);
				void itemTextChanged(QTreeWidgetItem *, int);
				void itemLocationChanged(QTreeWidgetItem *, int);

			public slots:
				
				void slotTreeKeyPressed(QKeyEvent * _event);
				void slotTreeKeyReleased(QKeyEvent * _event);
				void slotTreeItemActivated(QTreeWidgetItem * _item, int _column);
				void slotTreeItemChanged(QTreeWidgetItem * _item, int _column);
				void slotTreeItemClicked(QTreeWidgetItem * _item, int _column);
				void slotTreeItemCollapsed(QTreeWidgetItem * _item);
				void slotTreeItemDoubleClicked(QTreeWidgetItem * _item, int _column);
				void slotTreeItemExpanded(QTreeWidgetItem * _item);
				void slotTreeSelectionChanged();
				void slotTreeMouseMove(QMouseEvent *);
				void slotTreeLeave(QEvent *);

				//! @brief Will perform actions on the filter text changed event
				void slotFilterTextChanged(void);

				//! @brief Will perform actions on the filter enter pressed event
				void slotFilterKeyPressed(QKeyEvent * _event);

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
					ak::ui::core::textAlignment		_textAlignment = ak::ui::core::textAlignment::align_alignLeft,
					QIcon							_icon = QIcon()
				);

				//! @brief Will clear the memory
				void memFree(void);

				//! @brief Will delete the childs of this item and remove them from the map
				//! @param _itm The item to clear the childs from
				void clearItem(
					ak::ui::qt::treeItem *			_itm
				);

				treeBase *									my_tree;					//! The tree widget this class is managing
				lineEdit *									my_filter;					//! The filter used in this tree
				QWidget *									my_widget;
				QVBoxLayout *								my_layout;					//! The layout of the tree
				bool										my_filterCaseSensitive;		//! If true, the filter is case sensitive
				bool										my_filterRefreshOnChange;	//! If true, the filter will always be applied when the text changes
				bool										my_ignoreEvents;

				ID											my_focusedItem;

				QString										my_headerText;				//! The header text of the tree

				ak::ID										my_currentId;				//! The current id of the items

				std::map<ak::ID, ak::ui::qt::treeItem *>	my_items;
				typedef std::map<ak::ID,
					ak::ui::qt::treeItem *>::iterator		my_itemsIterator;

				bool										my_selectAndDeselectChildren;
				bool										my_itemsAreEditable;		//! If true then the items in this tree are editable
			};

			// #########################################################################################################################################

			// #########################################################################################################################################

			// #########################################################################################################################################

			//! This class represents a tree
			class UICORE_API_EXPORT treeBase : public QTreeWidget, public ak::ui::core::aWidget {
				Q_OBJECT
			public:
				//! @brief Constructor
				treeBase(
					const colorStyle *		_colorStyle = nullptr
				);

				virtual ~treeBase();

				// #######################################################################################################
				// Event handling

				//! @brief Emits a key pressend signal a key is pressed
				virtual void keyPressEvent(QKeyEvent * _event) override;

				//! @brief Emits a key released signal a key is released
				virtual void keyReleaseEvent(QKeyEvent * _event) override;

				//! @brief Emits a mouse move event
				virtual void mouseMoveEvent(QMouseEvent *event) override;

				//! @brief Emits a FocusLeft event
				virtual void leaveEvent(QEvent *event) override;

				// #######################################################################################################

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				// ####################################################################################################################################

				//! @brief Will add the provided item to the top level and store its information
				//! @param _item The item to add
				void AddTopLevelItem(
					treeItem *						_item
				);

				//! @brief Will return the top level item with the provided text
				//! Returns nullptr if the item does not exist
				treeItem * topLevelItem(
					const QString &					_text
				);

				//! @brief Will return the top level item with the provided ID
				//! Returns nullptr if the item does not exist
				treeItem * topLevelItem(
					ak::ID							_id
				);

				//! @brief Will return the names of all top level items
				std::vector<QString> topLevelItemsText(void);

				//! @brief Will clear the tree
				void Clear(void);

				//! @brief Will remove the topLevelItem with the provided ID
				//! Will not destroy the item
				//! @throw ak::Exception if the provided ID is invalid
				void removeTopLevelItem(
					ak::ID							_id
				);

				// ####################################################################################################################################

				//! @brief Will extract the id form the QTreeWidgetItem (which must be a treeItem)
				static ak::ID getItemId(
					QTreeWidgetItem *				_item
				);

			signals:
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);
				void mouseMove(QMouseEvent *);
				void leave(QEvent *);

			private:
				std::map<ak::ID, treeItem *>			my_topLevelItems;
				typedef std::map<ak::ID,
					treeItem *>::iterator			my_topLevelItemsIterator;
			};

			// #########################################################################################################################################

			// #########################################################################################################################################

			// #########################################################################################################################################

			class UICORE_API_EXPORT treeItem : public QTreeWidgetItem, public core::aObject {
			public:

				//! @brief Constructor
				//! @param _newId The ID of this item
				//! @param _type The type of this item
				treeItem(
					ak::ID							_newId,
					treeItem *						_parent = nullptr,
					int								_type = 0
				);

				//! @brief Constructor
				//! @param _view The view that creates this item
				//! @param _newId The ID of this item
				//! @param _type The type of this item
				treeItem(
					ak::ui::qt::treeBase *			_view,
					ak::ID							_newId,
					treeItem *						_parent = nullptr,
					int								_type = 0
				);

				//! @brief Deconstructor
				virtual ~treeItem();

				// ##############################################################################################

				// Setter

				//! @brief Will add a new child to this item
				//! @param _child The child to add
				void AddChild(
					treeItem *						_child
				);

				//! @brief Will set the provided item as the parent item
				//! @param _parent The parent item
				void setParentItem(
					treeItem *						_parent
				);

				//! @brief Will set the enabled state for every child of this item
				//! @param _enabled If true all childs of this item will be enabled
				void setChildsEnabled(
					bool							_enabled
				);

				//! @brief Will select/deselect the childs of this item
				//! @param _selected If true all childs of this item will be selected
				void setChildsSelected(
					bool							_selected
				);

				//! @brief Will set the visible state for every child of this item
				//! @param _visible If true all childs of this item will be visible
				void setChildsVisible(
					bool							_visible
				);

				//! @brief Will expand the most top level item that is selected and is a parent of this item. If this is the top level item, this item will be expanded
				void ensureTopLevelSelectionVisible();

				//! @brief Will expand all parent items of this item
				//! @param _expandThis If true, this item will be expanded
				void expandAllParents(
					bool							_expandThis
				);

				//! @brief Will collapse this item and all of its childs
				void collapse(void);

				// ##############################################################################################

				// Getter

				//! @brief Will return a pointer to a top level child with the provided id.
				//! Returns nullptr if the item does not exist.
				treeItem * findChild(
					ak::ID							_id
				);

				//! @brief Will return a pointer to a top level child with the provided text.
				//! Returns nullptr if the item does not exist.
				treeItem * findChild(
					const QString &					_text
				);

				//! @brief Will return the ID of the specified item
				//! @param _itemPath The path of the requested item
				//! @param _delimiter The delimiter of the item path
				ak::ID getItemID(
					const QStringList &				_itemPath,
					int								_currentIndex
				);

				//! @brief Will erase this child from the information storage
				//! This function should only be called from the child itself to clear up its data
				//! @param _id The ID of the item to erase
				void eraseChild(
					ak::ID							_id
				);

				//! @brief Will return all next level childs of this item
				const std::list<treeItem *> & childs(void);

				//! @brief Will return all childs of this item
				const std::list<treeItem *> & allChilds(void);

				//! @brief Will return the IDs of all childs at this item
				const std::list<ak::ID> & allChildsIDs(void);

				//! @brief Will return the ammound of childs this item has
				int childCount(void) const;

				//! @brief Will return the id of the child
				ak::ID id(void) const;

				//! @brief Will return the id of the parent item
				//! Returns -1 if there is no parent item
				ak::ID parentId(void) const;

				//! @brief Will set the stored text, this value is only used to evaluate the changed event
				void setStoredText(const QString & _text) { my_text = _text; }

				//! @brief Will return the stored text
				QString storedText(void) const { return my_text; }

				//! @brief Will unhide this item if hidden
				//! @param _expandParents If true all parent objects will be expanded
				void setVisible(
					bool							_expandParents = true
				);

				//! @brief Will return all items from root to this item as a vector where the first item is the root item
				//! @throw ak::Exception if the provided item ID is invalid
				std::list<QString> getItemPath();

				//! @brief Will return all items from root to this item as a string seperated with the provided delimiter where the first item is the root item
				//! @param _itemId The ID of the requested item
				//! @param _delimiter The delimiter between the items
				//! @throw ak::Exception if the provided item ID is invalid
				QString getItemPathString(
					char							_delimiter = '|'
				);

			private:
				treeItem *					my_parent;
				std::list<treeItem *>		my_childs;			//! Contains all childs of this item
				std::list<treeItem *>		my_allChilds;
				std::list<ak::ID>			my_allChildsIDs;
				ak::ID						my_id;
				QString						my_text;

				treeItem(treeItem &) = delete;
				treeItem & operator = (treeItem &) = delete;

			};

		} // namespace widget
	} // namespace ui
} // namespace ak