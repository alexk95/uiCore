/*
 * ak_ui_widget_defaultWelcomeScreen.h
 *
 *  Created on: October 09, 2020
 *	Last modified on: October 13, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aWidgetManager.h>	// base class (welcomeScreen)
#include <ak_core.h>					// eventType

// Qt header
#include <qobject.h>					// base class (signalLinker)
#include <qicon.h>

// C++ header
#include <vector>

// Forward declaration
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QListWidget;
class QListWidgetItem;

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class colorStyle;
		class defaultWelcomeScreenSignalLinker;
		namespace qt { class list; }

		namespace widget {

			class defaultWelcomeScreen : public ui::core::aWidgetManager {
			public:
				defaultWelcomeScreen(
					ak::messenger *			_messenger,
					ak::uidManager *		_uidManager,
					ak::ui::colorStyle *	_colorStyle = nullptr

				);
				virtual ~defaultWelcomeScreen();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				) override;

				// #############################################################################################################

				ak::ID addRecent(
					const QString &					_text
				);

				ak::ID addRecent(
					const QIcon &					_icon,
					const QString &					_text
				);

				// #############################################################################################################

				// Event handling

				void handleEventRecents(
					ak::ID							_item,
					ak::core::eventType				_event
				);

			private:

				void memFree(void);

				QVBoxLayout *						my_centralLayout;
				QWidget *							my_centralWidget;
				ui::qt::list *						my_listRecents;

				defaultWelcomeScreenSignalLinker *	my_signalLinker;

			}; // class defaultWelcomeScreen;
		} // namespace widget

		// ########################################################################################

		// ****************************************************************************************

		// ########################################################################################

		class defaultWelcomeScreenSignalLinker : public QObject {
			Q_OBJECT
		public:

			defaultWelcomeScreenSignalLinker(
				widget::defaultWelcomeScreen *	_screen
			);

			virtual ~defaultWelcomeScreenSignalLinker();

			void addLink(
				qt::list *						_object
			);

			// ########################################################################################

		private slots:


		private:

			widget::defaultWelcomeScreen *		my_screen;

			defaultWelcomeScreenSignalLinker() = delete;
			defaultWelcomeScreenSignalLinker(const defaultWelcomeScreenSignalLinker &) = delete;
			defaultWelcomeScreenSignalLinker & operator = (const defaultWelcomeScreenSignalLinker &) = delete;

		};

	}
}
