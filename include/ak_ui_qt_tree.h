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
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
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
