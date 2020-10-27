/*
 * ak_ui_widget_tabView.h
 *
 *  Created on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aWidgetManager.h>		// Base class
#include <ak_ui_core_aRestorable.h>			// Base class
#include <ak_globalDataTypes.h>				// UID and ID data types
#include <ak_ui_core.h>						// tabLocation type

// Qt header
#include <qobject.h>
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

			class tabView : public QObject, public ui::core::aWidgetManager, public ui::core::aRestorable {
				Q_OBJECT
			public:
				tabView(
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
					const ak::ui::colorStyle *				_colorStyle
				) override;

				//! @brief Will set the alias for this object
				//! @param _alias The alias to set
				//! @throw ak::Exception if the provided alias length is 0
				virtual void setAlias(
					const QString &							_alias
				) override;

				//! @brief Will create a rapidjson::Value representing this objects current state
				//! The value looks like this:
				//!	     { "Alias":"[ObjectAlias]","Type":"[ObjectType]","Settings":{...} }
				virtual void addObjectSettingsToValue(
					rapidjson::Value &						_array,
					rapidjson::Document::AllocatorType &	_allocator
				) override;

				//! @brief Will restore the settings from the provided JSON value which must have an object type
				//! The value looks like this:
				//!	     { { "[SettingsName]":"[SettingsValue]",... } }
				//! @param _settings The settings to restore
				virtual void restoreSettings(
					const rapidjson::Value &				_settings
				) override;

				// #######################################################################################################

				// Setter

				//! @brief Will close the specified tab
				//! @param _tab The tab to close
				void closeTab(
					ak::ID								_tabID
				);

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

				//! @brief Will set the closeabled state for the tabs in this tabView
				//! @param _closeable If true the tabs can be closed by the user
				void setTabsCloseable(
					bool								_closeable
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

				//! @brief Will return the currently set tabs closeable state
				bool tabsCloseable(void) const;

				//! @brief Will get the text of the specified tab
				//! @param _tab The tab to get the text from
				//! @param _text The text to set
				//! @throw ak::Exception if the specified tab is invalid
				QString tabText(
					ak::ID								_tab
				) const;

			private slots:
				void slotCurrentTabChanged(int index);
				void slotItemClicked(int index);
				void slotItemDoubleClicked(int index);
				void slotItemCloseRequested(int index);

			private:

				qt::tabView *					my_tabView;					//! The tab view this widget is managing
			};

		}
	}
}