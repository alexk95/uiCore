/*
 * Example.cpp
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// Application header
#include <Example.h>			// Corresponding header
#include <ExampleNotifier.h>	// Notifier

// AK header
#include <akAPI/uiAPI.h>
#include <akCore/aException.h>
#include <akGui/aColorStyle.h>
#include <akWidgets/aDockWidget.h>

// Qt header
#include <qstring.h>
#include <qsettings.h>

// C++ header
#include <vector>
#include <array>

#define TXT_Bright "Bright mode"
#define TXT_Dark "Dark mode"
#define ICO_Bright "Sun"
#define ICO_Dark "Moon"

#define APP_SETTINGS_VERSION "1.0"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>			// Writer
#include <rapidjson/stringbuffer.h>		// String buffer

// #################################################################################################

#include <aDockWatcherButtonWidget.h>

Example::Example()
	: my_settingColor(255,255,0)
{
	// Create own UID
	my_uid = ak::uiAPI::createUid();
	
	my_ui.mainWindow = ak::invalidUID;

	// Setup icon manager

	// App dir
	try { ak::uiAPI::addIconSearchPath(".\\Icons\\"); }
	catch (...) {}

	// Icons
	try { ak::uiAPI::addIconSearchPath(QString(qgetenv("UI_CORE_ROOT") + "\\Icons\\")); }
	catch (...) {}

	my_ui.logInDialog = ak::uiAPI::createLogInDialog(my_uid, true, "OpenTwin", "Username", "TheHashedPassword");

	// Create my notifier
	my_notifier = new ExampleNotifier(this);
	ak::uiAPI::registerUidNotifier(my_ui.logInDialog, my_notifier);

	// Show log in dialog
	ak::dialogResult result = ak::uiAPI::logInDialog::showDialog(my_ui.logInDialog);
	ak::uiAPI::object::destroy(my_ui.logInDialog);

	if (result == ak::resultOk) {
		// Fill UI
		setupUi();
		
		// Restore last color style settings
		QSettings settings("AK", "uiCoreExample");
		ak::uiAPI::restoreStateColorStyle(settings.value("UI.ColorStyle", "").toString().toStdString(), APP_SETTINGS_VERSION);

		// Check the current color style (after it was restored)
		const ak::aColorStyle * currentColorStyle = ak::uiAPI::getCurrentColorStyle();
		if (currentColorStyle->getColorStyleName() == "Default" || currentColorStyle->getColorStyleName() == "") {
			ak::uiAPI::toolButton::setText(my_ui.ttb_aColorStyle, TXT_Dark);
			ak::uiAPI::toolButton::setIcon(my_ui.ttb_aColorStyle, ICO_Dark, "32");
			ak::uiAPI::toolButton::setToolTip(my_ui.ttb_aColorStyle, "Set the color Style to Dark");
		}
		else {
			ak::uiAPI::toolButton::setText(my_ui.ttb_aColorStyle, TXT_Bright);
			ak::uiAPI::toolButton::setIcon(my_ui.ttb_aColorStyle, ICO_Bright, "32");
			ak::uiAPI::toolButton::setToolTip(my_ui.ttb_aColorStyle, "Set the color Style to Bright");
		}

		// Show the window
		ak::uiAPI::window::showMaximized(my_ui.mainWindow);
		
		// Create and start the timer to restore the last window state
		my_timerRestoreSettings = ak::uiAPI::createTimer(my_uid);
		ak::uiAPI::registerUidNotifier(my_timerRestoreSettings, my_notifier);
		ak::uiAPI::timer::shoot(my_timerRestoreSettings, 0);

		my_testButton->setEnabled(true);
		my_testButton->refreshData();

		ak::uiAPI::promptDialog::show("Test message tada aa ad ad awd kladg ad adjada dafsadf a dasfd kasdfkasdf adsk afsd afdkaf ksdf adg faksdka dkaf d", "Title", ak::promptIconLeft, "Test", "32");

		// Run the main application
		ak::uiAPI::exec();

		// Save state
		QString cfg = ak::uiAPI::saveStateWindow(APP_SETTINGS_VERSION).c_str();
		settings.setValue("UI.Config", cfg);
		cfg = ak::uiAPI::saveStateColorStyle(APP_SETTINGS_VERSION).c_str();
		settings.setValue("UI.ColorStyle", cfg);
	}
}

Example::~Example() {}

void Example::eventCallback(
	ak::UID					_sender,
	ak::eventType		_eventType,
	int						_info1,
	int						_info2
) {
	try {
		try {
			if (my_ui.mainWindow != ak::invalidUID) {
				QString str("Event { Sender=\"");
				str.append(QString::number(_sender));
				str.append("\"; EventType = \"");
				str.append(ak::uiAPI::special::toString(_eventType));
				str.append("\"; Info1=\"");
				str.append(QString::number(_info1));
				str.append("\"; Info2=\"");
				if (_eventType == ak::etKeyPressed) { str.append(ak::uiAPI::special::toString((ak::keyType)_info2)); }
				else { str.append(QString::number(_info2)); }
				str.append("\"; }\n");
				ak::uiAPI::textEdit::appendText(my_ui.outputWidget, str);
			}
			if (_eventType == ak::etClicked) {
				if (_sender == my_ui.logInDialog) {
					// Validate logic

					// ...

					QString str1 = ak::uiAPI::logInDialog::getUsername(my_ui.logInDialog);
					QString str2 = ak::uiAPI::logInDialog::getPassword(my_ui.logInDialog);
					bool d = ak::uiAPI::logInDialog::getSavePassword(my_ui.logInDialog);

					// Show main UI and close dialog
					ak::uiAPI::logInDialog::close(my_ui.logInDialog, ak::resultOk);
				}
				else if (_sender == my_ui.ttb_aExit) {
					// Close the main window
					ak::uiAPI::window::close(my_ui.mainWindow);
				}
				else  if (_sender == my_ui.ttb_aColorStyle) {

					// Disable notifier to not receive changed messages
					my_notifier->disable();

					// Change color style
					if (ak::uiAPI::toolButton::getText(my_ui.ttb_aColorStyle) == TXT_Bright) {
						ak::uiAPI::textEdit::appendText(my_ui.outputWidget, "Set: ColorStyle { Style=\"DefaultBright\"; }");
						ak::uiAPI::setDefaultColorStyle();
						ak::uiAPI::toolButton::setIcon(my_ui.ttb_aColorStyle, ICO_Dark, "32");
						ak::uiAPI::toolButton::setText(my_ui.ttb_aColorStyle, TXT_Dark);
						ak::uiAPI::toolButton::setToolTip(my_ui.ttb_aColorStyle, "Set the color Style to Dark");
					}
					else {
						ak::uiAPI::textEdit::appendText(my_ui.outputWidget, "Set: ColorStyle { Style=\"DefaultDark\"; }");
						ak::uiAPI::setDefaultDarkColorStyle();
						ak::uiAPI::toolButton::setIcon(my_ui.ttb_aColorStyle, ICO_Bright, "32");
						ak::uiAPI::toolButton::setText(my_ui.ttb_aColorStyle, TXT_Bright);
						ak::uiAPI::toolButton::setToolTip(my_ui.ttb_aColorStyle, "Set the color Style to Bright");
					}

					// Enable the notifier again
					my_notifier->enable();
				}
				else if (_sender == my_ui.ttb_aTest) {
					ak::uiAPI::window::setWaitingAnimationVisible(my_ui.mainWindow, true);
				}
				else if (_sender == my_ui.ttb_aTest2) {
					ak::uiAPI::window::setWaitingAnimationVisible(my_ui.mainWindow, false);
				}
			}
			else if (_sender == my_ui.propertiesWidget && _eventType == ak::etChanged) {
				QString msg("PropteryChange{ id=\"");
				msg.append(QString::number(_info1));
				msg.append("\"; Name=\"");
				msg.append(ak::uiAPI::propertyGrid::getItemName(my_ui.propertiesWidget, _info1));
				msg.append("\"; Type=\"");

				switch (ak::uiAPI::propertyGrid::getItemValueType(my_ui.propertiesWidget, _info1))
				{
				case ak::vtBool:
					if (ak::uiAPI::propertyGrid::getItemValueBool(my_ui.propertiesWidget, _info1)) {
						msg.append("Boolean\"; Value=\"True");
					}
					else { msg.append("Boolean\"; Value=\"False"); }
					break;
				case ak::vtColor:
					msg.append("Color\"; Value=\"");
					my_settingColor = ak::uiAPI::propertyGrid::getItemValueColor(my_ui.propertiesWidget, _info1);
					msg.append(my_settingColor.toRGBString(":"));
					break;
				case ak::vtDouble:
					msg.append("Double\"; Value=\"");
					msg.append(QString::number(ak::uiAPI::propertyGrid::getItemValueDouble(my_ui.propertiesWidget, _info1)));
					break;
				case ak::vtInt:
					msg.append("Integer\"; Value=\"");
					msg.append(QString::number(ak::uiAPI::propertyGrid::getItemValueInteger(my_ui.propertiesWidget, _info1)));
					break;
				case ak::vtSelection:
					msg.append("Selection\"; Value=\"");
					msg.append(ak::uiAPI::propertyGrid::getItemValueSelection(my_ui.propertiesWidget, _info1));
					break;
				case ak::vtString:
					msg.append("String\"; Value=\"");
					msg.append(ak::uiAPI::propertyGrid::getItemValueString(my_ui.propertiesWidget, _info1));
					break;
				default:
					assert(0); // Unknown type
					break;
				}
				msg.append("\"; }");
				ak::uiAPI::textEdit::appendText(my_ui.outputWidget, msg);
				
			}
			else if (_sender == my_timerRestoreSettings && _eventType == ak::eventType::etTimeout) {
				// Load last settings
				QSettings settings("AK", "uiCoreExample");
				QString lastConfigString = settings.value("UI.Config", "").toString();
				if (lastConfigString.length() > 0) {
					std::string s(lastConfigString.toStdString());
					ak::uiAPI::restoreStateWindow(s, APP_SETTINGS_VERSION);
				}
			}
		}
		catch (const ak::aException & e) { throw ak::aException(e, "Example::eventCallback()"); }
		catch (const std::exception & e) { throw ak::aException(e.what(), "Example::eventCallback()"); }
		catch (...) { throw ak::aException("Unknown error", "Example::eventCallback()"); }
	}
	catch (const ak::aException & e) {
		ak::uiAPI::promptDialog::show(e.what(), "Error");
	}
}

void Example::setupUi(void) {
	// Create default UI
	my_ui.mainWindow = ak::uiAPI::createWindow(my_uid);
	ak::uiAPI::object::setAlias(my_ui.mainWindow, "Window_Main");

	// From this point on exceptions can be displayed in a message box since the UI is created
	try {
		try {
			// Setup UI
			ak::uiAPI::window::setDockBottomLeftPriority(my_ui.mainWindow, ak::dockLeft);
			ak::uiAPI::window::setDockBottomRightPriority(my_ui.mainWindow, ak::dockRight);

			// Setup tab toolbar
			ak::uiAPI::window::setTabToolBarVisible(my_ui.mainWindow);
			my_ui.ttb_pFile = ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.mainWindow, "File");
			my_ui.ttb_gNONE = ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.ttb_pFile, "Test");
			my_ui.ttb_aExit = ak::uiAPI::createToolButton(my_uid, "Exit", "ExitAppBlue", "32");
			my_ui.ttb_aColorStyle = ak::uiAPI::createToolButton(my_uid, TXT_Bright, ICO_Bright, "32");
			my_ui.ttb_aTest = ak::uiAPI::createToolButton(my_uid, "Test", "Test", "32");
			my_ui.ttb_aTest2 = ak::uiAPI::createToolButton(my_uid, "Test 2", "Test", "32");
			ak::uiAPI::container::addObject(my_ui.ttb_gNONE, my_ui.ttb_aExit);
			ak::uiAPI::container::addObject(my_ui.ttb_gNONE, my_ui.ttb_aColorStyle);
			ak::uiAPI::container::addObject(my_ui.ttb_gNONE, my_ui.ttb_aTest);
			ak::uiAPI::container::addObject(my_ui.ttb_gNONE, my_ui.ttb_aTest2);

			// Create widgets
			my_ui.treeWidget = ak::uiAPI::createTree(my_uid);
			my_ui.propertiesWidget = ak::uiAPI::createPropertyGrid(my_uid);
			my_ui.outputWidget = ak::uiAPI::createTextEdit(my_uid, "Welcome to the test application\n");
			my_ui.tabViewWidget = ak::uiAPI::createTabView(my_uid);
			my_ui.tester = ak::uiAPI::createTextEdit(my_uid);
			my_ui.table1 = ak::uiAPI::createTable(my_uid, 2, 2);
			my_ui.table2 = ak::uiAPI::createTable(my_uid, 3, 3);
			my_ui.specialTabBar = ak::uiAPI::createSpecialTabBar(my_uid);

			// Create docks
			my_ui.dockOutput = ak::uiAPI::createDock(my_uid, "Output");
			my_ui.dockProperties = ak::uiAPI::createDock(my_uid, "Properties");
			my_ui.dockTester = ak::uiAPI::createDock(my_uid, "Tester");
			my_ui.dockTree = ak::uiAPI::createDock(my_uid, "Tree");

			ak::uiAPI::object::setAlias(my_ui.dockOutput, "Dock.Output");
			ak::uiAPI::object::setAlias(my_ui.dockProperties, "Dock.Properties");
			ak::uiAPI::object::setAlias(my_ui.dockTester, "Dock.Tester");
			ak::uiAPI::object::setAlias(my_ui.dockTree, "Dock.Tree");
			
			ak::uiAPI::dock::setCentralWidget(my_ui.dockOutput, my_ui.outputWidget);
			ak::uiAPI::dock::setCentralWidget(my_ui.dockProperties, my_ui.propertiesWidget);
			ak::uiAPI::dock::setCentralWidget(my_ui.dockTester, my_ui.tester);
			ak::uiAPI::dock::setCentralWidget(my_ui.dockTree, my_ui.treeWidget);

			my_testButton = new ak::aDockWatcherButtonWidget{ ak::uiAPI::getIcon("Test", "32"), "Docks" };
			my_testButton->setEnabled(false);
			my_testButton->addWatch(ak::uiAPI::object::get<ak::aDockWidget>(my_ui.dockOutput), "Output");
			my_testButton->addWatch(ak::uiAPI::object::get<ak::aDockWidget>(my_ui.dockProperties), "Properties");
			my_testButton->addWatch(ak::uiAPI::object::get<ak::aDockWidget>(my_ui.dockTester), "Tester");
			my_testButton->addWatch(ak::uiAPI::object::get<ak::aDockWidget>(my_ui.dockTree), "Tree");

			// Display docks
			ak::uiAPI::window::addDock(my_ui.mainWindow, my_ui.dockOutput, ak::dockBottom);
			ak::uiAPI::window::addDock(my_ui.mainWindow, my_ui.dockTree, ak::dockLeft);
			ak::uiAPI::window::addDock(my_ui.mainWindow, my_ui.dockProperties, ak::dockLeft);
			ak::uiAPI::window::tabifyDock(my_ui.mainWindow, my_ui.dockOutput, my_ui.dockTester);

			// Setup widgets
			ak::uiAPI::tree::setAutoSelectAndDeselectChildrenEnabled(my_ui.treeWidget, true);
			ak::uiAPI::tree::setMultiSelectionEnabled(my_ui.treeWidget, true);
			ak::uiAPI::tree::setFilterVisible(my_ui.treeWidget);
		
			ak::uiAPI::tabView::setSpecialTabBar(my_ui.tabViewWidget, my_ui.specialTabBar);
			ak::uiAPI::specialTabBar::addColor(my_ui.specialTabBar, 1, ak::aColor(255, 0, 0));

			ak::uiAPI::tabView::addTab(my_ui.tabViewWidget, my_ui.table1, "Test 1");
			ak::uiAPI::tabView::addTab(my_ui.tabViewWidget, my_ui.table2, "Test 2");

			ak::uiAPI::textEdit::setReadOnly(my_ui.outputWidget);

			ak::uiAPI::contextMenu::clear(my_ui.outputWidget);
			ak::uiAPI::contextMenu::addSeparator(my_ui.outputWidget);
			ak::uiAPI::contextMenu::addItem(my_ui.outputWidget, "Clear", "Clear", "32", ak::cmrClear);
			ak::uiAPI::contextMenu::addSeparator(my_ui.outputWidget);
			
			ak::uiAPI::propertyGrid::setGroupStateIcons(my_ui.propertiesWidget, "ArrowGreenDown", "32", "ArrowBlueRight", "32");

			// Set central widget
			//ak::uiAPI::window::setCentralWidget(my_ui.mainWindow, my_ui.tabViewWidget);
			ak::uiAPI::window::setCentralWidget(my_ui.mainWindow, my_testButton);

			// Register notifier
			ak::uiAPI::registerUidNotifier(my_ui.propertiesWidget, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.table1, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.table2, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.treeWidget, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.ttb_aExit, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.ttb_aColorStyle, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.tester, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.ttb_aTest, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.ttb_aTest2, my_notifier);
			ak::uiAPI::registerUidNotifier(my_ui.mainWindow, my_notifier);

			// Create default data
			defaultData();

			ak::uiAPI::window::setStatusLabelText(my_ui.mainWindow, "test");
			ak::uiAPI::window::setStatusProgressContinuous(my_ui.mainWindow);
			ak::uiAPI::window::setStatusLabelVisible(my_ui.mainWindow, true);
			ak::uiAPI::window::setStatusProgressVisible(my_ui.mainWindow, true);
			ak::uiAPI::window::setStatusLabelVisible(my_ui.mainWindow, false);

			// Tester
			ak::UID p = ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.mainWindow, "Tester1");
			ak::UID g1 = ak::uiAPI::createTabToolBarSubContainer(my_uid, p, "Tester1");
			ak::UID g2 = ak::uiAPI::createTabToolBarSubContainer(my_uid, p, "Tester2");
			ak::UID a1 = ak::uiAPI::createToolButton(my_uid, "A1", "Test", "32");
			ak::UID a2 = ak::uiAPI::createToolButton(my_uid, "A2", "Test", "32");
			ak::UID a3 = ak::uiAPI::createToolButton(my_uid, "A3", "Test", "32");
			ak::UID a4 = ak::uiAPI::createToolButton(my_uid, "A4", "Test", "32");
			ak::uiAPI::container::addObject(g1, a1);
			ak::uiAPI::container::addObject(g1, a2);
			ak::uiAPI::container::addObject(g2, a3);
			ak::uiAPI::container::addObject(g2, a4);
			
			my_testingData.push_back(a4);
			my_testingData.push_back(a3);
			my_testingData.push_back(a2);
			my_testingData.push_back(a1);
			my_testingData.push_back(g1);
			my_testingData.push_back(g2);
			my_testingData.push_back(p);

			ak::uiAPI::container::setEnabled(p, false);

			ak::uiAPI::window::setWaitingAnimation(my_ui.mainWindow, "SpinnerSun");

		}
		catch (const ak::aException & e) { throw ak::aException(e, "Example::Example()"); }
		catch (const std::exception & e) { throw ak::aException(e.what(), "Example::Example()"); }
		catch (...) { throw ak::aException("Unknown error", "Example::Example()"); }
	}
	catch (const std::exception & e) {
		ak::uiAPI::promptDialog::show(e.what(), "Error");
	}
}

void Example::defaultData(void) {
	// Tree

	//	|--	A
	//	|	|--	A1
	//	|	|	|--	A1A
	//	|	|	|--	A1B
	//	|	|	---	A1C
	//	|	|--	A2
	//	|	|	|--	A2A
	//	|	|	---	A2B
	//	|	---	A3
	//	|		|--	A3A
	//	|		|--	A3B
	//	|		---	A3C
	//	|--	B
	//	|	|--	B1
	//	|	|	|--	B1A
	//	|	|	---	B1B
	//	|	---	B2
	//	|		|-- B2A
	//	|		|--	B2B
	//	|		---	B2C
	//	---	C
	//		|--	C1
	//		|	|--	C1A
	//		|	---	C1B
	//		---	C2
	//			---	C2A

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A1|A1A", '|');
	
	ak::ID i = ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A1|A1B", '|');
	ak::uiAPI::tree::setItemIcon(my_ui.treeWidget, i, "Test", "32");
	
	ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A1|A1C", '|');

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A2|A2A", '|');
	ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A2|A2B", '|');

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A3|A3A", '|');
	
	i = ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A3|A3B", '|');
	ak::uiAPI::tree::setItemIcon(my_ui.treeWidget, i, "Sun", "32");

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "A|A3|A3C", '|');

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "B|B1|B1A", '|');
	ak::uiAPI::tree::addItem(my_ui.treeWidget, "B|B1|B1B", '|');

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "B|B2|B2A", '|');
	
	i = ak::uiAPI::tree::addItem(my_ui.treeWidget, "B|B2|B2B", '|');
	ak::uiAPI::tree::setItemIcon(my_ui.treeWidget, i, "Moon", "32");

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "B|B2|B2C", '|');

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "C|C1|C1A", '|');
	ak::uiAPI::tree::addItem(my_ui.treeWidget, "C|C1|C1B", '|');

	ak::uiAPI::tree::addItem(my_ui.treeWidget, "C|C2|C2A", '|');

	// Property grid

	// Create groups
	std::vector <ak::ID> ids;
	ak::uiAPI::propertyGrid::addGroup(my_ui.propertiesWidget, QColor(70, 70, 200, 200), "Test group");

	ids.push_back(ak::uiAPI::propertyGrid::addItem(my_ui.propertiesWidget, true, "Test group", "Test int", 13));
	ids.push_back(ak::uiAPI::propertyGrid::addItem(my_ui.propertiesWidget, true, "Test group", "Test string", "Some text"));

	ak::uiAPI::propertyGrid::addGroup(my_ui.propertiesWidget, QColor(200, 70, 70, 200), "Test group 2");
	ids.push_back(ak::uiAPI::propertyGrid::addItem(my_ui.propertiesWidget, true, "Test group 2", "Test double", 10.0));
	ids.push_back(ak::uiAPI::propertyGrid::addItem(my_ui.propertiesWidget, true, "Test group 2", "Test bool", true));
	
	std::vector<QString> v;
	v.push_back("Test");
	v.push_back("Some other item");
	v.push_back("And another setting");
	ak::uiAPI::propertyGrid::addGroup(my_ui.propertiesWidget, QColor(70, 200, 70, 200), "Test group 3");
	ids.push_back(ak::uiAPI::propertyGrid::addItem(my_ui.propertiesWidget, false, "Test group 3", "Test selection", v, "Test"));
	ids.push_back(ak::uiAPI::propertyGrid::addItem(my_ui.propertiesWidget, false, "Test group 3", "Test color", my_settingColor));

	for (auto theId : ids) {
		//ak::uiAPI::propertyGrid::setItemIsReadOnly(my_ui.propertiesWidget, theId);
	}
}
