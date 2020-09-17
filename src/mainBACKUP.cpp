/*
#include <iostream>

#include "UI_test.h"
#include <QtWidgets/QApplication>
#include <QSurfaceFormat>
#include <QMetaObject>
#include <QThread>
#include <qmessagebox.h>
#include "ak_exception.h"

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	// Enable anti-aliasing for the opengl rendering windows
	QSurfaceFormat format;
	format.setSamples(4);   // Either select 2 or 4 subsamples (settings above this value will lead to excessive start up times on systems with no dedicated GPU).
	QSurfaceFormat::setDefaultFormat(format);

	UI_test * main_window = nullptr;
	try {
		try {
			main_window = new UI_test();
			return app.exec();
		}
		catch (const ak::Exception & e) { throw ak::Exception(e, "main::main()"); }
		catch (const std::exception & e) { throw ak::Exception(e.what(), "main::main()"); }
		catch (...) { throw ak::Exception("Unknown error", "main::main()"); }
	}
	catch (const ak::Exception & e) {
		std::cout << "Fatal error: " << e.what() << std::endl;
		return -1;
	}

}

#include <thread> 

void mainApplicationThread()
{
	main(0, NULL);
}

extern "C"
{
	_declspec(dllexport) int init()
	{
		std::thread *app = new std::thread(mainApplicationThread);
		return 0;
	};
}
*/