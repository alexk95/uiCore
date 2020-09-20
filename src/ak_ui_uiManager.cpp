/*
 * ak_ui_uiManager.cpp
 *
 *  Created on: January 30, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include "ak_ui_uiManager.h"					// corresponding class
#include "ak_ui_uiManagerTimerSignalLinker.h"	// uiManagerTimerSignalLinker
#include "ak_exception.h"						// error handling
#include "ak_messenger.h"						// messenger
#include "ak_ui_objectManager.h"				// object manager
#include "ak_ui_colorStyle.h"					// colorStyle
#include "ak_ui_iconManager.h"					// iconManager
#include "ak_ui_qt_dock.h"						// dock
#include "ak_ui_core_ttbContainer.h"			// ttbContainer
#include "ak_ui_ttb_page.h"						// page
#include "ak_uidMangager.h"						// UID manager

// TabToolBar header
#include <TabToolbar/TabToolbar.h>				// tt::TabToolbar
#include <TabToolbar/Page.h>					// tt::Page

// Qt header
#include <qwidget.h>
#include <qmainwindow.h>						// QMainWindow
#include <qprogressbar.h>						// QProgressBar
#include <qstatusbar.h>							// QStatusBar
#include <qlabel.h>								// QLabel
#include <qtimer.h>								// QTimer
#include <qmessagebox.h>						// QMessageBox

// my_window->resizeDocks({ dock }, { 0 }, Qt::Horizontal); // This is the hack

ak::ui::uiManager::uiManager(
	ak::messenger *									_messenger,
	ak::uidManager *								_uidManager,
	ak::ui::iconManager *							_iconManager,
	ak::ui::objectManager *							_objectManager,
	ak::ui::colorStyle *							_colorStyle
) : ak::ui::core::aPaintable(ak::ui::core::objectType::oMainWindow),
	my_window(nullptr),
	my_messenger(nullptr),
	my_uidManager(nullptr),
	my_iconManager(nullptr),
	my_objectManager(nullptr),
	my_tabToolBar(nullptr),
	my_statusLabel(nullptr),
	my_progressBar(nullptr),
	my_timerLabelHide(nullptr),
	my_timerLabelShow(nullptr),
	my_timerProgressHide(nullptr),
	my_timerProgressShow(nullptr),
	my_timerSignalLinker(nullptr),
	my_timerShowMainWindow(nullptr)
{
	try {
		// Check parameter
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger"); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }
		if (_iconManager == nullptr) { throw ak::Exception("Is nullptr", "Check icon manager"); }
		if (_objectManager == nullptr) { throw ak::Exception("Is nullptr", "Check object manager"); }
		my_messenger = _messenger;
		my_uidManager = _uidManager;
		my_uid = my_uidManager->getId();
		my_iconManager = _iconManager;
		my_colorStyle = _colorStyle;
		my_objectManager = _objectManager;

		// Create main window
		my_window = new QMainWindow();
		if (my_window == nullptr) { throw ak::Exception("Failed to create", "Create main window"); }
		my_window->setAutoFillBackground(true);

		// Create tab Toolbar
		my_tabToolBar = new tt::TabToolbar(nullptr, 74U, 3U);
		my_tabToolBar->setVisible(false);
		my_window->addToolBar(my_tabToolBar);

		// Create progressbar
		my_progressBar = new QProgressBar;
		if (my_progressBar == nullptr) { throw ak::Exception("Failed to create", "Create progress bar"); }
		my_progressBar->setMinimumWidth(180);
		my_progressBar->setMaximumWidth(180);
		my_progressBar->setMinimum(0);
		my_progressBar->setValue(0);
		my_progressBar->setMaximum(100);
		my_progressBar->setVisible(false);

		// Create status label
		my_statusLabel = new QLabel;
		if (my_statusLabel == nullptr) { throw ak::Exception("Failed to create", "Create status label"); }
		my_statusLabel->setMinimumWidth(250);
		my_statusLabel->setMaximumWidth(250);
		my_statusLabel->setText(QString());

		my_window->statusBar()->addPermanentWidget(my_progressBar, 180);
		my_window->statusBar()->addPermanentWidget(my_statusLabel, 250);
		my_window->statusBar()->setVisible(true);

		// Setup timer
		my_timerLabelHide = new QTimer;
		if (my_timerLabelHide == nullptr) { throw ak::Exception("Failed to create", "Create timer for label hide"); }
		my_timerLabelShow = new QTimer;
		if (my_timerLabelShow == nullptr) { throw ak::Exception("Failed to create", "Create timer for label show"); }
		my_timerProgressHide = new QTimer;
		if (my_timerProgressHide == nullptr) { throw ak::Exception("Failed to create", "Create timer for progress bar hide"); }
		my_timerProgressShow = new QTimer;
		if (my_timerProgressShow == nullptr) { throw ak::Exception("Failed to create", "Create timer for progress bar show"); }
		my_timerShowMainWindow = new QTimer;
		if (my_timerShowMainWindow == nullptr) { throw ak::Exception("Failed to create", "Create timer for showing main window"); }
		my_timerLabelHide->setInterval(1500);
		my_timerLabelShow->setInterval(3000);
		my_timerProgressHide->setInterval(1500);
		my_timerProgressShow->setInterval(3000);
		my_timerShowMainWindow->setInterval(1);
		my_timerShowMainWindow->setSingleShot(true);

		// Create timer signal linker
		my_timerSignalLinker = new ak::ui::uiManagerTimerSignalLinker(this);
		if (my_timerSignalLinker == nullptr) { throw ak::Exception("Failed to create", "Create timer signal linker"); }

		// Link timer to the corresponding functions (realised via the timer type) 
		my_timerSignalLinker->addLink(my_timerLabelHide, ak::ui::uiManagerTimerSignalLinker::timerType::statusLabelHide);
		my_timerSignalLinker->addLink(my_timerLabelShow, ak::ui::uiManagerTimerSignalLinker::timerType::statusLabelShow);
		my_timerSignalLinker->addLink(my_timerProgressHide, ak::ui::uiManagerTimerSignalLinker::timerType::progressHide);
		my_timerSignalLinker->addLink(my_timerProgressShow, ak::ui::uiManagerTimerSignalLinker::timerType::progressShow);
		my_timerSignalLinker->addLink(my_timerShowMainWindow, ak::ui::uiManagerTimerSignalLinker::timerType::showWindow);

		if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

		// Show main window
		my_timerShowMainWindow->start();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::uiManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::uiManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::uiManager()"); }
}

ak::ui::uiManager::~uiManager() {
	
	// Delete the timer signal linker first, so all objects will be disconnected propertly
	if (my_timerSignalLinker != nullptr) { delete my_timerSignalLinker; my_timerSignalLinker = nullptr; }

}

void ak::ui::uiManager::setColorStyle(
	ak::ui::colorStyle *												_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check colorStyle"); }
		my_colorStyle = _colorStyle;

		// Main window
		my_window->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sMainWindow));
		my_window->statusBar()->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sStatusBar));

		// Progress bar
		my_progressBar->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sProgressBar));

		// Status label
		my_statusLabel->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sLabel));

		// TabToolbar
		my_tabToolBar->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sToolBar));
		my_tabToolBar->SetTabBarStylesheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTabWidget));
		my_tabToolBar->SetTabBarTabStylesheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTabBar));
		my_tabToolBar->SetHideButtonStylesheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sToolButton));

		for (my_mapTabToolBarContainerIterator itm = my_mapTabToolBarContainer.begin(); itm != my_mapTabToolBarContainer.end(); itm++) {
			assert(itm->second != nullptr); // Nullptr stored
			itm->second->setColorStyle(my_colorStyle);
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::setColorStyle()"); }
}

void ak::ui::uiManager::setCentralWidget(
	QWidget *														_centralWidget
) {
	try {
		if (_centralWidget == nullptr) { throw ak::Exception("Is nullptr", "Check central widget"); }
		my_window->setCentralWidget(_centralWidget);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::setCentralWidget()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::setCentralWidget()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::setCentralWidget()"); }
}

// #############################################################################################################

// Dock management

void ak::ui::uiManager::addDock(
	ak::ui::qt::dock *										_dock,
	ak::ui::core::dockLocation								_dockLocation
) {
	try {
		if (_dock == nullptr) { throw ak::Exception("Is nullptr", "Check dock"); }
		switch (_dockLocation)
		{
		case ak::ui::core::dockLeft:
			my_window->addDockWidget(Qt::LeftDockWidgetArea, _dock);
			break;
		case ak::ui::core::dockRight:
			my_window->addDockWidget(Qt::RightDockWidgetArea, _dock);
			break;
		case ak::ui::core::dockBottom:
			my_window->addDockWidget(Qt::BottomDockWidgetArea, _dock);
			break;
		default:
			assert(0); // Not implemented yet
			break;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::addDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::addDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::addDock()"); }
}

void ak::ui::uiManager::tabifyDock(
	ak::UID										_firstDockUid,
	ak::UID										_secondDockUid
) {
	try {
		// Get and cast dock 1
		ak::ui::core::aObject * d1 = my_objectManager->obj_get(_firstDockUid);
		if (d1->objectType() != ak::ui::core::objectType::oDock) { throw ak::Exception("Invalid object type, expected dock", "Check object type"); }
		ak::ui::qt::dock * dock1 = nullptr;
		dock1 = dynamic_cast<ak::ui::qt::dock *>(d1);
		if (dock1 == nullptr) { throw ak::Exception("Cast failed", "Cast dock 1"); }

		// Get and cast dock 2
		ak::ui::core::aObject * d2 = my_objectManager->obj_get(_secondDockUid);
		if (d2->objectType() != ak::ui::core::objectType::oDock) { throw ak::Exception("Invalid object type, expected dock", "Check object type"); }
		ak::ui::qt::dock * dock2 = nullptr;
		dock2 = dynamic_cast<ak::ui::qt::dock *>(d2);
		if (dock2 == nullptr) { throw ak::Exception("Cast failed", "Cast dock 2"); }
		
		// tabify dock
		my_window->tabifyDockWidget(dock1, dock2);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::tabifyDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::tabifyDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::tabifyDock()"); }
}

void ak::ui::uiManager::setDockPriorityBottomLeft(
	ak::ui::core::dockLocation						_dockLocation
) {
	try {
		switch (_dockLocation)
		{
		case ak::ui::core::dockLeft:
			my_window->setCorner(Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea); break;
		case ak::ui::core::dockBottom:
			my_window->setCorner(Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::BottomDockWidgetArea); break;
		default:
			throw ak::Exception("Invalid dock location", "Check dock location");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::setDockPriorityBottomLeft()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::setDockPriorityBottomLeft()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::setDockPriorityBottomLeft()"); }
}

void ak::ui::uiManager::setDockPriorityBottomRight(
	ak::ui::core::dockLocation						_dockLocation
) {
	try {
		switch (_dockLocation)
		{
		case ak::ui::core::dockRight:
			my_window->setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::RightDockWidgetArea); break;
		case ak::ui::core::dockBottom:
			my_window->setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::BottomDockWidgetArea); break;
		default:
			throw ak::Exception("Invalid dock location", "Check dock location");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::setDockPriorityBottomLeft()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::setDockPriorityBottomLeft()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::setDockPriorityBottomLeft()"); }
}

// #############################################################################################################

// Status

void ak::ui::uiManager::setStatusBarProgress(
	int											_progress
) {
	if (_progress < 0 || _progress > 100) { throw ak::Exception("Progress out of range", "ak::ui::uiManager::setStatusProgress()"); }
	setStatusBarContinuous(false);
	my_progressBar->setValue(_progress);
}

void ak::ui::uiManager::setStatusBarVisible(
	bool										_visible,
	bool										_showDelayed
) {
	if (_visible) {
		my_timerProgressShow->stop();
		if (my_timerProgressHide->isActive()) {
			my_timerProgressHide->stop();
			my_progressBar->setVisible(false);
		}
		if (_showDelayed) {
			my_timerProgressShow->start();
		}
		else { my_progressBar->setVisible(true); }
	}
	else {
		my_timerProgressHide->stop();
		if (my_timerProgressShow->isActive()) {
			my_timerProgressShow->stop();
			my_progressBar->setVisible(true);
		}
		if (_showDelayed) {
			my_timerProgressHide->start();
		}
		else { my_progressBar->setVisible(false); }
	}
}

void ak::ui::uiManager::setStatusBarContinuous(
	bool										_continuos
) {
	my_progressBarContinuous = _continuos;
	if (my_progressBarContinuous) {
		my_progressBar->setValue(0);
		my_progressBar->setRange(0, 0);
	}
	else {
		my_progressBar->setRange(0, 100);
	}
}

bool ak::ui::uiManager::getStatusBarVisible(void) const { return my_progressBar->isVisible(); }

bool ak::ui::uiManager::getStatusLabelVisible(void) const { return my_statusLabel->isVisible(); }

QString ak::ui::uiManager::getStatusLabelText(void) const { return my_statusLabel->text(); }

int ak::ui::uiManager::getStatusBarProgress(void) const { return my_progressBar->value(); }

bool ak::ui::uiManager::getStatusBarContinuous(void) const { return my_progressBarContinuous; }

void ak::ui::uiManager::setStatusLabelText(
	const QString &														_status
) { my_statusLabel->setText(_status); }

void ak::ui::uiManager::setStatusLabelVisible(
	bool																_visible,
	bool																_showDelayed
) {
	if (_visible) {
		my_timerLabelShow->stop();
		if (my_timerLabelHide->isActive()) {
			my_timerLabelHide->stop();
			my_statusLabel->setVisible(false);
		}
		if (_showDelayed) {
			my_timerLabelShow->start();
		}
		else { my_statusLabel->setVisible(true); }
	}
	else {
		my_timerLabelHide->stop();
		if (my_timerLabelShow->isActive()) {
			my_timerLabelShow->stop();
			my_statusLabel->setVisible(true);
		}
		if (_showDelayed) {
			my_timerLabelHide->start();
		}
		else { my_statusLabel->setVisible(false); }
	}
}

void ak::ui::uiManager::maximizeWindow(void) {
	my_window->showMaximized();
}

void ak::ui::uiManager::minimizeWindow(void) {
	my_window->showMinimized();
}

void ak::ui::uiManager::close(void) {
	my_window->close();
}

void ak::ui::uiManager::deleteObject(
	ak::UID													_objectUid
) {
	try {
		assert(0); // not implemented yet
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::removeObject()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::removeObject()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::removeObject()"); }
}

void ak::ui::uiManager::setShowStatusObjectDelayTimerInterval(int _interval) {
	bool labelShow = false;
	bool progressShow = false;
	// Check if timer is already running
	if (my_timerLabelShow->isActive()) { my_timerLabelShow->stop(); labelShow = true; }
	if (my_timerProgressShow->isActive()) { my_timerProgressShow->stop(); progressShow = true; }
	// Set new interval
	my_timerLabelShow->setInterval(_interval);
	my_timerProgressShow->setInterval(_interval);
	// Reastart timer if needed
	if (labelShow) { my_timerLabelShow->start(); }
	if (progressShow) { my_timerProgressShow->start(); }
}

void ak::ui::uiManager::setHideStatusObjectDelayTimerInterval(int _interval) {
	bool labelHide = false;
	bool progressHide = false;
	// Check if timer is already running
	if (my_timerLabelHide->isActive()) { my_timerLabelHide->stop(); labelHide = true; }
	if (my_timerProgressHide->isActive()) { my_timerProgressHide->stop(); progressHide = true; }
	// Set new interval
	my_timerLabelHide->setInterval(_interval);
	my_timerProgressHide->setInterval(_interval);
	// Reastart timer if needed
	if (labelHide) { my_timerLabelHide->start(); }
	if (progressHide) { my_timerProgressHide->start(); }
}

int ak::ui::uiManager::getShowStatusObjectDelayTimerInterval(void) const {
	return my_timerLabelShow->interval();
}

int ak::ui::uiManager::getHideStatusObjectDelayTimerInterval(void) const {
	return my_timerLabelHide->interval();
}

void ak::ui::uiManager::showMessageBox(
	const QString &														_message,
	const QString &														_title
) {
	try{
		QMessageBox msg;
		msg.setText(_message);
		msg.setWindowTitle(_title);
		msg.exec();
	}
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::showMessagebox()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::showMessagebox()"); }
}

//

void ak::ui::uiManager::setTabToolBarVisible(
	bool						_vis
) {
	try {
		my_tabToolBar->setVisible(_vis);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::setTabToolBarVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::setTabToolBarVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::setTabToolBarVisible()"); }
}

ak::ui::ttb::page * ak::ui::uiManager::createTabToolbarSubContainer(
	const QString &				_text
) {
	try {
		tt::Page * page = my_tabToolBar->AddPage(_text);
		ak::ui::ttb::page * p = new ak::ui::ttb::page(my_messenger, my_uidManager, page, _text);
		return p;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::createTabToolbarSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::createTabToolbarSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::createTabToolbarSubContainer()"); }
}

void ak::ui::uiManager::addTabToolbarWidget(
	ak::UID						_parentUid,
	ak::UID						_objectUid
) {
	try {
		assert(0); // Not implemented yet
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::addTabToolbarWidget()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::addTabToolbarWidget()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::addTabToolbarWidget()"); }
}

//

QMainWindow * ak::ui::uiManager::window(void) const { return my_window; }

void ak::ui::uiManager::setWindowTitle(
	const QString &														_title
) {
	my_window->setWindowTitle(_title);
}

QString ak::ui::uiManager::windowTitle(void) const {
	return my_window->windowTitle();
}