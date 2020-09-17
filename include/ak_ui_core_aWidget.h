/*
 * ak_ui_core_aWidget.h
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_ui_core_aPaintable.h"	// base class
#include "ak_globalDataTypes.h"		// UID and ID type
#include "ak_ui_core.h"				// objectType

// Forward declaration
class QWidget;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace core {

			//! This class provides a interface that represents a widget.
			//! Every class derived from this class must be able to provide a QWidget that is representing it
			class __declspec(dllexport) aWidget : public ak::ui::core::aPaintable {
			public:

				//! @brief Constructor
				//! @param _UID The initial UID of this object
				//! @param _references The initial reference count for this object
				//! @param _colorStyle The initial color style used in this paintable
				aWidget(
					ak::ui::core::objectType	_type,
					ak::ui::colorStyle *		_colorStyle = nullptr,
					ak::UID						 _UID = ak::invalidUID,
					int							_references = 1
				);

				//! @brief Deconstructor
				virtual ~aWidget();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) = 0;

				//! @brief Returns true the object is derived from aWidget
				virtual bool isWidgetType(void) const;

			private:
				// Block default constructor
				aWidget() : aPaintable(ak::ui::core::objectType::oNone) {}

				// Block copy constructor
				aWidget(const aWidget & _other) : aPaintable(ak::ui::core::objectType::oNone) {}

				// Block assignment operator
				aWidget & operator = (const aWidget & _other) { return *this; }

			};
		} // namespace core
	} // namespace ui
} // namespace ak