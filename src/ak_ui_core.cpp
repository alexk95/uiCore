/*
 * ak_ui_core.cpp
 *
 *  Created on: July 18, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_core.h"			// corresponding header

QString ak::ui::core::toQString(
	objectType				_objectType
) {
	switch (_objectType)
	{
	case ak::ui::core::oNone: return QString("None");
	case ak::ui::core::oAction: return QString("Action");
	case ak::ui::core::oComboBox: return QString("Combobox");
	case ak::ui::core::oComboButton: return QString("Combobutton");
	case ak::ui::core::oCheckBox: return QString("Checkbox");
	case ak::ui::core::oPictureBox: return QString("Picturebox");
	case ak::ui::core::oPropertyGrid: return QString("Property grid");
	case ak::ui::core::oPushButton: return QString("Pushbutton");
	case ak::ui::core::oTable: return QString("Table");
	case ak::ui::core::oTabToolbar: return QString("Tab toolbar");
	case ak::ui::core::oTabToolbarGroup: return QString("Tab toolbar Group");
	case ak::ui::core::oTabToolbarPage: return QString("Tab toolbar Page");
	case ak::ui::core::oTabToolbarSubgroup: return QString("Tab toolbar Subgroup");
	case ak::ui::core::oTextEdit: return QString("Text edit");
	case ak::ui::core::oTree: return QString("Tree");
	case ak::ui::core::objectType::oTreeItem: return QString("Tree item");
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
	case ak::ui::core::oNone: return QString("None");
	case ak::ui::core::oAction: return QString("Action");
	case ak::ui::core::oComboBox: return QString("Combobox");
	case ak::ui::core::oComboButton: return QString("Combobutton");
	case ak::ui::core::oCheckBox: return QString("Checkbox");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

QString ak::ui::core::toQString(
	dockLocation				_dockLocation
) {
	switch (_dockLocation)
	{
	case ak::ui::core::oNone: return QString("None");
	case ak::ui::core::oAction: return QString("Action");
	case ak::ui::core::oComboBox: return QString("Combobox");
	case ak::ui::core::oComboButton: return QString("Combobutton");
	case ak::ui::core::oCheckBox: return QString("Checkbox");
	case ak::ui::core::oPictureBox: return QString("Picturebox");
	case ak::ui::core::oPropertyGrid: return QString("Property grid");
	case ak::ui::core::oPushButton: return QString("Pushbutton");
	case ak::ui::core::oTable: return QString("Table");
	case ak::ui::core::oTabToolbar: return QString("Tab toolbar");
	case ak::ui::core::oTabToolbarGroup: return QString("Tab toolbar Group");
	case ak::ui::core::oTabToolbarPage: return QString("Tab toolbar Page");
	case ak::ui::core::oTabToolbarSubgroup: return QString("Tab toolbar Subgroup");
	case ak::ui::core::oTextEdit: return QString("Text edit");
	case ak::ui::core::oTree: return QString("Tree");
	default: assert(0); // Not implemented yet
	}
	return QString("");
}

Qt::AlignmentFlag ak::ui::core::toQtAlignment(
	textAlignment							_textAlignment
) {
	switch (_textAlignment)
	{
	case ak::ui::core::alignLeft: return Qt::AlignmentFlag::AlignLeft;
	case ak::ui::core::alignRight: return Qt::AlignmentFlag::AlignRight;
	case ak::ui::core::alignCenter: return Qt::AlignmentFlag::AlignCenter;
	default:
		assert(0);	// Not implemented yet
		return Qt::AlignmentFlag::AlignCenter;
		break;
	}
}