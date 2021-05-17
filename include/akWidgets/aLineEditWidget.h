/*
 *	File:		aLineEditWidget.h
 *	Package:	akWidgets
 *
 *  Created on: March 10, 2020
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
#include <qlineedit.h>				// Base class

// Forward declaration
class QKeyEvent;

namespace ak {

	//! @brief This class combines the functionallity of a QTextEdit and a ak::ui::core::aWidget
	class UICORE_API_EXPORT aLineEditWidget : public QLineEdit, public aWidget
	{
		Q_OBJECT
	public:
		//! @brief Constructor
		//! @param _parent The parent QWidget for this textEdit
		aLineEditWidget(QWidget * _parent = (QWidget *) nullptr);

		//! @brief Constructor
		//! @param _text The initial text for this text edit
		//! @param _parent The parent QWidget for this textEdit
		aLineEditWidget(const QString & _text, QWidget * _parent = (QWidget *) nullptr);

		//! @brief Deconstructor
		virtual ~aLineEditWidget();

		// #######################################################################################################
		// Event handling

		//! @brief Emits a returnPressed signal if the return key is pressed
		virtual void keyPressEvent(QKeyEvent * _event) override;

		virtual void keyReleaseEvent(QKeyEvent *event) override;

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

		void setErrorState(bool _error);

		//! @brief Will set wather the error state will have an impact on the foreground or the background
		//! @param _isForeground If true, the error state will change the text foreground color, otherwise back and foreground
		void setErrorStateIsForeground(bool _isForeground);

	signals:
		void keyPressed(QKeyEvent *);
		void keyReleased(QKeyEvent *);
		void finishedChanges();

	private slots:
		void slotEditingFinished();

	private:
		bool					my_isError;
		bool					my_errorIsForeground;
		QString					my_text;

		aLineEditWidget(const aLineEditWidget &) = delete;
		aLineEditWidget & operator = (const aLineEditWidget &) = delete;

	};
}