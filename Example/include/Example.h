/*
 * Example.h
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

#include <ak_globalDataTypes.h>			// ID and UID types
#include <ak_core.h>					// eventType type

// Forward declaration
class ExampleNotifier;

//! This class is managing the UI
class Example
{
public:
	//! @brief Constructor
	//! This constructor is creating all the objects and registering all callbacks
    Example();
	
	//! @brief Deconstructor
	virtual ~Example();

	//! @brief This function will be called when an event occurs
	//! @param _sender The UID of the sender
	//! @param _eventType The type of the event
	//! @param _info1 Additional information 1
	//! @param _info2 Additional information 2
	void eventCallback(
		ak::UID					_sender,
		ak::core::eventType		_eventType,
		int						_info1,
		int						_info2
	);

private:
  
	//! @brief Will fill the widgets with default information
	void defaultData(void);

	struct structUi {
		ak::UID				mainWindow;			//! The UID of the main window
		ak::UID				ttb_pFile;			//! The UID of the file page in the tab toolbar
		ak::UID				ttb_gNONE;			//! The UID of the group in the file page in the tab toolbar
		ak::UID				ttb_aExit;			//! The UID of exit action
		ak::UID				ttb_aColorStyle;	//! The UID of the colorStlye action
		ak::UID				dockTree;			//! The UID of the dock containing the tree widget
		ak::UID				dockProperties;		//! The UID of the dock containing the propertyGrid widget
		ak::UID				dockOutput;			//! The UID of the dock containing the output textEdit widget
		ak::UID				treeWidget;			//! The UID of the dock containing the tree widget
		ak::UID				propertiesWidget;	//! The UID of the propertyGrid widget
		ak::UID				outputWidget;		//! The UID of the output textEdit widget
		ak::UID				tabViewWidget;		//! The UID of the tabView widget
		ak::UID				table1;				//! The UID of the table on the first page at the tabView
		ak::UID				table2;				//! The UID of the table on the second page at the tabView
	};
	structUi					my_ui;			//! Contains all UIDs of the UI
	ak::UID						my_uid;			//! The UID of this application
	ExampleNotifier *			my_notifier;	//! The notifier that receives the callback messages
};
