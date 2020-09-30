/*
 * ak_ui_colorStyle.cpp
 *
 *  Created on: April 09, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_colorStyle.h>		// corresponding class

// Qt header
#include <qdir.h>					// QDir
#include <qfile.h>					// QFile

ak::ui::color ak::ui::colorStyle::getWindowMainForecolor(void) const { return my_windowMainForecolor; }

ak::ui::color ak::ui::colorStyle::getWindowMainBackcolor(void) const { return my_windowMainBackcolor; }

ak::ui::color ak::ui::colorStyle::getControlsMainForecolor(void) const { return my_controlsMainForecolor; }

ak::ui::color ak::ui::colorStyle::getControlsMainBackcolor(void) const { return my_controlsMainBackcolor; }

ak::ui::color ak::ui::colorStyle::getControlsErrorForecolor(void) const { return my_controlsErrorForecolor; }

void ak::ui::colorStyle::setWindowMainForecolor(
	const ak::ui::color &		_color
) {
	my_windowMainForecolor = _color;
}

void ak::ui::colorStyle::setWindowMainBackcolor(
	const ak::ui::color &		_color
) {
	my_windowMainBackcolor = _color;
}

void ak::ui::colorStyle::setControlsMainForecolor(
	const ak::ui::color &		_color
) {
	my_controlsMainForecolor = _color;
}

void ak::ui::colorStyle::setControlsMainBackcolor(
	const ak::ui::color &		_color
) {
	my_controlsMainBackcolor = _color;
}

void ak::ui::colorStyle::setControlsErrorForecolor(
	const ak::ui::color &		_color
) {
	my_controlsErrorForecolor = _color;
}

void ak::ui::colorStyle::addDirectory(
	const QString &					_directory
) {
	// Check weather the provided directory exists or not
	QDir dir(_directory);
	if (!dir.exists()) {
		throw ak::Exception("Provided directory does not exist!", "ak::ui::colorStyle::addDirectory()");
	}
	QString directory = _directory;
	// Check if the directory ends with a path delimiter, if not then append one
	if (!directory.endsWith('/') && !directory.endsWith('\\')) { directory.append('/'); }

	// Check if directory already exists
	for (int i = 0; i < my_directories.size(); i++) {
		if (my_directories.at(i).toLower() == directory.toLower()) { return; }
	}
	// Store data
	my_directories.push_back(directory);
}

bool ak::ui::colorStyle::removeDirectory(
	const QString &					_directory,
	bool							_throwException
) {
	QString directory = _directory;
	// Check if the provided direcotry ends with the path delimiter, if not then append one
	if (!directory.endsWith('/') && !directory.endsWith('\\')) { directory.append('/'); }
	for (int i = 0; i < my_directories.size(); i++) {
		if (my_directories.at(i).toLower() == directory.toLower()) { my_directories.erase(my_directories.begin() + i); return true; }
	}
	if (_throwException) { throw ak::Exception("The provided directory does not exist", "ak::ui::colorStyle::removeDirectory()"); }
	return false;
}

void ak::ui::colorStyle::setDirectories(
	const std::vector<QString> &	_directories
) {
	my_directories.clear();
	
	for (QString _directory : _directories) {
		// Check weather the provided directory exists or not
		QDir dir(_directory);
		if (!dir.exists()) {
			throw ak::Exception("Provided directory does not exist!", "ak::ui::colorStyle::addDirectory()");
		}
		QString directory = _directory;
		// Check if the directory ends with a path delimiter, if not then append one
		if (!directory.endsWith('/') && !directory.endsWith('\\')) { directory.append('/'); }

		// Check if directory already exists
		for (int i = 0; i < my_directories.size(); i++) {
			if (my_directories.at(i).toLower() == directory.toLower()) { return; }
		}
		// Store data
		my_directories.push_back(directory);
	}
}

QString ak::ui::colorStyle::getFilePath(
	const QString &			_fileName
) const {
	try {
		// Search in all directories for the file
		for (int i = 0; i < my_directories.size(); i++) {
			try {
				// Check if the file exist at the current directory
				QString f(my_directories.at(i));
				f.append("ColorStyles/");
				f.append(getColorStyleName());
				f.append("/");
				f.append(_fileName);

				QFile file(f);
				if (file.exists()) {
					//QStringList lst = QString(my_directories.at(i) + _fileName).split("\\");
					QStringList lst = f.split("\\");
					QString out = "";
					for (int c = 0; c < lst.length(); c++) {
						out.append(lst.at(c));
						if (c < lst.length() - 1) { out.append("/"); }
					}
					return out;
				}
			}
			catch (...) {
				throw ak::Exception(QString(QString("Failed to check file\nFile: ") + QString(_fileName) + QString("\nPath: ") +
					QString(my_directories.at(i))).toStdString().c_str(), "Check file");
			}
		}
		return QString("");
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::colorStyle::getFilePath()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::colorStyle::getFilePath()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::colorStyle::getFilePath()"); }
}
