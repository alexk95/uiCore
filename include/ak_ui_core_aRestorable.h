/*
 * ak_ui_core_aRestorable.h
 *
 *  Created on: October 01, 2020
 *	Last modified on: October 01, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
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
#define RESTORABLE_UI_COLORSTYLE "UI.ColorStyle"

#define RESTORABLE_CFG_SIZE_X "Size.Width"
#define RESTORABLE_CFG_SIZE_Y "Size.Height"
#define RESTORABLE_CFG_CHECKED "Checked"
#define RESTORABLE_CFG_STATE "State"


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

				//! @brief Will restore the settings from the provided JSON value which must have an object type
				//! The value looks like this:
				//!	     { { "[SettingsName]":"[SettingsValue]",... } }
				//! @param _settings The settings to restore
				virtual void restoreSettings(
					const rapidjson::Value &				_settings
				) = 0;

				//! @brief Will set the alias for this object
				//! @param _alias The alias to set
				//! @throw ak::Exception if the provided alias length is 0
				virtual void setAlias(
					const QString &							_alias
				) = 0;

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
