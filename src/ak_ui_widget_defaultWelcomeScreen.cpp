/*
 * ak_ui_widget_defaultWelcomeScreen.cpp
 *
 *  Created on: October 09, 2020
 *	Last modified on: October 13, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_widget_defaultWelcomeScreen.h>	// Corresponding header
#include <ak_messenger.h>						// messenger
#include <ak_uidMangager.h>						// UID manager
#include <ak_ui_colorStyle.h>					// color style
#include <ak_ui_qt_list.h>

// Qt header
#include <qwidget.h>							// QWidget
#include <qlayout.h>							// QHBoxLayout, QVBoxLayout

ak::ui::widget::defaultWelcomeScreen::defaultWelcomeScreen(
	messenger *				_messenger,
	uidManager *			_uidManager,
	ak::ui::colorStyle *	_colorStyle
)
	: aWidgetManager(ak::ui::core::oDefaultWelcomeScreen, _messenger, _uidManager, _colorStyle),
	my_centralLayout(nullptr), my_centralWidget(nullptr), my_listRecents(nullptr)
{
	// Create central widget
	my_centralWidget = new QWidget;
	my_centralLayout = new QVBoxLayout(my_centralWidget);
	
	// Create recents list
	my_listRecents = new ui::qt::list;
	my_listRecents->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

	my_centralLayout->addWidget(my_listRecents);

	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

}

ak::ui::widget::defaultWelcomeScreen::~defaultWelcomeScreen() { memFree(); }

QWidget * ak::ui::widget::defaultWelcomeScreen::widget(void) { return my_centralWidget; }

void ak::ui::widget::defaultWelcomeScreen::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // Nullptr provided

	my_centralWidget->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sWidget));

}

ak::ID ak::ui::widget::defaultWelcomeScreen::addRecent(
	const QString &					_text
) { return my_listRecents->AddItem(_text); }

ak::ID ak::ui::widget::defaultWelcomeScreen::addRecent(
	const QIcon &					_icon,
	const QString &					_text
) { return my_listRecents->AddItem(_icon, _text); }

// #############################################################################################################

				// Event handling

void ak::ui::widget::defaultWelcomeScreen::handleEventRecents(
	ak::ID							_item,
	ak::core::eventType				_event
) {

}

// ##############################################################################################
// Private functions

void ak::ui::widget::defaultWelcomeScreen::memFree(void) {

	if (my_listRecents != nullptr) {
		my_listRecents->Clear();
		delete my_listRecents; my_listRecents = nullptr;
	}

	if (my_centralLayout != nullptr) { delete my_centralLayout; my_centralLayout = nullptr; }
	if (my_centralWidget != nullptr) { delete my_centralWidget; my_centralWidget = nullptr; }
}

// ########################################################################################

// ****************************************************************************************

// ########################################################################################

ak::ui::defaultWelcomeScreenSignalLinker::defaultWelcomeScreenSignalLinker(
	widget::defaultWelcomeScreen *	_screen
) {
	assert(_screen != nullptr);
	my_screen = _screen;
}

ak::ui::defaultWelcomeScreenSignalLinker::~defaultWelcomeScreenSignalLinker() {

}

void ak::ui::defaultWelcomeScreenSignalLinker::addLink(
	qt::list *						_object
) {
	assert(_object != nullptr);
}