/*
 * ak_ui_core_aWidgetManager.h
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// AK header
#include <ak_ui_core_aWidget.h>		// base class
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_ui_core.h>				// objectType

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		//class iconManager;
		//class objectManager;
		class colorStyle;
		//class signalLinker;

		namespace core {

			//! This class is a interface representing a manager class that is managing an aWidget object.
			//! This class provides the main functionallity for such a manager class like the messenger, the uidManager or the objectManager
			class UICORE_API aWidgetManager : public ak::ui::core::aWidget {
			public:
				
				//! @brief Constructor
				//! @param _messenger The messenger used in this object manager, if nullptr a new one will be created
				//! @param _uidManager The UID manager used in this object manager, if nullptr a new one will be created
				//! @param _objectManager The object manager used in this object manager, if nullptr a new one will be created
				//! @param _iconManager The icon manager used in this object manager, if nullptr a new one will be created
				//! @param _UID The initial UID of this object
				//! @param _references The initial reference count for this object
				//! @param _colorStyle The initial color style used in this paintable
				aWidgetManager(
					ak::ui::core::objectType	_type,
					ak::messenger *				_messenger = nullptr,
					ak::uidManager *			_uidManager = nullptr,
					const ak::ui::colorStyle *	_colorStyle = nullptr,
					ak::UID						 _UID = ak::invalidUID,
					int							_references = 1
				);

				virtual ~aWidgetManager();

			protected:
				ak::messenger *					my_messenger;				//! The messenger this widget manager is using
				ak::uidManager *				my_uidManager;				//! The UID manager this widget manager is using
				//ak::ui::iconManager *			my_iconManager;				//! The icon manager this widget manager is using
				//ak::ui::objectManager *			my_objectManager;			//! The object manager this widget manager is using
				//ak::ui::signalLinker *			my_signalLinker;			//! The signal linker this widget manager is using

				//! @brief Will remove all objects created by this widget manager
				void memFree(void);

			private:

				bool							my_messengerCreated;		//! If true, then the messenger was created by this widget manager
				bool							my_uidManagerCreated;		//! If true, then the UID manager was created by this widget manager
				bool							my_objectManagerCreated;	//! If true, then the objectManager was created by this widget manager

				// Block default constructor
				aWidgetManager() = delete;

				// Block copy constructor
				aWidgetManager(const aWidgetManager & _other) = delete;

			};

		} // namespace core
	} // namespace ui
} // namespace ak