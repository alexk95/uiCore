/*
 * ak_ui_widget_tabView.cpp
 *
 *  Created on: September 18, 2020
 *	Last modified on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
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
#include <ak_ui_tabViewSignalLinker.h>		// signal linking


// Qt header
#include <qwidget.h>
#include <qtabwidget.h>

ak::ui::widget::tabView::tabView(
	iconManager *				_iconManager,
	messenger *					_messenger,
	uidManager *				_uidManager,
	objectManager *				_objectManager,
	colorStyle *				_colorStyle
) : ak::ui::core::aWidgetManager(ak::ui::core::objectType::oTabView, _messenger, _uidManager, _colorStyle),
	my_tabView(nullptr), my_tabViewSignalLinker(nullptr)
{
	try {
		// Creatte the tabView
		my_tabView = new ak::ui::qt::tabView(_colorStyle);
		if (my_tabView == nullptr) { throw ak::Exception("Failed to create", "Create tabView"); }
		
		// Set color style
		if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

		// Create signal linker
		my_tabViewSignalLinker = new ak::ui::tabViewSignalLinker(my_tabView, this);
		if (my_tabViewSignalLinker == nullptr) { throw ak::Exception("Failed to create", "Create signal linker"); }

		// Get id for myself
		my_uid = my_uidManager->getId();

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tabView::tabView()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tabView::tabView()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tabView::tabView()"); }
}

ak::ui::widget::tabView::~tabView() {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eDestroyed);
	if (my_tabView != nullptr) { delete my_tabView; my_tabView = nullptr; }
}

// #######################################################################################################

// Base class functions

QWidget * ak::ui::widget::tabView::widget(void) { return my_tabView->widget(); }

void ak::ui::widget::tabView::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) { 
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check colorStyle"); }
		my_tabView->setColorStyle(_colorStyle);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tabView::tabView()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tabView::tabView()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tabView::tabView()"); }
}

// #######################################################################################################

// Tab functions

void ak::ui::widget::tabView::tabEvent(
	int											_index,
	ak::ui::tabViewSignalLinker::tabEventType	_eventType
) {
	try {
		switch (_eventType)
		{
		case tabViewSignalLinker::tabEventType::teClicked:
			my_messenger->sendMessage(my_uid, ak::core::eventType::eClicked, _index, 0); break;
		case tabViewSignalLinker::tabEventType::teCloseRequested:
			my_messenger->sendMessage(my_uid, ak::core::eventType::eClosing, _index, 0); break;
		case tabViewSignalLinker::tabEventType::teCurrentChanged:
			my_messenger->sendMessage(my_uid, ak::core::eventType::eChanged, _index, 0); break;
		case tabViewSignalLinker::tabEventType::teDoubleClicked:
			my_messenger->sendMessage(my_uid, ak::core::eventType::eDoubleClicked, _index, 0); break;
		default:
			assert(0); // Not implemented yet
			break;
		}
	} catch (...) {
		assert(0); // Error handling not implemented
	}
}

// #######################################################################################################

// Setter

ak::ID ak::ui::widget::tabView::createTab(
	QWidget *									_widget,
	const QString &								_title,
	const QIcon &								_icon
) {
	try {
		return my_tabView->addTab(_widget, _icon, _title);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::tabView::createTab()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::tabView::createTab()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::tabView::createTab()"); }
}

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
	if (_tab < 0 || _tab >= my_tabView->count()) { throw ak::Exception("Tab index out of range", "ak::ui::widget::tabView::focusTab()"); }
	my_tabView->setCurrentIndex(_tab);
}

void ak::ui::widget::tabView::setTabText(
	ak::ID								_tab,
	const QString &						_text
) {
	if (_tab < 0 || _tab >= my_tabView->count()) { throw ak::Exception("Tab index out of range", "ak::ui::widget::tabView::focusTab()"); }
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
	if (_tab < 0 || _tab >= my_tabView->count()) { throw ak::Exception("Tab index out of range", "ak::ui::widget::tabView::focusTab()"); }
	return my_tabView->tabText(_tab);
}