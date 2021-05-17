/*
 *	File:		aColorEditButtonWidget.h
 *	Package:	akWidgets
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qobject.h>						// base class
#include <qstring.h>


// AK header
#include <akWidgets/aWidget.h>
#include <akCore/globalDataTypes.h>
#include <akCore/akCore.h>
#include <akGui/aColor.h>

// Forward declaration
class QHBoxLayout;
class QWidget;

namespace ak {

	// Forward declaration
	class aMessenger;
	class aUidManager;
	class aObjectManager;
	class aIconManager;
	class aColorStyle;
	class aPushButtonWidget;
	class aGraphicsWidget;

	class UICORE_API_EXPORT aColorEditButtonWidget : public QObject, public aWidget {
		Q_OBJECT
	public:

		aColorEditButtonWidget(
			const aColor &		_color,
			const QString &				_textOverride = QString(""),
			aColorStyle *		_colorStyle = nullptr
		);

		virtual ~aColorEditButtonWidget();

		// #############################################################################################################################
		// Base class functions

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *	_colorStyle
		) override;

		// #############################################################################################################################

		//! @brief Will set the widgets
		void setColor(
			const aColor &		_color
		);

		//! @brief Will set the enabled state of this colorEditButton
		void SetEnabled(
			bool						_enabled = true
		);

		//! @brief Will set the visible state of this colorEditButton
		void SetVisible(
			bool						_visible = true
		);

		//! @brief Will return the enabled state of this colorEditButton
		bool Enabled() const;

		//! @brief Returns the currently set color
		aColor color(void) const;

		//! @brief Will override the currently set text to the text provided
		//! @param _text The text to set
		void overrideText(
			const QString &				_text
		);

		//! @brief Will fill the background for the current parent widget
		//! @param _color The color to set
		void fillBackground(
			const aColor &			_color
		);

		//! @brief Will apply the stylesheet to the pushButton of this colorEditButton
		//! @param _sheet The styleSheet to apply
		void setPushButtonStyleSheet(
			const QString &				_sheet
		);

	signals:
		void changed(void);

	private slots:
		void slotButtonClicked();

	private:

		aGraphicsWidget *		my_view;					//! Graphics view required to display the color
		aPushButtonWidget *		my_button;					//! The button to change the color
		QHBoxLayout *			my_layout;					//! The layout used to place the widgets

		aColor					my_color;					//! The currently set color

		QWidget *				my_widget;

		//! Block default constructor
		aColorEditButtonWidget() = delete;

	};

} // namespace ak