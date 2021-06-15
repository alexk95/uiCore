/*
 *	File:		aListWidget.h
 *	Package:	akWidgets
 *
 *  Created on: October 13, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

 // AK header
#include <akCore/globalDataTypes.h>
#include <akWidgets/aWidget.h>

// Qt header
#include <qlistwidget.h>			// base class (list, listItem)
#include <qstring.h>
#include <qicon.h>

// C++ header
#include <map>

class QMouseEvent;
class QKeyEvent;
class QEvent;

namespace ak {

	// Forward declaration
	class aListWidgetItem;

	class UICORE_API_EXPORT aListWidget : public QListWidget, public aWidget {
		Q_OBJECT
	public:
		// Constructor

		aListWidget();
		// Deconstructor
		virtual ~aListWidget();

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *			_colorStyle
		) override;

		virtual void keyPressEvent(QKeyEvent * _event) override;
		virtual void keyReleaseEvent(QKeyEvent * _event) override;
		virtual void mouseMoveEvent(QMouseEvent *) override;
		virtual void enterEvent(QEvent *) override;
		virtual void leaveEvent(QEvent *) override;

		// ###########################################################################################################################################

		//! @brief Will add a new item to this list
		//! @param _text The item text
		ID AddItem(
			const QString &				_text
		);

		//! @brief Will add a new item to this list
		//! @param _icon The item icon
		//! @param _text The item text
		ID AddItem(
			const QIcon &				_icon,
			const QString &				_text
		);

		//! @brief Will remove all items
		void Clear(void);

		void setVerticalScrollbarAlwaysVisible(
			bool						_vis
		);

		aListWidgetItem * Item(
			const QString &				_text
		);

		aListWidgetItem * Item(
			ID							_id
		);

		QString itemText(
			ID							_id
		);

		void setItemText(
			ID							_id,
			const QString &				_text
		);

		void setItemIcon(
			ID							_id,
			const QIcon &				_icon
		);

		void removeItem(
			ID							_id
		);

		//QString itemText
	signals:
		void keyPressed(QKeyEvent *);
		void keyReleased(QKeyEvent *);
		void mouseMove(QMouseEvent *);
		void leave(QEvent *);

	private:

		//! Will clear the memory allocated by this object
		void memFree(void);

		bool							my_verticalScrollbarAlwaysVisible;

		ID								my_currentId;
		std::map<ID, aListWidgetItem *>	my_items;

	}; // class list

	// ###########################################################################################################################################

	// ###########################################################################################################################################

	// ###########################################################################################################################################

	class aListWidgetItem : public QListWidgetItem {
	public:
		aListWidgetItem(
			ID					_id = invalidID,
			aListWidget *		_view = nullptr,
			int					_type = 0
		);

		aListWidgetItem(
			const QString &		_text,
			ID					_id = invalidID,
			aListWidget *		_view = nullptr,
			int					_type = 0
		);

		aListWidgetItem(
			const QIcon &		_icon,
			const QString &		_text,
			ID					_id = invalidID,
			aListWidget *		_view = nullptr,
			int					_type = 0
		);

		virtual ~aListWidgetItem();

		//! @brief Will set the items ID
		//! @param _id The ID to set
		void setId(
			ID				_id
		);

		//! @brief Returns the items ID
		ID id(void) const;

	private:
		ID					my_id;

		aListWidgetItem(const aListWidgetItem &) = delete;
		aListWidgetItem & operator = (const aListWidgetItem &) = delete;

	};	// class listItem
} // namespace ak