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

//! This class is managing the UI
//! We derive from aNotifier to be able to register this class for uiCore event callbacks.
//! Also we derive from QObject to be able to use Qt signal-slot if we want to use cutom widgets. You can remove it if no custom widgets will be required
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

private:

	ak::UID			m_uid;		// The UID of this class, used to create controls

	TabToolbar *	m_ttb;		// The tab toolbar

	ak::UID			m_mainWindow;

	AppBase() = delete;
	AppBase(AppBase &) = delete;
	AppBase& operator = (AppBase&) = delete;
};