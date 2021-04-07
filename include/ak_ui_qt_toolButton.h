/*
 * ak_ui_qt_toolButton.h
 *
 *  Created on: November 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aWidget.h>			// Base class
#include <ak_globalDataTypes.h>

// Qt header
#include <qtoolbutton.h>				// Base class
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon

class QMenu;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			class action;
			class contextMenuItem;

			class UICORE_API_EXPORT toolButton : public QToolButton, public ui::core::aWidget {
				Q_OBJECT
			public:
				toolButton();

				toolButton(
					const QString &				_text
				);
				
				toolButton(
					const QIcon &				_icon,
					const QString &				_text
				);

				virtual ~toolButton();

				// #######################################################################################################
				
				// Event handling

				//! @brief Emits a key pressend signal a key is pressed
				virtual void keyPressEvent(QKeyEvent * _event) override;

				//! @brief Emits a key released signal a key is released
				virtual void keyReleaseEvent(QKeyEvent * _event) override;

				// #######################################################################################################
				
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override { return this; }

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				// #######################################################################################################

				//! @brief Will return the action of this tool button
				action * getAction() const { return my_action; }

				//! @brief Will set the toolTip and WhatsThis of this toolButton and its action
				//! @param _text The text to set
				void SetToolTip(
					const QString &						_text
				);

				//! @brief Will return the current toolTip of this toolButton
				QString ToolTip(void) const;

				//! @brief Will add a new menu item to the menu
				//! This toolButton will take over control over the contextMenuItem
				//! @param _item The item to add
				ak::ID addMenuItem(
					contextMenuItem *					_item
				);

				//! @brief Will add a menu seperator to the menu
				void addMenuSeperator(void);

				//! @brief Will clear the menu
				void clearMenu(void);

				//! @brief Will set the checked state of the specified menu item
				//! @param _itemID The ID of the item
				//! @param _checked The checked state to set
				void setMenuItemChecked(
					ak::ID								_itemID,
					bool								_checked = true
				);

				//! @brief Will disable the ability to check and uncheck the item (can be reenabled with setChecked)
				//! @param _itemID The ID of the item
				void setMenuItemNotCheckable(
					ak::ID								_itemID
				);

				//! @brief Will return the text of the specified menu item
				//! @param _itemID The ID of the menu item
				QString getMenuItemText(
					ak::ID								_itemID
				);

			signals:
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);
				void menuItemClicked(ak::ID);
				void menuItemCheckedChanged(ak::ID, bool);

			private slots:
				void slotClicked();
				void slotMenuItemClicked();
				void slotMenuItemCheckedChanged();

			private:

				//! @brief Initializes the components of this toolButton
				void ini(void);

				QMenu *								my_menu;
				std::vector<contextMenuItem *>		my_menuItems;
				qt::action *						my_action;

			};

		}
	}
}