#pragma once

#include <qtimer.h>					// Base class
#include <ak_ui_core_aObject.h>		// Base class

namespace ak {
	namespace ui {
		namespace qt {

			class timer : public QTimer, public ak::ui::core::aObject {
			public:
				timer();
				virtual ~timer();


			private:

			};

		}
	}
}