/*
 * ak_ui_uiManagerTimerSignalLinker.cpp
 *
 *  Created on: February 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_uiManagerTimerSignalLinker.h>	// corresponding class
#include <ak_exception.h>						// Error handling
#include <ak_ui_uiManager.h>					// uiManager

// Qt header
#include <qtimer.h>								// QTimer

ak::ui::uiManagerTimerSignalLinker::uiManagerTimerSignalLinker(
	ak::ui::uiManager *				_uiManager
) {
	if (_uiManager == nullptr) { throw ak::Exception("Is nullptr", "Check ui manager"); }
	my_uiManager = _uiManager;
}

ak::ui::uiManagerTimerSignalLinker::~uiManagerTimerSignalLinker() {
	for (int i = 0; i < my_links.size(); i++) {
		switch (my_links.at(i).type)
		{
		case ak::ui::uiManagerTimerSignalLinker::progressShow:
			disconnect(my_links.at(i).timer, SIGNAL(timeout()), this, SIGNAL(timerTimeoutProgressShow()));
			break;
		case ak::ui::uiManagerTimerSignalLinker::progressHide:
			disconnect(my_links.at(i).timer, SIGNAL(timeout()), this, SIGNAL(timerTimeoutProgressHide()));
			break;
		case ak::ui::uiManagerTimerSignalLinker::statusLabelShow:
			disconnect(my_links.at(i).timer, SIGNAL(timeout()), this, SIGNAL(timerTimeoutLabelShow()));
			break;
		case ak::ui::uiManagerTimerSignalLinker::statusLabelHide:
			disconnect(my_links.at(i).timer, SIGNAL(timeout()), this, SIGNAL(timerTimeoutLabelHide()));
			break;
		case ak::ui::uiManagerTimerSignalLinker::timerType::showWindow:
			disconnect(my_links.at(i).timer, SIGNAL(timeout()), this, SIGNAL(timerTimeoutShowWindow()));
			break;
		default:
			assert(0); // not implemented timer type
			break;
		}
	}
}

void ak::ui::uiManagerTimerSignalLinker::addLink(
	QTimer *											_timer,
	ak::ui::uiManagerTimerSignalLinker::timerType		_timerType
) {
	switch (_timerType)
	{
	case ak::ui::uiManagerTimerSignalLinker::progressShow:
		connect(_timer, SIGNAL(timeout()), this, SLOT(timerTimeoutProgressShow())); break;
	case ak::ui::uiManagerTimerSignalLinker::progressHide:
		connect(_timer, SIGNAL(timeout()), this, SLOT(timerTimeoutProgressHide())); break;
	case ak::ui::uiManagerTimerSignalLinker::statusLabelShow:
		connect(_timer, SIGNAL(timeout()), this, SLOT(timerTimeoutLabelShow())); break;
	case ak::ui::uiManagerTimerSignalLinker::statusLabelHide:
		connect(_timer, SIGNAL(timeout()), this, SLOT(timerTimeoutLabelHide())); break;
	case ak::ui::uiManagerTimerSignalLinker::showWindow:
		connect(_timer, SIGNAL(timeout()), this, SLOT(timerTimeoutShowWindow())); break;
	default:
		assert(0); // not implemented timer type
		break;
	}
	my_links.push_back(ak::ui::uiManagerTimerSignalLinker::timer{ _timer, _timerType });
}

void ak::ui::uiManagerTimerSignalLinker::timerTimeoutLabelHide() { my_uiManager->setStatusLabelVisible(false, false); }

void ak::ui::uiManagerTimerSignalLinker::timerTimeoutLabelShow() { my_uiManager->setStatusLabelVisible(true, false); }

void ak::ui::uiManagerTimerSignalLinker::timerTimeoutProgressHide() { my_uiManager->setStatusBarVisible(false, false); }

void ak::ui::uiManagerTimerSignalLinker::timerTimeoutProgressShow() { my_uiManager->setStatusBarVisible(true, false); }

void ak::ui::uiManagerTimerSignalLinker::timerTimeoutShowWindow() { my_uiManager->maximizeWindow(); }