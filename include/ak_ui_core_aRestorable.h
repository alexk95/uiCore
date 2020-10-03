/*
 * ak_ui_core_aRestorable.h
 *
 *  Created on: October 01, 2020
 *	Last modified on: October 01, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_globalDataTypes.h>

// Rapid JSON header
#include <rapidjson/document.h>		// RJSON Document

// Qt header
#include <qstring.h>

#define RESTORABLE_NAME_ALIAS "Alias"
#define RESTORABLE_NAME_TYPE "ObjectType"
#define RESTORABLE_NAME_SETTINGS "Settings"
#define RESTORABLE_UI_SETTINGS "UI.Settings"

#define RESTORABLE_CFG_SIZE_X "Size.Width"
#define RESTORABLE_CFG_SIZE_Y "Size.Height"


namespace ak {
	namespace ui {
		namespace core {

			//! Represents an object that can save and restore its settings
			class UICORE_API aRestorable {
			public:

				//! @brief Default constructor
				aRestorable();

				//! @brief Constructor
				//! @param _alias The initial alias
				aRestorable(
					const QString &							_alias
				);

				//! @brief Deconstructor
				virtual ~aRestorable();

				//! @brief Will create a rapidjson::Value representing this objects current state
				//! The value looks like this:
				//!	     { "Alias":"[ObjectAlias]","Type":"[ObjectType]","Settings":{...} }
				virtual void addObjectSettingsToValue(
					rapidjson::Value &						_array,
					rapidjson::Document::AllocatorType &	_allocator
				) = 0;

				//! @brief Will set the alias for this object
				//! @param _alias The alias to set
				//! @throw ak::Exception if the provided alias length is 0
				void setAlias(
					const QString &							_alias
				);

				//! @brief Will return the alias of this object
				QString alias(void) const;

			protected:
				QString								my_alias;

			private:
				aRestorable(const aRestorable &) = delete;

			};

		}
	}
}
