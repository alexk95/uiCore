#pragma once

// uiCore header
#include <akCore/globalDataTypes.h>		// UID type
#include <akGui/aWindowEventHandler.h>	// Window event notifications (close event)

// QT header
#include <qobject.h>

// C++ header
#include <string>

// Forward declarations
class TabToolbar;

//! This class is managing the UI
//! We derive from aWindowEventHandler to be able to register this class as a
//! window event handler for the main window
class AppBase : public ak::aWindowEventHandler
{
public:
	//! @brief Constructor
	//! This constructor is creating all the objects and registering all callbacks
	AppBase(int _argc, char ** _argv);

	//! @brief Deconstructor
	virtual ~AppBase();
	
	//! @brief This function will be called when the window is about to close
	//! If return false then the window will not be closed
	virtual bool closeEvent(void) override;

private:
	friend class TabToolbar;

	ak::UID			m_uid;			//! The UID of this class, used to create controls

	TabToolbar *	m_ttb;			//! The tab toolbar

	ak::UID			m_mainWindow;	//! The UID of the main window

	AppBase() = delete;
	AppBase(AppBase &) = delete;
	AppBase& operator = (AppBase&) = delete;
};