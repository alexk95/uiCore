/*
 * ak_ui_widget_colorEditButton.cpp
 *
 *  Created on: August 05, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include "ak_ui_widget_colorEditButton.h"	// corresponding class
#include "ak_exception.h"					// error handling
#include "ak_messenger.h"					// messenger
#include "ak_ui_signalLinker.h"				// signalLinker
#include "ak_ui_objectManager.h"			// objectManager
#include "ak_notifierColorEditButton.h"		// notifierColorEditButton
#include "ak_ui_colorStyle.h"				// colorStyle
#include "ak_ui_qt_pushButton.h"			// pushButton
#include "ak_uidMangager.h"					// UID manager

// Qt header
#include <qlayout.h>						// QHBoxLayout
#include <qgraphicsview.h>					// QGraphicsView
#include <qcolordialog.h>					// QColorDialog

ak::ui::widget::colorEditButton::colorEditButton(
	ak::messenger *								_messenger,
	ak::uidManager *							_uidManager,
	const ak::ui::color &						_color,
	ak::ui::iconManager *						_iconManager,
	const QString &								_textOverride,
	ak::ui::colorStyle *						_colorStyle
)
	: ak::ui::core::aWidgetManager(ak::ui::core::objectType::oColorEditButton, _iconManager),
	my_button(nullptr),
	my_layout(nullptr),
	my_externalMessenger(nullptr),
	my_notifier(nullptr),
	my_externalUidManager(nullptr),
	my_view(nullptr),
	my_widget(nullptr)
{
	try {
		// Check arguments
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger", ak::Exception::exceptionType::Nullptr); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager", ak::Exception::exceptionType::Nullptr); }

		// Apply arguments
		my_externalMessenger = _messenger;
		my_externalUidManager = _uidManager;

		my_notifier = new ak::notifierColorEditButton(this);

		// Create graphics view
		my_view = new QGraphicsView();
		my_view->setMaximumWidth(25);
		my_view->setMaximumHeight(25);
		my_view->setAutoFillBackground(true);
		my_view->setContentsMargins(QMargins(3, 3, 3, 3));

		// Create pushbutton
		my_button = new ak::ui::qt::pushButton();
		my_button->setContentsMargins(QMargins(0, 0, 0, 0));
		my_button->setMinimumWidth(50);

		if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }
		
		my_button->setUid(my_uidManager->getId());

		// Create layout
		my_layout = new QHBoxLayout();
		my_layout->setSpacing(0);
		my_layout->setContentsMargins(QMargins(1, 1, 1, 1));
		my_layout->addWidget(my_view, 20);
		my_layout->addWidget(my_button, 80);
		my_layout->setStretch(0, 2);
		my_layout->setStretch(1, 8);

		// Create main widget
		my_widget = new QWidget();
		my_widget->setLayout(my_layout);

		// Set the current color
		setColor(_color);

		// Override the color text if required
		if (_textOverride.length() > 0) { overrideText(_textOverride); }

		// Register receiver for the button
		my_messenger->registerReceiver(my_button->uid(), ak::core::messageType::mEvent, my_notifier);

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::colorEditButton::colorEditButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::colorEditButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::colorEditButton()"); }
}

ak::ui::widget::colorEditButton::~colorEditButton() {
	if (my_button != nullptr) { delete my_button; my_button = nullptr; }
	if (my_layout != nullptr) { delete my_layout; my_layout = nullptr; }
	if (my_notifier != nullptr) { delete my_notifier; my_notifier = nullptr; }
	if (my_view != nullptr) { delete my_view; my_view = nullptr; }
	if (my_widget != nullptr) { delete my_widget; my_widget = nullptr; }

	if (my_messenger != nullptr) {
		my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eDestroyed);
	}
}

QWidget * ak::ui::widget::colorEditButton::widget(void) { return my_widget; }

void ak::ui::widget::colorEditButton::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check colorStyle"); }
		my_colorStyle = _colorStyle;
		my_button->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sPushButton));
	} 
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::colorEditButton::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::setColorStyle()"); }
}

// #############################################################################################################################

ak::ui::color ak::ui::widget::colorEditButton::color(void) const { return my_color; }

void ak::ui::widget::colorEditButton::setColor(
	const ak::ui::color &							_color
) {
	try {
		my_notifier->disable();

		// Apply new color
		my_color = _color;
		my_view->setStyleSheet(my_color.toHexString(false, "background-color:#"));
		my_button->setText(my_color.toRGBString(","));

		my_notifier->enable();
	}
	catch (const ak::Exception & e) { my_notifier->enable(); throw ak::Exception(e, "ak::ui::widget::colorEditButton::setColor()"); }
	catch (const std::exception & e) { my_notifier->enable(); throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::setColor()"); }
	catch (...) { my_notifier->enable(); throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::setColor()"); }
}

void ak::ui::widget::colorEditButton::overrideText(
	const QString &								_text
) {
	try {
		my_notifier->disable();
		my_button->setText(_text);
		my_notifier->enable();
	}
	catch (const ak::Exception & e) { my_notifier->enable(); throw ak::Exception(e, "ak::ui::widget::colorEditButton::setColor()"); }
	catch (const std::exception & e) { my_notifier->enable(); throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::setColor()"); }
	catch (...) { my_notifier->enable(); throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::setColor()"); }
}

void ak::ui::widget::colorEditButton::widgetEvent(
	ak::UID										_sender,
	ak::core::eventType							_eventType,
	int											_info1,
	int											_info2
) {
	try {
		if (_sender == my_button->uid()) {
			if (_eventType == ak::core::eventType::eClicked) {
				// Show color dialog
				QColor c = QColorDialog::getColor(my_color.toQColor());
				ak::ui::color newColor(c);
				if (newColor != my_color) {
					setColor(my_color);
					// Send changed message
					my_messenger->sendMessage(my_uid, ak::core::messageType::mEvent, ak::core::eventType::eChanged);
				}
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::colorEditButton::widgetEvent()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::widgetEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::widgetEvent()"); }
}