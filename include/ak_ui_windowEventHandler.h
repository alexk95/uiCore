/*
 * ak_ui_windowEventHandler.h
 *
 *  Created on: November 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_globalDataTypes.h>

namespace ak {
	namespace ui {

		class UICORE_API windowEventHandler {
		public:
			windowEventHandler() {}

			virtual ~windowEventHandler() {}

			// ###################################################################################

			// Event handling routines

			//! @brief This function will be called by the window when the closeEvent occurs
			//! If the return value is true the closeEvent will be acceptet otherwise ignored 
			virtual bool closeEvent() { return true; }

		};

	}
}