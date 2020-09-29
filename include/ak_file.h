/*
 * ak_file.h
 *
 *  Created on: September 28, 2020
 *	Last modified on: September 28, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_globalDataTypes.h>

// Qt header
#include <qstring.h>				// QString
#include <qstringlist.h>			// QStringList

namespace ak {

	//! Represents a file and its information
	class file {
	public:

		//! @brief Constructor
		file();

		//! @brief Deconstructor
		virtual ~file();

		// #######################################################################################################

		// Setter

		//! @brief Will set this files UID
		//! @param _uid The UID to set
		void setUid(
			ak::UID					_uid
		);

		//! @brief Will load informations from the specified file
		//! @param _filePath If provided this path will be set as current file path
		void load(
			const QString &			_filePath = QString("")
		);

		//! @brief Will save the current set lines to the current set file path
		//! @param _append If true, the file will be opened on append mode
		void save(
			bool					_append = false
		);

		//! @brief Will save the current set lines to the provided file path
		//! @param _filePath The fule path to set as current file
		//! @param _append If true, the file will be opened on append mode
		void save(
			const QString &			_filePath,
			bool					_append = false
		);

		//! @brief Will set the current path for the file
		//! @param _path The file path to set
		void setPath(
			const QString &			_path
		);

		//! @brief Will set the current lines for the file
		//! @param _lines The lines to set
		void setLines(
			const QStringList &		_lines
		);

		//! @brief Will append the provided line to the current lines
		//! @param _line The line to add
		void addLine(
			const QString &			_line
		);

		//! @brief Will append the provided lines to the current lines
		//! @param _lines The lines to add
		void addLine(
			const QStringList &		_lines
		);

		// #######################################################################################################

		// Getter

		//! @brief Will return the files UID
		ak::UID uid(void) const;

		//! @brief Will return the files name
		QString name(void) const;

		//! @brief Will return the files path
		QString path(void) const;

		//! @brief Will return the files extension
		QString extension(void) const;

		//! @brief Will return the lines in this file
		QStringList lines(void) const;

		//! @brief Will return the count of the lines in this file
		int linesCount(void) const;

		//! @brief Will return true if the file has changed after it was loaded or saved the last time
		bool hasChanged(void) const;

	private:
		// #######################################################################################################
		
		//! @brief Will rebuild the current path
		void rebuildPath(void);

		//! @brief Will import the lines from the current file
		void importFile(void);

		//! @brief Will write the current lines to the file
		//! @param _append If true, the file will be open in append mode
		void exportFile(
			bool					_append
		) const;

		bool					my_changed;		//! If true the file has changed after it was loaded or saved the last time
		QString					my_name;		//! My file name
		QString					my_path;		//! My file path
		QString					my_extension;	//! My file extension
		QStringList				my_lines;		//! The lines in this file

		ak::UID					my_uid;			//! My UID

		file(const file &) = delete;

		file & operator = (const file &) = delete;

	};

}