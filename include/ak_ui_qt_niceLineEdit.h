/*
 * ak_ui_widget_niceLineEdit.h
 *
 *  Created on: April 02, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <qwidget.h>

#include <ak_ui_core_aWidget.h>
#include <ak_globalDataTypes.h>

class QHBoxLayout;
class QKeyEvent;

namespace ak {
	namespace ui {

		namespace qt {
			class lineEdit;
			class label;

			class UICORE_API_EXPORT niceLineEdit : public QWidget, public ui::core::aWidget {
				Q_OBJECT
			public:
				niceLineEdit(const QString & _initialText, const QString & _infoLabelText);
				virtual ~niceLineEdit();

				// #######################################################################################################

				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				// #######################################################################################################

				void setText(const QString & _text);
				void setErrorState(bool _error);
				void setInfoLabelText(const QString & _text);

				QString text(void) const;
				QString infoLabelText(void) const;

			private slots:
				void slotCursorPositionChanged(int, int);
				void slotSelectionChanged();
				void slotKeyPressed(QKeyEvent * _event);
				void slotKeyReleased(QKeyEvent * _event);
				void slotEditingFinished(void);
				void slotTextChanged(const QString & _text);
				void slotReturnPressed();

			signals:
				void cursorPositionChanged(int, int);
				void selectionChanged();
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);
				void editingFinished();
				void textChanged(const QString & _text);
				void returnPressed();

			private:
				QHBoxLayout *			my_layout;
				qt::lineEdit *			my_lineEdit;
				qt::label *				my_label;
				QString					my_styleSheet;

			};

		}
	}
}