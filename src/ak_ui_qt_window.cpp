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
#include <qwidget.h>
#include <qframe.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qmovie.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag


ak::ui::qt::window::window()
	: QMainWindow(), ui::core::aPaintable(ui::core::objectType::oMainWindow), my_waitingWidget(nullptr), my_centralWidget(nullptr)
{
	my_waitingWidget = new QLabel();
	my_centralWidget = new windowCentralWidget();
	setCentralWidget(my_centralWidget);
}

ak::ui::qt::window::~window() {}

// #######################################################################################################

// Base class functions

void ak::ui::qt::window::setColorStyle(
	const ui::colorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr);		// Nullptr provided
	my_colorStyle = _colorStyle;

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

void ak::ui::qt::window::SetCentralWidget(
	QWidget *								_widget
) {
	setCentralWidget(my_centralWidget);
	my_centralWidget->setChild(_widget);
}

void ak::ui::qt::window::setWaitingAnimationVisible(
	bool									_visible
) {
	my_centralWidget->setWaitingAnimationVisible(_visible);
}

void ak::ui::qt::window::setWaitingAnimation(
	QMovie *							_movie
) {
	my_centralWidget->setWaitingAnimation(_movie);
}

// ###########################################################################################################################################################

ak::ui::qt::windowCentralWidget::windowCentralWidget()
	: my_waitingLabelSize(20,20), my_waitingAnimation(nullptr), my_waitingAnimationVisible(false)
{
	my_childWidget = nullptr;
	my_waitingLabel = new QLabel("");
	my_waitingLabel->setAttribute(Qt::WA_NoSystemBackground);
	my_waitingLabel->setParent(this);
	my_waitingLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
	my_waitingLabel->hide();
}

ak::ui::qt::windowCentralWidget::~windowCentralWidget() {

}

void ak::ui::qt::windowCentralWidget::setChild(
	QWidget *		_widget
) {
	if (my_childWidget != nullptr) { my_childWidget->hide(); }
	my_childWidget = _widget;
	if (my_childWidget != nullptr) {
		_widget->setParent(this);
		_widget->show();
	}
	if (my_waitingAnimationVisible) { setWaitingAnimationVisible(true); }
}

void ak::ui::qt::windowCentralWidget::resizeEvent(QResizeEvent * _event) {
	if (my_childWidget != nullptr) {
		my_childWidget->resize(size());
	}
	my_waitingLabel->resize(size());
}

void ak::ui::qt::windowCentralWidget::setWaitingAnimationVisible(
	bool			_visible
) {
	assert(my_waitingAnimation != nullptr); // No waiting animation set yet
	my_waitingAnimationVisible = _visible;
	if (my_waitingAnimationVisible) {
		if (my_childWidget == nullptr) {
			my_waitingLabel->setParent(this);
		}
		else {
			my_waitingLabel->setParent(my_childWidget);
		}
		my_waitingLabel->setMovie(my_waitingAnimation);
		my_waitingAnimation->start();
		my_waitingLabel->setGeometry(rect());
		my_waitingLabel->show();
	}
	else {
		my_waitingAnimation->stop();
		my_waitingLabel->setParent(nullptr);
		my_waitingLabel->hide();
	}
}

void ak::ui::qt::windowCentralWidget::setWaitingAnimation(
	QMovie *							_movie
) {
	assert(_movie != nullptr); // Nullptr provided
	my_waitingAnimation = _movie;
}