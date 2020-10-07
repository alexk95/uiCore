/*
 * ak_ui_qt_timer.h
 *
 *  Created on: September 25, 2020
 *	Last modified on: September 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// Qt header
#include <qtimer.h>					// Base class

// AK header
#include <ak_ui_core_aObject.h>		// Base class

namespace ak {
	namespace ui {
		namespace qt {

			class timer : public QTimer, public ak::ui::core::aObject {
			public:
				timer();
				virtual ~timer();
			};

		}
	}
}