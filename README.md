**!! NOTE !!**
The uiCore library is still in development and will be updated on a regular base. At this point multiple features to access the core components are missing and will be implemented soon.

-------------

<p align="center">
	uiCore
</p>

<p align="center">
  <img src="https://github.com/alexk95/uiCore/master/uiCore_ExampleApplication.jpg">
</p>

The **uiCore** is a library meant to be used with **Qt**.

This library takes away the effort to create and manage the fundamental objects for a technical user interface (UI).

To create the user inteface no widget creation is required. The API provides all common objects, but also allowes external QWidgets to be displayed.

Features:
  - **Skriptable UI** - The UI will be created from code by using the API that provides the functions to modify the UI.
  - **Cross-plattform** - The uiCore depends only on **Qt** and the **TabToolbar** library.
  - **Configurable** - The uiCore can be used and configured with ease.
  - **Microservice ready** - Object creation can be performed by using only base data types. Event notification is handled with messages that are send via the uiCore message system.
  - **Fast and easy creation** - When creating a new application and the efford for creating the whole UI wants to be skipped, this is the perfect solution.

-------------

**Create an application based on on the uiCore with VisualStudio**

-------------

**Requirements**

1) Qt must be installed
2) Create an Environment variable (System settings or batch file) for the %QTDIR%,
	which should result in something like this: ...\Qt\5.11.2\msvc2017_64
3) Install the Qt VS Tool Add-on for Visual Studio
4) In the Qt VS Tool options add a new Qt Version with the name: $(QTDIR)
	This will disable the path selection and read the path from the environment variable.

-------------

**Project settings**

1) Create a new Qt Widgets Application (Core, Gui and Widgets must be selected in the dialog)
2) Create an Evironment variable (System Settings or batch file) for the %UICORE_LIB_ROOT%
3) Configuration Properties (For a x64 build):
	Add:		VC++ Directories->Include Directories:				$(ProjectDir)include;
	Ensure:		C/C++->General->Additional Include Directories contains:	$(Qt_INCLUDEPATH_);
	Add:		C/C++->General->Additional Include Directories:			%UI_CORE_ROOT%\include;
	Ensure:		Linker->General->Additional Library Directories contains:	$(Qt_LIBPATH_);
	Add <Debug>:	Linker->General->Additional Library Directories:		%UI_CORE_ROOT%\x64\Debug;
	Add <Release>:	Linker->General->Additional Library Directories:		%UI_CORE_ROOT%\x64\Release;
	Ensure:		Linker->Input->Additional Dependencies contains:		$(Qt_LIBS_);
	Add <Debug>:	Linker->Input->Additional Dependencies:				%UI_CORE_ROOT%\x64\Debug\uiCore.lib;
	Add <Release>:	Linker->Input->Additional Dependencies:				%UI_CORE_ROOT%\x64\Release\uiCore.lib;
	Set <Debug>:	Debugging->Environment:						PATH=$(QtDllPath);%UI_CORE_ROOT%\x64\Debug;%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar\DLL; ...<other paths>... ;%PATH%
	Set <Release>:	Debugging->Environment:						PATH=$(QtDllPath);%UI_CORE_ROOT%\x64\Release;%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar\DLL; ...<other paths>... ;%PATH%

-------------

**Configuration Properties for all Qt Application (fixes a bug for executing in VS2017)**

	Add:		Custom Build Step->General->CommandLine:			copy $(MSBuildProjectDirectory)\qt.conf $(OutputPath)
	Add:		Custom Build Step->General->Outputs:				$(OutputPath)\qt.conf
	Select:		Custom Build Step->General->Execute After:			ClCompile
	Select:		Custom Build Step->General->Execute Before:			Link
This will create a "qt.conf" file which will be placed in the output directory.
This file must be in the same directory as the executable.

-------------
**Legal notes**

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
