/*
 * ak_file.cpp
 *
 *  Created on: September 28, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_file.h>		// Corresponding header
#include <ak_exception.h>	// Error handling

// Qt header
#include <qfile.h>

ak::file::file()
	: my_changed(false), my_extension(), my_lines(), my_name(), my_path(), my_uid(ak::invalidUID)
{}

ak::file::~file() {}

// ####################################################################################

// Setter

void ak::file::setUid(
	ak::UID					_uid
) { my_uid = _uid; }

void ak::file::load(
	const QString &			_filePath
) {
	try {
		if (_filePath.length() > 0) { setPath(_filePath); }
		importFile();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::file::load()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::file::load()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::file::load()"); }
}

void ak::file::save(
	bool					_append
) { exportFile(_append); }

void ak::file::save(
	const QString &			_filePath,
	bool					_append
) { setPath(_filePath); exportFile(_append); }

void ak::file::setPath(
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

void ak::file::setLines(
	const QStringList &		_lines
) { my_lines = _lines; my_changed = true; }

void ak::file::addLine(
	const QString &			_line
) { my_lines.push_back(_line); my_changed = true; }

void ak::file::addLine(
	const QStringList &		_lines
) { my_lines.append(_lines); my_changed = true; }

// ####################################################################################

// Getter

ak::UID ak::file::uid(void) const { return my_uid; }

QString ak::file::name() const { return my_name; }

QString ak::file::path() const { return my_path; }

QString ak::file::extension() const { return my_extension; }

QStringList ak::file::lines() const { return my_lines; }

int ak::file::linesCount() const { return my_lines.count(); }

bool ak::file::hasChanged() const { return my_changed; }

// ####################################################################################

// Private members

void ak::file::rebuildPath() {
	QStringList fileName = my_path.split("\\", QString::SplitBehavior::SkipEmptyParts);
	my_path = fileName.at(0);

	// Rebuild path
	for (int i = 1; i < fileName.count(); i++) { my_path.append('/'); my_path.append(fileName.at(i)); }
}

void ak::file::importFile() {
	try {
		// Check file
		QFile f(my_path);
		if (!f.exists()) { throw ak::Exception("The provided file deos not exist", "Check file"); }

		// Open file
		if (!f.open(QIODevice::ReadOnly)) { throw ak::Exception("Failed to open file for reading", "Open file"); }

		// Read file
		my_lines.clear();
		while (!f.atEnd()) { QString line = f.readLine(); my_lines.push_back(line); }
		f.close();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::file::importFile()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::file::importFile()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::file::importFile()"); }
}

void ak::file::exportFile(
	bool					_append
) const {
	try {
		QFile f(my_path);
		if (_append) {
			if (!f.open(QIODevice::OpenModeFlag::Append)) { throw ak::Exception("Failed to open file", "Check file"); }
		}
		else {
			if (!f.open(QIODevice::OpenModeFlag::WriteOnly)) { throw ak::Exception("Failed to open file", "Check file"); }
		}
		for (QString str : my_lines) {
			std::string s(str.toStdString());
			f.write(s.c_str());
		}
		f.close();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::file::importFile()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::file::importFile()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::file::importFile()"); }
}