/*
 * ak_ui_iconManager.h
 *
 *  Created on: March 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <ak_globalDataTypes.h>

// C++ header
#include <map>						// Icon storage
#include <vector>					// Directory name storage

// Qt header
#include <qstring.h>				// QString

// Forward declaration
class QIcon;
class QPixmap;
class QMovie;

namespace std { class mutex; }

namespace ak {
	namespace ui {

		//! @brief The icon manager is used to manage the application icons
		//! The iconManager will store and provide icons.
		//! The icons will be searched inside the provided directories.
		//! The directories will be searched in the order they have been added.
		//! The icon found first will be returned
		class UICORE_API_EXPORT iconManager {
		public:
			//! @brief Constructor will initialize the file extension with png
			//! @param _mainDirectory The main directory for the icon manager
			//! @throw ak::Exception if the provided directory does not exist or an object creation failed
			iconManager(
				const QString &									_mainDirectory = QString("")
			);

			//! @brief Deconstructor
			virtual ~iconManager();

			//! @brief Will add the provided directory to the iconManager
			//! @param _directory The new search directory to add
			//! @throw ak::Exception if the provided directory does not exist
			void addDirectory(
				const QString &									_directory
			);

			//! @brief Will remove the provided directory from the iconManager, returns false if directory does not exist before
			//! @param _throwException If true, an exception will be thrown if the provided directory does not exist
			//! @throw ak::Exception if the provided directory does not exist
			bool removeDirectory(
				const QString &									_directory
			);

			//! @brief Will return the icon loaded from the files location
			//! @param _iconName The name of the icon (without extension)
			//! @param _iconPath The path of the icon. The path will decribes the subfolders in the set search directories
			const QIcon * icon(
				const QString &									_iconName,
				const QString &									_iconPath
			);

			//! @brief Will return the pixmap loaded from the files location
			//! @param _imageName The name of the image (without extension)
			const QPixmap * pixmap(
				const QString &									_imageName
			);

			//! @brief Will return the movie loaded from the files location
			//! @param _imageName The name of the image (without extension)
			QMovie * movie(
				const QString &									_imageName
			);

			//! @brief Will set the file extension for the files
			//! @param _extension The filename extension (must contain the separator (i.e. ".png"))
			void setFileExtension(
				const QString &									_extension
			);

			//! @brief Returns the currently set file extension
			QString getFileExtension();

			//! @brief Will return search directories if this icon manager
			std::vector<QString> searchDirectories(void) const;

		protected:
			std::map<QString, std::map<QString, QIcon * > * >	my_icons;					//! The map containing all imported icons
			typedef std::map<
				QString,
				std::map<QString, QIcon * > *
			>::iterator											my_iconsIterator;			//! Iterator used to iterate through the icon map
			typedef std::map<QString, QIcon * >::iterator		my_iconsSizeIterator;       //! Iterator used to iterate through the icons of a specific icon size in the icon map

			std::map<QString, QPixmap *>						my_pixmaps;
			typedef std::map<QString, QPixmap *>::iterator		my_pixmapsIterator;

			std::map<QString, QMovie *>							my_movies;
			typedef std::map<QString, QMovie *>::iterator		my_moviesIterator;

			std::vector<QString>								my_directories;				//! Contains all set directories
			QString												my_fileExtension;			//! The currently set file extension

			std::mutex *										my_mutex;					//! The mutex used for thread protection

			//! @brief Imports an icon according to its name and size
			//! Mutex will not be locked. Caller must take care of it
			//! @param _iconName The name of the requested icon (Without extension)
			//! @param _iconSize The size of the requested icon (According to the file structure)
			//! @throw ak::Exception if the icon creation failed
			QIcon * createIcon(
				const QString &									_iconName,
				const QString &									_iconSize
			);

			//! @brief Imports an pixmap according to its name and size
			//! Mutex will not be locked. Caller must take care of it
			//! @param _imageName The name of the requested pixmap (Without extension)
			//! @throw ak::Exception if the icon creation failed
			QPixmap * createPixmap(
				const QString &									_imageName
			);

			//! @brief Imports a movie according to its name and size
			//! Mutex will not be locked. Caller must take care of it
			//! @param _imageName The name of the requested movie (gif file without extension)
			//! @throw ak::Exception if the icon creation failed
			QMovie * createMovie(
				const QString &									_imageName
			);

		};

	} // namespace ui
} // namespace ak
