/*
 * ak_ui_widget_tabView.cpp
 *
 *  Created on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_widget_tabView.h>			// corresponding class
#include <ak_ui_qt_tabView.h>				// tabView
#include <ak_exception.h>					// Error handling
#include <ak_messenger.h>					// messenger
#include <ak_uidMangager.h>					// UID manager
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_objectManager.h>			// objectManager
#include <ak_ui_iconManager.h>				// iconManager

// Qt header
#include <qwidget.h>
#include <qtabwidget.h>

ak::ui::widget::tabView::tabView(
	messenger *					_messenger,
	uidManager *				_uidManager,
	objectManager *				_objectManager,
	colorStyle *				_colorStyle
) : ak::ui::core::aWidgetManager(ak::ui::core::objectType::oTabView, _messenger, _uidManager, _colorStyle),
my_tabView(nullptr)
{
	// Creatte the tabView
	my_tabView = new ak::ui::qt::tabView(_colorStyle);

	// Set color style
	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

	// Get id for myself
	my_uid = my_uidManager->getId();

	// Connect item signals
	connect(my_tabView, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)));
	connect(my_tabView, SIGNAL(tabBarClicked(int)), this, SLOT(slotItemClicked(int)));
	connect(my_tabView, SIGNAL(tabCloseRequested(int)), this, SLOT(slotItemCloseRequested(int)));
	connect(my_tabView, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(slotItemDoubleClicked(int)));
}

ak::ui::widget::tabView::~tabView() {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eDestroyed);

	disconnect(my_tabView, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)));
	disconnect(my_tabView, SIGNAL(tabBarClicked(int)), this, SLOT(slotItemClicked(int)));
	disconnect(my_tabView, SIGNAL(tabCloseRequested(int)), this, SLOT(slotItemCloseRequested(int)));
	disconnect(my_tabView, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(slotItemDoubleClicked(int)));

	if (my_tabView != nullptr) { delete my_tabView; my_tabView = nullptr; }
}

// #######################################################################################################

// Base class functions

QWidget * ak::ui::widget::tabView::widget(void) { return my_tabView->widget(); }

void ak::ui::widget::tabView::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_tabView->setColorStyle(_colorStyle);
}

// #######################################################################################################

// Setter

ak::ID ak::ui::widget::tabView::createTab(
	QWidget *									_widget,
	const QString &								_title,
	const QIcon &								_icon
) { return my_tabView->addTab(_widget, _icon, _title); }

void ak::ui::widget::tabView::setTabLocation(
	ak::ui::core::tabLocation			_location
) {
	switch (_location)
	{
	case ak::ui::core::tabLocation::tab_locationDown: my_tabView->setTabPosition(QTabWidget::TabPosition::South); break;
	case ak::ui::core::tabLocation::tab_locationLeft: my_tabView->setTabPosition(QTabWidget::TabPosition::West); break;
	case ak::ui::core::tabLocation::tab_locationRight: my_tabView->setTabPosition(QTabWidget::TabPosition::East); break;
	case ak::ui::core::tabLocation::tab_locationUp: my_tabView->setTabPosition(QTabWidget::TabPosition::North); break;
	default:
		break;
	}
}

void ak::ui::widget::tabView::focusTab(
	ak::ID								_tab
) {
	assert(_tab >= 0 && _tab < my_tabView->count()); // Tab index out of range
	my_tabView->setCurrentIndex(_tab);
}

void ak::ui::widget::tabView::setTabText(
	ak::ID								_tab,
	const QString &						_text
) {
	assert(_tab >= 0 && _tab < my_tabView->count()); // Tab index out of range
	my_tabView->setTabText(_tab, _text);
}

void ak::ui::widget::tabView::setEnabled(
	bool															_enabled
) { my_tabView->setEnabled(_enabled); }

void ak::ui::widget::tabView::setVisible(
	bool															_visible
) { my_tabView->setVisible(_visible); }

// #######################################################################################################

// Getter

int ak::ui::widget::tabView::tabCount(void) const { return my_tabView->count(); }

std::vector<QString> ak::ui::widget::tabView::tabTitles(void) const {
	std::vector<QString> ret;
	for (int i = 0; i < my_tabView->count(); i++) { ret.push_back(my_tabView->tabText(i)); }
	return ret;
}

ak::ID ak::ui::widget::tabView::focusedTab(void) const { return my_tabView->currentIndex(); }

QString ak::ui::widget::tabView::tabText(
	ak::ID								_tab
) const {
	assert(_tab >= 0 && _tab < my_tabView->count()); // Tab index out of range
	return my_tabView->tabText(_tab);
}

// #######################################################################################################

// Slots

void ak::ui::widget::tabView::slotCurrentTabChanged(int index) {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eChanged, index);
}

void ak::ui::widget::tabView::slotItemClicked(int index) {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eClicked, index);
}

void ak::ui::widget::tabView::slotItemDoubleClicked(int index) {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eDoubleClicked, index);
}

void ak::ui::widget::tabView::slotItemCloseRequested(int index) {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eClosing, index);
}