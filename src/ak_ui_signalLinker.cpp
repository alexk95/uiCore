/*
 * ak_ui_signalLinker.cpp
 *
 *  Created on: February 21, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_signalLinker.h>		// corresponding class
#include <ak_exception.h>			// error handling
#include <ak_messenger.h>			// messenger

// AK qt header
#include <ak_ui_qt_action.h>
#include <ak_ui_qt_checkBox.h>
#include <ak_ui_qt_dock.h>
#include <ak_ui_qt_pushButton.h>
#include <ak_ui_qt_table.h>
#include <ak_ui_qt_textEdit.h>
#include <ak_ui_qt_comboBox.h>
#include <ak_ui_qt_comboButton.h>
#include <ak_ui_qt_tree.h>
#include <ak_singletonAllowedMessages.h>

// Qt header
#include <qmessagebox.h>			// QMessageBox
#include <qdockwidget.h>

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
			itm->second.object->disconnect(itm->second.object, SIGNAL(pressed()), this, SLOT(slotPressed()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(released()), this, SLOT(slotReleased()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oComboBox:
			itm->second.object->disconnect(itm->second.object, SIGNAL(activated(int)), this, SLOT(slotIndexActivated(int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(currentIndexChanged(int)), this, SLOT(slotIndexChanged(int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oComboButton:
			itm->second.object->disconnect(itm->second.object, SIGNAL(clicked()), this, SLOT(slotClicked()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(changed()), this, SLOT(slotChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oDock:
			break;
		case ak::ui::core::objectType::oPushButton:
			itm->second.object->disconnect(itm->second.object, SIGNAL(clicked()), this, SLOT(slotClicked()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oTable:
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellActivated(int, int)), this, SLOT(tableCellActivated(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellChanged(int, int)), this, SLOT(tableCellChanged(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellClicked(int, int)), this, SLOT(tableCellClicked(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableCellDoubleClicked(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(cellEntered(int, int)), this, SLOT(tableCellEntered(int, int)));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
			break;
		case ak::ui::core::objectType::oTextEdit:
			itm->second.object->disconnect(itm->second.object, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
			itm->second.object->disconnect(itm->second.object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
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
	ak::UID										_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oAction });
		_object->connect(_object, SIGNAL(changed()), this, SLOT(slotChanged()));
		_object->connect(_object, SIGNAL(triggered()), this, SLOT(slotClicked()));
		_object->connect(_object, SIGNAL(hovered()), this, SLOT(slotFocused()));
		_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::action)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::action)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::action)"); }
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::checkBox *								_object,
	ak::UID										_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oCheckBox });
		_object->connect(_object, SIGNAL(clicked()), this, SLOT(slotClicked()));
		_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
		_object->connect(_object, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
		_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::checkBox)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::checkBox)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::checkBox)"); }
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::comboBox *								_object,
	ak::UID										_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oComboBox });
		_object->connect(_object, SIGNAL(activated(int)), this, SLOT(slotIndexActivated(int)));
		_object->connect(_object, SIGNAL(currentIndexChanged(int)), this, SLOT(slotIndexChanged(int)));
		_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::comboBox)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::comboBox)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::comboBox)"); }
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::comboButton *							_object,
	ak::UID										_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oComboButton });
		_object->connect(_object, SIGNAL(clicked()), this, SLOT(slotClicked()));
		_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
		_object->connect(_object, SIGNAL(changed()), this, SLOT(slotChanged()));
		_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::comboButton)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::comboButton)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::comboButton)"); }
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::dock *												_object,
	ak::UID													_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oDock });
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::comboButton)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::comboButton)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::comboButton)"); }
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::pushButton *							_object,
	ak::UID										_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oPushButton });
		_object->connect(_object, SIGNAL(clicked()), this, SLOT(slotClicked()));
		_object->connect(_object, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
		_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::pushButton)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::pushButton)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::pushButton)"); }
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::table *								_object,
	ak::UID										_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oTable });
		_object->connect(_object, SIGNAL(cellActivated(int, int)), this, SLOT(tableCellActivated(int, int)));
		_object->connect(_object, SIGNAL(cellChanged(int, int)), this, SLOT(tableCellChanged(int, int)));
		_object->connect(_object, SIGNAL(cellClicked(int, int)), this, SLOT(tableCellClicked(int, int)));
		_object->connect(_object, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableCellDoubleClicked(int, int)));
		_object->connect(_object, SIGNAL(cellEntered(int, int)), this, SLOT(tableCellEntered(int, int)));
		_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::table)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::table)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::table)"); }
}

ak::UID ak::ui::signalLinker::addLink(
	ak::ui::qt::textEdit *								_object,
	ak::UID										_objectUid
) {
	try {
		if (_objectUid == ak::invalidUID) { _objectUid = my_uidManager->getId(); }
		if (my_objects.count(_objectUid) > 0) { throw ak::Exception("Object with the provided uid already exists", "Check UID"); }
		_object->setUid(_objectUid);
		my_objects.insert_or_assign(_objectUid, struct_object{ _object, ak::ui::core::objectType::oTextEdit });
		_object->connect(_object, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
		_object->connect(_object, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
		_object->connect(_object, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
		_object->connect(_object, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
		return _objectUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::signalLinker::addLink(ak::ui::qt::textEdit)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::signalLinker::addLink(ak::ui::qt::textEdit)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::signalLinker::addLink(ak::ui::qt::textEdit)"); }
}


// ###################################################################################

// SLOTS

void ak::ui::signalLinker::slotChanged() {
	if (!ak::singletonAllowedMessages::instance()->changedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eChanged, 0, 0);
}

void ak::ui::signalLinker::slotClicked() {
	if (!ak::singletonAllowedMessages::instance()->clickedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eClicked, 0, 0);
}

void ak::ui::signalLinker::slotCursorPositionChanged() {
	if (!ak::singletonAllowedMessages::instance()->cursorPositionChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eCursorPosotionChanged, 0, 0);
}

void ak::ui::signalLinker::slotFocused() {
	if (!ak::singletonAllowedMessages::instance()->focusedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eFocused, 0, 0);
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

void ak::ui::signalLinker::slotSelectionChanged() {
	if (!ak::singletonAllowedMessages::instance()->selectionChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eSelectionChanged, 0, 0);
}

void ak::ui::signalLinker::slotStateChanged(int _state) {
	if (!ak::singletonAllowedMessages::instance()->stateChangedEvent()) { return; }
	raiseEventProtected(getSenderUid(sender()), ak::core::eventType::eStateChanged, _state, 0);
}

void ak::ui::signalLinker::slotTextChanged() {
	if (!ak::singletonAllowedMessages::instance()->textChangedEvent()) { return; }
	// Cast object
	ak::ui::core::aObject * obj = nullptr;
	obj = dynamic_cast<ak::ui::core::aObject *>(sender());
	assert(obj != nullptr); // Cast failed
	raiseEventProtected(obj->uid(), ak::core::eventType::eTextChanged, 0, 0);
	if (obj->objectType() == ak::ui::core::objectType::oTextEdit) {
		// Cast text edit
		ak::ui::qt::textEdit * txt = nullptr;
		txt = dynamic_cast<ak::ui::qt::textEdit *>(obj);
		assert(txt != nullptr); // Cast failed
		// Perform action
		txt->performAutoScrollToBottom();
	}
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
		my_messanger->sendMessage(_senderUid, ak::core::messageType::mEvent, _eventType, _info1, _info2);
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
	catch (const ak::Exception & e) {
		QMessageBox msg;
		msg.setWindowTitle("Signal error");
		msg.setText(e.what());
		msg.exec();
	}
	catch (const std::exception & e) {
		QMessageBox msg;
		msg.setWindowTitle("Signal error");
		msg.setText(e.what());
		msg.exec();
	}
	catch (...) {
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