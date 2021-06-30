/*
 *	File:		aDockWatcherWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: June 30, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akWidgets/aDockWatcherWidget.h>
#include <akGui/aColorStyle.h>
#include <akGui/aContextMenuItem.h>

#include <qdockwidget.h>
#include <qmenu.h>

ak::aDockWatcherWidget::aDockWatcherWidget(const QString & _title)
	: aWidget(otDockWatcher), my_isEnabled(true)
{
	setText(_title);
	my_menu = new QMenu;
	setMenu(my_menu);
}

ak::aDockWatcherWidget::aDockWatcherWidget(const QIcon & _icon, const QString & _title)
	: aWidget(otDockWatcher), my_isEnabled(true)
{
	setIcon(_icon);
	setText(_title);
	my_menu = new QMenu;
	setMenu(my_menu);
	connect(this, &QToolButton::clicked, this, &aDockWatcherWidget::slotClicked);
}

ak::aDockWatcherWidget::~aDockWatcherWidget() {

}

// #######################################################################################################

QWidget * ak::aDockWatcherWidget::widget(void) { return this; }

void ak::aDockWatcherWidget::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls, "QToolButton{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus |
		cafBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected |
		cafBackgroundColorSelected, "QToolButton:pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorHeader |
		cafBackgroundColorHeader | cafBorderColorHeader,
		"QToolTip{", "border: 1px;}"));
	setStyleSheet(sheet);

	if (my_menu != nullptr) {
		sheet = my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu{", "}");
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu::item{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus | cafBackgroundColorFocus, "QMenu::item:selected{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected | cafBackgroundColorSelected, "QMenu::item:pressed{", "}"));
		my_menu->setStyleSheet(sheet);
	}
}

// #######################################################################################################

void ak::aDockWatcherWidget::refreshData(void) {
	for (auto itm : my_dockMap) {
		itm.second->setChecked(itm.first->isVisible());
	}
}

void ak::aDockWatcherWidget::addWatch(QDockWidget * _dock) {
	auto action = new aContextMenuItem(_dock->windowTitle(), cmrNone);
	my_menu->addAction(action);
	action->setCheckable(true);
	action->setChecked(_dock->isVisible());
	my_dockMap.insert_or_assign(_dock, action);
	my_actionMap.insert_or_assign(action, _dock);

	connect(_dock, &QDockWidget::visibilityChanged, this, &aDockWatcherWidget::slotVisibilityChanged);
	connect(action, &QAction::toggled, this, &aDockWatcherWidget::slotMenuItemChanged);
}

void ak::aDockWatcherWidget::removeWatch(QDockWidget * _dock) {
	auto itm = my_dockMap.find(_dock);
	assert(itm != my_dockMap.end());
	my_actionMap.erase(itm->second);
	disconnect(_dock, &QDockWidget::visibilityChanged, this, &aDockWatcherWidget::slotVisibilityChanged);
	disconnect(itm->second, &QAction::toggled, this, &aDockWatcherWidget::slotMenuItemChanged);
	my_menu->removeAction(itm->second);
	delete itm->second;
	my_dockMap.erase(_dock);
}

void ak::aDockWatcherWidget::slotVisibilityChanged(bool _visible) {
	if (!my_isEnabled) { return; }
	QDockWidget * actualDock = nullptr;
	actualDock = dynamic_cast<QDockWidget *>(sender());
	assert(actualDock != nullptr);	// Sender is not a dock
	auto itm = my_dockMap.find(actualDock);
	assert(itm != my_dockMap.end());
	itm->second->setChecked(actualDock->isVisible());
}

void ak::aDockWatcherWidget::slotClicked() {
	if (my_dockMap.size() > 0) {
		showMenu();
	}
}

void ak::aDockWatcherWidget::slotMenuItemChanged() {
	aContextMenuItem * itm = nullptr;
	itm = dynamic_cast<aContextMenuItem *>(sender());
	assert(itm != nullptr);
	auto d = my_actionMap.find(itm);
	assert(d != my_actionMap.end());
	d->second->setVisible(itm->isChecked());
}