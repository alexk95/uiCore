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
	: QMainWindow(), aPaintable(otMainWindow), m_waitingWidget(nullptr), m_centralWidget(nullptr)
{
	m_waitingWidget = new QLabel();
	m_centralWidget = new aWindowCentralWidget();
	setCentralWidget(m_centralWidget);
}

ak::aWindow::~aWindow() { A_OBJECT_DESTROYING }

// #######################################################################################################

// Base class functions

void ak::aWindow::setColorStyle(
	aColorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr);		// Nullptr provided
	m_colorStyle = _colorStyle;

	QString sheet(m_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));

	setStyleSheet(sheet);

	// Double paint to not mess up the tab toolbar
	sheet = m_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow, "QMainWindow{", "}\n");

	sheet.append(m_colorStyle->toStyleSheet(cafForegroundColorWindow | cafBackgroundColorWindow,
		"QTabBar{", "}\n"));
	sheet.append(m_colorStyle->toStyleSheet(cafBackgroundColorHeader | cafForegroundColorHeader,
		"QTabBar::tab{", "}\n"));
	sheet.append(m_colorStyle->toStyleSheet(cafBackgroundColorFocus | cafForegroundColorFocus,
		"QTabBar::tab:hover{", "}\n"));
	sheet.append(m_colorStyle->toStyleSheet(cafBackgroundColorSelected | cafForegroundColorSelected,
		"QTabBar::tab:selected{", "}"));

	setStyleSheet(sheet);

	statusBar()->setStyleSheet(m_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));
	
}

void ak::aWindow::closeEvent(QCloseEvent * _event) {
	for (auto handler : m_eventHandler) {
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
	assert(m_eventHandler.find(_eventHandler) == m_eventHandler.end());	// Event handler already set
	m_eventHandler.insert_or_assign(_eventHandler, true);
}

void ak::aWindow::removeEventHandler(
	aWindowEventHandler *					_eventHandler
) { m_eventHandler.erase(_eventHandler); }

void ak::aWindow::SetCentralWidget(
	QWidget *								_widget
) {
	setCentralWidget(m_centralWidget);
	m_centralWidget->setChild(_widget);
}

void ak::aWindow::setWaitingAnimationVisible(
	bool									_visible
) {
	m_centralWidget->setWaitingAnimationVisible(_visible);
}

void ak::aWindow::setWaitingAnimation(
	QMovie *							_movie
) {
	m_centralWidget->setWaitingAnimation(_movie);
}

// ###########################################################################################################################################################

ak::aWindowCentralWidget::aWindowCentralWidget()
	: m_waitingLabelSize(20,20), m_waitingAnimation(nullptr), m_waitingAnimationVisible(false)
{
	m_childWidget = nullptr;
	m_waitingLabel = new QLabel("");
	m_waitingLabel->setAttribute(Qt::WA_NoSystemBackground);
	m_waitingLabel->setParent(this);
	m_waitingLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
	m_waitingLabel->hide();
}

ak::aWindowCentralWidget::~aWindowCentralWidget() {

}

void ak::aWindowCentralWidget::setChild(
	QWidget *		_widget
) {
	QSize s = size();
	auto oldChild = m_childWidget;
	m_childWidget = _widget;
	if (m_childWidget != nullptr) {
		_widget->setParent(this);
		_widget->resize(s);
		_widget->show();
	}
	if (oldChild != nullptr) { oldChild->hide(); }
	if (m_waitingAnimationVisible) { setWaitingAnimationVisible(true); }

}

void ak::aWindowCentralWidget::resizeEvent(QResizeEvent * _event) {
	if (m_childWidget != nullptr) {
		m_childWidget->resize(size());
	}
	m_waitingLabel->resize(size());
}

void ak::aWindowCentralWidget::setWaitingAnimationVisible(
	bool			_visible
) {
	assert(m_waitingAnimation != nullptr); // No waiting animation set yet
	m_waitingAnimationVisible = _visible;
	if (m_waitingAnimationVisible) {
		if (m_childWidget == nullptr) {
			m_waitingLabel->setParent(this);
		}
		else {
			m_waitingLabel->setParent(m_childWidget);
		}
		m_waitingLabel->setMovie(m_waitingAnimation);
		m_waitingAnimation->start();
		m_waitingLabel->setGeometry(rect());
		m_waitingLabel->show();
	}
	else {
		m_waitingAnimation->stop();
		m_waitingLabel->setParent(nullptr);
		m_waitingLabel->hide();
	}
}

void ak::aWindowCentralWidget::setWaitingAnimation(
	QMovie *							_movie
) {
	assert(_movie != nullptr); // Nullptr provided
	m_waitingAnimation = _movie;
}