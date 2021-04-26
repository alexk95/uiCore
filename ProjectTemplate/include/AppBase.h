#pragma once

#include <ak_globalDataTypes.h>

class AppBase {
public:
	AppBase(int _argc, char ** _argv);
	virtual ~AppBase();


private:
	ak::UID			my_uid;
	ak::UID			my_mainWindow;			// The mian window of the application (just as an example)


	AppBase() = delete;
	AppBase(AppBase &) = delete;
	AppBase & operator = (AppBase &) = delete;
};