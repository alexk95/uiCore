/*
 * Example.h
 *
 *  Created on: July 01, 2021
 *	Last modified on: July 01, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// uiCore header
#include <akCore/globalDataTypes.h>		// UID type
#include <akCore/aNotifier.h>			// Allows callbacks for objects
#include <akGui/aWindowEventHandler.h>	// Window event notifications (close event)

// QT header
#include <qobject.h>

// C++ header
#include <string>

// Forward declarations
class TabToolbar;
class myCustomCentralWidget;
namespace ak { class aColorStyle; }

//! This class is managing the UI
//! We derive from QObject to be able to use Qt signal-slot if we want to use cutom widgets. You can remove it if no custom widgets will be required
//!
//! We derive from aWindowEventHandler to get notified when the window is about to close, this important if the user chooses to close the window by pressing the "regular"
//! close button and not a exit option 
class AppBase : public QObject, public ak::aNotifier, public ak::aWindowEventHandler
{
	Q_OBJECT
public:
	//! @brief Constructor
	//! This constructor is creating all the objects and registering all callbacks
	AppBase(int _argc, char ** _argv);
	
	//! @brief Deconstructor
	virtual ~AppBase();

	//! @brief This function will be called when an event occurs
	//! The function only applies for objects that were created with the uiAPI
	//! @param _sender The UID of the sender
	//! @param _eventType The type of the event
	//! @param _info1 Additional information 1
	//! @param _info2 Additional information 2
	virtual void notify(
		ak::UID					_sender,
		ak::eventType			_eventType,
		int						_info1,
		int						_info2
	) override;

	//! @brief This function will be called when the window is about to close
	//! If return false then the window will not be closed
	virtual bool closeEvent(void) override;
	
	//! @brief This function is for new component testing
	void testFunction(void);

private:
	// We add the TabToolbar as a friend to grant acces to the UIDs and data
	friend class TabToolbar;

	ak::UID					m_uid;				//! The UID of this class, used to create controls

	TabToolbar *			m_ttb;				//! The tab toolbar
	myCustomCentralWidget *	m_centralWidget;	//! Our own central widget

	ak::UID					m_mainWindow;		//! The UID of the main window

	AppBase() = delete;
	AppBase(AppBase &) = delete;
	AppBase& operator = (AppBase&) = delete;
};
