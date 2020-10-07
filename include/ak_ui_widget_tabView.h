/*
 * ak_ui_widget_tabView.h
 *
 *  Created on: September 18, 2020
 *	Last modified on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// AK header
#include <ak_ui_core_aWidgetManager.h>		// Base class
#include <ak_globalDataTypes.h>				// UID and ID data types
#include <ak_ui_tabViewSignalLinker.h>		// tab event type
#include <ak_ui_core.h>						// tabLocation type

// Qt header
#include <qstring.h>
#include <qicon.h>

// C++ header
#include <vector>

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class colorStyle;
		class iconManager;
		class objectManager;
		class tabViewSignalLinker;
		namespace qt { class tabView; }

		namespace widget {

			class tabView : public ak::ui::core::aWidgetManager {
			public:
				tabView(
					iconManager *						_iconManager,
					messenger *							_messenger,
					uidManager *						_uidManager,
					objectManager *						_objectManager,
					colorStyle *						_colorStyle = (colorStyle *) nullptr
				);
				
				virtual ~tabView();

				// #######################################################################################################
				
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *				_colorStyle
				) override;

				// #######################################################################################################
				
				// Tab functions

				//! @brief Will send an event message
				//! @param _index The index of the tab page where the even occured
				//! @param _eventType The type of the event
				void tabEvent(
					int									_index,
					tabViewSignalLinker::tabEventType	_eventType
				);

				// #######################################################################################################

				// Setter

				//! @brief Will create a new tab page
				//! Returns the ID of the new tab
				//! @param _title The title of the new tab page
				ak::ID createTab(
					QWidget *							_widget,
					const QString &						_title,
					const QIcon &						_icon = QIcon()
				);

				//! @brief Will set the tab location of the tab view
				//! @param _location The location to set
				void setTabLocation(
					ak::ui::core::tabLocation			_location
				);

				//! @brief Will set the current focus on the specified tab
				//! @param _tab The tab to focus
				//! @throw ak::Exception if the tab is invalid
				void focusTab(
					ak::ID								_tab
				);

				//! @brief Will set the text of the specified tab
				//! @param _tab The tab to set the text at
				//! @param _text The text to set
				//! @throw ak::Exception if the specified tab is invalid
				void setTabText(
					ak::ID								_tab,
					const QString &						_text
				);

				//! @brief Will set the enabled state of this tabView
				//! @param _enabled If true it will be enabled
				void setEnabled(
					bool															_enabled
				);

				//! @brief Will set the visible state of this tabView
				//! @param _enabled If true it will be visible
				void setVisible(
					bool															_visible
				);

				// #######################################################################################################

				// Getter

				//! @brief Will return the tab count of this tabView
				int tabCount(void) const;

				//! @brief Will return the titles of all tabs in this tabView
				std::vector<QString> tabTitles(void) const;

				//! @brief Will return the currently focused tab
				ak::ID focusedTab(void) const;

				//! @brief Will get the text of the specified tab
				//! @param _tab The tab to get the text from
				//! @param _text The text to set
				//! @throw ak::Exception if the specified tab is invalid
				QString tabText(
					ak::ID								_tab
				) const;

			private:

				qt::tabView *					my_tabView;					//! The tab view this widget is managing
				tabViewSignalLinker *			my_tabViewSignalLinker;		//! The signal linker connecting the widgets callbacks to the event function

			};

		}
	}
}