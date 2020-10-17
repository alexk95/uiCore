/*
 * ak_ui_qt_list.h
 *
 *  Created on: October 13, 2020
 *	Last modified on: October 13, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

 // AK header
#include <ak_ui_core_aWidget.h>		// base class (list)
#include <ak_globalDataTypes.h>		// ak::ID type

// Qt header
#include <qlistwidget.h>			// base class (list, listItem)

// C++ header
#include <map>

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			// Forward declaration
			class listItem;

			class list : public QListWidget, public ak::ui::core::aWidget {
			public:
				// Constructor

				list();
				// Deconstructor
				virtual ~list();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				) override;

				// ###########################################################################################################################################

				//! @brief Will add a new item to this list
				//! @param _text The item text
				ak::ID AddItem(
					const QString &					_text
				);

				//! @brief Will add a new item to this list
				//! @param _icon The item icon
				//! @param _text The item text
				ak::ID AddItem(
					const QIcon &					_icon,
					const QString &					_text
				);

				//! @brief Will remove all items
				void Clear(void);

				listItem * Item(
					const QString &					_text
				);

				//QString itemText

			private:
				
				//! Will clear the memory allocated by this object
				void memFree(void);

				ak::ID								my_currentId;
				std::map<ak::ID, listItem *>		my_items;
				typedef std::map<ak::ID,
					listItem *>::iterator			my_itemsIterator;

			}; // class list

			// ###########################################################################################################################################

			// ###########################################################################################################################################

			// ###########################################################################################################################################

			class listItem : public QListWidgetItem {
			public:
				listItem(
					ak::ID				_id = ak::invalidID,
					list *				_view = nullptr,
					int					_type = 0
				);

				listItem(
					const QString &		_text,
					ak::ID				_id = ak::invalidID,
					list *				_view = nullptr,
					int					_type = 0
				);

				listItem(
					const QIcon &		_icon,
					const QString &		_text,
					ak::ID				_id = ak::invalidID,
					list *				_view = nullptr,
					int					_type = 0
				);

				virtual ~listItem();

				//! @brief Will set the items ID
				//! @param _id The ID to set
				void setId(
					ak::ID				_id
				);

				//! @brief Returns the items ID
				ak::ID id(void) const;

			private:
				ak::ID					my_id;

				listItem(const listItem &) = delete;
				listItem & operator = (const listItem &) = delete;

			};	// class listItem

		} // namespace qt
	} // namespace ui
} // namespace ak