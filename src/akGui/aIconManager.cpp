/*
 *	File:		aIconManager.cpp
 *	Package:	akGui
 *
 *  Created on: March 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aIconManager.h>
#include <akCore/aException.h>

// Qt header
#include <qicon.h>					// QIcon
#include <qdir.h>					// Check directories
#include <qfile.h>					// Check file
#include <qpixmap.h>
#include <qmovie.h>

// C++ header
#include <mutex>					// Thread protection

#define PATH_PIXMAPS "Images/"
#define PATH_MOVIES "Animations/"

ak::aIconManager::aIconManager(
	const QString &					_mainDirectory
) : my_fileExtension(".png"),
	my_mutex(nullptr)
{
	my_mutex = new std::mutex();
	// Check if a main directory was provided
	if (_mainDirectory.length() > 0) {
		QDir dir(_mainDirectory);
		if (!dir.exists()) {
			throw aException("Directory does not exist!", "ak::aIconManager::addDirectory()");
		}
		// Check for the directory delimiter
		QString directory = _mainDirectory;
		if (!directory.endsWith('/') && !directory.endsWith('\\')) { directory.append('/'); }
		// Add directory
		my_directories.push_back(directory);
	}
}

ak::aIconManager::~aIconManager() {
	if (my_mutex != nullptr) { 
		my_mutex->lock(); my_mutex->unlock();  delete my_mutex;  my_mutex = nullptr;
	}
	// Delete all imported icons
	for (auto icon = my_icons.begin(); icon != my_icons.end(); icon++) {
		if (icon->second != nullptr) {
			for (auto size = icon->second->begin(); size != icon->second->end(); size++) {
				if (size->second != nullptr) { delete size->second; size->second = nullptr; }
			}
			delete icon->second; icon->second = nullptr;
		}
	}

	for (auto pix = my_pixmaps.begin(); pix != my_pixmaps.end(); pix++) {
		delete pix->second;
	}
}

void ak::aIconManager::addDirectory(
	const QString &					_directory
) {
	// Lock the mutex
	my_mutex->lock();

	// Check if the specified directory does exist
	QDir dir(_directory);
	if (!dir.exists()) {
		my_mutex->unlock();
		throw aException("Provided directory does not exist!", "ak::aIconManager::addDirectory()");
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

bool ak::aIconManager::removeDirectory(
	const QString &					_directory
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
	assert(0); // The provided directory does not exist
	return false;
}

const QIcon * ak::aIconManager::icon(
	const QString &					_iconName,
	const QString &					_iconSize
) {
	try {
		my_mutex->lock();
		auto ico = my_icons.find(_iconName);
		if (ico == my_icons.end()) {
			// icon does not exist at all
			QIcon * newIcon = createIcon(_iconName, _iconSize);
			std::map<QString, QIcon *> * newMap = nullptr;
			newMap = new std::map<QString, QIcon *>;
			newMap->insert_or_assign(_iconSize, newIcon);
			my_icons.insert_or_assign(_iconName, newMap);
			my_mutex->unlock();
			return newIcon;
		}
		else {
			auto size = ico->second->find(_iconSize);
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
	catch (const aException & e) { my_mutex->unlock(); throw aException(e, "ak::aIconManager::icon()"); }
	catch (const std::exception & e) { my_mutex->unlock(); throw aException(e.what(), "ak::aIconManager::icon()"); }
	catch (...) { my_mutex->unlock(); throw aException("Unknown error", "ak::aIconManager::icon()"); }
}

const QPixmap * ak::aIconManager::pixmap(
	const QString &									_imageName
) {
	try {
		my_mutex->lock();
		auto itm = my_pixmaps.find(_imageName);
		if (itm != my_pixmaps.end()) { return itm->second; }
		QPixmap * pix = createPixmap(_imageName);
		my_pixmaps.insert_or_assign(_imageName, pix);
		my_mutex->unlock();
		return pix;
	}
	catch (const aException & e) { my_mutex->unlock(); throw aException(e, "ak::aIconManager::pixmap()"); }
	catch (const std::exception & e) { my_mutex->unlock(); throw aException(e.what(), "ak::aIconManager::pixmap()"); }
	catch (...) { my_mutex->unlock(); throw aException("Unknown error", "ak::aIconManager::pixmap()"); }
}

QMovie * ak::aIconManager::movie(
	const QString &									_imageName
) {
	try {
		my_mutex->lock();
		auto itm = my_movies.find(_imageName);
		if (itm != my_movies.end()) { return itm->second; }
		QMovie * mv = createMovie(_imageName);
		my_movies.insert_or_assign(_imageName, mv);
		my_mutex->unlock();
		return mv;
	}
	catch (const aException & e) { my_mutex->unlock(); throw aException(e, "ak::aIconManager::movie()"); }
	catch (const std::exception & e) { my_mutex->unlock(); throw aException(e.what(), "ak::aIconManager::movie()"); }
	catch (...) { my_mutex->unlock(); throw aException("Unknown error", "ak::aIconManager::movie()"); }
}

void ak::aIconManager::setFileExtension(
	const QString &									_extension
) { 
	my_mutex->lock();
	my_fileExtension = _extension;
	my_mutex->unlock();
}

QString ak::aIconManager::getFileExtension() { 
	my_mutex->lock();
	QString ret = my_fileExtension;
	my_mutex->unlock();
	return ret;
}


QIcon * ak::aIconManager::createIcon(
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
				ico = new QIcon(file.fileName());
				return ico;
			}
		}
		throw aException("Icon does not exist", "Check status");
	}
	catch (const aException & e) { throw aException(e, "ak::aIconManager::createIcon()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aIconManager::createIcon()"); }
	catch (...) { throw aException("Unknown error", "ak::aIconManager::createIcon()"); }
}

QPixmap * ak::aIconManager::createPixmap(
	const QString &									_imageName
) {
	// Mutex not required, caller must take care of the mutex
	try {
		for (int i = 0; i < my_directories.size(); i++) {
			QFile file(my_directories.at(i) + PATH_PIXMAPS + _imageName + my_fileExtension);
			// Check if the file exist
			if (file.exists()) {
				QPixmap * ico = nullptr;
				ico = new (std::nothrow) QPixmap(file.fileName());
				if (ico == nullptr) { throw aException("Failed to create", "Create icon"); }
				return ico;
			}
		}
		throw aException("Image does not exist", "Check status");
	}
	catch (const aException & e) { throw aException(e, "ak::aIconManager::createPixmap()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aIconManager::createPixmap()"); }
	catch (...) { throw aException("Unknown error", "ak::aIconManager::createPixmap()"); }
}

QMovie * ak::aIconManager::createMovie(
	const QString &									_imageName
) {
	// Mutex not required, caller must take care of the mutex
	try {
		for (int i = 0; i < my_directories.size(); i++) {
			QFile file(my_directories.at(i) + PATH_MOVIES + _imageName + ".gif");
			// Check if the file exist
			if (file.exists()) {
				QMovie * mv = nullptr;
				mv = new (std::nothrow) QMovie(file.fileName());
				if (mv == nullptr) { throw aException("Failed to create", "Create movie"); }
				return mv;
			}
		}
		throw aException("Movie does not exist", "Check status");
	}
	catch (const aException & e) { throw aException(e, "ak::aIconManager::createMovie()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aIconManager::createMovie()"); }
	catch (...) { throw aException("Unknown error", "ak::aIconManager::createMovie()"); }
}

std::vector<QString> ak::aIconManager::searchDirectories(void) const { return my_directories; }