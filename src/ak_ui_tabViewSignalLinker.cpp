/*
 * ak_ui_tabViewSignalLinker.h
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
#include <ak_ui_tabViewSignalLinker.h>		// Corresponding header
#include <ak_ui_qt_tabView.h>				// widget
#include <ak_ui_widget_tabView.h>			// manager
#include <ak_exception.h>					// Error handling

ak::ui::tabViewSignalLinker::tabViewSignalLinker(
	qt::tabView *			_widget,
	widget::tabView *		_manager
) : my_widget(nullptr), my_manager(nullptr)
{
	try {
		if (_widget == nullptr) { throw ak::Exception("Is nullptr", "Check widget"); }
		if (_manager == nullptr) { throw ak::Exception("Is nullptr", "Check manager"); }
		my_widget = _widget;
		my_manager = _manager;
		connect(my_widget, SIGNAL(currentChanged(int)), this, SLOT(slotItemCurrentChanged(int)));
		connect(my_widget, SIGNAL(tabBarClicked(int)), this, SLOT(slotItemClicked(int)));
		connect(my_widget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(slotItemDoubleClicked(int)));
		connect(my_widget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotItemCloseRequested(int)));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::tabViewSignalLinker::tabViewSignalLinker()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::tabViewSignalLinker::tabViewSignalLinker()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::tabViewSignalLinker::tabViewSignalLinker()"); }
}

ak::ui::tabViewSignalLinker::~tabViewSignalLinker() {
	disconnect(my_widget, SIGNAL(currentChanged(int)), this, SLOT(slotItemCurrentChanged(int)));
	disconnect(my_widget, SIGNAL(tabBarClicked(int)), this, SLOT(slotItemClicked(int)));
	disconnect(my_widget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(slotItemDoubleClicked(int)));
	disconnect(my_widget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotItemCloseRequested(int)));
}

void ak::ui::tabViewSignalLinker::slotItemCurrentChanged(int index) {
	my_manager->tabEvent(index, tabEventType::teCurrentChanged);
}

void ak::ui::tabViewSignalLinker::slotItemClicked(int index) {
	my_manager->tabEvent(index, tabEventType::teClicked);
}

void ak::ui::tabViewSignalLinker::slotItemDoubleClicked(int index) {
	my_manager->tabEvent(index, tabEventType::teDoubleClicked);
}

void ak::ui::tabViewSignalLinker::slotItemCloseRequested(int index) {
	my_manager->tabEvent(index, tabEventType::teCloseRequested);
}