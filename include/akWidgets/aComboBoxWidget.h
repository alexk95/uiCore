/*
 *	File:		aComboBoxWidget.h
 *	Package:	akWidgets
 *
 *  Created on: April 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

 // Qt header
#include <qcombobox.h>				// base class

// AK header
#include <akWidgets/aWidget.h>
#include <akCore/globalDataTypes.h>

// Forward declaration
class QKeyEvent;

namespace ak {

	//! @brief This class combines the functionallity of a QComboBox and a ak::ui::core::aObject
	class UICORE_API_EXPORT aComboBoxWidget : public QComboBox, public aWidget
	{
		Q_OBJECT
	public:
		//! @brief Constructor
		//! @param _parent The parent QWidget for this ComboBox
		aComboBoxWidget(QWidget * _parent = (QWidget *) nullptr);

		//! @brief Deconstructor
		virtual ~aComboBoxWidget();

		// #######################################################################################################
		// Event handling

		//! @brief Emits a key pressend signal a key is pressed
		virtual void keyPressEvent(QKeyEvent * _event) override;

		//! @brief Emits a key released signal a key is released
		virtual void keyReleaseEvent(QKeyEvent * _event) override;

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

	signals:
		void keyPressed(QKeyEvent *);
		void keyReleased(QKeyEvent *);

	};
} // namespace ak
