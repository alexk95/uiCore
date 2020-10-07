/*
 * ak_ui_dialog_prompt.cpp
 *
 *  Created on: October 07, 2020
 *	Last modified on: October 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// AK header
#include <ak_ui_core_aDialog.h>		// base class
#include <ak_ui_core.h>				// promtType type

// Qt header
#include <qobject.h>

namespace ak {
	namespace ui {
		namespace dialog {
			
			class propmt : public ui::core::aDialog {
				Q_OBJECT
			public:
				propmt();

			private:

			};

		} // namespace dialog
	} // namespace ui
} // namespace ak