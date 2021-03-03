/*
 * ak_ui_application.h
 *
 *  Created on: October 23, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qapplication.h>				// base class
#include <ak_globalDataTypes.h>

namespace ak {
	namespace ui {

		class UICORE_API_EXPORT application : public QApplication {
		public:
			application();
			virtual ~application();

		private:
			application(const application &) = delete;
		};

	}
}
