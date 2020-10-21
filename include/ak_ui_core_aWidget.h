/*
 * ak_ui_core_aWidget.h
 *
 *  Created on: July 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aPaintable.h>	// base class
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_ui_core.h>				// objectType

// Forward declaration
class QWidget;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;
		namespace qt { class dock; }

		namespace core {

			//! This class provides a interface that represents a widget.
			//! Every class derived from this class must be able to provide a QWidget that is representing it
			class UICORE_API aWidget : public ak::ui::core::aPaintable {
			public:

				//! @brief Constructor
				//! @param _UID The initial UID of this object
				//! @param _references The initial reference count for this object
				//! @param _colorStyle The initial color style used in this paintable
				aWidget(
					ak::ui::core::objectType	_type,
					const ak::ui::colorStyle *	_colorStyle = nullptr,
					ak::UID						 _UID = ak::invalidUID,
					int							_references = 1
				);

				//! @brief Deconstructor
				virtual ~aWidget();

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) = 0;

				//! @brief Returns true the object is derived from aWidget
				virtual bool isWidgetType(void) const override;

				//! @brief Will set the dock this widget is displayed in
				void setParentDock(
					ak::ui::qt::dock *						_dock
				);

				//! @brief Will return the parent dock this widget is displayed in
				ak::ui::qt::dock * parentDock(void) const;

			protected:
				ak::ui::qt::dock *				my_parentDock;

			private:

				// Block default constructor
				aWidget() = delete;

				// Block copy constructor
				aWidget(const aWidget & _other) = delete;

				// Block assignment operator
				aWidget & operator = (const aWidget & _other) = delete;
			};
		} // namespace core
	} // namespace ui
} // namespace ak