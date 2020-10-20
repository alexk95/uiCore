/*
 * ak_ui_qt_action.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// Qt header
#include <qwidgetaction.h>					// base class
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon
#include <qtoolbutton.h>				// ToolButtonPopupMode

// AK header
#include <ak_ui_core_aPaintable.h>			// base class

#include <ak_globalDataTypes.h>

class QWidget;

namespace ak {
	namespace ui {
		class colorStyle;

		namespace qt {

			//! @brief This class combines the functionallity of a QAction and a ak::ui::core::aPaintable
			class action : public QWidgetAction, public ak::ui::core::aPaintable
			{
				Q_OBJECT
			public:

				//! @brief Constructor
				//! @param _popupMode The popup mode used for this Action
				//! @param _parent The parent QWidget for this Action
				action(
					QToolButton::ToolButtonPopupMode _popupMode = QToolButton::ToolButtonPopupMode::InstantPopup,
					QObject * _parent = (QObject *) nullptr);

				//! @brief Constructor
				//! @param _text The initial text for this Action
				//! @param _popupMode The popup mode used for this Action
				//! @param _parent The parent QWidget for this Action
				action(
					const QString & _text,
					QToolButton::ToolButtonPopupMode _popupMode = QToolButton::ToolButtonPopupMode::InstantPopup,
					QObject * _parent = (QObject *) nullptr);

				//! @brief Constructor
				//! @param _icon The initial icon used for this Action
				//! @param _text The initial text for this Action
				//! @param _popupMode The popup mode used for this Action
				//! @param _parent The parent QWidget for this Action
				action(
					const QIcon & _icon,
					const QString & _text,
					QToolButton::ToolButtonPopupMode _popupMode = QToolButton::ToolButtonPopupMode::InstantPopup,
					QObject * _parent = (QObject *) nullptr);

				//! @brief Deconstructor
				virtual ~action();

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *					_colorStyle
				) override;

				virtual QWidget * createWidget(QWidget *parent) override;
				virtual void deleteWidget(QWidget *widget) override;

				// #######################################################################################################

				//! @brief Retuns the popup mode this Action is using
				QToolButton::ToolButtonPopupMode popupMode(void);

			private:
				QWidget *									my_widget;
				QToolButton::ToolButtonPopupMode			my_popupMode;		//! The popup Mode of this Action
			};
		} // namespace qt
	} // namespace ui
} // namespace ak
