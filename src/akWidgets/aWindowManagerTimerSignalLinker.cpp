/*
 *	File:		aWindowManagerTimerSignalLinker.cpp
 *	Package:	akWidgets
 *
 *  Created on: February 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>
#include <akWidgets/aWindowManager.h>
#include <akWidgets/aWindowManagerTimerSignalLinker.h>

// Qt header
#include <qtimer.h>								// QTimer

ak::aWindowManagerTimerSignalLinker::aWindowManagerTimerSignalLinker(
	aWindowManager *				_uiManager
) {
	if (_uiManager == nullptr) { throw aException("Is nullptr", "Check ui manager"); }
	my_uiManager = _uiManager;
}

ak::aWindowManagerTimerSignalLinker::~aWindowManagerTimerSignalLinker() {
	for (int i = 0; i < my_links.size(); i++) {
		switch (my_links.at(i).type)
		{
		case progressShow:
			disconnect(my_links.at(i).timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutProgressShow);
			break;
		case progressHide:
			disconnect(my_links.at(i).timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutProgressHide);
			break;
		case statusLabelShow:
			disconnect(my_links.at(i).timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutLabelShow);
			break;
		case statusLabelHide:
			disconnect(my_links.at(i).timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutLabelHide);
			break;
		case timerType::showWindow:
			disconnect(my_links.at(i).timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutShowWindow);
			break;
		default:
			assert(0); // not implemented timer type
			break;
		}
	}
}

void ak::aWindowManagerTimerSignalLinker::addLink(
	QTimer *											_timer,
	timerType		_timerType
) {
	switch (_timerType)
	{
	case progressShow:
		connect(_timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutProgressShow); break;
	case progressHide:
		connect(_timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutProgressHide); break;
	case statusLabelShow:
		connect(_timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutLabelShow); break;
	case statusLabelHide:
		connect(_timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutLabelHide); break;
	case showWindow:
		connect(_timer, &QTimer::timeout, this, &aWindowManagerTimerSignalLinker::timerTimeoutShowWindow); break;
	default:
		assert(0); // not implemented timer type
		break;
	}
	my_links.push_back(timer{ _timer, _timerType });
}

void ak::aWindowManagerTimerSignalLinker::timerTimeoutLabelHide() { my_uiManager->setStatusLabelVisible(false, false); }

void ak::aWindowManagerTimerSignalLinker::timerTimeoutLabelShow() { my_uiManager->setStatusLabelVisible(true, false); }

void ak::aWindowManagerTimerSignalLinker::timerTimeoutProgressHide() { my_uiManager->setStatusBarVisible(false, false); }

void ak::aWindowManagerTimerSignalLinker::timerTimeoutProgressShow() { my_uiManager->setStatusBarVisible(true, false); }

void ak::aWindowManagerTimerSignalLinker::timerTimeoutShowWindow() { my_uiManager->showMaximized(); }