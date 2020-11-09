/*
 * ak_ui_qt_window.cpp
 *
 *  Created on: November 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_window.h>			// Corresponding header
#include <ak_ui_colorStyle.h>			// ColorStyle
#include <ak_ui_windowEventHandler.h>	// windowEventHandler

// Qt header
#include <qevent.h>
#include <qstatusbar.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::window::window()
	: QMainWindow(), ui::core::aPaintable(ui::core::objectType::oMainWindow)
{}

ak::ui::qt::window::~window() {}

// #######################################################################################################

// Base class functions

void ak::ui::qt::window::setColorStyle(
	const ui::colorStyle *					_colorStyle
) {
	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow));

	setStyleSheet(sheet);

	// Double paint to not mess up the tab toolbar
	sheet = my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow, "QMainWindow{", "}\n");
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow | TYPE_COLORAREA::caBackgroundColorWindow,
		"QTabBar{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caForegroundColorHeader,
		"QTabBar::tab{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorFocus | TYPE_COLORAREA::caForegroundColorFocus,
		"QTabBar::tab:hover{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorSelected | TYPE_COLORAREA::caForegroundColorSelected,
		"QTabBar::tab:selected{", "}"));
	setStyleSheet(sheet);

	statusBar()->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow));
	
}

void ak::ui::qt::window::closeEvent(QCloseEvent * _event) {
	for (auto handler : my_eventHandler) {
		if (!handler.first->closeEvent()) {
			_event->ignore();
			return;
		}
	}
	_event->accept();
	QMainWindow::closeEvent(_event);
}

// #######################################################################################################

// Event handling

void ak::ui::qt::window::addEventHandler(
	windowEventHandler *					_eventHandler
) {
	assert(my_eventHandler.find(_eventHandler) == my_eventHandler.end());	// Event handler already set
	my_eventHandler.insert_or_assign(_eventHandler, true);
}

void ak::ui::qt::window::removeEventHandler(
	windowEventHandler *					_eventHandler
) { my_eventHandler.erase(_eventHandler); }