/*
 * ak_ui_core_aDialog.h
 *
 *  Created on: October 06, 2020
 *	Last modified on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_ui_core.h>		// dialogResult type

// Qt header
#include <qdialog.h>

namespace ak {
	namespace ui {
		namespace core {

			class aDialog : public QDialog {
				Q_OBJECT
			public:
				//! @brief Constructor
				//! @param _parent The parent widget
				aDialog(QWidget * _parent);

				//! @brief Deconstructor
				virtual ~aDialog();

				//! @brief Will set the current result of this dialog
				void setResult(
					ui::core::dialogResult _result
				);

				//! @brief Will return the dialog result of this dialog
				ui::core::dialogResult result(void) const;

				//! @brief Will show this dialog an return the result
				ui::core::dialogResult showDialog(void);

			protected:
				ui::core::dialogResult			my_result;		//! The current result of this dialog
			};

		} // namespace core
	} // namespace ui
} // namespace ak