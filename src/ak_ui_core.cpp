/*
 * ak_ui_core.cpp
 *
 *  Created on: July 18, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_core.h"			// corresponding header
#include <qevent.h>				// QKeyEvent

QString ak::ui::core::toQString(
	objectType				_objectType
) {
	switch (_objectType)
	{
	case ak::ui::core::oNone: return QString("None");
	case ak::ui::core::oAction: return QString("Action");
	case ak::ui::core::oComboBox: return QString("ComboBox");
	case ak::ui::core::oComboButton: return QString("ComboButton");
	case ak::ui::core::oCheckBox: return QString("CheckBox");
	case ak::ui::core::oPictureBox: return QString("PictureBox");
	case ak::ui::core::oPropertyGrid: return QString("PropertyGrid");
	case ak::ui::core::oPushButton: return QString("PushButton");
	case ak::ui::core::oTable: return QString("Table");
	case ak::ui::core::oTabToolbar: return QString("TabToolBar");
	case ak::ui::core::oTabToolbarGroup: return QString("TabToolBarGroup");
	case ak::ui::core::oTabToolbarPage: return QString("TabBoolBarPage");
	case ak::ui::core::oTabToolbarSubgroup: return QString("TabToolBarSubGroup");
	case ak::ui::core::oTextEdit: return QString("TextEdit");
	case ak::ui::core::oTree: return QString("Tree");
	case ak::ui::core::objectType::oTreeItem: return QString("TreeItem");
	case ak::ui::core::objectType::oWidget: return QString("Widget");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::ui::core::toQString(
	textAlignment				_textAlignment
) {
	switch (_textAlignment)
	{
	case ak::ui::core::align_alignCenter: return QString("AlignCenter");
	case ak::ui::core::align_alignLeft: return QString("AlignLeft");
	case ak::ui::core::align_alignRight: return QString("AlignRight");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::ui::core::toQString(
	dockLocation				_dockLocation
) {
	switch (_dockLocation)
	{
	case ak::ui::core::dock_dockBottom: return QString("DockBottom");
	case ak::ui::core::dock_dockLeft: return QString("DockLeft");
	case ak::ui::core::dock_dockRight: return QString("DockRight");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::ui::core::toQString(
	keyType									_keyType
) {
	switch (_keyType)
	{
	case ak::ui::core::key_Unknown: return QString("KeyUnknown");
	case ak::ui::core::key_Alt: return QString("KeyAlt");
	case ak::ui::core::key_AltGr: return QString("KeyAltGr");
	case ak::ui::core::key_Control: return QString("KeyControl");
	case ak::ui::core::key_Delete: return QString("KeyDelete");
	case ak::ui::core::key_Escape: return QString("KeyEscape");
	case ak::ui::core::key_F1: return QString("KeyF1");
	case ak::ui::core::key_F10: return QString("KeyF10");
	case ak::ui::core::key_F11: return QString("KeyF11");
	case ak::ui::core::key_F12: return QString("KeyF12");
	case ak::ui::core::key_F2: return QString("KeyF2");
	case ak::ui::core::key_F3: return QString("KeyF3");
	case ak::ui::core::key_F4: return QString("KeyF4");
	case ak::ui::core::key_F5: return QString("KeyF5");
	case ak::ui::core::key_F6: return QString("KeyF6");
	case ak::ui::core::key_F7: return QString("KeyF7");
	case ak::ui::core::key_F8: return QString("KeyF8");
	case ak::ui::core::key_F9: return QString("KeyF9");
	case ak::ui::core::key_Return: return QString("KeyReturn");
	case ak::ui::core::key_Space: return QString("KeySpace");
	default:
		assert(0); // Not implemented yet
		return QString("");
	}
}

//! @brief Will return the key type of the provided QKeyEvent
//! If the key is not included in the keyType type a keyType::key_Unknown will be returned
//! @param _event The event to extract the keyType from
ak::ui::core::keyType ak::ui::core::getKey(
	QKeyEvent *								_event
) {
	assert(_event != nullptr); // Nullptr provided
	switch (_event->key())
	{
	case Qt::Key::Key_Alt: return keyType::key_Alt; break;
	case Qt::Key::Key_AltGr: return keyType::key_AltGr; break;
	case Qt::Key::Key_Control: return keyType::key_Control; break;
	case Qt::Key::Key_Delete: return keyType::key_Delete; break;
	case Qt::Key::Key_Backspace: return keyType::key_Delete; break;
	case Qt::Key::Key_Escape: return keyType::key_Escape; break;
	case Qt::Key::Key_F1: return keyType::key_F1; break;
	case Qt::Key::Key_F10: return keyType::key_F10; break;
	case Qt::Key::Key_F11: return keyType::key_F11; break;
	case Qt::Key::Key_F12: return keyType::key_F12; break;
	case Qt::Key::Key_F2: return keyType::key_F2; break;
	case Qt::Key::Key_F3: return keyType::key_F3; break;
	case Qt::Key::Key_F4: return keyType::key_F4; break;
	case Qt::Key::Key_F5: return keyType::key_F5; break;
	case Qt::Key::Key_F6: return keyType::key_F6; break;
	case Qt::Key::Key_F7: return keyType::key_F7; break;
	case Qt::Key::Key_F8: return keyType::key_F8; break;
	case Qt::Key::Key_F9: return keyType::key_F9; break;
	case Qt::Key::Key_Return: return keyType::key_Return; break;
	case Qt::Key::Key_Space: return keyType::key_Space; break;
	default: return keyType::key_Unknown;
	}
}

Qt::AlignmentFlag ak::ui::core::toQtAlignment(
	textAlignment							_textAlignment
) {
	switch (_textAlignment)
	{
	case ak::ui::core::align_alignLeft: return Qt::AlignmentFlag::AlignLeft;
	case ak::ui::core::align_alignRight: return Qt::AlignmentFlag::AlignRight;
	case ak::ui::core::align_alignCenter: return Qt::AlignmentFlag::AlignCenter;
	default:
		assert(0);	// Not implemented yet
		return Qt::AlignmentFlag::AlignCenter;
		break;
	}
}