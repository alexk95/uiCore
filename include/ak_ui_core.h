/*
 * ak_ui_core.h
 *
 *  Created on: July 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qstring.h>		// QString
#include <qflags.h>			// QFlags

// Forward declaration
class QKeyEvent;

namespace ak {
	namespace ui {
		namespace core {
			
			//! Describes the dock location of a dock widget
			enum dockLocation {
				dock_dockLeft,
				dock_dockRight,
				dock_dockBottom
			};

			//! Descirbes the tab location
			enum tabLocation {
				tab_locationUp,
				tab_locationDown,
				tab_locationLeft,
				tab_locationRight
			};

			//! Describes the text alignment
			enum textAlignment {
				align_alignLeft,
				align_alignRight,
				align_alignCenter
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
				oDefaultWelcomeScreen,
				oDock,
				oLabel,
				oLineEdit,
				oList,
				oLogInDialog,
				oMainWindow,
				oMenu,
				oPictureBox,
				oPrompt,
				oPropertyGrid,
				oPropertyGridItem,
				oPushButton,
				oTable,
				oTabToolbar,
				oTabToolbarGroup,
				oTabToolbarPage,
				oTabToolbarSubgroup,
				oTabView,
				oTextEdit,
				oTimer,
				oTree,
				oTreeItem,
				oWidget
			};

			//! Describes the dialog result
			enum dialogResult {
				resultYes,
				resultNo,
				resultOk,
				resultCancel,
				resultRetry,
				resultIgnore,
				resultNone
			};

			//! Describes the promt type
			enum promptType {
				promptOk,
				promptYesNo,
				promptYesNoCancel,
				promptOkCancel,
				promptRetryCancel,
				promptIgnoreRetryCancel
			};

			enum keyType {
				key_Unknown,
				key_Alt,
				key_AltGr,
				key_Control,
				key_Delete,
				key_Escape,
				key_F1,
				key_F10,
				key_F11,
				key_F12,
				key_F2,
				key_F3,
				key_F4,
				key_F5,
				key_F6,
				key_F7,
				key_F8,
				key_F9,
				key_Return,
				key_Space
			};

			enum colorAreaFlag {
				caBackgroundColorWindow = 0x0,
				caForegroundColorWindow = 0x1,
				caBorderColorWindow = 0x2,
				caBackgroundColorHeader = 0x4,
				caForegroundColorHeader = 0x8,
				caBorderColorHeader = 0x10,
				caBackgroundColorControls = 0x20,
				caForegroundColorControls = 0x40,
				caBorderColorControls = 0x80,
				caDefaultBorderControls = 0x100,
				caDefaultBorderWindow = 0x200,
				caBackgroundColorAlternate = 0x400,
				caForegroundColorError = 0x800,
				caBackgroundColorFocus = 0x1000,
				caBackgroundColorSelected = 0x2000,
				caForegroundColorFocus = 0x4000,
				caForegroundColorSelected = 0x8000,
				caBackgroundColorTransparent = 0x10000,
				caImagesTree = 0x20000,
				caImagesDock = 0x40000,
				caDefaultBorderHeader = 0x80000
			};

			inline colorAreaFlag operator | (colorAreaFlag a, colorAreaFlag b)
			{ return static_cast<colorAreaFlag>(static_cast<int>(a) | static_cast<int>(b)); }

			bool flagIsSet(colorAreaFlag _flags, colorAreaFlag _testFlag);

			//! @brief Create a string representation of the provided elementType
			//! @param _elementType The elementtype to be converted
			QString toQString(
				objectType								_objectType
			);

			//! @brief Create a string representation of the provided textAlignment
			//! @param _textAlignment The text alignment to be converted
			QString toQString(
				textAlignment							_textAlignment
			);

			//! @brief Create a string representation of the provided tabLocation
			//! @param _tabLocation The tab location to be converted
			QString toQString(
				tabLocation								_tabLocation
			);

			//! @brief Create a string representation of the provided dockLocation
			//! @param _dockLocation The dock location to be converted
			QString toQString(
				dockLocation							_dockLocation
			);

			//! @brief Create a string representation of the provided dialog result
			//! @param _dialogResult The dialog result to be converted
			QString toQString(
				dialogResult							_dialogResult
			);

			//! @brief Create a string representation of the provided prompt type
			//! @param _promptType The prompt type to be converted
			QString toQString(
				promptType								_promptType
			);

			//! @brief Create a string representation of the provided keyType
			QString toQString(
				keyType									_keyType
			);

			//! @brief Will return the key type of the provided QKeyEvent
			//! If the key is not included in the keyType type a keyType::key_Unknown will be returned
			//! @param _event The event to extract the keyType from
			keyType getKey(
				QKeyEvent *								_event
			);

			//! @brief Will return the corresponding Qt text alignment
			//! @param _textAlignment The text alignment to convert
			Qt::AlignmentFlag toQtAlignment(
				textAlignment							_textAlignment
			);

		}
	}
}