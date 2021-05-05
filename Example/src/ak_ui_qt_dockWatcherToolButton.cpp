#include <ak_ui_qt_dockWatcherToolButton.h>
#include <ak_ui_qt_contextMenuItem.h>
#include <ak_ui_colorStyle.h>

#include <qdockwidget.h>
#include <qmenu.h>

ak::ui::qt::dockWatcherToolButton::dockWatcherToolButton(const QString & _title)
	: core::aWidget(core::oDockWatcherToolButton), my_isEnabled(true)
{
	setText(_title);
	my_menu = new QMenu;
	setMenu(my_menu);
}

ak::ui::qt::dockWatcherToolButton::dockWatcherToolButton(const QIcon & _icon, const QString & _title)
	: core::aWidget(core::oDockWatcherToolButton), my_isEnabled(true)
{
	setIcon(_icon);
	setText(_title);
	my_menu = new QMenu;
	setMenu(my_menu);
	connect(this, &QToolButton::clicked, this, &dockWatcherToolButton::slotClicked);
}

ak::ui::qt::dockWatcherToolButton::~dockWatcherToolButton() {

}

// #######################################################################################################

QWidget * ak::ui::qt::dockWatcherToolButton::widget(void) { return this; }

void ak::ui::qt::dockWatcherToolButton::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(core::caForegroundColorControls |
		core::caBackgroundColorControls, "QToolButton{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorFocus |
		core::caBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorSelected |
		core::caBackgroundColorSelected, "QToolButton:pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorHeader |
		core::caBackgroundColorHeader | core::caBorderColorHeader,
		"QToolTip{", "border: 1px;}"));
	setStyleSheet(sheet);

	if (my_menu != nullptr) {
		sheet = my_colorStyle->toStyleSheet(core::caForegroundColorDialogWindow | core::caBackgroundColorDialogWindow, "QMenu{", "}");
		sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorDialogWindow | core::caBackgroundColorDialogWindow, "QMenu::item{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorFocus | core::caBackgroundColorFocus, "QMenu::item:selected{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorSelected | core::caBackgroundColorSelected, "QMenu::item:pressed{", "}"));
		my_menu->setStyleSheet(sheet);
	}
}

// #######################################################################################################

void ak::ui::qt::dockWatcherToolButton::refreshData(void) {
	for (auto itm : my_dockMap) {
		itm.second->setChecked(itm.first->isVisible());
	}
}

void ak::ui::qt::dockWatcherToolButton::addWatch(QDockWidget * _dock, const QString & _text) {
	auto action = new contextMenuItem(_text, core::contextMenuRole::crNone);
	my_menu->addAction(action);
	action->setCheckable(true);
	action->setChecked(_dock->isVisible());
	my_dockMap.insert_or_assign(_dock, action);
	my_actionMap.insert_or_assign(action, _dock);

	connect(_dock, &QDockWidget::visibilityChanged, this, &dockWatcherToolButton::slotVisibilityChanged);
	connect(action, &QAction::toggled, this, &dockWatcherToolButton::slotMenuItemChanged);
}

void ak::ui::qt::dockWatcherToolButton::removeWatch(QDockWidget * _dock) {
	auto itm = my_dockMap.find(_dock);
	assert(itm != my_dockMap.end());
	my_actionMap.erase(itm->second);
	disconnect(_dock, &QDockWidget::visibilityChanged, this, &dockWatcherToolButton::slotVisibilityChanged);
	disconnect(itm->second, &QAction::toggled, this, &dockWatcherToolButton::slotMenuItemChanged);
	my_menu->removeAction(itm->second);
	delete itm->second;
	my_dockMap.erase(_dock);
}

void ak::ui::qt::dockWatcherToolButton::slotVisibilityChanged(bool _visible) {
	if (!my_isEnabled) { return; }
	QDockWidget * actualDock = nullptr;
	actualDock = dynamic_cast<QDockWidget *>(sender());
	assert(actualDock != nullptr);	// Sender is not a dock
	auto itm = my_dockMap.find(actualDock);
	assert(itm != my_dockMap.end());
	itm->second->setChecked(actualDock->isVisible());
}

void ak::ui::qt::dockWatcherToolButton::slotClicked() {
	if (my_dockMap.size() > 0) {
		showMenu();
	}
}

void ak::ui::qt::dockWatcherToolButton::slotMenuItemChanged() {
	contextMenuItem * itm = nullptr;
	itm = dynamic_cast<contextMenuItem *>(sender());
	assert(itm != nullptr);
	auto d = my_actionMap.find(itm);
	assert(d != my_actionMap.end());
	d->second->setVisible(itm->isChecked());
}