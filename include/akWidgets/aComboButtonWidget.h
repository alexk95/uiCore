/*
 *	File:		aComboButtonWidget.h
 *	Package:	akWidgets
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

 // Qt header
#include <qpushbutton.h>				// base class
#include <qstring.h>					// QString

// AK header
#include <akWidgets/aWidget.h>
#include <akCore/globalDataTypes.h>
#include <akCore/akCore.h>
#include <akWidgets/aComboButtonWidgetItem.h>
#include <akCore/aUidMangager.h>

// Forward declaration
class QMenu;
class QKeyEvent;

namespace ak {

	// Forward declaration
	class aColorStyle;

	//! @brief This class represents a combo button which consists of a QPushButton and it's Menu
	//! This class behaves like a combo box
	class UICORE_API_EXPORT aComboButtonWidget : public QPushButton, public aWidget
	{
		Q_OBJECT
	public:
		//! @brief Constructor
		//! @param _initialText The initial text of this combo button
		//! @param _colorStyle The color style used for this combo button
		//! @param _parent The parent QWidget for this combo button
		//! @throw ak::Exception if the creation of the menu failed
		aComboButtonWidget(
			const QString &									_initialText = QString(""),
			const aColorStyle *								_colorStyle = nullptr,
			QWidget *										_parent = (QWidget *) nullptr
		);

		//! @brief Deconstructor
		virtual ~aComboButtonWidget();

		// #######################################################################################################
		// Event handling

		//! @brief Emits a key pressend signal a key is pressed
		virtual void keyPressEvent(QKeyEvent * _event) override;

		//! @brief Emits a key released signal a key is released
		virtual void keyReleaseEvent(QKeyEvent * _event) override;

		// #######################################################################################################

		//! @brief Will remove all items from the combo button
		void clearItems(void);

		//! @brief Will set the items for the combo button
		//! @param _menu The items to set as the menu for this combo button
		//! @throw ak::Exception if the creation of a new item failed
		void setItems(
			const std::vector<QString> &					_menu
		);

		//! @brief Will add a new item to the combo button
		//! @param _other The new item to add
		//! @throw ak::Exception if the creation of the new item failed
		ID addItem(
			const QString &									_other
		);

		//! @brief Will return the ammount of items in the combo button
		int getItemCount(void) const;

		// #######################################################################################################
		// Base class functions

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *			_colorStyle
		) override;

	signals:
		void keyPressed(QKeyEvent *);
		void keyReleased(QKeyEvent *);
		void changed(void);

	private slots:
		void slotItemTriggered();

	private:
		QMenu *													my_menu;				//! The QMenu of this combo button
		aUidManager												my_itemsUIDmanager;		//! The UID manager used for the menu items of this combo button
		std::vector<aComboButtonWidgetItem *>					my_items;				//! The items of this combo button
		std::map<UID, size_t>								my_itemUids;			//! The map containing the index of a item UID

		aComboButtonWidget(aComboButtonWidget &) = delete;
		aComboButtonWidget & operator = (aComboButtonWidget &) = delete;

	};
}