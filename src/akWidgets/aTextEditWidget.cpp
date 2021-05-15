/*
 * ak_ui_qt_aTextEditWidget.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aColorStyle.h>
#include <akGui/aContextMenuItem.h>
#include <akWidgets/aTextEditWidget.h>

// Qt header
#include <qscrollbar.h>
#include <qmenu.h>

ak::aTextEditWidget::aTextEditWidget(QWidget * _parent)
: QTextEdit(_parent), aWidget(otTextEdit), my_autoScrollToBottom(false), my_contextMenu(nullptr)
{ ini(); }
ak::aTextEditWidget::aTextEditWidget(const QString & _text, QWidget * _parent)
: QTextEdit(_text, _parent), aWidget(otTextEdit), my_autoScrollToBottom(false), my_contextMenu(nullptr)
{ ini(); }

ak::aTextEditWidget::~aTextEditWidget()
{
	A_OBJECT_DESTROYING
	disconnect(this, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}

// #######################################################################################################
// Event handling

void ak::aTextEditWidget::keyPressEvent(QKeyEvent *_event)
{
	QTextEdit::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::aTextEditWidget::keyReleaseEvent(QKeyEvent *_event) {
	QTextEdit::keyReleaseEvent(_event); emit keyReleased(_event);
}

void ak::aTextEditWidget::slotChanged() {
	performAutoScrollToBottom();
}

void ak::aTextEditWidget::slotCustomMenuRequested(const QPoint & _pos) {
	if (my_contextMenuItems.size() != 0) { my_contextMenu->exec(viewport()->mapToGlobal(_pos)); }
}

void ak::aTextEditWidget::slotContextMenuItemClicked() {
	aContextMenuItem * item = nullptr;
	item = dynamic_cast<aContextMenuItem *>(sender());
	assert(item != nullptr); // Cast failed
	switch (item->role())
	{
	case cmrClear: clear(); break;
	case cmrNone: break;
	default:
		assert(0); // Unknown role
		break;
	}
	emit(contextMenuItemClicked(item->id()));
}

// #######################################################################################################

QWidget * ak::aTextEditWidget::widget(void) { return this; }

void ak::aTextEditWidget::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls, "QTextEdit{", "}"));
	this->setStyleSheet(sheet);
	sheet = my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu{", "}");
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu::item{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus | cafBackgroundColorFocus, "QMenu::item:selected{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected | cafBackgroundColorSelected, "QMenu::item:pressed{", "}"));
	my_contextMenu->setStyleSheet(sheet);
}

// #######################################################################################################

// Context menu

ak::ID ak::aTextEditWidget::addContextMenuItem(
	aContextMenuItem *			_item
) {
	assert(_item != nullptr); // Nullptr provided
	_item->setId(++my_currentContextMenuItemId);
	my_contextMenuItems.push_back(_item);
	connect(_item, SIGNAL(triggered(bool)), this, SLOT(slotContextMenuItemClicked()));
	my_contextMenu->addAction(_item);
	if (my_colorStyle != nullptr) {
		QString sheet{ my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu{", "}") };
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu::item{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus | cafBackgroundColorFocus, "QMenu::item:selected{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected | cafBackgroundColorSelected, "QMenu::item:pressed{", "}"));
		my_contextMenu->setStyleSheet(sheet);
	}
	return _item->id();
}

void ak::aTextEditWidget::addContextMenuSeparator(void) {
	my_contextMenu->addSeparator();
}

void ak::aTextEditWidget::clearContextMenu(void) {
	for (auto itm : my_contextMenuItems) {
		aContextMenuItem * item = itm;
		delete item;
	}
	my_contextMenuItems.clear();
	my_contextMenu->clear();
	my_currentContextMenuItemId = ak::invalidID;
}

// #######################################################################################################

void ak::aTextEditWidget::setAutoScrollToBottom(
	bool							_autoScroll
) { my_autoScrollToBottom = _autoScroll; }

bool ak::aTextEditWidget::autoScrollToBottom(void) const { return my_autoScrollToBottom; }

void ak::aTextEditWidget::performAutoScrollToBottom(void) {
	if (my_autoScrollToBottom) {
		//ensureCursorVisible();
		QScrollBar * bar = verticalScrollBar();
		if (bar->isVisible()) { bar->setValue(bar->maximum());}
	}
}

// #######################################################################################################

void ak::aTextEditWidget::ini(void) {
	my_currentContextMenuItemId = ak::invalidID;
	connect(this, SIGNAL(textChanged()), this, SLOT(slotChanged()));
	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotCustomMenuRequested(const QPoint &)));
	my_contextMenu = new QMenu(this);
	aContextMenuItem * newItem = new aContextMenuItem("Clear", cmrClear);
	addContextMenuItem(newItem);
}
