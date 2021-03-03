/*
 * ak_ui_ttb_group.h
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *
 *	This class is using a modified version of the "Qt TabToolbar" library.
 *	The changes to the library are listed in the documentation which was
 *	provided with this source code package.
 */

#pragma once

 // C++ header
#include <vector>						// vector<>

// Qt header
#include <qstring.h>					// QString

// AK header
#include <ak_ui_core_ttbContainer.h>	// base class
#include <ak_ui_core.h>					// objectType
#include <ak_globalDataTypes.h>

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

			class UICORE_API_EXPORT group : public ak::ui::core::ttbContainer {
			public:
				group(
					ak::messenger *				_messenger,
					tt::Group *					_group,
					const QString &				_text
				);

				virtual ~group();

				//! @brief Will add the provided child to this container
				//! @param _child The child to add
				virtual void addChild(
					ak::ui::core::aObject *		_child
				) override;

				//! @brief Will add a new sub container to this container
				//! @param _text The initial text of the container
				virtual ttbContainer * createSubContainer(
					const QString &				_text = QString("")
				) override;

				//! @brief Will destry all sub container created by this container
				virtual void destroyAllSubContainer(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				//! @brief Will remove the child from this object (not destroy it)
				//! This function should be called from the deconstructor of a child
				//! @param _child The child to remove
				virtual void removeChildObject(
					aObject *								_child
				) override;

				//! @brief Will set the enabled state of this container
				//! @param _enbaled The enabled state to set
				virtual void setEnabled(
					bool						_enabled
				) override;

			private:
				tt::Group *								my_group;
				//std::vector<ak::ui::ttb::subGroup *>	my_subgroups;

				// Block default constructor
				group() = delete;

				// Block copy constructor
				group(const group & _other) = delete;

			};

		} // namespace ttb
	} // namespace ui
} // namespace ak