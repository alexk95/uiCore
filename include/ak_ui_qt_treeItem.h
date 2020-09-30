/*
 * ak_ui_qt_treeItem.h
 *
 *  Created on: August 30, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <list>						// vector<>

// Qt header
#include <qtreewidget.h>			// Base class
#include <qstring.h>				// QString

// AK header
#include <ak_ui_core_aObject.h>		// Base class
#include <ak_globalDataTypes.h>		// UID and ID type

namespace ak {
	namespace ui {
		namespace qt {

			class tree;

			class treeItem : public ak::ui::core::aObject, public QTreeWidgetItem {
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
					ak::ui::qt::tree *				_view,
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

				//! @brief Will select/deselect the childs of this item
				//! @param _selected If true all childs of this item will be selected
				void setChildsSelected(
					bool							_selected
				);

				//! @brief Will expand all parent items of this item.
				//! @param _expandThis If true this item will be expanded aswell
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

				treeItem(treeItem &) = delete;
				treeItem & operator = (treeItem &) = delete;

			};

		} // namespace qt
	} // namespace ui
} // namespace ak