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
#include <vector>

#define TXT_Bright "Bright mode"
#define TXT_Dark "Dark mode"
#define ICO_Bright "Sun"
#define ICO_Dark "Moon"

Example::Example()
{
	// Create own UID
	my_uid = ak::uiAPI::createUid();
	
	// Setup icon manager

	// App dir
	try { ak::uiAPI::addIconSearchPath(".\\Icons\\"); }
	catch (...) {}

	// Icons
	try { ak::uiAPI::addIconSearchPath(QString(qgetenv("UI_CORE_ROOT") + "\\Icons\\")); }
	catch (...) {}

	// Set default dark color style
	ak::uiAPI::setDefaultDarkColorStyle();

	// Create default UI
	my_ui.mainWindow = ak::uiAPI::createUiManager(my_uid);

	// From this point on exceptions can be displayed in a message box since the UI is created
	try {
		try {
			// Setup UI
			ak::uiAPI::setDockBottomLeftPriority(my_ui.mainWindow, ak::ui::core::dockLocation::dockLeft);
			ak::uiAPI::setDockBottomRightPriority(my_ui.mainWindow, ak::ui::core::dockLocation::dockRight);

			// Setup tab toolbar
			ak::uiAPI::obj::setTabToolBarVisible(my_ui.mainWindow);
			my_ui.ttb_pFile = ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.mainWindow, "File");
			my_ui.ttb_gNONE = ak::uiAPI::createTabToolBarSubContainer(my_uid, my_ui.ttb_pFile, "");
			my_ui.ttb_aExit = ak::uiAPI::createAction(my_uid, "Exit", "ExitAppBlue", "32");
			my_ui.ttb_aColorStyle = ak::uiAPI::createAction(my_uid, TXT_Bright, ICO_Bright, "32");
			my_ui.tester = ak::uiAPI::createAction(my_uid, "Delete", "Delete", "32");
			ak::uiAPI::obj::addObjectToContainer(my_ui.ttb_gNONE, my_ui.ttb_aExit);
			ak::uiAPI::obj::addObjectToContainer(my_ui.ttb_gNONE, my_ui.ttb_aColorStyle);
			ak::uiAPI::obj::addObjectToContainer(my_ui.ttb_gNONE, my_ui.tester);

			// Create docks
			my_ui.dockOutput = ak::uiAPI::createDock(my_uid, "Output");
			my_ui.dockProperties = ak::uiAPI::createDock(my_uid, "Properties");
			my_ui.dockTree = ak::uiAPI::createDock(my_uid, "Tree");
			//my_ui.dockTester = ak::uiAPI::createDock(my_uid, "Tester");

			// Create widgets
			my_ui.treeWidget = ak::uiAPI::createTree(my_uid);
			my_ui.propertiesWidget = ak::uiAPI::createPropertyGrid(my_uid);
			my_ui.outputWidget = ak::uiAPI::createTextEdit(my_uid, "Welcome to the test application\n");
			my_ui.tabViewWidget = ak::uiAPI::createTabView(my_uid);
			my_ui.table1 = ak::uiAPI::createTable(my_uid, 2, 2);
			my_ui.table2 = ak::uiAPI::createTable(my_uid, 3, 3);

			// Setup widgets
			ak::uiAPI::obj::setAutoSelectAndDeselectChildrenEnabled(my_ui.treeWidget, true);
			ak::uiAPI::obj::setMultiSelectionEnabled(my_ui.treeWidget, true);
			ak::uiAPI::obj::setReadOnly(my_ui.outputWidget);
			ak::uiAPI::obj::addTab(my_ui.tabViewWidget, my_ui.table1, "Test 1");
			ak::uiAPI::obj::addTab(my_ui.tabViewWidget, my_ui.table2, "Test 2");

			// Set widgets to docks
			ak::uiAPI::obj::setCentralWidget(my_ui.dockOutput, my_ui.outputWidget);
			ak::uiAPI::obj::setCentralWidget(my_ui.dockProperties, my_ui.propertiesWidget);
			ak::uiAPI::obj::setCentralWidget(my_ui.dockTree, my_ui.treeWidget);
			//ak::uiAPI::obj::setCentralWidget(my_ui.dockTester, my_ui.tester);

			// Set central widget
			ak::uiAPI::obj::setCentralWidget(my_ui.mainWindow, my_ui.tabViewWidget);

			// Display docks
			ak::uiAPI::addDock(
				my_ui.mainWindow,
				my_ui.dockOutput,
				ak::ui::core::dockBottom
			);
			ak::uiAPI::addDock(
				my_ui.mainWindow,
				my_ui.dockTree,
				ak::ui::core::dockLeft
			);
			ak::uiAPI::addDock(
				my_ui.mainWindow,
				my_ui.dockProperties,
				ak::ui::core::dockLeft
			);

			//ak::uiAPI::addDock(
			//	my_ui.mainWindow,
			//	my_ui.dockTester,
			//	ak::ui::core::dockRight
			//);
			
			// Create my notifier
			my_notifier = new ExampleNotifier(this);

			// Register notifier
			ak::uiAPI::registerNotifier(my_ui.propertiesWidget, my_notifier, ak::core::messageType::mEvent);
			ak::uiAPI::registerNotifier(my_ui.table1, my_notifier, ak::core::messageType::mEvent);
			ak::uiAPI::registerNotifier(my_ui.table2, my_notifier, ak::core::messageType::mEvent);
			ak::uiAPI::registerNotifier(my_ui.treeWidget, my_notifier, ak::core::messageType::mEvent);
			ak::uiAPI::registerNotifier(my_ui.ttb_aExit, my_notifier, ak::core::messageType::mEvent);
			ak::uiAPI::registerNotifier(my_ui.ttb_aColorStyle, my_notifier, ak::core::messageType::mEvent);
			ak::uiAPI::registerNotifier(my_ui.tester, my_notifier);

			// Create default data
			defaultData();

		}
		catch (const ak::Exception & e) { throw ak::Exception(e, "Example::Example()"); }
		catch (const std::exception & e) { throw ak::Exception(e.what(), "Example::Example()"); }
		catch (...) { throw ak::Exception("Unknown error", "Example::Example()"); }
	}
	catch (const std::exception & e) { ak::uiAPI::showMessageBox(my_ui.mainWindow, e.what(), "Error"); }
}

Example::~Example() {}

void Example::eventCallback(
	ak::UID					_sender,
	ak::core::eventType		_eventType,
	int						_info1,
	int						_info2
) {
	try {
		std::string str("Event { Sender=\"");
		str.append(std::to_string(_sender));
		str.append("\"; EventType = \"");
		str.append(ak::uiAPI::toString(_eventType));
		str.append("\"; Info1=\"");
		str.append(std::to_string(_info1));
		str.append("\"; Info2=\"");
		str.append(std::to_string(_info2));
		str.append("\"; }");
		ak::uiAPI::obj::appendText(my_ui.outputWidget, str.c_str());

		if (_eventType == ak::core::eventType::eClicked) {
			if (_sender == my_ui.ttb_aExit) {
				// Close the main window
				ak::uiAPI::close(my_ui.mainWindow);
			}
			else  if (_sender == my_ui.ttb_aColorStyle) {

				// Disable notifier to not receive changed messages
				my_notifier->disable();

				// Change color style
				if (ak::uiAPI::obj::getText(my_ui.ttb_aColorStyle) == TXT_Bright) {
					ak::uiAPI::obj::appendText(my_ui.outputWidget, "Set: ColorStyle { Style=\"DefaultBright\"; }");
					ak::uiAPI::setDefaultBrightColorStyle();
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
			else if (_sender == my_ui.tester) {
				std::vector<ak::ID> v;
				v.push_back(ak::uiAPI::itm::getID(my_ui.treeWidget, "A|A1|A1B"));
				v.push_back(ak::uiAPI::itm::getID(my_ui.treeWidget, "B"));
				ak::uiAPI::obj::deleteItems(my_ui.treeWidget, v);
			}
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "Example::eventCallback()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "Example::eventCallback()"); }
	catch (...) { throw ak::Exception("Unknown error", "Example::eventCallback()"); }
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
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A1|A1B");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A1|A1C");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A2|A2A");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A2|A2B");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A3|A3A");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A3|A3B");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "A|A3|A3C");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B1|B1A");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B1|B1B");

	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B2|B2A");
	ak::uiAPI::obj::createItem(my_ui.treeWidget, "B|B2|B2B");
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
	
}
