/*
	The following copyright affects the following files in the uiCore repository:
	.../uiCore/Example/Example.vcxproj
	.../uiCore/Example/include/Example.h
	.../uiCore/Example/src/Example.cpp
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

#include <Example.h>	// Core class

#include <ak_uiAPI.h>	// uiAPI

int main(int argc, char *argv[])
{
	// Initialize the uiAPI. This call is mandatory
	ak::uiAPI::ini(true, argc, argv);

	// Create the core class which is creating and managing the UI
    Example w;

	// Call the execute function to start the event handling
	return ak::uiAPI::exec();
}
