/*
 * ak_ui_signalLinker.cpp
 *
 *  Created on: February 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_signalLinker.h>				// corresponding class
#include <ak_exception.h>					// error handling
#include <ak_messenger.h>					// messenger
#include <ak_singletonAllowedMessages.h>	// allowed messages

// AK qt header
#include <ak_ui_qt_action.h>
#include <ak_ui_qt_checkBox.h>
#include <ak_ui_qt_colorEditButton.h>
#include <ak_ui_qt_comboBox.h>
#include <ak_ui_qt_comboButton.h>
#include <ak_ui_qt_dock.h>
#include <ak_ui_qt_propertyGrid.h>
#include <ak_ui_qt_lineEdit.h>
#include <ak_ui_qt_pushButton.h>
#include <ak_ui_qt_table.h>
#include <ak_ui_qt_textEdit.h>
#include <ak_ui_qt_timer.h>
#include <ak_ui_qt_toolButton.h>
#include <ak_ui_qt_tree.h>
#include <ak_ui_qt_niceLineEdit.h>

// Qt header
#include <qmessagebox.h>			// QMessageBox
#include <qdockwidget.h>
#include <qtreewidget.h>
#include <qstring.h>
#include <qevent.h>

ak::ui::signalLinker::signalLinker(
	ak::messenger *								_messanger,
	ak::uidManager *							_uidManager
)	: my_messanger(nullptr),
	my_uidManager(nullptr),
	my_uid(0)
{
	try {
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }
		if (_messanger == nullptr) { throw ak::Exception("Is nullptr", "Check messanger"); }
		my_uidManager = _uidManager;
		my_messanger = _messanger;
		my_uid = my_uidManager->getId();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::signalLinker()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::signalLinker()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::signalLinker()"); }
}

ak::ui::signalLinker::~signalLinker()
{
	for (my_objectsIterator itm = my_objects.begin(); itm != my_objects.end(); itm++) {
		switch (itm->second.type)
		{
		case ak::ui::core::objectType::oAction:
			itm->second.object->disconnect(itm->second.object, SIGNAL(changed()), this, SLOT(slotChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(triggered()), this, SLOT(slotClicked()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(hovered()), this, SLOT(slotFocused()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
			break;
		case ak::ui::core::objectType::oCheckBox:
			itm->second.object->disconnect(itm->second.object, SIGNAL(clicked()), this, SLOT(slotClicked()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(released()), this, SLOT(slotReleased()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oColorEditButton:
			itm->second.object->disconnect(itm->second.object, SIGNAL(changed()), this, SLOT(slotChanged()));
			break;
		case ak::ui::core::objectType::oComboBox:
			itm->second.object->disconnect(itm->second.object, SIGNAL(activated(int)), this, SLOT(slotIndexActivated(int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(currentIndexChanged(int)), this, SLOT(slotIndexChanged(int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oComboButton:
			itm->second.object->disconnect(itm->second.object, SIGNAL(clicked()), this, SLOT(slotClicked()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(changed()), this, SLOT(slotChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oDock:
			itm->second.object->disconnect(itm->second.object, SIGNAL(visibilityChanged(bool)), this, SLOT(slotVisibilityChanged(bool)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(closing()), this, SLOT(slotClosing()));
			break;
		case ak::ui::core::oPropertyGrid:
			itm->second.object->disconnect(itm->second.object, SIGNAL(cleared()), this, SLOT(slotCleared()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemChanged(int)), this, SLOT(slotItemChanged(int)));
			break;
		case ak::ui::core::objectType::oLineEdit:
			itm->second.object->disconnect(itm->second.object, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slotCursorPositionChanged(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(textChanged()), this, SLOT(slotChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			break;
		case core::objectType::oNiceLineEdit:
			itm->second.object->disconnect(itm->second.object, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slotCursorPositionChanged(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(textChanged(const QString &)), this, SLOT(slotChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(editingFinished()), this, SLOT(slotEditingFinished()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()));
			break;
		case ak::ui::core::objectType::oPushButton:
			itm->second.object->disconnect(itm->second.object, SIGNAL(clicked()), this, SLOT(slotClicked()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oTable:
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellActivated(int, int)), this, SLOT(tableCellActivated(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellChanged(int, int)), this, SLOT(tableCellChanged(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellClicked(int, int)), this, SLOT(tableCellClicked(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableCellDoubleClicked(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellEntered(int, int)), this, SLOT(tableCellEntered(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oTextEdit:
			itm->second.object->disconnect(itm->second.object, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slotCursorPositionChanged(int , int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(textChanged()), this, SLOT(slotChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(contextMenuItemClicked(ak::ID)), this, SLOT(slotContextMenuItemClicked(ak::ID)));
			break;
		case ak::ui::core::objectType::oTimer:
			itm->second.object->disconnect(itm->second.object, SIGNAL(timeout()), this, SLOT(slotTimeout()));
			break;
		case ak::ui::core::objectType::oToolButton:
			itm->second.object->disconnect(itm->second.object, SIGNAL(btnClicked()), this, SLOT(slotClicked()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(slotKeyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(menuItemClicked(ak::ID)), this, SLOT(slotContextMenuItemClicked(ak::ID)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(menuItemCheckedChanged(ak::ID, bool)), this, SLOT(slotContextMenuItemCheckedChanged(ak::ID, bool)));
			break;
		case core::oTree:
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cleared()), this, SLOT(slotCleared()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(focusLost()), this, SLOT(slotFocusLost()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(slotTreeItemActivated(QTreeWidgetItem *, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slotTreeItemChanged(QTreeWidgetItem *, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slotTreeItemClicked(QTreeWidgetItem *, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(slotTreeItemCollapsed(QTreeWidgetItem *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotTreeItemDoubleClicked(QTreeWidgetItem *, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemFocused(QTreeWidgetItem *)), this, SLOT(slotTreeItemFocused(QTreeWidgetItem *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(slotTreeItemExpanded(QTreeWidgetItem *)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemTextChanged(QTreeWidgetItem *, int)), this, SLOT(slotTreeItemChanged(QTreeWidgetItem *, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(itemLocationChanged(QTreeWidgetItem *, int)), this, SLOT(slotTreeItemLocationChanged(QTreeWidgetItem *, int)));
			break;
		default:
			assert(0); // Not implemented object type
			break;
		}
	}
}

// ###################################################################################

// Add objects

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::action *								_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oAction });
	_object->connect(_object, SIGNAL(changed()), this, SLOT(slotChanged()));
	_object->connect(_object, SIGNAL(triggered()), this, SLOT(slotClicked()));
	_object->connect(_object, SIGNAL(hovered()), this, SLOT(slotFocused()));
	_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::checkBox *								_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oCheckBox });
	_object->connect(_object, SIGNAL(clicked()), this, SLOT(slotClicked()));
	_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
	_object->connect(_object, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::colorEditButton *						_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oColorEditButton });
	_object->connect(_object, SIGNAL(changed()), this, SLOT(slotChanged()));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::comboBox *								_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oComboBox });
	_object->connect(_object, SIGNAL(activated(int)), this, SLOT(slotIndexActivated(int)));
	_object->connect(_object, SIGNAL(currentIndexChanged(int)), this, SLOT(slotIndexChanged(int)));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::comboButton *							_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oComboButton });
	_object->connect(_object, SIGNAL(clicked()), this, SLOT(slotClicked()));
	_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
	_object->connect(_object, SIGNAL(changed()), this, SLOT(slotChanged()));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::lineEdit *								_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oLineEdit });
	_object->connect(_object, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slotCursorPositionChanged(int, int)));
	_object->connect(_object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
	_object->connect(_object, SIGNAL(textChanged(const QString &)), this, SLOT(slotChanged()));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::niceLineEdit *							_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oNiceLineEdit });
	_object->connect(_object, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slotCursorPositionChanged(int, int)));
	_object->connect(_object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
	_object->connect(_object, SIGNAL(textChanged(const QString &)), this, SLOT(slotChanged()));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	_object->connect(_object, SIGNAL(editingFinished()), this, SLOT(slotEditingFinished()));
	_object->connect(_object, &qt::niceLineEdit::returnPressed, this, &signalLinker::slotReturnPressed);
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::dock *									_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oDock });
	_object->connect(_object, SIGNAL(visibilityChanged(bool)), this, SLOT(slotVisibilityChanged(bool)));
	_object->connect(_object, SIGNAL(closing()), this, SLOT(slotClosing()));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::propertyGrid *							_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oPropertyGrid });
	_object->connect(_object, &qt::propertyGrid::cleared, this, &signalLinker::slotCleared);
	_object->connect(_object, &qt::propertyGrid::itemChanged, this, &signalLinker::slotItemChanged);
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::pushButton *							_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oPushButton });
	_object->connect(_object, SIGNAL(clicked()), this, SLOT(slotClicked()));
	_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::table *									_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oTable });
	_object->connect(_object, SIGNAL(cellActivated(int, int)), this, SLOT(tableCellActivated(int, int)));
	_object->connect(_object, SIGNAL(cellChanged(int, int)), this, SLOT(tableCellChanged(int, int)));
	_object->connect(_object, SIGNAL(cellClicked(int, int)), this, SLOT(tableCellClicked(int, int)));
	_object->connect(_object, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableCellDoubleClicked(int, int)));
	_object->connect(_object, SIGNAL(cellEntered(int, int)), this, SLOT(tableCellEntered(int, int)));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::textEdit *								_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oTextEdit });
	_object->connect(_object, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(slotCursorPositionChanged(int, int)));
	_object->connect(_object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
	_object->connect(_object, SIGNAL(textChanged()), this, SLOT(slotChanged()));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	_object->connect(_object, SIGNAL(contextMenuItemClicked(ak::ID)), this, SLOT(slotContextMenuItemClicked(ak::ID)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::timer *									_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oTimer });
	_object->connect(_object, SIGNAL(timeout()), this, SLOT(slotTimeout()));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::toolButton *							_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oToolButton });
	_object->connect(_object, SIGNAL(btnClicked()), this, SLOT(slotClicked()));
	_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	_object->connect(_object, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	_object->connect(_object, SIGNAL(menuItemClicked(ak::ID)), this, SLOT(slotContextMenuItemClicked(ak::ID)));
	_object->connect(_object, SIGNAL(menuItemCheckedChanged(ak::ID, bool)), this, SLOT(slotContextMenuItemCheckedChanged(ak::ID, bool)));
	return _objectUid;
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::tree *									_object,
	ak::UID												_objectUid
) {
	if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
	assert(my_objects.count(_objectUid) == 0); // Object with the provided UID already exists
	_object->setUid(_objectUid);
	my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oTree });
	_object->connect(_object, &qt::tree::keyPressed, this, &signalLinker::slotKeyPressed);
	_object->connect(_object, &qt::tree::keyReleased, this, &signalLinker::slotKeyReleased);
	_object->connect(_object, &qt::tree::cleared, this, &signalLinker::slotCleared);
	_object->connect(_object, &qt::tree::focusLost, this, &signalLinker::slotFocusLost);
	_object->connect(_object, &qt::tree::selectionChanged, this, &signalLinker::slotSelectionChanged);
	_object->connect(_object, &qt::tree::itemActivated, this, &signalLinker::slotTreeItemActivated);
	_object->connect(_object, &qt::tree::itemChanged, this, &signalLinker::slotTreeItemChanged);
	_object->connect(_object, &qt::tree::itemClicked, this, &signalLinker::slotTreeItemClicked);
	_object->connect(_object, &qt::tree::itemCollapsed, this, &signalLinker::slotTreeItemCollapsed);
	_object->connect(_object, &qt::tree::itemDoubleClicked, this, &signalLinker::slotTreeItemDoubleClicked);
	_object->connect(_object, &qt::tree::itemFocused, this, &signalLinker::slotTreeItemFocused);
	_object->connect(_object, &qt::tree::itemExpanded, this, &signalLinker::slotTreeItemExpanded);
	_object->connect(_object, &qt::tree::itemTextChanged, this, &signalLinker::slotTreeItemChanged);
	_object->connect(_object, &qt::tree::itemLocationChanged, this, &signalLinker::slotTreeItemLocationChanged);
	return _objectUid;
}

// ###################################################################################

// SLOTS

void ak::ui::signalLinker::slotChanged() {
	if (!ak::singletonAllowedMessages::instance()->changedEvent()) { return; }
	// Cast object
	ak::ui::core::aObject * obj = nullptr;
	obj = dynamic_cast<ak::ui::core::aObject *>(sender());
	assert(obj != nullptr); // Cast failed
	raiseEventProtected(obj->uid(), ak::core::eventType::eChanged, 0, 0);
}

void ak::ui::signalLinker::slotCleared() {
	if (!ak::singletonAllowedMessages::instance()->clearedEvent()) { return; }
	// Cast object
	ak::ui::core::aObject * obj = nullptr;
	obj = dynamic_cast<ak::ui::core::aObject *>(sender());
	assert(obj != nullptr); // Cast failed
	raiseEventProtected(obj->uid(), ak::core::eventType::eCleared, 0, 0);
}

void ak::ui::signalLinker::slotItemChanged(ak::ID _itemID) {
	if (!ak::singletonAllowedMessages::instance()->changedEvent()) { return; }
	// Cast object
	ak::ui::core::aObject * obj = nullptr;
	obj = dynamic_cast<ak::ui::core::aObject *>(sender());
	assert(obj != nullptr); // Cast failed
	raiseEventProtected(obj->uid(), ak::core::eventType::eChanged, _itemID, 0);
}

void ak::ui::signalLinker::slotClicked() {
	if (!ak::singletonAllowedMessages::instance()->clickedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eClicked, 0, 0);
}

void ak::ui::signalLinker::slotCursorPositionChanged(int _oldPos, int _newPos) {
	if (!ak::singletonAllowedMessages::instance()->cursorPositionChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eCursorPosotionChanged, _oldPos, _newPos);
}

void ak::ui::signalLinker::slotFocused() {
	if (!ak::singletonAllowedMessages::instance()->focusedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eFocused, 0, 0);
}

void ak::ui::signalLinker::slotFocusLost() {
	if (!ak::singletonAllowedMessages::instance()->focusedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eFocusLeft, 0, 0);
}

void ak::ui::signalLinker::slotIndexActivated(int _index) {
	if (!ak::singletonAllowedMessages::instance()->activatedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eActivated, _index, 0);
}

void ak::ui::signalLinker::slotIndexChanged(int _index) {
	if (!ak::singletonAllowedMessages::instance()->indexChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eIndexChanged, _index, 0);
}

void ak::ui::signalLinker::slotReleased() {
	if (!ak::singletonAllowedMessages::instance()->releasedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eReleased, 0, 0);
}

void ak::ui::signalLinker::slotKeyPressed(QKeyEvent * _key) {
	if (!ak::singletonAllowedMessages::instance()->keyPressedEvent()) { return; }
	ui::core::keyType k = ui::core::getKey(_key);
	if (k != ui::core::key_Unknown) { raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eKeyPressed, 0, k); }
}

void ak::ui::signalLinker::slotKeyReleased(QKeyEvent * _key) {
	if (!ak::singletonAllowedMessages::instance()->keyPressedEvent()) { return; }
	ui::core::keyType k = ui::core::getKey(_key);
	if (k != ui::core::key_Unknown) { raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eKeyReleased, 0, k); }
}

void ak::ui::signalLinker::slotSelectionChanged() {
	if (!ak::singletonAllowedMessages::instance()->selectionChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eSelectionChanged, 0, 0);
}

void ak::ui::signalLinker::slotStateChanged(int _state) {
	if (!ak::singletonAllowedMessages::instance()->stateChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eStateChanged, _state, 0);
}

void ak::ui::signalLinker::slotToggled(bool _checked)
{
	if (_checked) {
		if (!ak::singletonAllowedMessages::instance()->toggledCheckedEvent()) { return; }
		raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eToggeledChecked, 0, 0);
	}
	else {
		if (!ak::singletonAllowedMessages::instance()->toggledUncheckedEvent()) { return; }
		raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eToggeledUnchecked, 0, 0);
	}
}

void ak::ui::signalLinker::slotTimeout(void) { raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eTimeout, 0, 0); }

void ak::ui::signalLinker::slotContextMenuItemClicked(ak::ID _itemId) { raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eContextMenuItemClicked, _itemId, 0); }

void ak::ui::signalLinker::slotContextMenuItemCheckedChanged(ak::ID _itemId, bool _isChecked) {
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eContextMenuItemCheckedChanged, _itemId, _isChecked);
}

void ak::ui::signalLinker::slotVisibilityChanged(bool _visibility) {
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eContextMenuItemCheckedChanged, _visibility, 0);
}

void ak::ui::signalLinker::slotClosing(void) {
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eClosing, 0, 0);
}

void ak::ui::signalLinker::slotReturnPressed(void) {
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eReturnPressed, 0, 0);
}

void ak::ui::signalLinker::slotEditingFinished(void) {
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eEditingFinished, 0, 0);
}

// ##### Table

void ak::ui::signalLinker::tableCellActivated(int _row, int _coloumn) {
	if (!ak::singletonAllowedMessages::instance()->activatedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eActivated, _row, _coloumn);
}

void ak::ui::signalLinker::tableCellChanged(int _row, int _coloumn) {
	if (!ak::singletonAllowedMessages::instance()->changedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eChanged, _row, _coloumn);
}

void ak::ui::signalLinker::tableCellClicked(int _row, int _coloumn) {
	if (!ak::singletonAllowedMessages::instance()->clickedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eClicked, _row, _coloumn);
}

void ak::ui::signalLinker::tableCellDoubleClicked(int _row, int _coloumn) {
	if (!ak::singletonAllowedMessages::instance()->doubleClickedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eDoubleClicked, _row, _coloumn);
}

void ak::ui::signalLinker::tableCellEntered(int _row, int _coloumn) {
	if (!ak::singletonAllowedMessages::instance()->focusedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eFocused, _row, _coloumn);
}

// ##### Tree

void ak::ui::signalLinker::slotTreeItemActivated(QTreeWidgetItem * _item, int _column) {
	if (!ak::singletonAllowedMessages::instance()->activatedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eActivated, qt::treeBase::getItemId(_item), _column);
}

void ak::ui::signalLinker::slotTreeItemChanged(QTreeWidgetItem * _item, int _column) {
	if (!ak::singletonAllowedMessages::instance()->changedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eChanged, qt::treeBase::getItemId(_item), _column);
}

void ak::ui::signalLinker::slotTreeItemClicked(QTreeWidgetItem * _item, int _column) {
	if (!ak::singletonAllowedMessages::instance()->clickedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eClicked, qt::treeBase::getItemId(_item), _column);
}

void ak::ui::signalLinker::slotTreeItemCollapsed(QTreeWidgetItem * _item) {
	if (!ak::singletonAllowedMessages::instance()->collapsedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eCollpased, qt::treeBase::getItemId(_item), 0);
}

void ak::ui::signalLinker::slotTreeItemDoubleClicked(QTreeWidgetItem * _item, int _column) {
	if (!ak::singletonAllowedMessages::instance()->doubleClickedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eDoubleClicked, qt::treeBase::getItemId(_item), _column);
}

void ak::ui::signalLinker::slotTreeItemExpanded(QTreeWidgetItem * _item) {
	if (!ak::singletonAllowedMessages::instance()->expandedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eExpanded, qt::treeBase::getItemId(_item), 0);
}

void ak::ui::signalLinker::slotTreeItemFocused(QTreeWidgetItem * _item) {
	if (!ak::singletonAllowedMessages::instance()->focusedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eFocused, qt::treeBase::getItemId(_item), 0);
}

void ak::ui::signalLinker::slotTreeItemLocationChanged(QTreeWidgetItem * _item) {
	if (!ak::singletonAllowedMessages::instance()->locationChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eLocationChanged, qt::treeBase::getItemId(_item), 0);
}

// ###################################################################################

// Private members

void ak::ui::signalLinker::raiseEvent(
	ak::UID													_senderUid,
	ak::core::eventType												_eventType,
	int															_info1,
	int															_info2
) {
	try {
		if (my_messanger == nullptr) { throw ak::Exception("Is nullptr", "Check messanger"); }
		my_messanger->sendMessage(_senderUid, _eventType, _info1, _info2);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::raiseEvent()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::raiseEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::raiseEvent()"); }
}

void ak::ui::signalLinker::raiseEventProtected(
	ak::UID													_senderUid,
	ak::core::eventType												_eventType,
	int															_info1,
	int															_info2
) {
	try { raiseEvent(_senderUid, _eventType, _info1, _info2); }
	catch (const std::exception & e) {
		assert(0);
		QMessageBox msg;
		msg.setWindowTitle("Signal error");
		msg.setText(e.what());
		msg.exec();
	}
	catch (...) {
		assert(0);
		QMessageBox msg;
		msg.setWindowTitle("Signal error");
		msg.setText("Unknown error at ak::ui::signalLinker::raiseEventProtected()");
		msg.exec();
	}
}

ak::UID ak::ui::signalLinker::getSenderUid(
	QObject * _sender
) { 
	ak::ui::core::aObject * obj = dynamic_cast<ak::ui::core::aObject *>(_sender);
	assert(obj != nullptr); // Invalid object stored
	return obj->uid();
}