/*
 * ak_ui_qt_dock.h
 *
 *  Created on: August 10, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// Qt header
#include <qdockwidget.h>			// base class
#include <qstring.h>				// QString

// AK header
#include <ak_ui_core_aWidget.h>		// base class
#include <ak_ui_core_aRestorable.h>	// base class
#include <ak_ui_core.h>				// objectType and keyType
#include <ak_globalDataTypes.h>		// UID and ID type

// Rapid JSON header
#include <rapidjson/document.h>		// RJSON document

// Forward declaration
class QKeyEvent;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			class dock : public QDockWidget, public ak::ui::core::aWidget, public ak::ui::core::aRestorable
			{
				Q_OBJECT
			public:
				dock(
					const QString &				_title = QString(""),
					ak::ui::colorStyle *		_colorStyle = nullptr,
					QWidget *					_parent = nullptr,
					Qt::WindowFlags				_flags = Qt::WindowFlags()
				);

				virtual ~dock();

				// #######################################################################################################

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				) override;
			
				//! @brief Will create a rapidjson::Value representing this objects current state
				//! The value looks like this:
				//!	     { "Alias":"[ObjectAlias]","Type":"[ObjectType]","Settings":{...} }
				virtual void addObjectSettingsToValue(
					rapidjson::Value &						_array,
					rapidjson::Document::AllocatorType &	_allocator
				) override;

			private slots:
				void slotDockLocationChanged(Qt::DockWidgetArea _area);

			private:

				ak::ui::core::dockLocation		my_location;

				// Block default constructor
				dock() = delete;

				// Copy constructor
				dock(const dock & _o) = delete;
			};

		} // namespace qt
	} // namespace ui
} // namespace ak