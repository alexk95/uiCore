/*
 * ak_ui_tabViewSignalLinker.h
 *
 *  Created on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <qobject.h>			// Base class

namespace ak {
	namespace ui {

		// Forward declaration
		namespace qt { class tabView; }
		namespace widget { class tabView; }


		class tabViewSignalLinker : public QObject {
			Q_OBJECT
		public:

			enum tabEventType {
				teCurrentChanged,
				teClicked,
				teDoubleClicked,
				teCloseRequested
			};

			tabViewSignalLinker(
				qt::tabView *			_widget,
				widget::tabView *		_manager
			);

			virtual ~tabViewSignalLinker();

		private slots:
			void slotItemCurrentChanged(int index);
			void slotItemClicked(int index);
			void slotItemDoubleClicked(int index);
			void slotItemCloseRequested(int index);

		private:

			qt::tabView *				my_widget;
			widget::tabView *			my_manager;

			tabViewSignalLinker() = delete;
			tabViewSignalLinker(const tabViewSignalLinker &) = delete;
			tabViewSignalLinker operator = (const tabViewSignalLinker &) = delete;
		};

	}
}