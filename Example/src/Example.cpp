/*
 * Example.cpp
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include <Example.h>			// Corresponding header
#include <ExampleNotifier.h>	// Notifier
#include <ak_exception.h>		// Error handling
#include <ak_uiAPI.h>			// uiAPI
#include <ak_ui_core.h>			// dockLocation type

#include <qstring.h>
#include <qsettings.h>

#include <vector>
#include <array>

#define TXT_Bright "Bright mode"
#define TXT_Dark "Dark mode"
#define ICO_Bright "Sun"
#define ICO_Dark "Moon"

#define WELCOME_ID_RECENTS 0
#define WELCOME_ID_OPEN 1
#define WELCOME_ID_NEW 2

#include <rapidjson/document.h>
#include <rapidjson/writer.h>			// Writer
#include <rapidjson/stringbuffer.h>		// String buffer

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

	ak::ui::core::dialogResult result = ak::uiAPI::dialog::show(my_ui.logInDialog);
	ak::uiAPI::obj::destroy(my_ui.logInDialog);

	if (result == ak::ui::core::resultOk) {
		setupUi();
		
		my_tester = ak::uiAPI::createTimer(my_uid);
		ak::uiAPI::registerUidNotifier(my_tester, my_notifier);
		ak::uiAPI::obj::shoot(my_tester, 0);
		
		ak::uiAPI::obj::showMaximized(my_ui.mainWindow);
		
		ak::uiAPI::exec();
	}
}

Example::~Example() {}

void Example::eventCallback(
	ak::UID					_sender,
	ak::core::eventType		_eventType,
	int						_info1,
	int						_info2
) {
	try {
		if (my_ui.mainWindow != ak::invalidUID) {
			QString str("Event { Sender=\"");
			str.append(QString::number(_sender));
			str.append("\"; EventType = \"");
			str.append(ak::uiAPI::toString(_eventType));
			str.append("\"; Info1=\"");
			str.append(QString::number(_info1));
			str.append("\"; Info2=\"");
			if (_eventType == ak::core::eKeyPressed) { str.append(ak::uiAPI::toString((ak::ui::core::keyType)_info2)); }
			else { str.append(QString::number(_info2)); }
			str.append("\"; }");
			ak::uiAPI::obj::appendText(my_ui.outputWidget, str);
		}
		if (_eventType == ak::core::eventType::eClicked) {
			if (_sender == my_ui.logInDialog) {
				// Validate logic

				// ...

				QString str1 = ak::uiAPI::dialog::username(my_ui.logInDialog);
				QString str2 = ak::uiAPI::dialog::password(my_ui.logInDialog);
				bool d = ak::uiAPI::dialog::savePassword(my_ui.logInDialog);

				// Show main UI and close dialog
				ak::uiAPI::dialog::close(my_ui.logInDialog, ak::ui::core::resultOk);
			}
			else if (_sender == my_ui.ttb_aExit) {
				// Close the main window
				QSettings settings("AK", "uiCoreExample");
				QString cfg = ak::uiAPI::getSettingsJSON().c_str();
				settings.setValue("UI.Config", cfg);
				ak::uiAPI::closeWindow(my_ui.mainWindow);
			}
			else  if (_sender == my_ui.ttb_aColorStyle) {

				// Disable notifier to not receive changed messages
				my_notifier->disable();

				// Change color style
				if (ak::uiAPI::obj::getText(my_ui.ttb_aColorStyle) == TXT_Bright) {
					ak::uiAPI::obj::appendText(my_ui.outputWidget, "Set: ColorStyle { Style=\"DefaultBright\"; }");
					ak::uiAPI::setDefaultColorStyle();
					ak::uiAPI::obj::setIcon(my_ui.ttb_aColorStyle, ICO_Dark, "32");
					ak::uiAPI::obj::setText(my_ui.ttb_aColorStyle, TXT_Dark);
				}
				else {
					ak::uiAPI::obj::appendText(my_ui.outputWidget, "Set: ColorStyle { Style=\"DefaultDark\"; }");
					ak::uiAPI::setDefaultDarkColorStyle();
					ak::uiAPI::obj::setIcon(my_ui.ttb_aColorStyle, ICO_Bright, "32");
					ak::uiAPI::obj::setText(my_ui.ttb_aColorStyle, TXT_Bright);
				}

				// Enable the notifier again
				my_notifier->enable();
			}
			else if (_sender == my_ui.ttb_aTest) {
				ak::uiAPI::obj::setEnabled(my_ui.ttb_aTest, false);
				ak::uiAPI::obj::setEnabled(my_ui.ttb_aTest2, true);
				ak::uiAPI::obj::setVisible(my_ui.dockProperties, false);
				ak::uiAPI::obj::setVisible(my_ui.dockOutput, false);
				//ak::uiAPI::obj::setVisible(my_ui.dockTester, false);
				ak::uiAPI::obj::setVisible(my_ui.dockTree, false);
			}
			else if (_sender == my_ui.ttb_aTest2) {
				ak::uiAPI::obj::setEnabled(my_ui.ttb_aTest, true);
				ak::uiAPI::obj::setEnabled(my_ui.ttb_aTest2, false);
				ak::uiAPI::obj::setVisible(my_ui.dockProperties, true);
				ak::uiAPI::obj::setVisible(my_ui.dockOutput, true);
				//ak::uiAPI::obj::setVisible(my_ui.dockTester, true);
				ak::uiAPI::obj::setVisible(my_ui.dockTree, true);
			}
		}
		else if (_sender == my_ui.propertiesWidget && _eventType == ak::core::eChanged) {
			QString msg("PropteryChange{ id=\"");
			msg.append(QString::number(_info1));
			msg.append("\"; Name=\"");
			msg.append(ak::uiAPI::itm::getText(my_ui.propertiesWidget, _info1));
			msg.append("\"; Type=\"");

			switch (ak::uiAPI::itm::getValueType(my_ui.propertiesWidget, _info1))
			{
			case ak::core::valueType::vBool:
				if (ak::uiAPI::itm::getValueBoolean(my_ui.propertiesWidget, _info1)) {
					msg.append("Boolean\"; Value=\"True");
				}
				else { msg.append("Boolean\"; Value=\"False"); }
				break;
			case ak::core::valueType::vColor:
				msg.append("Color\"; Value=\"");
				my_settingColor = ak::uiAPI::itm::getValueColor(my_ui.propertiesWidget, _info1);
				msg.append(my_settingColor.toRGBString(":"));
				break;
			case ak::core::valueType::vDouble:
				msg.append("Double\"; Value=\"");
				msg.append(QString::number(ak::uiAPI::itm::getValueDouble(my_ui.propertiesWidget, _info1)));
				break;
			case ak::core::valueType::vInt:
				msg.append("Integer\"; Value=\"");
				msg.append(QString::number(ak::uiAPI::itm::getValueInteger(my_ui.propertiesWidget, _info1)));
				break;
			case ak::core::valueType::vSelection:
				msg.append("Selection\"; Value=\"");
				msg.append(ak::uiAPI::itm::getValueSelection(my_ui.propertiesWidget, _info1));
				break;
			case ak::core::valueType::vString:
				msg.append("String\"; Value=\"");
				msg.append(ak::uiAPI::itm::getValueString(my_ui.propertiesWidget, _info1));
				break;
			default:
				assert(0); // Unknown type
				break;
			}
			msg.append("\"; }");
			ak::uiAPI::obj::appendText(my_ui.outputWidget, msg);
		}
		else if (_sender == my_tester && _eventType == ak::core::eventType::eTimeout) {
			// Load last settings
			QSettings settings("AK", "uiCoreExample");
			QString lastConfigString = settings.value("UI.Config", "").toString();
			if (lastConfigString.length() > 0) {
				std::string s(lastConfigString.toStdString());
				
				ak::uiAPI::setupSettings(s.c_str());

				QString currentColorStyleName = ak::uiAPI::getCurrentColorStyleName();
				if (currentColorStyleName == "Default" || currentColorStyleName == "") {
					ak::uiAPI::obj::setText(my_ui.ttb_aColorStyle, TXT_Dark);
					ak::uiAPI::obj::setIcon(my_ui.ttb_aColorStyle, ICO_Dark, "32");
				}
				else {
					ak::uiAPI::obj::setText(my_ui.ttb_aColorStyle, TXT_Bright);
					ak::uiAPI::obj::setIcon(my_ui.ttb_aColorStyle, ICO_Bright, "32");
				}

			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "Example::eventCallback()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "Example::eventCallback()"); }
	catch (...) { throw ak::Exception("Unknown error", "Example::eventCallback()"); }
}

void Example::setupUi(void) {
	// Create default UI
	my_ui.mainWindow = ak::uiAPI::createUiManager(my_uid);
	ak::uiAPI::obj::setAlias(my_ui.mainWindow, "MainWindow");

	// From this point on exceptions can be displayed in a message box since the UI is created
	try {
		try {
			// Setup UI
			ak::uiAPI::setDockBottomLeftPriority(my_ui.mainWindow, ak::ui::core::dock_dockLeft);
			ak::uiAPI::setDockBottomRightPriority(my_ui.mainWindow, ak::ui::core::dock_dockRight);

			// Setup tab toolbar
			ak::uiAPI::obj::setTabToolBarVisible(my_ui.mainWindow);
			my_ui.ttb_pFile = ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.mainWindow, "File");
			ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.mainWindow, "Tester1");
			ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.mainWindow, "Tester2");
			ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.mainWindow, "Tester3");
			my_ui.ttb_gNONE = ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.ttb_pFile, "Test");
			my_ui.ttb_aExit = ak::uiAPI::createAction(my_uid, "Exit", "ExitAppBlue", "32");
			my_ui.ttb_aColorStyle = ak::uiAPI::createAction(my_uid, TXT_Bright, ICO_Bright, "32");
			my_ui.ttb_aTest = ak::uiAPI::createAction(my_uid, "Test", "Test", "32");
			my_ui.ttb_aTest2 = ak::uiAPI::createAction(my_uid, "Test 2", "Test", "32");
			ak::uiAPI::obj::addObjectToContainer(my_ui.ttb_gNONE, my_ui.ttb_aExit);
			ak::uiAPI::obj::addObjectToContainer(my_ui.ttb_gNONE, my_ui.ttb_aColorStyle);
			ak::uiAPI::obj::addObjectToContainer(my_ui.ttb_gNONE, my_ui.ttb_aTest);
			ak::uiAPI::obj::addObjectToContainer(my_ui.ttb_gNONE, my_ui.ttb_aTest2);

			// Create widgets
			my_ui.treeWidget = ak::uiAPI::createTree(my_uid);
			my_ui.propertiesWidget = ak::uiAPI::createPropertyGrid(my_uid);
			my_ui.outputWidget = ak::uiAPI::createTextEdit(my_uid, "Welcome to the test application\n");
			my_ui.tabViewWidget = ak::uiAPI::createTabView(my_uid);
			my_ui.tester = ak::uiAPI::createTextEdit(my_uid);
			my_ui.table1 = ak::uiAPI::createTable(my_uid, 2, 2);
			my_ui.table2 = ak::uiAPI::createTable(my_uid, 3, 3);
			my_ui.welcomeScreen = ak::uiAPI::createDefaultWelcomeScreen(my_uid);

			// Create docks
			my_ui.dockOutput = ak::uiAPI::createDock(my_uid, "Output");
			my_ui.dockProperties = ak::uiAPI::createDock(my_uid, "Properties");
			my_ui.dockTester = ak::uiAPI::createDock(my_uid, "Tester");
			my_ui.dockTree = ak::uiAPI::createDock(my_uid, "Tree");

			ak::uiAPI::obj::setAlias(my_ui.dockOutput, "Dock.Output");
			ak::uiAPI::obj::setAlias(my_ui.dockProperties, "Dock.Properties");
			ak::uiAPI::obj::setAlias(my_ui.dockTester, "Dock.Tester");
			ak::uiAPI::obj::setAlias(my_ui.dockTree, "Dock.Tree");
			
			// Set widgets to docks
			ak::uiAPI::obj::setCentralWidget(my_ui.dockOutput, my_ui.outputWidget);
			ak::uiAPI::obj::setCentralWidget(my_ui.dockProperties, my_ui.propertiesWidget);
			ak::uiAPI::obj::setCentralWidget(my_ui.dockTester, my_ui.tester);
			ak::uiAPI::obj::setCentralWidget(my_ui.dockTree, my_ui.treeWidget);

			// Display docks
			ak::uiAPI::addDock(my_ui.mainWindow, my_ui.dockOutput, ak::ui::core::dock_dockBottom);
			ak::uiAPI::addDock(my_ui.mainWindow, my_ui.dockTree, ak::ui::core::dock_dockLeft);
			ak::uiAPI::addDock(my_ui.mainWindow, my_ui.dockProperties, ak::ui::core::dock_dockLeft);
			//ak::uiAPI::tabifyDock(my_ui.mainWindow, my_ui.dockOutput, my_ui.dockTester);

			ak::uiAPI::obj::setVisible(my_ui.dockProperties, false);
			ak::uiAPI::obj::setVisible(my_ui.dockOutput, false);
			//ak::uiAPI::obj::setVisible(my_ui.dockTester, false);
			ak::uiAPI::obj::setVisible(my_ui.dockTree, false);

			// Setup widgets
			ak::uiAPI::obj::setAutoSelectAndDeselectChildrenEnabled(my_ui.treeWidget, true);
			ak::uiAPI::obj::setMultiSelectionEnabled(my_ui.treeWidget, true);
			ak::uiAPI::obj::setReadOnly(my_ui.outputWidget);
			ak::uiAPI::obj::setFilterVisible(my_ui.treeWidget);
			
			for (int fill = 0; fill < 30; fill++) {
				QString txt("Test ");
				if (fill > 0) {
					txt.append(QString::number(fill));
				}
				ak::uiAPI::obj::addItem(my_ui.welcomeScreen, WELCOME_ID_RECENTS, txt, "Test", "32");
			}
		
			ak::uiAPI::obj::addItem(my_ui.welcomeScreen, WELCOME_ID_OPEN, "Open", "Test", "32");
			ak::uiAPI::obj::addItem(my_ui.welcomeScreen, WELCOME_ID_NEW, "Create new", "Test", "32");

			ak::uiAPI::obj::addTab(my_ui.tabViewWidget, my_ui.welcomeScreen, "Welcome");
			ak::uiAPI::obj::addTab(my_ui.tabViewWidget, my_ui.table1, "Test 1");
			ak::uiAPI::obj::addTab(my_ui.tabViewWidget, my_ui.table2, "Test 2");
			
			ak::uiAPI::obj::setEnabled(my_ui.ttb_aTest2, false);

			// Setup aliases
			ak::uiAPI::obj::setAlias(my_ui.dockOutput, "Dock_Output");
			ak::uiAPI::obj::setAlias(my_ui.dockProperties, "Dock_Properties");
			ak::uiAPI::obj::setAlias(my_ui.dockTester, "Dock_Tester");
			ak::uiAPI::obj::setAlias(my_ui.dockTree, "Dock_Tree");
			ak::uiAPI::obj::setAlias(my_ui.mainWindow, "Window_Main");

			// Set central widget
			ak::uiAPI::obj::setCentralWidget(my_ui.mainWindow, my_ui.tabViewWidget);

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
			ak::uiAPI::registerUidNotifier(my_ui.welcomeScreen, my_notifier);

			// Create default data
			defaultData();

			ak::uiAPI::setStatusLabelText(my_ui.mainWindow, "test");
			ak::uiAPI::setStatusProgressContinuous(my_ui.mainWindow);
			ak::uiAPI::setStatusLabelVisible(my_ui.mainWindow, true);
			ak::uiAPI::setStatusProgressVisible(my_ui.mainWindow, true);
			ak::uiAPI::setStatusLabelVisible(my_ui.mainWindow, false);
		}
		catch (const ak::Exception & e) { throw ak::Exception(e, "Example::Example()"); }
		catch (const std::exception & e) { throw ak::Exception(e.what(), "Example::Example()"); }
		catch (...) { throw ak::Exception("Unknown error", "Example::Example()"); }
	}
	catch (const std::exception & e) { ak::uiAPI::special::showMessageBox(my_ui.mainWindow, e.what(), "Error"); }
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

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A1|A1A");
	
	ak::ID i = ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A1|A1B");
	ak::uiAPI::itm::setIcon(my_ui.treeWidget, i, "Test", "32");
	
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A1|A1C");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A2|A2A");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A2|A2B");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A3|A3A");
	
	i = ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A3|A3B");
	ak::uiAPI::itm::setIcon(my_ui.treeWidget, i, "Sun", "32");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A3|A3C");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B1|B1A");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B1|B1B");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B2|B2A");
	
	i = ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B2|B2B");
	ak::uiAPI::itm::setIcon(my_ui.treeWidget, i, "Moon", "32");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B2|B2C");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "C|C1|C1A");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "C|C1|C1B");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "C|C2|C2A");

	// Property grid

	ak::uiAPI::obj::addProperty(my_ui.propertiesWidget, "Test int", 13);
	ak::uiAPI::obj::addProperty(my_ui.propertiesWidget, "Test string", "Some text");
	ak::uiAPI::obj::addProperty(my_ui.propertiesWidget, "Test double", 10.0);
	ak::uiAPI::obj::addProperty(my_ui.propertiesWidget, "Test bool", true);	
	std::vector<QString> v;
	v.push_back("Test");
	v.push_back("Some other item");
	v.push_back("And another setting");
	ak::uiAPI::obj::addProperty(my_ui.propertiesWidget, "Test selection", v, "Test");
	ak::uiAPI::obj::addProperty(my_ui.propertiesWidget, "Test color", my_settingColor);
	
}
