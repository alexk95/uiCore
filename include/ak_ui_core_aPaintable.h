/*
 * ak_ui_core_aPaintable.h
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_ui_core_aObject.h>		// base class
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_ui_core.h>				// objectType

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace core {

			//! This class represents a object that can be painted.
			//! More specific a object where the color style may be applied to, either by using a stylesheet or simply by using setBack- or setForeColor
			class UICORE_API aPaintable : public ak::ui::core::aObject {
			public:

				//! @brief Constructor
				//! @param _UID The initial UID of this object
				//! @param _references The initial reference count for this object
				//! @param _colorStyle The initial color style used in this paintable
				aPaintable(
					ak::ui::core::objectType		_type,
					ak::ui::colorStyle *			_colorStyle = nullptr,
					ak::UID							_UID = ak::invalidUID,
					int								_references = 1
				);

				//! @brief Deconstructor
				virtual ~aPaintable();

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				) = 0;

				//! @brief Returns true because the object is derived from a aPaintable
				virtual bool isPaintableType(void) const override;

			protected:
				ak::ui::colorStyle *				my_colorStyle;			//! The current objects color style

			private:
				// Block default constructor
				aPaintable() = delete;

				// Block copy constructor
				aPaintable(const aPaintable & _other) = delete;

				// Block assignment operator
				aPaintable & operator = (const aPaintable & _other) = delete;
			};
		} // namespace core
	} // namespace ui
} // namespace ak
