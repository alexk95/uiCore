/*
 * ak_ui_core_aObject.h
 *
 *  Created on: July 26, 2020
 *	Last modified on: October 01, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_ui_core.h>				// objectType

// Qt header
#include <qstring.h>				// QString

// Rapid JSON header
#include <rapidjson/document.h>		// rapidjson::Value

namespace ak {
	namespace ui {
		namespace core {

			//! This class is used to store the main information of any object used
			//! Information contained is the UID and the reference counter
			class UICORE_API aObject {
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

				// ################################################################################

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

				//! @brief Will set the alias for this object
				//! @param _alias The alias to set
				//! @throw ak::Exception if the provided alias length is 0
				void setAlias(
					const QString &					_alias
				);

				//! @brief Will return the alias of this object
				QString alias(void) const;

				//! @brief Will return true if a settings value is creatable
				//! A settings value is createable if a alias is provided and the object is suitable to be rebuild later (chek documentation)
			//	virtual bool settingsValueCreatable(void) const = 0;

				//! @brief Returns true if the object is an object derived from aPaintable
				virtual bool isPaintableType(void) const;

				//! @brief Returns true if the object is an object derived from aWidget
				virtual bool isWidgetType(void) const;

				//! @brief Will create a rapidjson::Value representing this objects current state
				//! The value looks like this:
				//!	     { "Alias":"[ObjectAlias]","Type":"[ObjectType]","Settings":{...} }
			//	virtual rapidjson::Value createSettingsValue(void) const = 0;

			protected:
				ak::UID								my_uid;				//! The objects UID
				int									my_references;		//! The objects references
				ak::ui::core::objectType			my_objectType;		//! The object type of this object
				QString								my_alias;

			private:
				// Block default constructor
				aObject() = delete;
			};
		}
	}
}