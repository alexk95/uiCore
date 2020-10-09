/*
 * ak_ui_widget_colorEditButton.cpp
 *
 *  Created on: August 05, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_widget_colorEditButton.h>	// corresponding class
#include <ak_exception.h>					// error handling
#include <ak_messenger.h>					// messenger
#include <ak_ui_signalLinker.h>				// signalLinker
#include <ak_ui_objectManager.h>			// objectManager
#include <ak_notifierColorEditButton.h>		// notifierColorEditButton
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_qt_pushButton.h>			// pushButton
#include <ak_uidMangager.h>					// UID manager

// Qt header
#include <qlayout.h>						// QHBoxLayout
#include <qgraphicsview.h>					// QGraphicsView
#include <qcolordialog.h>					// QColorDialog

ak::ui::widget::colorEditButton::colorEditButton(
	ak::messenger *								_messenger,
	ak::uidManager *							_uidManager,
	const ak::ui::color &						_color,
	const QString &								_textOverride,
	ak::ui::colorStyle *						_colorStyle
)
	: ak::ui::core::aWidgetManager(ak::ui::core::objectType::oColorEditButton),
	my_button(nullptr),
	my_layout(nullptr),
	my_externalMessenger(nullptr),
	my_notifier(nullptr),
	my_externalUidManager(nullptr),
	my_view(nullptr),
	my_widget(nullptr),
	my_signalLinker(nullptr)
{
	try {
		my_signalLinker = new ui::signalLinker(my_messenger, my_uidManager);

		// Check arguments
		assert(_messenger != nullptr); // Is nullptr
		assert(_uidManager != nullptr); // Is nullptr

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

		my_signalLinker->addLink(my_button);

		// Create main widget
		my_widget = new QWidget();

		// Create layout
		my_layout = new QHBoxLayout(my_widget);
		my_layout->setSpacing(0);
		my_layout->setContentsMargins(QMargins(1, 1, 1, 1));
		my_layout->addWidget(my_view, 20);
		my_layout->addWidget(my_button, 80);
		my_layout->setStretch(0, 2);
		my_layout->setStretch(1, 8);

		// Set the current color
		setColor(_color);

		// Override the color text if required
		if (_textOverride.length() > 0) { overrideText(_textOverride); }

		if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

		// Register receiver for the button
		my_messenger->registerUidReceiver(my_button->uid(), my_notifier);

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::colorEditButton::colorEditButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::colorEditButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::colorEditButton()"); }
}

ak::ui::widget::colorEditButton::~colorEditButton() {
	aWidgetManager::memFree();

	if (my_button != nullptr) { delete my_button; my_button = nullptr; }
	if (my_layout != nullptr) { delete my_layout; my_layout = nullptr; }
	if (my_notifier != nullptr) { delete my_notifier; my_notifier = nullptr; }
	if (my_view != nullptr) { delete my_view; my_view = nullptr; }
	if (my_widget != nullptr) { delete my_widget; my_widget = nullptr; }

	if (my_messenger != nullptr) { my_messenger->sendMessage(my_uid, ak::core::eventType::eDestroyed); }
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

void ak::ui::widget::colorEditButton::setEnabled(
	bool											_enabled
) { my_button->setEnabled(_enabled); }

void ak::ui::widget::colorEditButton::setVisible(
	bool											_visible
) { my_button->setVisible(_visible); }

bool ak::ui::widget::colorEditButton::enabled() const { return my_button->isEnabled(); }

ak::ui::color ak::ui::widget::colorEditButton::color(void) const { return my_color; }

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
					setColor(newColor);
					// Send changed message
					my_externalMessenger->sendMessage(my_uid, ak::core::eventType::eChanged);
				}
			} else if (_eventType == ak::core::eKeyPressed || _eventType == ak::core::eKeyReleased) {
				my_externalMessenger->sendMessage(my_uid, _eventType, _info1, _info2);
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::colorEditButton::widgetEvent()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::widgetEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::widgetEvent()"); }
}