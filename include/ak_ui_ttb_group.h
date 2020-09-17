/*
 * ak_ui_ttb_group.h
 *
 *  Created on: August 05, 2020
 *	Last modified on: August 27, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *
 *	This class is using a modified version of the "Qt TabToolbar" library.
 *	The changes to the library are listed in the documentation.
 */

#pragma once

 // Qt header
#include <vector>						// vector<>
#include <qstring.h>					// QString

// AK header
#include "ak_ui_core_ttbContainer.h"	// base class
#include "ak_ui_core.h"					// objectType

// Forward declaration
namespace tt { class Group; }

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace ttb {

			// Forward declaration
			class subGroup;

			class group : public ak::ui::core::ttbContainer {
			public:
				group(
					ak::messenger *				_messenger,
					ak::uidManager *			_uidManager,
					tt::Group *					_group,
					const QString &				_text
				);

				virtual ~group();

				//! @brief Will add the provided child to this container
				//! @param _child The child to add
				virtual void addChild(
					ak::ui::core::aObject *		_child
				);

				//! @brief Will add a new sub container to this container
				//! @param _text The initial text of the container
				virtual ttbContainer * createSubContainer(
					const QString &				_text = QString("")
				);

				//! @brief Will destry all sub container created by this container
				virtual void destroyAllSubContainer(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw sim::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

			private:
				tt::Group *								my_group;
				QString									my_text;
				std::vector<ak::ui::ttb::subGroup *>	my_subgroups;

				// Block default constructor
				group() = delete;

				// Block copy constructor
				group(const group & _other) = delete;

			};

		} // namespace ttb
	} // namespace ui
} // namespace ak