/*
 * ak_ui_qt_table.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

 // Qt header
#include <qtablewidget.h>			// base class

// AK header
#include "ak_ui_core_aWidget.h"		// base class

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			//! @brief This class combines the functionallity of a QTableWidget and a sim::object
			class table : public QTableWidget, public ak::ui::core::aWidget
			{
				Q_OBJECT
			public:

				//! @brief Constructor
				//! @param _parent The parent QWidget for this table
				table(QWidget * _parent = (QWidget *) nullptr);

				//! @brief Constructor
				//! @param _rows The initial row count
				//! @param _columns The initial columns count
				//! @param _parent The parent QWidget for this table
				table(int _rows, int _columns, QWidget * _parent = (QWidget *) nullptr);

				//! @brief Deconstructor
				virtual ~table();

				// #######################################################################################################
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw sim::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

			};
		}
	}
}