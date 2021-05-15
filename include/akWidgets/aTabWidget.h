/*
 * ak_ui_qt_tabView.h
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
#include <akCore/globalDataTypes.h>
#include <akWidgets/aWidget.h>

// Qt header
#include <qtabwidget.h>
#include <qstring.h>
#include <qstringlist.h>

// C++ header
#include <vector>

// Forward declaration
class QWidget;
class QTabBar;

namespace ak {

	class UICORE_API_EXPORT aTabWidget : public QTabWidget, public aWidget {
		Q_OBJECT
	public:

		//! @brief Constructor
		//! @param _colorStyle The initial color style to set
		//! @param _parent The parent widget
		aTabWidget(
			aColorStyle *			_colorStyle = (aColorStyle *) nullptr,
			QWidget *				_parent = (QWidget *) nullptr
		);

		virtual ~aTabWidget();

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

		// #######################################################################################################

		// Setter

		//! @brief Will set the tab location of the tab view
		//! @param _location The location to set
		void setTabLocation(
			tabLocation			_location
		);

		void setCustomTabBar(
			QTabBar *							_tabBar
		);

		// #######################################################################################################

		// Getter

		QStringList tabTitles(void) const;

	};
} // namespace ak