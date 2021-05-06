/*
 * ak_ui_widget_welcomeScreen.h
 *
 *  Created on: October 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

/*

// AK header
#include <ak_ui_core_aWidget.h>			// base class (welcomeScreen)
#include <ak_core.h>					// eventType
#include <ak_globalDataTypes.h>			// ak::ID and ak::UID types

// Qt header
#include <qobject.h>					// base class (signalLinker)
#include <qicon.h>

// C++ header
#include <vector>

// Forward declaration
class QWidget;
class QLabel;
class QLayout;
class QListWidget;
class QListWidgetItem;

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class colorStyle;
		
		namespace qt {
			class list; class listItem;

			class UICORE_API_EXPORT welcomeScreen : public QObject, public ui::core::aWidget {
				Q_OBJECT
			public:
				welcomeScreen(
					ak::ui::colorStyle *	_colorStyle = nullptr

				);
				virtual ~welcomeScreen();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				// #############################################################################################################

				//! @brief Will add a new item at the specified group
				//! @param _group The group to add the item at (0: Recents, 1: Open, 2: New)
				//! @param _text The text of the new item
				ak::ID addItem(
					ak::ID							_group,
					const QString &					_text
				);

				//! @brief Will add a new item at the specified group
				//! @param _group The group to add the item at (0: Recents, 1: Open, 2: New)
				//! @param _icon The icon of the new item
				//! @param _text The text of the new item
				ak::ID addItem(
					ak::ID							_group,
					const QIcon &					_icon,
					const QString &					_text
				);

				//! @brief Will add a new item at the  group
				//! @param _text The text of the new item
				ak::ID addItemAtRecents(
					const QString &					_text
				);

				//! @brief Will add a new item at the  group
				//! @param _icon The icon of the new item
				//! @param _text The text of the new item
				ak::ID addItemAtRecents(
					const QIcon &					_icon,
					const QString &					_text
				);

				//! @brief Will add a new item at the  group
				//! @param _text The text of the new item
				ak::ID addItemAtOpen(
					const QString &					_text
				);

				//! @brief Will add a new item at the  group
				//! @param _icon The icon of the new item
				//! @param _text The text of the new item
				ak::ID addItemAtOpen(
					const QIcon &					_icon,
					const QString &					_text
				);

				//! @brief Will add a new item at the  group
				//! @param _text The text of the new item
				ak::ID addItemAtNew(
					const QString &					_text
				);

				//! @brief Will add a new item at the  group
				//! @param _icon The icon of the new item
				//! @param _text The text of the new item
				ak::ID addItemAtNew(
					const QIcon &					_icon,
					const QString &					_text
				);

				//! @brief Will remove all items from all groups
				void clear(void);

				//! @brief Will remove all items from the specified group
				//! @param _group The group to remove all items from
				void clear(
					ID							_group
				);

				QString itemText(
					ID							_group,
					ID							_item
				);

				QString itemTextAtRecent(
					ID							_item
				);

				QString itemTextAtOpen(
					ID							_item
				);

				QString itemTextAtNew(
					ID							_item
				);

				void setItemText(
					ID							_group,
					ID							_item,
					const QString &				_text
				);

				void setItemIcon(
					ID							_group,
					ID							_item,
					const QIcon &				_icon
				);

				static QString groupName(
					ID							_group
				);

				void setObjectName(
					const QString &				_name
				);

				QString objectName(void) const;

				// #############################################################################################################

				// Event handling

				void handleEvent(
					UID							_group,
					ID							_item,
					ak::core::eventType			_event
				);

			signals:


			private slots:
				void slotItemClicked(QListWidgetItem *);
				void slotItemDoubleClicked(QListWidgetItem *);

			private:

				void memFree(void);

				struct structLayoutWidget {
					QLayout *					layout;
					QWidget *					widget;
				};

				struct structLayout
				{
					structLayoutWidget			Central;
					structLayoutWidget			Recents;
					structLayoutWidget			RecentsCentralDummy;
					structLayoutWidget			OpenNewCentral;
					structLayoutWidget			Open;
					structLayoutWidget			New;
				};

				struct structEntries {
					QLabel *					Label;
					ui::qt::list *				List;
				};

				void memFree(
					structLayoutWidget &		_layoutWidget
				);

				void memFree(
					structEntries &				_entry
				);

				static structLayoutWidget toLayoutWidgetCombo(
					QLayout *					_layout,
					QWidget *					_widget
				);

				structLayout						my_screen;

				structEntries						my_recents;
				structEntries						my_open;
				structEntries						my_new;

				int									my_headerLabelTextSize;

			}; // class defaultWelcomeScreen;
		} // namespace qt

	}
}

*/
