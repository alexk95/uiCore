/*
 * ak_ui_core_aObject.h
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_globalDataTypes.h"		// UID and ID type
#include "ak_ui_core.h"				// objectType

namespace ak {
	namespace ui {
		namespace core {

			//! This class is used to store the main information of any object used
			//! Information contained is the UID and the reference counter
			class __declspec(dllexport) aObject {
			public:

				//! @brief Constructor
				//! @param _UID The initial UID of this object
				//! @param _references The initial reference count for this object
				aObject(
					ak::ui::core::objectType		_type,
					ak::UID							_UID = ak::invalidUID,
					int								_references = 1
				);

				//! @brief Copy constructor
				//! @param _other The other object
				aObject(
					const ak::ui::core::aObject &	_other
				);

				//! @brief Assignment operator
				//! @param _other The other object
				core::aObject & operator = (
					const core::aObject &			_other
					);

				//! @brief Deconstructor
				virtual ~aObject();

				//! @brief Will set the objects UID
				void setUid(
					ak::UID							_UID
				);

				//! @brief Returns the objects UID
				ak::UID uid(void) const;

				//! @brief Will set the objects references
				void setReferences(
					int								_references
				);

				//! @brief Returns the objects references
				int references(void) const;

				//! @brief Inkrements the objects references
				int incrReferences(void);

				//! @brief Decrements the objects references
				int decrReferences(void);

				//! @brief Returns the objects type
				ak::ui::core::objectType objectType(void) const;

				//! @brief Returns true if the object is an object derived from aPaintable
				virtual bool isPaintableType(void) const;

				//! @brief Returns true if the object is an object derived from aWidget
				virtual bool isWidgetType(void) const;

			protected:
				ak::UID								my_uid;				//! The objects UID
				int									my_references;		//! The objects references
				ak::ui::core::objectType			my_objectType;		//! The object type of this object

			private:
				// Block default constructor
				aObject() = delete;
			};
		}
	}
}