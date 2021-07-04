#pragma once

#include <akCore/globalDataTypes.h>
#include <akCore/aNotifier.h>

class AppBase;

class TabToolbar : public ak::aNotifier {
public:
	TabToolbar(AppBase * _app);

	virtual ~TabToolbar();

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

private:
	AppBase *		m_app;

	TabToolbar() = delete;
	TabToolbar(TabToolbar&) = delete;
	TabToolbar& operator = (TabToolbar&) = delete;
};