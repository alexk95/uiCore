/*
 * ak_ui_colorStyle.cpp
 *
 *  Created on: April 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_colorStyle.h>		// corresponding class

// Qt header
#include <qdir.h>					// QDir
#include <qfile.h>					// QFile

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
				throw Exception(QString(QString("Failed to check file\nFile: ") + QString(_fileName) + QString("\nPath: ") +
					QString(my_directories.at(i))).toStdString().c_str(), "Check file");
			}
		}
		throw Exception(QString(QString("The requested file: \"") + QString(_fileName) + QString("\" does not exist in the icon search directories")).toStdString(),
			"Find file", Exception::exceptionType::FileNotFound);
	}
	catch (const Exception & e) { throw Exception(e, "ak::ui::colorStyle::getFilePath()"); }
	catch (const std::exception & e) { throw Exception(e.what(), "ak::ui::colorStyle::getFilePath()"); }
	catch (...) { throw Exception("Unknown error", "ak::ui::colorStyle::getFilePath()"); }
}
