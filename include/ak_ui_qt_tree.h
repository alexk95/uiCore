/*
 * ak_ui_qt_tree.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <map>
#include <vector>

 // Qt header
#include <qtreewidget.h>					// base class
#include <qevent.h>							// mouse event

// AK header
#include "ak_ui_core_aWidget.h"				// base class
#include "ak_globalDataTypes.h"				// UID and ID types

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			// Forward declaration
			class treeItem;

			//! This class represents a tree
			class tree : public QTreeWidget, public ak::ui::core::aWidget {
				Q_OBJECT
			public:
				//! @brief Constructor
				tree(
					ak::ui::colorStyle *			_colorStyle = nullptr
				);

				virtual ~tree();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw sim::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

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

				// ####################################################################################################################################

				//! @brief Will extract the id form the QTreeWidgetItem (which must be a treeItem)
				static ak::ID getItemId(
					QTreeWidgetItem *				_item
				);

			private:
				std::map<ak::ID,treeItem *>			my_topLevelItems;
				typedef std::map<ak::ID,
					treeItem *>::iterator			my_topLevelItemsIterator;
			};

		} // namespace qt
	} // namespace ui
} // namespace ak


/*

namespace ak {
	namespace ui {
		
		// Forward declaration
		namespace widget { class tree; }

		namespace qt {

			// Forward declaration
			class treeItem;

			class tree : public QTreeWidget, public ak::ui::core::aWidget
			{
			public:
				tree(
					ak::ui::widget::tree *				_creator,
					ak::ui::colorStyle *				_colorStyle = nullptr
				);
				
				virtual ~tree();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw sim::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

				virtual void mouseMoveEvent(
					QMouseEvent *						_event
				);
				
				virtual void leaveEvent(
					QEvent *							_event
				);

				void selectItem(
					QModelIndex							_item
				);
				
				void deselectItem(
					QModelIndex							_item
				);

				void selectChildren(
					QModelIndex							_item
				);
				
				void deselectChildren(
					QModelIndex							_item
				);
				
				void deselectParentFolders(
					QModelIndex							_item
				);

				void selectChildren(
					QTreeWidgetItem *					_item
				);
				
				void deselectChildren(
					QTreeWidgetItem *					_item
				);
				
				void deselectParentFolders(
					QTreeWidgetItem *					_item
				);

				virtual void keyPressEvent(
					QKeyEvent *							_event
				);

				void setSendSelectionChangedNotification(
					bool								_f
				);

				bool getSendSelectionChangedNotification(void);

				void addItemToList(
					QModelIndex							_item,
					std::list<treeItem *> &				_itemList
				);

				std::string getItemName(
					QModelIndex							_item
				);

			private:
				ak::ui::widget::tree *					my_tree;
				bool									my_sendSelectionChangedNotification;
			};

		} // namespace qt
	} // namespace ui
} // namespace ak

*/