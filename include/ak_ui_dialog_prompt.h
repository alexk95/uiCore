/*
 * ak_ui_dialog_prompt.h
 *
 *  Created on: October 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aDialog.h>		// base class
#include <ak_ui_core_aPaintable.h>	// base class
#include <ak_ui_core.h>				// promtType type

// Qt header
#include <qobject.h>				// QObject

// Forward declaration
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

namespace ak {
	namespace ui {

		namespace qt { class label; class pushButton; }

		namespace dialog {
			
			class prompt : public ui::core::aDialog, public ak::ui::core::aPaintable {
				Q_OBJECT
			public:
				prompt(
					const QString &				_message,
					const QString &				_title,
					ak::ui::core::promptType	_type,
					QWidget *					_parent = nullptr
				);

				virtual ~prompt();

				//! @brief Will set the current color style for this dialog
				virtual void setColorStyle(
					const ak::ui::colorStyle *	_colorStyle
				) override;

			private slots:
				void slotOk(void);
				void slotCancel(void);
				void slotYes(void);
				void slotNo(void);
				void slotRetry(void);
				void slotIgnore(void);

			private:

				QVBoxLayout *		my_layout;

				QHBoxLayout *		my_buttonsLayout;
				QWidget *			my_buttonsWidget;

				QHBoxLayout *		my_infoLayout;
				QWidget *			my_infoWidget;
				
				qt::label *			my_label;
				qt::pushButton *	my_button1;
				qt::pushButton *	my_button2;
				qt::pushButton *	my_button3;

			};

		} // namespace dialog
	} // namespace ui
} // namespace ak