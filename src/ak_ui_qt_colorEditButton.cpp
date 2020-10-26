/*
 * ak_ui_widget_colorEditButton.cpp
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_colorEditButton.h>	// corresponding class
#include <ak_exception.h>					// error handling
#include <ak_messenger.h>					// messenger
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_qt_pushButton.h>			// pushButton
#include <ak_uidMangager.h>					// UID manager
#include <ak_ui_qt_graphicsView.h>

// Qt header
#include <qlayout.h>						// QHBoxLayout
#include <qcolordialog.h>					// QColorDialog

#define MY_OBJECTNAME "CentralWidget_ColorEditButton"

ak::ui::qt::colorEditButton::colorEditButton(
	const ak::ui::color &						_color,
	const QString &								_textOverride,
	ak::ui::colorStyle *						_colorStyle
)
	: ak::ui::core::aWidget(ak::ui::core::objectType::oColorEditButton),
	my_button(nullptr),
	my_layout(nullptr),
	my_view(nullptr)
{
	try {
		// Check arguments

		my_widget = new QWidget();
		my_widget->setContentsMargins(QMargins(0, 0, 0, 0));
		my_widget->setObjectName(MY_OBJECTNAME);

		// Create layout
		my_layout = new QHBoxLayout(my_widget);
		my_layout->setContentsMargins(QMargins(0, 0, 0, 0));

		// Create graphics view
		my_view = new ak::ui::qt::graphicsView();
		my_view->setHeightForWidthActive(true);
		my_view->setMaximumWidth(30);
		my_view->setMaximumHeight(30);
		
		// Create pushbutton
		my_button = new ak::ui::qt::pushButton();
		my_button->setContentsMargins(QMargins(0, 0, 0, 0));

		my_layout->addWidget(my_view);
		my_layout->addWidget(my_button);
		//my_layout->setStretch(1, 1);

		// Set the current color
		setColor(_color);

		// Override the color text if required
		if (_textOverride.length() > 0) { overrideText(_textOverride); }

		if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

		connect(my_button, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::colorEditButton::colorEditButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::colorEditButton::colorEditButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::colorEditButton::colorEditButton()"); }
}

ak::ui::qt::colorEditButton::~colorEditButton() {
	if (my_button != nullptr) { delete my_button; my_button = nullptr; }
	if (my_layout != nullptr) { delete my_layout; my_layout = nullptr; }
	if (my_view != nullptr) { delete my_view; my_view = nullptr; }
}

QWidget * ak::ui::qt::colorEditButton::widget(void) { return my_widget; }

void ak::ui::qt::colorEditButton::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	try {
		assert(_colorStyle != nullptr); // nullptr provided
		my_colorStyle = _colorStyle;
		my_button->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sPushButton));
	} 
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::colorEditButton::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::colorEditButton::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::colorEditButton::setColorStyle()"); }
}

// #############################################################################################################################

void ak::ui::qt::colorEditButton::setColor(
	const ak::ui::color &							_color
) {
	my_color = _color;
	my_view->setStyleSheet(my_color.toHexString(false, "background-color:#"));
	my_button->setText(my_color.toRGBString(","));
}

void ak::ui::qt::colorEditButton::SetEnabled(
	bool											_enabled
) { my_button->setEnabled(_enabled); }

void ak::ui::qt::colorEditButton::SetVisible(
	bool											_visible
) { my_button->setVisible(_visible); }

bool ak::ui::qt::colorEditButton::Enabled() const { return my_button->isEnabled(); }

ak::ui::color ak::ui::qt::colorEditButton::color(void) const { return my_color; }

void ak::ui::qt::colorEditButton::overrideText(
	const QString &								_text
) {
	my_button->setText(_text);
}

void ak::ui::qt::colorEditButton::fillBackground(
	const ui::color &			_color
) {
	QString sheet("#" MY_OBJECTNAME "{background-color:#");
	sheet.append(_color.toHexString(true));
	sheet.append(";}\n");
	my_widget->setAutoFillBackground(true);
	my_widget->setStyleSheet(sheet);
}

void ak::ui::qt::colorEditButton::slotButtonClicked() {
	// Show color dialog
	QColorDialog dia(my_color.toQColor());
	if (dia.exec() == 1) {
		ak::ui::color newColor(dia.currentColor());
		if (newColor != my_color) {
			setColor(newColor);
			// Send changed message
			emit changed();
		}
	}
}