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
#include <qlabel.h>
#include <qlayout.h>							// QHBoxLayout, QVBoxLayout
#include <qpixmap.h>
#include <qbitmap.h>

ak::ui::widget::defaultWelcomeScreen::defaultWelcomeScreen(
	messenger *				_messenger,
	uidManager *			_uidManager,
	ak::ui::colorStyle *	_colorStyle
)
	: aWidgetManager(ak::ui::core::oDefaultWelcomeScreen, _messenger, _uidManager, _colorStyle)
{
	my_new.List = nullptr;
	my_new.Label = nullptr;
	my_open.List = nullptr;
	my_open.Label = nullptr;
	my_recents.List = nullptr;
	my_recents.Label = nullptr;


	// Initialize screen data
	my_screen.Central = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.New = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.Open = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.OpenNewCentral = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.Recents = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.RecentsCentralDummy = toLayoutWidgetCombo(nullptr, nullptr);

	// Create central widget
	my_screen.Central = toLayoutWidgetCombo(new QHBoxLayout, new QWidget);
	
	// Create recents object
	my_screen.RecentsCentralDummy = toLayoutWidgetCombo(new QHBoxLayout, new QWidget);
	my_screen.Central.layout->addWidget(my_screen.RecentsCentralDummy.widget);
	my_screen.Recents = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.RecentsCentralDummy.layout->addWidget(my_screen.Recents.widget);
	
	my_recents.List = new ui::qt::list;
	my_recents.List->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	my_recents.Label = new QLabel("Recent");
	my_recents.Label->setBuddy(my_recents.List);

	my_screen.Recents.layout->addWidget(my_recents.Label);
	my_screen.Recents.layout->addWidget(my_recents.List);

	// Create open new 
	my_screen.OpenNewCentral = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.Central.layout->addWidget(my_screen.OpenNewCentral.widget);

	// Create open
	my_screen.Open = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.OpenNewCentral.layout->addWidget(my_screen.Open.widget);

	my_open.List = new ui::qt::list;
	my_open.List->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	my_open.Label = new QLabel("Open");
	my_open.Label->setBuddy(my_open.List);

	my_screen.Open.layout->addWidget(my_open.Label);
	my_screen.Open.layout->addWidget(my_open.List);

	// Create new
	my_screen.New = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.OpenNewCentral.layout->addWidget(my_screen.New.widget);

	my_new.List = new ui::qt::list;
	my_new.List->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	my_new.Label = new QLabel("New");
	my_new.Label->setBuddy(my_new.List);

	my_screen.New.layout->addWidget(my_new.Label);
	my_screen.New.layout->addWidget(my_new.List);

	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

}

ak::ui::widget::defaultWelcomeScreen::~defaultWelcomeScreen() { memFree(); }

QWidget * ak::ui::widget::defaultWelcomeScreen::widget(void) { return my_screen.Central.widget; }

void ak::ui::widget::defaultWelcomeScreen::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {

	assert(_colorStyle != nullptr); // Nullptr provided

	QString sheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sWidget));
	if (my_screen.Central.widget != nullptr) { my_screen.Central.widget->setStyleSheet(sheet); }
	if (my_screen.New.widget != nullptr) { my_screen.New.widget->setStyleSheet(sheet); }
	if (my_screen.Open.widget != nullptr) { my_screen.Open.widget->setStyleSheet(sheet); }
	if (my_screen.OpenNewCentral.widget != nullptr) { my_screen.OpenNewCentral.widget->setStyleSheet(sheet); }
	if (my_screen.Recents.widget != nullptr) { my_screen.Recents.widget->setStyleSheet(sheet); }

	sheet = my_colorStyle->getStylesheet(colorStyle::styleableObject::sList);
	if (my_new.List != nullptr) { my_new.List->setStyleSheet(sheet); }
	if (my_open.List != nullptr) { my_new.List->setStyleSheet(sheet); }
	if (my_recents.List != nullptr) { my_new.List->setStyleSheet(sheet); }

	sheet = my_colorStyle->getStylesheet(colorStyle::styleableObject::sLabel);
	if (my_new.Label != nullptr) { my_new.Label->setStyleSheet(sheet); }
	if (my_open.Label != nullptr) { my_new.Label->setStyleSheet(sheet); }
	if (my_recents.Label != nullptr) { my_new.Label->setStyleSheet(sheet); }

}

ak::ID ak::ui::widget::defaultWelcomeScreen::addRecent(
	const QString &					_text
) { return my_recents.List->AddItem(_text); }

ak::ID ak::ui::widget::defaultWelcomeScreen::addRecent(
	const QIcon &					_icon,
	const QString &					_text
) { return my_recents.List->AddItem(_icon, _text); }

ak::ID ak::ui::widget::defaultWelcomeScreen::addOpen(
	const QIcon &					_icon,
	const QString &					_text
) { return my_open.List->AddItem(_icon, _text); }

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

	memFree(my_new);
	memFree(my_open);
	memFree(my_recents);

	memFree(my_screen.Central);
	memFree(my_screen.New);
	memFree(my_screen.Open);
	memFree(my_screen.OpenNewCentral);
	memFree(my_screen.Recents);
}

void ak::ui::widget::defaultWelcomeScreen::memFree(
	structLayoutWidget &		_layoutWidget
) {
	if (_layoutWidget.layout != nullptr) { delete _layoutWidget.layout; _layoutWidget.layout = nullptr; }
	if (_layoutWidget.widget != nullptr) { delete _layoutWidget.widget; _layoutWidget.widget = nullptr; }
}

void ak::ui::widget::defaultWelcomeScreen::memFree(
	structEntries &				_entry
) {
	if (_entry.List != nullptr) { _entry.List->Clear(); delete _entry.List; _entry.List = nullptr; }
	if (_entry.Label != nullptr) { delete _entry.Label; _entry.Label = nullptr; }
}

ak::ui::widget::defaultWelcomeScreen::structLayoutWidget ak::ui::widget::defaultWelcomeScreen::toLayoutWidgetCombo(
	QLayout *					_layout,
	QWidget *					_widget
) {
	structLayoutWidget ret;
	ret.layout = _layout;
	ret.widget = _widget;
	_widget->setLayout(_layout);
	return ret;
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