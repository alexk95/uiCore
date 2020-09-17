/*
 * ak_ui_qt_action.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// Qt header
#include <qaction.h>					// base class
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon
#include <qtoolbutton.h>				// ToolButtonPopupMode

// AK header
#include "ak_ui_core_aObject.h"			// base class

namespace ak {
	namespace ui {
		namespace qt {

			//! @brief This class combines the functionallity of a QAction and a sim::aPaintable
			class action : public QAction, public ak::ui::core::aObject
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

				//! @brief Retuns the popup mode this Action is using
				QToolButton::ToolButtonPopupMode popupMode(void);

			private:
				QToolButton::ToolButtonPopupMode			my_popupMode;		//! The popup Mode of this Action
			};
		} // namespace qt
	} // namespace ui
} // namespace ak
