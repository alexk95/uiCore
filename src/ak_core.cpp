/*
 * ak_core.cpp
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_core.h>		// corresponding class

QString ak::core::toQString(
	eventType				_eventType
) {
	switch (_eventType)
	{
	case ak::core::eUnknownEvent: return QString("UnknownEvent");
	case ak::core::eActivated: return QString("Activated");
	case ak::core::eChanged: return QString("Changed");
	case ak::core::eCleared: return QString("Cleared");
	case ak::core::eClicked: return QString("Clicked");
	case ak::core::eClosing: return QString("Closing");
	case ak::core::eCollpased: return QString("Collapsed");
	case ak::core::eCursorPosotionChanged: return QString("CursorPositionChanged");
	case ak::core::eDestroyed: return QString("Destroyed");
	case ak::core::eDoubleClicked: return QString("DoubleClicked");
	case ak::core::eExpanded: return QString("Expanded");
	case ak::core::eFocused: return QString("Focused");
	case ak::core::eFocusLeft: return QString("FocusLeft");
	case ak::core::eIndexChanged: return QString("IndexChanged");
	case ak::core::eInvalidEntry: return QString("InvalidEntry");
	case ak::core::eItemTextChanged: return QString("ItemTextChanged");
	case ak::core::eKeyPressed: return QString("KeyPressed");
	case ak::core::eKeyReleased: return QString("KeyReleased");
	case ak::core::eLocationChanged: return QString("LocationChanged");
	case ak::core::eReleased: return QString("Released");
	case ak::core::eSelectionChanged: return QString("SelectionChanged");
	case ak::core::eStateChanged: return QString("StateChanged");
	case ak::core::eTabToolbarChanged: return QString("TabToolbarChanged");
	case ak::core::eTabToolbarClicked: return QString("TabToolbarClicked");
	case ak::core::eTimeout: return QString("Timeout");
	case ak::core::eToggeledChecked: return QString("ToggeledToChecked");
	case ak::core::eToggeledUnchecked: return QString("ToggeledToUnchecked");
	case ak::core::eContextMenuItemClicked: return QString("ContextMenuItemClicked");
	case ak::core::eContextMenuItemCheckedChanged: return QString("ContextMenuItemCheckedChanged");
	case ak::core::eVisibilityChanged: return QString("VisibilityChanged");
	default: assert(0); // not implemented yet
	}
	return QString("");
}

QString ak::core::toQString(
	settingsRestoreErrorCode				_settingsRestoreErrorCode
) {
	switch (_settingsRestoreErrorCode)
	{
	case ak::core::srecNone: return QString("None");
	case ak::core::srecAppVersionMismatch: return QString("AppVersionMismatch");
	case ak::core::srecSettingsVersionMismatch: return QString("SettingsVersionMismatch");
	case ak::core::srecEmptySettingsString: return QString("EmptySettingsString");
	default: assert(0); // not implemented yet
		return QString("");
	}
}

QString ak::core::toQString(
	valueType				_valueType
) {
	switch (_valueType)
	{
	case ak::core::valueType::vArray: return QString("Array"); break;
	case ak::core::valueType::vBool: return QString("Bool"); break;
	case ak::core::valueType::vColor: return QString("Color"); break;
	case ak::core::valueType::vDouble: return QString("Double"); break;
	case ak::core::valueType::vFloat: return QString("Float"); break;
	case ak::core::valueType::vInt: return QString("Integer"); break;
	case ak::core::valueType::vInt64: return QString("64bitInteger"); break;
	case ak::core::valueType::vLosslessDouble: return QString("LosslessDouble"); break;
	case ak::core::valueType::vLosslessFloat: return QString("LosslessFloat"); break;
	case ak::core::valueType::vNull: return QString("Null"); break;
	case ak::core::valueType::vNumber: return QString("Number"); break;
	case ak::core::valueType::vObject: return QString("Object"); break;
	case ak::core::valueType::vSelection: return QString("Selection"); break;
	case ak::core::valueType::vString: return QString("String"); break;
	case ak::core::valueType::vUint: return QString("UnsignedInteger"); break;
	case ak::core::valueType::vUint64: return QString("Unsigned64bitInteger"); break;
	default: assert(0); // not implemented yet
	}
	return QString("");
}

bool ak::core::numbers::isDecimal(const char *str) {
	bool failed;
	validateNumber<double>(str, failed);
	return !failed;
}

bool ak::core::numbers::isDecimal(const std::string &str) {
	bool failed;
	validateNumber<double>(str, failed);
	return !failed;
}

bool ak::core::numbers::isDecimal(const QString &str) {
	bool failed;
	validateNumber<double>(str.toStdString(), failed);
	return !failed;
}

bool ak::core::numbers::isInteger(const char *str) {
	bool failed;
	validateNumber<int>(str, failed);
	return !failed;
}

bool ak::core::numbers::isInteger(const std::string &str) {
	bool failed;
	validateNumber<int>(str, failed);
	return !failed;
}

bool ak::core::numbers::isInteger(const QString &str) {
	bool failed;
	validateNumber<int>(str.toStdString(), failed);
	return !failed;
}

bool ak::core::numbers::isNumericOnly(const char *str) {
	QString s{ str };
	for (auto c : s) {
		if (!c.isNumber()) { return false; }
	}
	return true;
}

bool ak::core::numbers::isNumericOnly(const std::string &str) {
	QString s{ str.c_str() };
	for (auto c : s) {
		if (!c.isNumber()) { return false; }
	}
	return true;
}

bool ak::core::numbers::isNumericOnly(const QString &str) {
	for (auto c : str) {
		if (!c.isNumber()) { return false; }
	}
	return true;
}

// ########################################################################################
// String

std::vector<int> ak::core::str::toCharIndex(
	const char *					str
) {
	std::vector<int> out;
	if (str != nullptr) {
		while (*str != 0) { out.push_back((int)*str); str++; }
	}
	out.push_back(0);
	return out;
}