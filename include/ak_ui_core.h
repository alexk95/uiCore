/*
 * ak_ui_core.h
 *
 *  Created on: July 18, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// Qt header
#include <qstring.h>		// QString

namespace ak {
	namespace ui {
		namespace core {
			
			//! Describes the dock location of a dock widget
			enum dockLocation {
				dockLeft,
				dockRight,
				dockBottom
			};

			//! Describes the text alignment
			enum textAlignment {
				alignLeft,
				alignRight,
				alignCenter
			};

			//! Describes the type of an object
			enum objectType {
				oNone,
				oAction,
				oColorEditButton,
				oComboBox,
				oComboButton,
				oComboButtonItem,
				oCheckBox,
				oDock,
				oMainWindow,
				oMenu,
				oPictureBox,
				oPropertyGrid,
				oPropertyGridItem,
				oPushButton,
				oTable,
				oTabToolbar,
				oTabToolbarGroup,
				oTabToolbarPage,
				oTabToolbarSubgroup,
				oTextEdit,
				oTree,
				oTreeItem,
				oWidget
			};

			//! @brief Create a string representation of the provided ak::core::elementType
			//! @param _elementType The elementtype to be converted
			QString toQString(
				objectType								_objectType
			);

			//! @brief Create a string representation of the provided ak::core::textAlignment
			//! @param _textAlignment The text alignment to be converted
			QString toQString(
				textAlignment							_textAlignment
			);

			//! @brief Create a string representation of the provided ak::core::dockLocation
			//! @param _dockLocation The dock location to be converted
			QString toQString(
				dockLocation							_dockLocation
			);

			//! @brief Will return the corresponding Qt text alignment
			//! @param _textAlignment The text alignment to convert
			Qt::AlignmentFlag toQtAlignment(
				textAlignment							_textAlignment
			);

		}
	}
}