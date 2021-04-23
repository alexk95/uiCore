/*
 * ak_ui_core.cpp
 *
 *  Created on: July 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_core.h>			// corresponding header

// Qt header
#include <qevent.h>				// QKeyEvent

bool ak::ui::core::flagIsSet(colorAreaFlag _flags, colorAreaFlag _testFlag) { return ((_flags & _testFlag) == _testFlag); }

QString ak::ui::core::toQString(
	objectType				_objectType
) {
	switch (_objectType)
	{
	case ui::core::oNone: return QString("None");
	case ui::core::oAction: return QString("Action");
	case ui::core::oComboBox: return QString("ComboBox");
	case ui::core::oComboButton: return QString("ComboButton");
	case ui::core::oCheckBox: return QString("CheckBox");
	case ui::core::oDefaultWelcomeScreen: return ("DefaultWelcomeScreen");
	case ui::core::oDock: return QString("Dock");
	case ui::core::oGraphicsView: return QString("GraphicsView");
	case ui::core::oLabel: return QString("Label");
	case ui::core::oLineEdit: return QString("LineEdit");
	case ui::core::oList: return QString("List");
	case ui::core::oLogInDialog: return QString("LogInDialog");
	case ui::core::oMainWindow: return QString("Window");
	case ui::core::oOptionsDialog: return QString("OptionsDialog");
	case ui::core::oPrompt: return QString("Prompt");
	case ui::core::oPropertyGrid: return QString("PropertyGrid");
	case ui::core::oPushButton: return QString("PushButton");
	case ui::core::oRadioButton: return QString("RadioButton");
	case ui::core::oTable: return QString("Table");
	case ui::core::oTabToolbar: return QString("TabToolBar");
	case ui::core::oTabToolbarGroup: return QString("TabToolBarGroup");
	case ui::core::oTabToolbarPage: return QString("TabBoolBarPage");
	case ui::core::oTabToolbarSubgroup: return QString("TabToolBarSubGroup");
	case ui::core::oTabView: return QString("TabView");
	case ui::core::oTextEdit: return QString("TextEdit");
	case ui::core::oTimer: return QString("Timer");
	case ui::core::oToolButton: return QString("ToolButton");
	case ui::core::oTree: return QString("Tree");
	case ui::core::objectType::oTreeItem: return QString("TreeItem");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::ui::core::toQString(
	textAlignment				_textAlignment
) {
	switch (_textAlignment)
	{
	case ui::core::align_alignCenter: return QString("AlignCenter");
	case ui::core::align_alignLeft: return QString("AlignLeft");
	case ui::core::align_alignRight: return QString("AlignRight");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::ui::core::toQString(
	tabLocation								_tabLocation
) {
	switch (_tabLocation)
	{
	case ui::core::tabLocation::tab_locationDown: return QString("Down");
	case ui::core::tabLocation::tab_locationLeft: return QString("Left");
	case ui::core::tabLocation::tab_locationRight: return QString("Right");
	case ui::core::tabLocation::tab_locationUp: return QString("Up");
	default: assert(0); // Not implemented yet
		break;
	}
	return QString("");
}

QString ak::ui::core::toQString(
	dockLocation				_dockLocation
) {
	switch (_dockLocation)
	{
	case ui::core::dock_dockBottom: return QString("DockBottom");
	case ui::core::dock_dockLeft: return QString("DockLeft");
	case ui::core::dock_dockRight: return QString("DockRight");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::ui::core::toQString(
	dialogResult							_dialogResult
) {
	switch (_dialogResult)
	{
	case ui::core::resultYes: return QString("Result.Yes");
	case ui::core::resultNo: return QString("Result.No");
	case ui::core::resultOk: return QString("Result.Ok");
	case ui::core::resultCancel: return QString("Result.Cancel");
	case ui::core::resultIgnore: return QString("Result.Ignore");
	case ui::core::resultRetry: return QString("Result.Retry");
	case ui::core::resultNone: return QString("Result.None");
	default:
		assert(0);
		return QString();
	}
}

QString ak::ui::core::toQString(
	promptType								_promptType
) {
	switch (_promptType)
	{
	case ui::core::promptOk: return QString("Prompt.Ok");
	case ui::core::promptYesNo: return QString("Prompt.YesNo");
	case ui::core::promptYesNoCancel: return QString("Prompt.YesNoCancel");
	case ui::core::promptOkCancel: return QString("Prompt.OkCancel");
	case ui::core::promptRetryCancel: return QString("Prompt.RetryCancel");
	case ui::core::promptIgnoreRetryCancel: return QString("Prompt.IgnoreRetryCancel");
	default:
		assert(0); // Unknown type
		return QString();
	}
}

QString ak::ui::core::toQString(
	keyType									_keyType
) {
	switch (_keyType)
	{
	case ui::core::key_Unknown: return QString("KeyUnknown");
	case ui::core::key_Alt: return QString("KeyAlt");
	case ui::core::key_AltGr: return QString("KeyAltGr");
	case ui::core::key_Control: return QString("KeyControl");
	case ui::core::key_Delete: return QString("KeyDelete");
	case ui::core::key_Escape: return QString("KeyEscape");
	case ui::core::key_F1: return QString("KeyF1");
	case ui::core::key_F10: return QString("KeyF10");
	case ui::core::key_F11: return QString("KeyF11");
	case ui::core::key_F12: return QString("KeyF12");
	case ui::core::key_F2: return QString("KeyF2");
	case ui::core::key_F3: return QString("KeyF3");
	case ui::core::key_F4: return QString("KeyF4");
	case ui::core::key_F5: return QString("KeyF5");
	case ui::core::key_F6: return QString("KeyF6");
	case ui::core::key_F7: return QString("KeyF7");
	case ui::core::key_F8: return QString("KeyF8");
	case ui::core::key_F9: return QString("KeyF9");
	case ui::core::key_Return: return QString("KeyReturn");
	case ui::core::key_Space: return QString("KeySpace");
	default:
		assert(0); // Not implemented yet
		return QString("");
	}
}

QString ak::ui::core::toQString(
	contextMenuRole							_role
) {
	switch (_role)
	{
	case ak::ui::core::crNone: return QString("None");
	case ak::ui::core::crClear: return QString("Clear");
	default:
		assert(0); // Not implemented yet
		return QString("");
	}
}

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
	case ui::core::align_alignLeft: return Qt::AlignmentFlag::AlignLeft;
	case ui::core::align_alignRight: return Qt::AlignmentFlag::AlignRight;
	case ui::core::align_alignCenter: return Qt::AlignmentFlag::AlignCenter;
	default:
		assert(0);	// Not implemented yet
		return Qt::AlignmentFlag::AlignCenter;
		break;
	}
}