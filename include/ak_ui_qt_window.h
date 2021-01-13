/*
 * ak_ui_qt_window.h
 *
 *  Created on: November 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aPaintable.h>		// Base class

// Qt header
#include <qmainwindow.h>				// Base class
#include <qwidget.h>
#include <qsize.h>

// C++ header
#include <map>

// Forward declaration
class QCloseEvent;
class QLabel;
class QResizeEvent;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;
		class windowEventHandler;

		namespace qt {

			class windowCentralWidget;

			class window : public QMainWindow, public ui::core::aPaintable {
				Q_OBJECT
			public:
				window();
				virtual ~window();

				// #######################################################################################################

				// Base class functions

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				virtual void setColorStyle(
					const ui::colorStyle *					_colorStyle
				) override;

				virtual void closeEvent(QCloseEvent * _event) override;

				// #######################################################################################################

				// Event handling

				//! @brief Will add the provided event handler to this window
				//! @param _eventHandler The event handler to add
				void addEventHandler(
					windowEventHandler *					_eventHandler
				);

				//! @brief Will remove the provided event handler from this window
				//! @param _eventHandler The event handler to remove
				void removeEventHandler(
					windowEventHandler *					_eventHandler
				);

				void SetCentralWidget(
					QWidget *								_widget
				);

				void setWaitingAnimationVisible(
					bool									_visible
				);

				void setWaitingAnimation(
					QMovie *							_movie
				);

			private:

				windowCentralWidget *						my_centralWidget;
				QLabel *									my_waitingWidget;

				std::map<windowEventHandler *, bool>		my_eventHandler;
				typedef std::map<windowEventHandler *,
					bool>::iterator							my_eventHandlerIterator;

			};

			// ###########################################################################################################################################################

			class windowCentralWidget : public QWidget {
			public:
				windowCentralWidget();
				virtual ~windowCentralWidget();

				void setChild(
					QWidget *		_widget
				);

				void setWaitingAnimationVisible(
					bool			_visible
				);

				void setWaitingAnimation(
					QMovie *							_movie
				);

			private:

				virtual void resizeEvent(QResizeEvent *event) override;

				QMovie *			my_waitingAnimation;
				QLabel *			my_waitingLabel;
				QWidget *			my_childWidget;
				QSize				my_waitingLabelSize;
				bool				my_waitingAnimationVisible;
			};

		}
	}
}
