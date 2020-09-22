/*
	The following copyright affects the following files in the uiCore repository:
	.../uiCore/Example/Example.vcxproj
	.../uiCore/Example/include/Example.h
	.../uiCore/Example/include/ExampleNotifier.h
	.../uiCore/Example/src/Example.cpp
	.../uiCore/Example/src/ExampleNotifier.cpp
	.../uiCore/Example/src/main.cpp

	Copyright (c) 2020 Alexander Küster

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, without conditions:

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

#include <ak_globalDataTypes.h>			// ID and UID types
#include <ak_core.h>					// eventType type

class ExampleNotifier;

class Example
{
public:
    Example();
	
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
		ak::UID				mainWindow;
		ak::UID				ttb_pFile;
		ak::UID				ttb_gNONE;
		ak::UID				ttb_aExit;
		ak::UID				ttb_aColorStyle;
		ak::UID				dockTree;
		ak::UID				dockProperties;
		ak::UID				dockOutput;
		ak::UID				treeWidget;
		ak::UID				propertiesWidget;
		ak::UID				outputWidget;
		ak::UID				tabViewWidget;
		ak::UID				table1;
		ak::UID				table2;

		ak::UID				ttb_delete;

	};

	int cc;
	structUi					my_ui;			//! Contains all UIDs of the UI
	ak::UID						my_uid;			//! The UID of this application
	ExampleNotifier *			my_notifier;	//! The notifier that receives the callback messages
};
