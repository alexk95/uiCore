/*
 *	File:		aWindowManager.cpp
 *	Package:	akWidgets
 *
 *  Created on: January 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>
#include <akCore/aMessenger.h>
#include <akCore/aUidMangager.h>

#include <akGui/aColorStyle.h>
#include <akGui/aIconManager.h>
#include <akGui/aObjectManager.h>
#include <akGui/aTtbContainer.h>

#include <akWidgets/aDockWidget.h>
#include <akWidgets/aTtbPage.h>
#include <akWidgets/aWindow.h>
#include <akWidgets/aWindowManager.h>
#include <akWidgets/aWindowManagerTimerSignalLinker.h>

// TabToolBar header
#include <TabToolbar/TabToolbar.h>				// tt::TabToolbar
#include <TabToolbar/Page.h>					// tt::Page

// Qt header
#include <qwidget.h>							// QWidget
#include <qprogressbar.h>						// QProgressBar
#include <qstatusbar.h>							// QStatusBar
#include <qlabel.h>								// QLabel
#include <qtimer.h>								// QTimer
#include <qmessagebox.h>						// QMessageBox
#include <qbytearray.h>							// QByteArray
#include <qmovie.h>	

// my_window->resizeDocks({ dock }, { 0 }, Qt::Horizontal); // This is the hack

ak::aWindowManager::aWindowManager(
	aMessenger *									_messenger,
	aUidManager *								_uidManager,
	aColorStyle *							_colorStyle
) : aPaintable(otMainWindow),
my_window(nullptr),
my_messenger(nullptr),
my_uidManager(nullptr),
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
	// Check parameter
	assert(_messenger != nullptr); // Nullptr provided
	assert(_uidManager != nullptr); // Nullptr provided
	my_messenger = _messenger;
	my_uidManager = _uidManager;
	my_uid = my_uidManager->getId();
	my_colorStyle = _colorStyle;

	// Create main window
	my_window = new aWindow();
	my_window->setAutoFillBackground(true);
	my_window->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);

	// Create tab Toolbar
	my_tabToolBar = new tt::TabToolbar();
	my_tabToolBar->setVisible(false);
	my_tabToolBar->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
	my_window->addToolBar(my_tabToolBar);

	// Connect tab Toolbar tab signals
	connect(my_tabToolBar, SIGNAL(tabClicked(int)), this, SLOT(slotTabToolbarTabClicked(int)));
	connect(my_tabToolBar, SIGNAL(currentTabChanged(int)), this, SLOT(slotTabToolbarTabCurrentTabChanged(int)));

	// Create progressbar
	my_progressBar = new QProgressBar;
	my_progressBar->setMinimumWidth(180);
	my_progressBar->setMaximumWidth(180);
	my_progressBar->setMinimum(0);
	my_progressBar->setValue(0);
	my_progressBar->setMaximum(100);
	my_progressBar->setVisible(false);

	// Create status label
	my_statusLabel = new QLabel;
	my_statusLabel->setMinimumWidth(250);
	my_statusLabel->setMaximumWidth(250);
	my_statusLabel->setText(QString());

	my_window->statusBar()->addPermanentWidget(my_progressBar, 180);
	my_window->statusBar()->addPermanentWidget(my_statusLabel, 250);
	my_window->statusBar()->setVisible(true);

	// Setup timer
	my_timerLabelHide = new QTimer;
	my_timerLabelShow = new QTimer;
	my_timerProgressHide = new QTimer;
	my_timerProgressShow = new QTimer;
	my_timerShowMainWindow = new QTimer;
	my_timerLabelHide->setInterval(1500);
	my_timerLabelShow->setInterval(3000);
	my_timerProgressHide->setInterval(1500);
	my_timerProgressShow->setInterval(3000);
	my_timerShowMainWindow->setInterval(1);
	my_timerShowMainWindow->setSingleShot(true);

	// Create timer signal linker
	my_timerSignalLinker = new aWindowManagerTimerSignalLinker(this);
	
	// Link timer to the corresponding functions (realised via the timer type) 
	my_timerSignalLinker->addLink(my_timerLabelHide, ak::aWindowManagerTimerSignalLinker::timerType::statusLabelHide);
	my_timerSignalLinker->addLink(my_timerLabelShow, ak::aWindowManagerTimerSignalLinker::timerType::statusLabelShow);
	my_timerSignalLinker->addLink(my_timerProgressHide, ak::aWindowManagerTimerSignalLinker::timerType::progressHide);
	my_timerSignalLinker->addLink(my_timerProgressShow, ak::aWindowManagerTimerSignalLinker::timerType::progressShow);
	my_timerSignalLinker->addLink(my_timerShowMainWindow, ak::aWindowManagerTimerSignalLinker::timerType::showWindow);

	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

	// Show main window
	//my_timerShowMainWindow->start();
	my_window->resize(800, 600);
	//	my_window->showMaximized();
}

ak::aWindowManager::~aWindowManager() {
	A_OBJECT_DESTROYING
	// Delete the timer signal linker first, so all objects will be disconnected propertly
	if (my_timerSignalLinker != nullptr) { delete my_timerSignalLinker; my_timerSignalLinker = nullptr; }

}

void ak::aWindowManager::setColorStyle(
	aColorStyle *								_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	my_window->setColorStyle(_colorStyle);

	my_progressBar->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));
	my_statusLabel->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorTransparent));

	// TTB
	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow, "QToolBar{border: 0px;", "}"));
	if (sheet.isEmpty()) {
		sheet = "QToolBar{border: 0px;}";
	}
	my_tabToolBar->SetStylesheet(sheet);
	
	sheet = my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow, "QTabWidget{", "}");
	sheet.append(my_colorStyle->toStyleSheet(cafBorderColorHeader, "QTabWidget::pane{border: 0px solid; border-top-width: 1px; border-bottom-width: 1px;", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafBackgroundColorHeader | cafForegroundColorHeader,
		"QTabWidget::tab-bar{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafBackgroundColorHeader | cafForegroundColorHeader,
		"QTabBar::tab{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafBackgroundColorFocus | cafForegroundColorFocus,
		"QTabBar::tab:hover{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafBackgroundColorSelected | cafForegroundColorSelected,
		"QTabBar::tab:selected{", "}"));
	
	if (sheet.length() == 0) {
		// Provide manual stylesheet to fix styling issue in toolBar
		sheet = "QTabWidget{}\n"
				"QTabWidget::pane{border: 0px solid #707070; border-top-width: 1px; border-bottom-width: 1px;}\n"
			;
	}

	my_tabToolBar->SetTabBarStylesheet(sheet);
	my_tabToolBar->SetHideButtonStylesheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));
}

void ak::aWindowManager::removeChildObject(
	aObject *								_child
) {
	aObject::removeChildObject(_child);
	if (_child->type() == otTabToolbarPage) {
		aTtbPage * page = nullptr;
		page = dynamic_cast<aTtbPage *>(_child);
		assert(page != nullptr); // Cast failed
		my_tabToolBar->DestroyPage(page->index());
		for (int i = my_tabToolBarContainer.size() - 1; i >= 0; i--) {
			if (my_tabToolBarContainer[i] == page) {
				my_tabToolBarContainer.erase(my_tabToolBarContainer.begin() + i);
			}
		}
	}
	else {
		my_window->takeCentralWidget();
		my_window->SetCentralWidget(nullptr);
	}
}

void ak::aWindowManager::setAlias(
	const QString &							_alias
) {
	aObject::setAlias(_alias);
	my_window->setObjectName(my_alias);
	my_window->statusBar()->setObjectName(my_alias + "__StatusBar");
	my_progressBar->setObjectName(my_alias + "__ProgressBar");
	my_statusLabel->setObjectName(my_alias + "__StatusLabel");
	//my_tabToolBar->setObjectName(my_alias + "__TTB_ToolBar");
	//my_tabToolBar->setObjectName(my_alias + "__TTB_TabBar");
	//my_tabToolBar->setObjectName(my_alias + "__TTB_HideButton");
}

void ak::aWindowManager::addObjectSettingsToValue(
	rapidjson::Value &						_array,
	rapidjson::Document::AllocatorType &	_allocator
) {
	assert(_array.GetType() == rapidjson::Type::kArrayType); // Value is not an array type
	
	// Initialize object
	rapidjson::Value root;
	root.SetObject();

	// Add alias
	std::string str(my_alias.toStdString());
	rapidjson::Value nAlias(str.c_str(), _allocator);
	root.AddMember(RESTORABLE_NAME_ALIAS, nAlias, _allocator);

	// Add object type
	str = ak::toQString(my_objectType).toStdString();
	rapidjson::Value nType(str.c_str(), _allocator);
	root.AddMember(RESTORABLE_NAME_TYPE, nType, _allocator);

	// Create settings
	rapidjson::Value settings;
	settings.SetObject();

	QByteArray lastConfig(my_window->saveState());
	rapidjson::Value state(rapidjson::kArrayType);
	for (auto itm : lastConfig) {
		rapidjson::Value nV(itm);
		state.PushBack(nV, _allocator);
	}
	settings.AddMember(RESTORABLE_CFG_STATE, state, _allocator);

	// Add settings
	root.AddMember(RESTORABLE_NAME_SETTINGS, settings, _allocator);
	_array.PushBack(root, _allocator);
}

void ak::aWindowManager::restoreSettings(
	const rapidjson::Value &				_settings
) {
	assert(_settings.IsObject()); // Value is not an object

	if (_settings.HasMember(RESTORABLE_CFG_STATE)) {
		assert(_settings[RESTORABLE_CFG_STATE].IsArray());	// State setting is not an array type
		auto state = _settings[RESTORABLE_CFG_STATE].GetArray();

		// Create 
		QByteArray actualState;
		for (auto itm = state.Begin(); itm != state.End(); itm++) {
			assert(itm->IsInt());	// Array contains a non integer value
			actualState.push_back(itm->GetInt());
		}

		slotRestoreSetting(actualState);
		//QMetaObject::invokeMethod(this, "slotRestoreSetting", Qt::ConnectionType::QueuedConnection, Q_ARG(QByteArray, actualState));
	}
}

void ak::aWindowManager::setCentralWidget(
	QWidget *														_centralWidget
) {
	assert(_centralWidget != nullptr); // nullptr provided
	my_window->takeCentralWidget();
	my_window->SetCentralWidget(_centralWidget);
}

// #############################################################################################################

// Dock management

void ak::aWindowManager::addDock(
	aDockWidget *										_dock,
	dockLocation								_dockLocation
) {
	try {
		if (_dock == nullptr) { throw aException("Is nullptr", "Check dock"); }
		switch (_dockLocation)
		{
		case dockLeft:
			my_window->addDockWidget(Qt::LeftDockWidgetArea, _dock);
			break;
		case dockRight:
			my_window->addDockWidget(Qt::RightDockWidgetArea, _dock);
			break;
		case dockBottom:
			my_window->addDockWidget(Qt::BottomDockWidgetArea, _dock);
			break;
		default:
			assert(0); // Not implemented yet
			break;
		}
		my_window->resizeDocks({ _dock }, { 0 }, Qt::Orientation::Vertical);
		my_window->resizeDocks({ _dock }, { 0 }, Qt::Orientation::Horizontal);
		//_dock->resize(200, 200);
	}
	catch (const aException & e) { throw aException(e, "ak::aWindowManager::addDock()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aWindowManager::addDock()"); }
	catch (...) { throw aException("Unknown error", "ak::aWindowManager::addDock()"); }
}

void ak::aWindowManager::tabifyDock(
	aDockWidget *							_mainDock,
	aDockWidget *							_subDock
) {
	assert(_mainDock != nullptr); // Cast failed
	assert(_subDock != nullptr); // Cast failed

	// tabify dock
	my_window->tabifyDockWidget(_mainDock, _subDock);
	//_subDock->resize(200, 200);
	my_window->resizeDocks({ _subDock }, { 0 }, Qt::Orientation::Vertical);
	my_window->resizeDocks({ _subDock }, { 0 }, Qt::Orientation::Horizontal);
	//_subDock->resize(200, 200);
	_mainDock->raise();
}

void ak::aWindowManager::setDockPriorityBottomLeft(
	dockLocation						_dockLocation
) {
	try {
		switch (_dockLocation)
		{
		case dockLeft:
			my_window->setCorner(Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea); break;
		case dockBottom:
			my_window->setCorner(Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::BottomDockWidgetArea); break;
		default:
			throw aException("Invalid dock location", "Check dock location");
		}
	}
	catch (const aException & e) { throw aException(e, "ak::aWindowManager::setDockPriorityBottomLeft()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aWindowManager::setDockPriorityBottomLeft()"); }
	catch (...) { throw aException("Unknown error", "ak::aWindowManager::setDockPriorityBottomLeft()"); }
}

void ak::aWindowManager::setDockPriorityBottomRight(
	dockLocation						_dockLocation
) {
	try {
		switch (_dockLocation)
		{
		case dockRight:
			my_window->setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::RightDockWidgetArea); break;
		case dockBottom:
			my_window->setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::BottomDockWidgetArea); break;
		default:
			throw aException("Invalid dock location", "Check dock location");
		}
	}
	catch (const aException & e) { throw aException(e, "ak::aWindowManager::setDockPriorityBottomLeft()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aWindowManager::setDockPriorityBottomLeft()"); }
	catch (...) { throw aException("Unknown error", "ak::aWindowManager::setDockPriorityBottomLeft()"); }
}

// #############################################################################################################

// Status

void ak::aWindowManager::setStatusBarProgress(
	int											_progress
) {
	if (_progress < 0 || _progress > 100) { throw aException("Progress out of range", "ak::aWindowManager::setStatusProgress()"); }
	setStatusBarContinuous(false);
	my_progressBar->setValue(_progress);
}

void ak::aWindowManager::setStatusBarVisible(
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

void ak::aWindowManager::setStatusBarContinuous(
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

bool ak::aWindowManager::getStatusBarVisible(void) const { return my_progressBar->isVisible(); }

bool ak::aWindowManager::getStatusLabelVisible(void) const { return my_statusLabel->isVisible(); }

QString ak::aWindowManager::getStatusLabelText(void) const { return my_statusLabel->text(); }

int ak::aWindowManager::getStatusBarProgress(void) const { return my_progressBar->value(); }

bool ak::aWindowManager::getStatusBarContinuous(void) const { return my_progressBarContinuous; }

void ak::aWindowManager::setStatusLabelText(
	const QString &														_status
) { my_statusLabel->setText(_status); }

void ak::aWindowManager::setStatusLabelVisible(
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

void ak::aWindowManager::showMaximized(void) { my_window->showMaximized(); }

void ak::aWindowManager::showMinimized(void) { my_window->showMinimized(); }

void ak::aWindowManager::close(void) { my_window->close(); }

void ak::aWindowManager::setShowStatusObjectDelayTimerInterval(int _interval) {
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

void ak::aWindowManager::setHideStatusObjectDelayTimerInterval(int _interval) {
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

int ak::aWindowManager::getShowStatusObjectDelayTimerInterval(void) const {
	return my_timerLabelShow->interval();
}

int ak::aWindowManager::getHideStatusObjectDelayTimerInterval(void) const {
	return my_timerLabelHide->interval();
}

void ak::aWindowManager::setWaitingAnimationVisible(
	bool									_visible
) { my_window->setWaitingAnimationVisible(_visible); }

void ak::aWindowManager::setWaitingAnimation(
	QMovie *							_movie
) { my_window->setWaitingAnimation(_movie); }

// #############################################################################################################

void ak::aWindowManager::setTabToolBarVisible(
	bool						_vis
) {
	my_tabToolBar->setVisible(_vis);
}

ak::aTtbPage * ak::aWindowManager::createTabToolbarSubContainer(
	const QString &				_text
) {
	tt::Page * page = my_tabToolBar->AddPage(_text);
	ak::aTtbPage * p = new ak::aTtbPage(my_messenger, page, _text);
	if (my_colorStyle != nullptr) { p->setColorStyle(my_colorStyle); setColorStyle(my_colorStyle); }
	my_tabToolBarContainer.push_back(p);
	return p;
}

ak::aTtbContainer * ak::aWindowManager::getTabToolBarSubContainer(
	const QString &				_text
) {
	for (auto itm : my_tabToolBarContainer) {
		if (itm->text() == _text) {
			return itm;
		}
	}
	return nullptr;
}

void ak::aWindowManager::addTabToolbarWidget(
	ak::UID						_parentUid,
	ak::UID						_objectUid
) {
	assert(0); // Not implemented yet
}

ak::ID ak::aWindowManager::currentTabToolbarTab(void) const { return my_tabToolBar->CurrentTab(); }

int ak::aWindowManager::tabToolbarTabCount(void) const { return my_tabToolBar->TabCount(); }

void ak::aWindowManager::setCurrentTabToolBarTab(
	ak::ID						_tabID
) {
	assert(_tabID >= 0 && _tabID < my_tabToolBar->TabCount());	// Index out of range
	my_tabToolBar->SetCurrentTab(_tabID);
}

void ak::aWindowManager::setCurrentTabToolBarTab(
	const std::string					&_tabName
) {
	int index = 0;
	for (auto itm : my_tabToolBarContainer) {
		if (itm->text() == _tabName.c_str()) {
			my_tabToolBar->SetCurrentTab(index);
			break;
		}
		index++;
	}
}

void ak::aWindowManager::setCentralWidgetMinimumSize(
	const QSize &				_size
) {
	QWidget * w = my_window->centralWidget();
	if (w != nullptr) {
		w->setMinimumSize(_size);
	}
}

// #############################################################################################################

ak::aWindow * ak::aWindowManager::window(void) const { return my_window; }

void ak::aWindowManager::setWindowTitle(
	const QString &														_title
) { my_window->setWindowTitle(_title); }

QString ak::aWindowManager::windowTitle(void) const { return my_window->windowTitle(); }

void ak::aWindowManager::addEventHandler(
	aWindowEventHandler *					_eventHandler
) { my_window->addEventHandler(_eventHandler); }

void ak::aWindowManager::removeEventHandler(
	aWindowEventHandler *					_eventHandler
) { my_window->removeEventHandler(_eventHandler); }

// #############################################################################################################

// Slots

void ak::aWindowManager::slotRestoreSetting(
	const QByteArray &					_actualState
) {
	my_window->restoreState(_actualState);
}

void ak::aWindowManager::slotTabToolbarTabClicked(int _index) {
	my_messenger->sendMessage(my_uid, etTabToolbarClicked, _index);
}
void ak::aWindowManager::slotTabToolbarTabCurrentTabChanged(int _index) {
	my_messenger->sendMessage(my_uid, etTabToolbarChanged, _index);
}