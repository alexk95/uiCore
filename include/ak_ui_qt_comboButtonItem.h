/*
 * ak_ui_qt_comboButtonItem.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

 // Qt header
#include <qaction.h>				// base class
#include <qicon.h>					// QIcon
#include <qstring.h>				// QString

// AK header
#include "ak_ui_core_aObject.h"		// base class

namespace ak {
	namespace ui {
		namespace qt {

			//! @brief This class represents a menu item of a combo button
			class comboButtonItem : public QAction, public ak::ui::core::aObject
			{
				Q_OBJECT
			public:

				//! @brief Constructor
				//! @param _parent The parent QWidget for this combo button item
				comboButtonItem(
					QObject * _parent = (QObject *) nullptr);

				//! @brief Constructor
				//! @param _text The initial text of this combo button item
				//! @param _parent The parent QWidget for this combo button item
				comboButtonItem(
					const QString & _text,
					QObject * _parent = (QObject *) nullptr);

				//! @brief Constructor
				//! @param _icon The initial icon of this combo button item
				//! @param _text The initial text of this combo button item
				//! @param _parent The parent QWidget for this combo button item
				comboButtonItem(
					const QIcon & _icon,
					const QString & _text,
					QObject * _parent = (QObject *) nullptr);

				//! @brief Copy constructor
				//! @param _other The other combo button item
				comboButtonItem(
					const comboButtonItem &	_other
				);

				//! @brief Assignment operator
				//! @param _other The other combo button item
				comboButtonItem & operator = (const comboButtonItem & _other);

				//! @brief Deconstructor
				virtual ~comboButtonItem();
			};
		}
	}
}
