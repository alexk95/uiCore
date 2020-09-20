/*
 * ak_ui_iconManager.cpp
 *
 *  Created on: March 20, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include "ak_ui_iconManager.h"		// corresponding class
#include "ak_exception.h"			// Error handling

// Qt header
#include <qicon.h>					// QIcon
#include <qdir.h>					// Check directories
#include <qfile.h>					// Check file

// C++ header
#include <mutex>					// Thread protection

ak::ui::iconManager::iconManager(
	const QString &					_mainDirectory
) : my_fileExtension(".png"),
	my_mutex(nullptr)
{
	my_mutex = new std::mutex();
	if (my_mutex == nullptr) { throw ak::Exception("Failed to create mutex", "ak::ui::iconManager::iconManager()"); }
	// Check if a main directory was provided
	if (_mainDirectory.length() > 0) {
		QDir dir(_mainDirectory);
		if (!dir.exists()) {
			throw ak::Exception("Directory does not exist!", "ak::ui::iconManager::addDirectory()");
		}
		// Check for the directory delimiter
		QString directory = _mainDirectory;
		if (!directory.endsWith('/') && !directory.endsWith('\\')) { directory.append('/'); }
		// Add directory
		my_directories.push_back(directory);
	}
}

ak::ui::iconManager::~iconManager() {
	if (my_mutex != nullptr) { 
		my_mutex->lock(); my_mutex->unlock();  delete my_mutex;  my_mutex = nullptr;
	}
	// Delete all imported icons
	for (my_iconsIterator icon = my_icons.begin(); icon != my_icons.end(); icon++) {
		if (icon->second != nullptr) {
			for (my_iconsSizeIterator size = icon->second->begin(); size != icon->second->end(); size++) {
				if (size->second != nullptr) { delete size->second; size->second = nullptr; }
			}
			delete icon->second; icon->second = nullptr;
		}
	}
}

void ak::ui::iconManager::addDirectory(
	const QString &					_directory
) {
	// Lock the mutex
	my_mutex->lock();

	// Check if the specified directory does exist
	QDir dir(_directory);
	if (!dir.exists()) {
		my_mutex->unlock();
		throw ak::Exception("Provided directory does not exist!", "ak::ui::iconManager::addDirectory()");
	}
	// Check for the directory ending
	QString directory = _directory;
	directory.replace('\\', '/');
	if (!directory.endsWith('/') && !directory.endsWith('\\')) { directory.append('/'); }

	// Check if directory already exists
	for (int i = 0; i < my_directories.size(); i++) {
		if (my_directories.at(i).toLower() == directory.toLower()) { my_mutex->unlock(); return; }
	}
	my_directories.push_back(directory);
	my_mutex->unlock();
}

bool ak::ui::iconManager::removeDirectory(
	const QString &					_directory,
	bool							_throwException
) {
	// Lock the mutex
	my_mutex->lock();

	// Check for the directory ending to be able to check the
	QString directory = _directory;
	directory.replace('\\', '/');
	if (!directory.endsWith('/') && !directory.endsWith('\\')) { directory.append('/'); }
	for (int i = 0; i < my_directories.size(); i++) {
		// Check if the names are equal
		if (my_directories.at(i).toLower() == directory.toLower()) {
			my_directories.erase(my_directories.begin() + i);
			my_mutex->unlock();
			return true;
		}
	}
	my_mutex->unlock();
	if (_throwException) { throw ak::Exception("The provided directory does not exist", "ak::ui::iconManager::removeDirectory()"); }
	return false;
}

const QIcon * ak::ui::iconManager::icon(
	const QString &					_iconName,
	const QString &					_iconSize
) {
	try {
		my_mutex->lock();
		my_iconsIterator ico = my_icons.find(_iconName);
		if (ico == my_icons.end()) {
			// icon does not exist at all
			QIcon * newIcon = createIcon(_iconName, _iconSize);
			std::map<QString, QIcon *> * newMap = nullptr;
			newMap = new (std::nothrow) std::map<QString, QIcon *>;
			if (newMap == nullptr) { delete newIcon; throw ak::Exception("Failed to create", "Create size map"); }
			newMap->insert_or_assign(_iconSize, newIcon);
			my_icons.insert_or_assign(_iconName, newMap);
			my_mutex->unlock();
			return newIcon;
		}
		else {
			my_iconsSizeIterator size = ico->second->find(_iconSize);
			if (size == ico->second->end()) {
				// icon in this size does not exist
				QIcon * newIcon = createIcon(_iconName, _iconSize);
				ico->second->insert_or_assign(_iconSize, newIcon);
				my_mutex->unlock();
				return newIcon;
			}
			else {
				// icon does exist
				my_mutex->unlock();
				return size->second;
			}
		}
	}
	catch (const ak::Exception & e) { my_mutex->unlock(); throw ak::Exception(e, "ak::ui::iconManager::icon()"); }
	catch (const std::exception & e) { my_mutex->unlock(); throw ak::Exception(e.what(), "ak::ui::iconManager::icon()"); }
	catch (...) { my_mutex->unlock(); throw ak::Exception("Unknown error", "ak::ui::iconManager::icon()"); }
}

void ak::ui::iconManager::setFileExtension(
	const QString &									_extension
) { 
	my_mutex->lock();
	my_fileExtension = _extension;
	my_mutex->unlock();
}

QString ak::ui::iconManager::getFileExtension() { 
	my_mutex->lock();
	QString ret = my_fileExtension;
	my_mutex->unlock();
	return ret;
}


QIcon * ak::ui::iconManager::createIcon(
	const QString &									_iconName,
	const QString &									_iconSize
) {
	// Mutex not required, caller must take care of the mutex
	try {
		for (int i = 0; i < my_directories.size(); i++) {
			QFile file(my_directories.at(i) + _iconSize + '/' + _iconName + my_fileExtension);
			// Check if the file exist
			if (file.exists()) {
				QIcon * ico = nullptr;
				ico = new (std::nothrow) QIcon(file.fileName());
				if (ico == nullptr) { throw ak::Exception("Failed to create", "Create icon"); }
				return ico;
			}
		}
		throw ak::Exception("Icon does not exist", "Check status");
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::iconManager::createIcon()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::iconManager::createIcon()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::iconManager::createIcon()"); }
}

std::vector<QString> ak::ui::iconManager::searchDirectories(void) const { return my_directories; }