/*
 *	File:		aWindow.cpp
 *	Package:	akWidgets
 *
 *  Created on: November 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aColorStyle.h>
#include <akGui/aWindowEventHandler.h>
#include <akWidgets/aWindow.h>

// Qt header
#include <qevent.h>
#include <qwidget.h>
#include <qframe.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qmovie.h>
#include <qscreen.h>

ak::aWindow::aWindow()
	: QMainWindow(), aPaintable(otMainWindow), my_waitingWidget(nullptr), my_centralWidget(nullptr)
{
	my_waitingWidget = new QLabel();
	my_centralWidget = new aWindowCentralWidget();
	setCentralWidget(my_centralWidget);
}

ak::aWindow::~aWindow() { A_OBJECT_DESTROYING }

// #######################################################################################################

// Base class functions

void ak::aWindow::setColorStyle(
	aColorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr);		// Nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));

	setStyleSheet(sheet);

	// Double paint to not mess up the tab toolbar
	sheet = my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow, "QMainWindow{", "}\n");

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorWindow | cafBackgroundColorWindow,
		"QTabBar{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafBackgroundColorHeader | cafForegroundColorHeader,
		"QTabBar::tab{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafBackgroundColorFocus | cafForegroundColorFocus,
		"QTabBar::tab:hover{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafBackgroundColorSelected | cafForegroundColorSelected,
		"QTabBar::tab:selected{", "}"));

	setStyleSheet(sheet);

	statusBar()->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));
	
}

void ak::aWindow::closeEvent(QCloseEvent * _event) {
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

void ak::aWindow::addEventHandler(
	aWindowEventHandler *					_eventHandler
) {
	assert(my_eventHandler.find(_eventHandler) == my_eventHandler.end());	// Event handler already set
	my_eventHandler.insert_or_assign(_eventHandler, true);
}

void ak::aWindow::removeEventHandler(
	aWindowEventHandler *					_eventHandler
) { my_eventHandler.erase(_eventHandler); }

void ak::aWindow::SetCentralWidget(
	QWidget *								_widget
) {
	setCentralWidget(my_centralWidget);
	my_centralWidget->setChild(_widget);
}

void ak::aWindow::setWaitingAnimationVisible(
	bool									_visible
) {
	my_centralWidget->setWaitingAnimationVisible(_visible);
}

void ak::aWindow::setWaitingAnimation(
	QMovie *							_movie
) {
	my_centralWidget->setWaitingAnimation(_movie);
}

// ###########################################################################################################################################################

ak::aWindowCentralWidget::aWindowCentralWidget()
	: my_waitingLabelSize(20,20), my_waitingAnimation(nullptr), my_waitingAnimationVisible(false)
{
	my_childWidget = nullptr;
	my_waitingLabel = new QLabel("");
	my_waitingLabel->setAttribute(Qt::WA_NoSystemBackground);
	my_waitingLabel->setParent(this);
	my_waitingLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
	my_waitingLabel->hide();
}

ak::aWindowCentralWidget::~aWindowCentralWidget() {

}

void ak::aWindowCentralWidget::setChild(
	QWidget *		_widget
) {
	if (my_childWidget != nullptr) { my_childWidget->hide(); }
	my_childWidget = _widget;
	if (my_childWidget != nullptr) {
		_widget->setParent(this);
		_widget->resize(size());
		_widget->show();
	}
	if (my_waitingAnimationVisible) { setWaitingAnimationVisible(true); }
}

void ak::aWindowCentralWidget::resizeEvent(QResizeEvent * _event) {
	if (my_childWidget != nullptr) {
		my_childWidget->resize(size());
	}
	my_waitingLabel->resize(size());
}

void ak::aWindowCentralWidget::setWaitingAnimationVisible(
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

void ak::aWindowCentralWidget::setWaitingAnimation(
	QMovie *							_movie
) {
	assert(_movie != nullptr); // Nullptr provided
	my_waitingAnimation = _movie;
}