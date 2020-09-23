/*
 * ak_ui_qt_comboButton.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

 // Qt header
#include <qpushbutton.h>				// base class
#include <qstring.h>					// QString

// AK header
#include "ak_ui_core_aWidget.h"			// base class
#include "ak_ui_core.h"					// objectType
#include "ak_globalDataTypes.h"			// UID and ID type
#include "ak_ui_qt_comboButtonItem.h"	// comboButtonItem
#include "ak_uidMangager.h"				// uidManager

// Forward declaration
class QMenu;
class QKeyEvent;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			//! @brief This class represents a combo button which consists of a QPushButton and it's Menu
			//! This class behaves like a combo box
			class comboButton : public QPushButton, public ak::ui::core::aWidget
			{
				Q_OBJECT
			public:
				//! @brief Constructor
				//! @param _initialText The initial text of this combo button
				//! @param _colorStyle The color style used for this combo button
				//! @param _parent The parent QWidget for this combo button
				//! @throw ak::Exception if the creation of the menu failed
				comboButton(
					const QString &										_initialText = QString(""),
					ak::ui::colorStyle *								_colorStyle = nullptr,
					QWidget *											_parent = (QWidget *) nullptr
				);

				//! @brief Deconstructor
				virtual ~comboButton();

				// #######################################################################################################
				// Event handling

				//! @brief Emits a returnPressed signal if the return key is pressed
				virtual void keyPressEvent(QKeyEvent * _event) override;

				// #######################################################################################################

				//! @brief Will remove all items from the combo button
				void clearItems(void);

				//! @brief Will set the items for the combo button
				//! @param _menu The items to set as the menu for this combo button
				//! @throw ak::Exception if the creation of a new item failed
				void setItems(
					const std::vector<ak::ui::qt::comboButtonItem> &		_menu
				);

				//! @brief Will add a new item to the combo button
				//! @param _other The new item to add
				//! @throw ak::Exception if the creation of the new item failed
				int addItem(
					const ak::ui::qt::comboButtonItem &					_other
				);

				//! @brief Will return the ammount of items in the combo button
				int getItemCount(void) const;

				// #######################################################################################################
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

			signals:
				void keyPressed(QKeyEvent *);
				void changed(void);

			private slots:
				void slotItemTriggered();

			private:
				QMenu *														my_menu;				//! The QMenu of this combo button
				ak::uidManager												my_itemsUIDmanager;		//! The UID manager used for the menu items of this combo button
				std::vector<ak::ui::qt::comboButtonItem *>					my_items;				//! The items of this combo button
				std::map<ak::UID, size_t>									my_itemUids;			//! The map containing the index of a item UID
				typedef std::map <ak::UID, size_t > ::iterator				my_itemUidsIterator;	//! Iterator used to iterate through the item UID map

			};
		}
	}
}
