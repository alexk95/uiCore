/*
 * ak_ui_qt_textEdit.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_textEdit.h>	// corresponding class
#include <ak_exception.h>		// error handling
#include <ak_ui_colorStyle.h>	// colorStyle
#include <ak_ui_qt_contextMenuItem.h>
#include <ak_ui_core.h>

// Qt header
#include <qscrollbar.h>
#include <qmenu.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::textEdit::textEdit(QWidget * _parent)
: QTextEdit(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(false), my_contextMenu(nullptr)
{ ini(); }
ak::ui::qt::textEdit::textEdit(const QString & _text, QWidget * _parent)
: QTextEdit(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(false), my_contextMenu(nullptr)
{ ini(); }

ak::ui::qt::textEdit::~textEdit()
{
	A_OBJECT_DESTROYING
	disconnect(this, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}

// #######################################################################################################
// Event handling

void ak::ui::qt::textEdit::keyPressEvent(QKeyEvent *_event)
{
	QTextEdit::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::ui::qt::textEdit::keyReleaseEvent(QKeyEvent *_event) {
	QTextEdit::keyReleaseEvent(_event); emit keyReleased(_event);
}

void ak::ui::qt::textEdit::slotChanged() {
	performAutoScrollToBottom();
}

void ak::ui::qt::textEdit::slotCustomMenuRequested(const QPoint & _pos) {
	if (my_contextMenuItems.size() != 0) { my_contextMenu->exec(viewport()->mapToGlobal(_pos)); }
}

void ak::ui::qt::textEdit::slotContextMenuItemClicked() {
	qt::contextMenuItem * item = nullptr;
	item = dynamic_cast<qt::contextMenuItem *>(sender());
	assert(item != nullptr); // Cast failed
	switch (item->role())
	{
	case ui::core::contextMenuRole::crClear: clear(); break;
	case ui::core::contextMenuRole::crNone: break;
	default:
		assert(0); // Unknown role
		break;
	}
	emit(contextMenuItemClicked(item->id()));
}

// #######################################################################################################

QWidget * ak::ui::qt::textEdit::widget(void) { return this; }

void ak::ui::qt::textEdit::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorControls, "QTextEdit{", "}"));
	this->setStyleSheet(sheet);
	sheet = my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorDialogWindow | TYPE_COLORAREA::caBackgroundColorDialogWindow, "QMenu{", "}");
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorDialogWindow | TYPE_COLORAREA::caBackgroundColorDialogWindow, "QMenu::item{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorFocus | TYPE_COLORAREA::caBackgroundColorFocus, "QMenu::item:selected{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorSelected | TYPE_COLORAREA::caBackgroundColorSelected, "QMenu::item:pressed{", "}"));
	my_contextMenu->setStyleSheet(sheet);
}

// #######################################################################################################

// Context menu

ak::ID ak::ui::qt::textEdit::addContextMenuItem(
	contextMenuItem *			_item
) {
	assert(_item != nullptr); // Nullptr provided
	_item->setId(++my_currentContextMenuItemId);
	my_contextMenuItems.push_back(_item);
	connect(_item, SIGNAL(triggered(bool)), this, SLOT(slotContextMenuItemClicked()));
	my_contextMenu->addAction(_item);
	if (my_colorStyle != nullptr) {
		QString sheet{ my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorDialogWindow | TYPE_COLORAREA::caBackgroundColorDialogWindow, "QMenu{", "}") };
		sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorDialogWindow | TYPE_COLORAREA::caBackgroundColorDialogWindow, "QMenu::item{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorFocus | TYPE_COLORAREA::caBackgroundColorFocus, "QMenu::item:selected{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorSelected | TYPE_COLORAREA::caBackgroundColorSelected, "QMenu::item:pressed{", "}"));
		my_contextMenu->setStyleSheet(sheet);
	}
	return _item->id();
}

void ak::ui::qt::textEdit::addContextMenuSeparator(void) {
	my_contextMenu->addSeparator();
}

void ak::ui::qt::textEdit::clearContextMenu(void) {
	for (auto itm : my_contextMenuItems) {
		contextMenuItem * item = itm;
		delete item;
	}
	my_contextMenuItems.clear();
	my_contextMenu->clear();
	my_currentContextMenuItemId = ak::invalidID;
}

// #######################################################################################################

void ak::ui::qt::textEdit::setAutoScrollToBottom(
	bool							_autoScroll
) { my_autoScrollToBottom = _autoScroll; }

bool ak::ui::qt::textEdit::autoScrollToBottom(void) const { return my_autoScrollToBottom; }

void ak::ui::qt::textEdit::performAutoScrollToBottom(void) {
	if (my_autoScrollToBottom) {
		//ensureCursorVisible();
		QScrollBar * bar = verticalScrollBar();
		if (bar->isVisible()) { bar->setValue(bar->maximum());}
	}
}

// #######################################################################################################

void ak::ui::qt::textEdit::ini(void) {
	my_currentContextMenuItemId = ak::invalidID;
	connect(this, SIGNAL(textChanged()), this, SLOT(slotChanged()));
	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotCustomMenuRequested(const QPoint &)));
	my_contextMenu = new QMenu(this);
	contextMenuItem * newItem = new contextMenuItem("Clear", ui::core::contextMenuRole::crClear);
	addContextMenuItem(newItem);
}
