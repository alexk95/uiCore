/*
 *	File:		aDockWidget.h
 *	Package:	akWidgets
 *
 *  Created on: August 10, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qdockwidget.h>			// base class
#include <qstring.h>				// QString

// AK header
#include <akCore/globalDataTypes.h>
#include <akCore/akCore.h>
#include <akWidgets/aWidget.h>
#include <akGui/aRestorable.h>		// base class

// Rapid JSON header
#include <rapidjson/document.h>		// RJSON document

// Forward declaration
class QKeyEvent;
class QCloseEvent;

namespace ak {

	class aDockWidget : public QDockWidget, public aWidget, public aRestorable
	{
		Q_OBJECT
	public:
		aDockWidget(
			const QString &							_title = QString(""),
			aColorStyle *					_colorStyle = nullptr,
			QWidget *								_parent = nullptr,
			Qt::WindowFlags							_flags = Qt::WindowFlags()
		);

		virtual ~aDockWidget();

		// #######################################################################################################

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			aColorStyle *					_colorStyle
		) override;

		//! @brief Will remove the child from this object (not destroy it)
		//! This function should be called from the deconstructor of a child
		virtual void removeChildObject(
			aObject *									_child
		) override;

		virtual void closeEvent(
			QCloseEvent *								_event
		) override;

		// #######################################################################################################

		//! @brief Will set the alias for this object
		//! @param _alias The alias to set
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

	signals:
		void closing();

	private slots:
		void slotDockLocationChanged(
			Qt::DockWidgetArea						_area
		);

	private:

		dockLocation		m_location;

		// Block default constructor
		aDockWidget() = delete;

		// Copy constructor
		aDockWidget(const aDockWidget & _o) = delete;
	};
} // namespace ak