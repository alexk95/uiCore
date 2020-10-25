/*
 * ak_ui_core_aDialog.h
 *
 *  Created on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_globalDataTypes.h>	// API type
#include <ak_ui_core.h>			// dialogResult type

// Qt header
#include <qdialog.h>			// Base class

// Forward declaration
class QWidget;

namespace ak {
	namespace ui {
		namespace core {

			class UICORE_API aDialog : public QDialog {
			public:
				//! @brief Constructor
				//! @param _parent The parent widget
				aDialog(QWidget * _parent = nullptr);

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