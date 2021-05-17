/*
 *	File:		aFile.cpp
 *	Package:	akCore
 *
 *  Created on: September 28, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aFile.h>		// Corresponding header
#include <akCore/aException.h>	// Error handling

// Qt header
#include <qfile.h>

ak::aFile::aFile()
	: my_changed(false), my_extension(), my_lines(), my_name(), my_path(), my_uid(ak::invalidUID)
{}

ak::aFile::~aFile() {}

// ####################################################################################

// Setter

void ak::aFile::setUid(
	ak::UID					_uid
) { my_uid = _uid; }

void ak::aFile::load(
	const QString &			_filePath
) {
	try {
		if (_filePath.length() > 0) { setPath(_filePath); }
		importFile();
	}
	catch (const aException & e) { throw aException(e, "ak::aFile::load()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aFile::load()"); }
	catch (...) { throw aException("Unknown error", "ak::aFile::load()"); }
}

void ak::aFile::save(
	bool					_append
) { exportFile(_append); }

void ak::aFile::save(
	const QString &			_filePath,
	bool					_append
) { setPath(_filePath); exportFile(_append); }

void ak::aFile::setPath(
	const QString &			_path
) {
	my_path = _path;
	rebuildPath();

	// Filter file name and extension
	QStringList fileName = my_path.split("/", QString::SplitBehavior::KeepEmptyParts);
	QStringList extension = fileName.at(fileName.count() - 1).split(".");
	my_name = extension.at(0);
	for (int i = 1; i < extension.count() - 1; i++) {
		my_name.append('.'); my_name.append(extension.at(i));
	}
	my_extension = extension.at(extension.count() - 1);
}

void ak::aFile::setLines(
	const QStringList &		_lines
) { my_lines = _lines; my_changed = true; }

void ak::aFile::addLine(
	const QString &			_line
) { my_lines.push_back(_line); my_changed = true; }

void ak::aFile::addLine(
	const QStringList &		_lines
) { my_lines.append(_lines); my_changed = true; }

// ####################################################################################

// Getter

ak::UID ak::aFile::uid(void) const { return my_uid; }

QString ak::aFile::name() const { return my_name; }

QString ak::aFile::path() const { return my_path; }

QString ak::aFile::extension() const { return my_extension; }

QStringList ak::aFile::lines() const { return my_lines; }

int ak::aFile::linesCount() const { return my_lines.count(); }

bool ak::aFile::hasChanged() const { return my_changed; }

// ####################################################################################

// Private members

void ak::aFile::rebuildPath() {
	QStringList fileName = my_path.split("\\", QString::SplitBehavior::SkipEmptyParts);
	my_path = fileName.at(0);

	// Rebuild path
	for (int i = 1; i < fileName.count(); i++) { my_path.append('/'); my_path.append(fileName.at(i)); }
}

void ak::aFile::importFile() {
	try {
		// Check file
		QFile f(my_path);
		if (!f.exists()) { assert(0); throw aException("The provided file deos not exist", "Check file"); }

		// Open file
		if (!f.open(QIODevice::ReadOnly)) { assert(0); throw aException("Failed to open file for reading", "Open file"); }

		// Read file
		my_lines.clear();
		while (!f.atEnd()) { QString line = f.readLine(); my_lines.push_back(line); }
		f.close();
	}
	catch (const aException & e) { throw aException(e, "ak::aFile::importFile()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aFile::importFile()"); }
	catch (...) { throw aException("Unknown error", "ak::aFile::importFile()"); }
}

void ak::aFile::exportFile(
	bool					_append
) const {
	try {
		QFile f(my_path);
		if (_append) {
			if (!f.open(QIODevice::OpenModeFlag::Append)) { assert(0); throw aException("Failed to open file", "Check file"); }
		}
		else {
			if (!f.open(QIODevice::OpenModeFlag::WriteOnly)) { assert(0); throw aException("Failed to open file", "Check file"); }
		}
		for (QString str : my_lines) {
			std::string s(str.toStdString());
			f.write(s.c_str());
		}
		f.close();
	}
	catch (const aException & e) { throw aException(e, "ak::aFile::importFile()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aFile::importFile()"); }
	catch (...) { throw aException("Unknown error", "ak::aFile::importFile()"); }
}