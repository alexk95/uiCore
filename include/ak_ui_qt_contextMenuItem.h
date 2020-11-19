/*
 * ak_ui_qt_contextMenuItem.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aObject.h>			// Base class

// Qt header
#include <qaction.h>				// Base class
#include <qstring.h>				// QString
#include <qicon.h>					// QIcon

namespace ak {
	namespace ui {
		namespace qt {

			class contextMenuItem : public QAction, public ui::core::aObject {
				Q_OBJECT
			public:
				contextMenuItem(
					const QString &				_text,
					ui::core::contextMenuRole	_role
				);

				contextMenuItem(
					const QIcon &				_icon,
					const QString &				_text,
					ui::core::contextMenuRole	_role
				);

				virtual ~contextMenuItem();

				ui::core::contextMenuRole role(void) const;

				void setId(
					ak::ID						_id
				);

				ak::ID id(void) const;

			private:

				ui::core::contextMenuRole		my_role;
				ak::ID							my_id;

				contextMenuItem() = delete;
				contextMenuItem(const contextMenuItem &) = delete;

			};

		}
	}
}
