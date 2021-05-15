#include <aDockWatcherButtonWidget.h>

#include <akGui/aColorStyle.h>
#include <akGui/aContextMenuItem.h>

#include <qdockwidget.h>
#include <qmenu.h>

ak::aDockWatcherButtonWidget::aDockWatcherButtonWidget(const QString & _title)
	: aWidget(otDockWatcherToolButton), my_isEnabled(true)
{
	setText(_title);
	my_menu = new QMenu;
	setMenu(my_menu);
}

ak::aDockWatcherButtonWidget::aDockWatcherButtonWidget(const QIcon & _icon, const QString & _title)
	: aWidget(otDockWatcherToolButton), my_isEnabled(true)
{
	setIcon(_icon);
	setText(_title);
	my_menu = new QMenu;
	setMenu(my_menu);
	connect(this, &QToolButton::clicked, this, &aDockWatcherButtonWidget::slotClicked);
}

ak::aDockWatcherButtonWidget::~aDockWatcherButtonWidget() {

}

// #######################################################################################################

QWidget * ak::aDockWatcherButtonWidget::widget(void) { return this; }

void ak::aDockWatcherButtonWidget::setColorStyle(
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

void ak::aDockWatcherButtonWidget::refreshData(void) {
	for (auto itm : my_dockMap) {
		itm.second->setChecked(itm.first->isVisible());
	}
}

void ak::aDockWatcherButtonWidget::addWatch(QDockWidget * _dock, const QString & _text) {
	auto action = new aContextMenuItem(_text, cmrNone);
	my_menu->addAction(action);
	action->setCheckable(true);
	action->setChecked(_dock->isVisible());
	my_dockMap.insert_or_assign(_dock, action);
	my_actionMap.insert_or_assign(action, _dock);

	connect(_dock, &QDockWidget::visibilityChanged, this, &aDockWatcherButtonWidget::slotVisibilityChanged);
	connect(action, &QAction::toggled, this, &aDockWatcherButtonWidget::slotMenuItemChanged);
}

void ak::aDockWatcherButtonWidget::removeWatch(QDockWidget * _dock) {
	auto itm = my_dockMap.find(_dock);
	assert(itm != my_dockMap.end());
	my_actionMap.erase(itm->second);
	disconnect(_dock, &QDockWidget::visibilityChanged, this, &aDockWatcherButtonWidget::slotVisibilityChanged);
	disconnect(itm->second, &QAction::toggled, this, &aDockWatcherButtonWidget::slotMenuItemChanged);
	my_menu->removeAction(itm->second);
	delete itm->second;
	my_dockMap.erase(_dock);
}

void ak::aDockWatcherButtonWidget::slotVisibilityChanged(bool _visible) {
	if (!my_isEnabled) { return; }
	QDockWidget * actualDock = nullptr;
	actualDock = dynamic_cast<QDockWidget *>(sender());
	assert(actualDock != nullptr);	// Sender is not a dock
	auto itm = my_dockMap.find(actualDock);
	assert(itm != my_dockMap.end());
	itm->second->setChecked(actualDock->isVisible());
}

void ak::aDockWatcherButtonWidget::slotClicked() {
	if (my_dockMap.size() > 0) {
		showMenu();
	}
}

void ak::aDockWatcherButtonWidget::slotMenuItemChanged() {
	aContextMenuItem * itm = nullptr;
	itm = dynamic_cast<aContextMenuItem *>(sender());
	assert(itm != nullptr);
	auto d = my_actionMap.find(itm);
	assert(d != my_actionMap.end());
	d->second->setVisible(itm->isChecked());
}