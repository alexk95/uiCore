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
				oGraphicsView,
				oLabel,
				oLineEdit,
				oList,
				oLogInDialog,
				oMainWindow,
				oMenu,
				oOptionsDialog,
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
				oToolButton,
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
				caBackgroundColorWindow			= 1 << 0,
				caForegroundColorWindow			= 1 << 1,
				caBorderColorWindow				= 1 << 2,
				caBackgroundColorHeader			= 1 << 3,
				caForegroundColorHeader			= 1 << 4,
				caBorderColorHeader				= 1 << 5,
				caBackgroundColorControls		= 1 << 6,
				caForegroundColorControls		= 1 << 7,
				caBorderColorControls			= 1 << 8,
				caDefaultBorderControls			= 1 << 9,
				caDefaultBorderWindow			= 1 << 10,
				caBackgroundColorAlternate		= 1 << 11,
				caForegroundColorError			= 1 << 12,
				caBackgroundColorFocus			= 1 << 13,
				caBackgroundColorSelected		= 1 << 14,
				caForegroundColorFocus			= 1 << 15,
				caForegroundColorSelected		= 1 << 16,
				caBackgroundColorTransparent	= 1 << 17,
				caImagesTree					= 1 << 18,
				caImagesDock					= 1 << 19,
				caDefaultBorderHeader			= 1 << 20,
				caBackgroundColorDialogWindow	= 1 << 21,
				caForegroundColorDialogWindow	= 1 << 22,
				caBackgroundColorButton			= 1 << 23,
				caForegroundColorButton			= 1 << 24
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