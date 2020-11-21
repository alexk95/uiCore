/*
 * ak_ui_core_ttbContainer.h
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <vector>

// Qt header
#include <qstring.h>

// AK header
#include <ak_ui_core_aPaintable.h>	// base class
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_ui_core.h>				// objectType

#define TTB_CONT_DEL_PARENTCHECK assert(my_parentObject != nullptr); my_parentObject->removeChildObject(this);
#define TTB_CONTAINER_DESTROYING TTB_CONT_DEL_PARENTCHECK

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace core {

			//! This class provides a interface that represents a tab toolbar container.
			
			class ttbContainer : public ak::ui::core::aPaintable {
			public:

				//! @brief Constructor, initializes the uid of this container
				//! @param _UID The initial UID of this object
				//! @param _references The initial reference count for this object
				//! @param _colorStyle The initial color style used in this paintable
				ttbContainer(
					ak::messenger *				_messenger,
					ak::ui::core::objectType	_type,
					const ak::ui::colorStyle *	_colorStyle = nullptr,
					ak::UID						 _UID = ak::invalidUID,
					int							_references = 1
				);

				//! @brief Deconstructor
				virtual ~ttbContainer();

				//! @brief Returns the count of sub containers
				int subContainerCount(void) const;

				//! @brief Will add the provided child to this container
				//! @param _child The child to add
				virtual void addChild(
					ak::ui::core::aObject *		_child
				) = 0;

				//! @brief Will add a new sub container to this container
				//! @param _text The initial text of the container
				virtual ttbContainer * createSubContainer(
					const QString &				_text = QString("")
				) = 0;

				//! @brief Will return the sub container with the specified text
				//! Returns nullptr if no sub container with the specified text exists
				//! @param _text The text of the sub container to find
				ak::ui::core::ttbContainer * getSubContainer(
					const QString &				_text
				);

				virtual void destroyAllSubContainer(void) = 0;

				//! @brief Will return the text of this ttb container
				QString text(void) const { return my_text; }

			protected:
				ak::messenger *					my_messenger;
				std::vector<ttbContainer *>		my_subContainer;
				QString							my_text;

			private:
				// Block default constructor
				ttbContainer() = delete;

				// Block copy constructor
				ttbContainer(const ttbContainer & _other) = delete;

				// Block assignment operator
				ttbContainer & operator = (const ttbContainer & _other) = delete;
			};
		} // namespace core
	} // namespace ui
} // namespace ak