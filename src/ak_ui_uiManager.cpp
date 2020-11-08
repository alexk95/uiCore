/*
 * ak_ui_uiManager.cpp
 *
 *  Created on: January 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_uiManager.h>					// corresponding class
#include <ak_ui_uiManagerTimerSignalLinker.h>	// uiManagerTimerSignalLinker
#include <ak_exception.h>						// error handling
#include <ak_messenger.h>						// messenger
#include <ak_ui_objectManager.h>				// object manager
#include <ak_ui_colorStyle.h>					// colorStyle
#include <ak_ui_iconManager.h>					// iconManager
#include <ak_ui_qt_dock.h>						// dock
#include <ak_ui_core_ttbContainer.h>			// ttbContainer
#include <ak_ui_ttb_page.h>						// page
#include <ak_uidMangager.h>						// UID manager

// TabToolBar header
#include <TabToolbar/TabToolbar.h>				// tt::TabToolbar
#include <TabToolbar/Page.h>					// tt::Page

// Qt header

#include <qwidget.h>							// QWidget
#include <qmainwindow.h>						// QMainWindow
#include <qprogressbar.h>						// QProgressBar
#include <qstatusbar.h>							// QStatusBar
#include <qlabel.h>								// QLabel
#include <qtimer.h>								// QTimer
#include <qmessagebox.h>						// QMessageBox
#include <qbytearray.h>							// QByteArray

// my_window->resizeDocks({ dock }, { 0 }, Qt::Horizontal); // This is the hack
#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::uiManager::uiManager(
	ak::messenger *									_messenger,
	ak::uidManager *								_uidManager,
	ak::ui::colorStyle *							_colorStyle
) : ak::ui::core::aPaintable(ak::ui::core::objectType::oMainWindow),
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
	try {
		// Check parameter
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger"); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }
		my_messenger = _messenger;
		my_uidManager = _uidManager;
		my_uid = my_uidManager->getId();
		my_colorStyle = _colorStyle;

		// Create main window
		my_window = new QMainWindow();
		if (my_window == nullptr) { throw ak::Exception("Failed to create", "Create main window"); }
		my_window->setAutoFillBackground(true);

		// Create tab Toolbar
		my_tabToolBar = new tt::TabToolbar();
		my_tabToolBar->setVisible(false);
		my_window->addToolBar(my_tabToolBar);

		// Connect tab Toolbar tab signals
		connect(my_tabToolBar, SIGNAL(tabClicked(int)), this, SLOT(slotTabToolbarTabClicked(int)));
		connect(my_tabToolBar, SIGNAL(currentTabChanged(int)), this, SLOT(slotTabToolbarTabCurrentTabChanged(int)));

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
		//my_timerShowMainWindow->start();
		my_window->resize(800, 600);
	//	my_window->showMaximized();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::uiManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::uiManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::uiManager()"); }
}

ak::ui::uiManager::~uiManager() {
	A_OBJECT_DESTROYING
	// Delete the timer signal linker first, so all objects will be disconnected propertly
	if (my_timerSignalLinker != nullptr) { delete my_timerSignalLinker; my_timerSignalLinker = nullptr; }

}

void ak::ui::uiManager::setColorStyle(
	const ak::ui::colorStyle *								_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow));
	
	my_window->setStyleSheet(sheet);

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
	my_window->setStyleSheet(sheet);

	my_window->statusBar()->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow));
	my_progressBar->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow));
	my_statusLabel->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorTransparent));

	// TTB
	sheet = my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow, "QToolBar{border: 0px;", "}");
	if (sheet.isEmpty()) {
		sheet = "QToolBar{border: 0px;}";
	}
	my_tabToolBar->SetStylesheet(sheet);
	
	sheet = my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow, "QTabWidget{", "}");
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBorderColorHeader, "QTabWidget::pane{border: 0px solid; border-top-width: 1px; border-bottom-width: 1px;", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caForegroundColorHeader,
		"QTabWidget::tab-bar{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caForegroundColorHeader,
		"QTabBar::tab{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorFocus | TYPE_COLORAREA::caForegroundColorFocus,
		"QTabBar::tab:hover{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorSelected | TYPE_COLORAREA::caForegroundColorSelected,
		"QTabBar::tab:selected{", "}"));

	if (sheet.length() == 0) {
		// Provide manual stylesheet to fix styling issue in toolBar
		sheet = "QTabWidget{}\n"
				"QTabWidget::pane{border: 0px solid #707070; border-top-width: 1px; border-bottom-width: 1px;}\n"
			;
	}

	my_tabToolBar->SetTabBarStylesheet(sheet);
	my_tabToolBar->SetHideButtonStylesheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow));
	for (my_mapTabToolBarContainerIterator itm = my_mapTabToolBarContainer.begin(); itm != my_mapTabToolBarContainer.end(); itm++) {
		assert(itm->second != nullptr); // Nullptr stored
		//itm->second->setColorStyle(my_colorStyle);
	}

}

void ak::ui::uiManager::removeChildObject(
	aObject *								_child
) {
	ui::core::aObject::removeChildObject(_child);
	if (_child->objectType() == ak::ui::core::objectType::oTabToolbarPage) {
		ttb::page * page = nullptr;
		page = dynamic_cast<ttb::page *>(_child);
		assert(page != nullptr); // Cast failed
		my_tabToolBar->DestroyPage(page->index());
	}
	else {
		my_window->takeCentralWidget();
		my_window->setCentralWidget(nullptr);
	}
}

void ak::ui::uiManager::setAlias(
	const QString &							_alias
) {
	ui::core::aObject::setAlias(_alias);
	my_window->setObjectName(my_alias);
	my_window->statusBar()->setObjectName(my_alias + "__StatusBar");
	my_progressBar->setObjectName(my_alias + "__ProgressBar");
	my_statusLabel->setObjectName(my_alias + "__StatusLabel");
	//my_tabToolBar->setObjectName(my_alias + "__TTB_ToolBar");
	//my_tabToolBar->setObjectName(my_alias + "__TTB_TabBar");
	//my_tabToolBar->setObjectName(my_alias + "__TTB_HideButton");
}

void ak::ui::uiManager::addObjectSettingsToValue(
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
	str = ak::ui::core::toQString(my_objectType).toStdString();
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

void ak::ui::uiManager::restoreSettings(
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

void ak::ui::uiManager::setCentralWidget(
	QWidget *														_centralWidget
) {
	assert(_centralWidget != nullptr); // nullptr provided
	my_window->takeCentralWidget();
	my_window->setCentralWidget(_centralWidget);
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
		case ak::ui::core::dock_dockLeft:
			my_window->addDockWidget(Qt::LeftDockWidgetArea, _dock);
			break;
		case ak::ui::core::dock_dockRight:
			my_window->addDockWidget(Qt::RightDockWidgetArea, _dock);
			break;
		case ak::ui::core::dock_dockBottom:
			my_window->addDockWidget(Qt::BottomDockWidgetArea, _dock);
			break;
		default:
			assert(0); // Not implemented yet
			break;
		}
		//_dock->resize(200, 200);
		my_window->resizeDocks({ _dock }, { 0 }, Qt::Orientation::Vertical);
		my_window->resizeDocks({ _dock }, { 0 }, Qt::Orientation::Horizontal);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::uiManager::addDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::uiManager::addDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::uiManager::addDock()"); }
}

void ak::ui::uiManager::tabifyDock(
	ak::ui::qt::dock *							_mainDock,
	ak::ui::qt::dock *							_subDock
) {
	assert(_mainDock != nullptr); // Cast failed
	assert(_subDock != nullptr); // Cast failed

	// tabify dock
	my_window->tabifyDockWidget(_mainDock, _subDock);
	my_window->resizeDocks({ _subDock }, { 0 }, Qt::Orientation::Vertical);
	my_window->resizeDocks({ _subDock }, { 0 }, Qt::Orientation::Horizontal);
	_mainDock->raise();
}

void ak::ui::uiManager::setDockPriorityBottomLeft(
	ak::ui::core::dockLocation						_dockLocation
) {
	try {
		switch (_dockLocation)
		{
		case ak::ui::core::dock_dockLeft:
			my_window->setCorner(Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea); break;
		case ak::ui::core::dock_dockBottom:
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
		case ak::ui::core::dock_dockRight:
			my_window->setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::RightDockWidgetArea); break;
		case ak::ui::core::dock_dockBottom:
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

void ak::ui::uiManager::showMaximized(void) { my_window->showMaximized(); }

void ak::ui::uiManager::showMinimized(void) { my_window->showMinimized(); }

void ak::ui::uiManager::close(void) { my_window->close(); }

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

// #############################################################################################################

void ak::ui::uiManager::setTabToolBarVisible(
	bool						_vis
) {
	my_tabToolBar->setVisible(_vis);
}

ak::ui::ttb::page * ak::ui::uiManager::createTabToolbarSubContainer(
	const QString &				_text
) {
	tt::Page * page = my_tabToolBar->AddPage(_text);
	ak::ui::ttb::page * p = new ak::ui::ttb::page(my_messenger, page, _text);
	if (my_colorStyle != nullptr) { p->setColorStyle(my_colorStyle); setColorStyle(my_colorStyle); }
	return p;
}

void ak::ui::uiManager::addTabToolbarWidget(
	ak::UID						_parentUid,
	ak::UID						_objectUid
) {
	assert(0); // Not implemented yet
}

ak::ID ak::ui::uiManager::currentTabToolbarTab(void) const { return my_tabToolBar->CurrentTab(); }

int ak::ui::uiManager::tabToolbarTabCount(void) const { return my_tabToolBar->TabCount(); }

void ak::ui::uiManager::setCurrentTabToolBarTab(
	ak::ID						_tabID
) {
	assert(_tabID >= 0 && _tabID < my_tabToolBar->TabCount());	// Index out of range
	my_tabToolBar->SetCurrentTab(_tabID);
}

// #############################################################################################################

QMainWindow * ak::ui::uiManager::window(void) const { return my_window; }

void ak::ui::uiManager::setWindowTitle(
	const QString &														_title
) { my_window->setWindowTitle(_title); }

QString ak::ui::uiManager::windowTitle(void) const {
	return my_window->windowTitle();
}

// #############################################################################################################

// Slots

void ak::ui::uiManager::slotRestoreSetting(
	const QByteArray &					_actualState
) {
	my_window->restoreState(_actualState);
}

void ak::ui::uiManager::slotTabToolbarTabClicked(int _index) {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eTabToolbarClicked, _index);
}
void ak::ui::uiManager::slotTabToolbarTabCurrentTabChanged(int _index) {
	my_messenger->sendMessage(my_uid, ak::core::eventType::eTabToolbarChanged, _index);
}