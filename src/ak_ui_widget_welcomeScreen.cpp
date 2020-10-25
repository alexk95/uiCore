/*
 * ak_ui_widget_welcomeScreen.cpp
 *
 *  Created on: October 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_widget_welcomeScreen.h>	// Corresponding header
#include <ak_messenger.h>						// messenger
#include <ak_uidMangager.h>						// UID manager
#include <ak_ui_colorStyle.h>					// color style
#include <ak_ui_qt_list.h>

// Qt header
#include <qobject.h>							// QObject
#include <qwidget.h>							// QWidget
#include <qlabel.h>
#include <qlayout.h>							// QHBoxLayout, QVBoxLayout
#include <qpixmap.h>
#include <qbitmap.h>
#include <qscrollbar.h>
#include <qfont.h>
#include <qcolor.h>

#define MY_ID_RECENTS 0
#define MY_ID_OPEN 1
#define MY_ID_NEW 2

ak::ui::widget::welcomeScreen::welcomeScreen(
	messenger *				_messenger,
	uidManager *			_uidManager,
	ak::ui::colorStyle *	_colorStyle
)
	: aWidgetManager(ak::ui::core::oDefaultWelcomeScreen, _messenger, _uidManager, _colorStyle), my_signalLinker(nullptr),
	my_headerLabelTextSize(24)
{
	my_new.List = nullptr;
	my_new.Label = nullptr;
	my_open.List = nullptr;
	my_open.Label = nullptr;
	my_recents.List = nullptr;
	my_recents.Label = nullptr;


	// Initialize screen data
	my_screen.Central = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.New = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.Open = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.OpenNewCentral = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.Recents = toLayoutWidgetCombo(nullptr, nullptr);
	my_screen.RecentsCentralDummy = toLayoutWidgetCombo(nullptr, nullptr);

	// Create signal linker
	my_signalLinker = new welcomeScreenSignalLinker(this);

	// Create central widget
	my_screen.Central = toLayoutWidgetCombo(new QHBoxLayout, new QWidget);
	
	// Create recents object
	my_screen.RecentsCentralDummy = toLayoutWidgetCombo(new QHBoxLayout, new QWidget);
	my_screen.Central.layout->addWidget(my_screen.RecentsCentralDummy.widget);
	my_screen.Recents = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.RecentsCentralDummy.layout->addWidget(my_screen.Recents.widget);
	
	my_recents.List = new ui::qt::list;
	my_recents.List->setUid(MY_ID_RECENTS);
	my_recents.List->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	my_recents.List->verticalScrollBar()->setStyleSheet(
		"QScrollBar::add-line:vertical{border: 0px; background-color: #00000000; width: 0px; height: 0px;}"
		"QScrollBar::sub-line:vertical{border: 0px; background-color: #00000000; width: 0px; height: 0px;}"
	);
	my_recents.List->setVerticalScrollbarAlwaysVisible(false);
	my_signalLinker->addLink(my_recents.List);
	my_recents.Label = new QLabel("Recent");
	my_recents.Label->setBuddy(my_recents.List);
	QFont font = my_recents.Label->font();
	font.setPointSize(my_headerLabelTextSize);
	my_recents.Label->setFont(font);

	my_screen.Recents.layout->addWidget(my_recents.Label);
	my_screen.Recents.layout->addWidget(my_recents.List);

	// Create open new 
	my_screen.OpenNewCentral = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.Central.layout->addWidget(my_screen.OpenNewCentral.widget);

	// Create open
	my_screen.Open = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.OpenNewCentral.layout->addWidget(my_screen.Open.widget);

	my_open.List = new ui::qt::list;
	my_open.List->setUid(MY_ID_OPEN);
	my_open.List->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	my_open.List->setVerticalScrollbarAlwaysVisible(false);
	my_signalLinker->addLink(my_open.List);
	my_open.Label = new QLabel("Open");
	my_open.Label->setBuddy(my_open.List);
	font = my_open.Label->font();
	font.setPointSize(my_headerLabelTextSize);
	my_open.Label->setFont(font);

	my_screen.Open.layout->addWidget(my_open.Label);
	my_screen.Open.layout->addWidget(my_open.List);

	// Create new
	my_screen.New = toLayoutWidgetCombo(new QVBoxLayout, new QWidget);
	my_screen.OpenNewCentral.layout->addWidget(my_screen.New.widget);

	my_new.List = new ui::qt::list;
	my_new.List->setUid(MY_ID_NEW);
	my_new.List->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	my_new.List->setVerticalScrollbarAlwaysVisible(false);
	my_signalLinker->addLink(my_new.List);
	my_new.Label = new QLabel("New");
	my_new.Label->setBuddy(my_new.List);
	font = my_new.Label->font();
	font.setPointSize(my_headerLabelTextSize);
	my_new.Label->setFont(font);

	my_screen.New.layout->addWidget(my_new.Label);
	my_screen.New.layout->addWidget(my_new.List);

	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }
	else {
		my_open.List->setStyleSheet("QListWidget{"
			"border-color:#00000000;"
			"}\n");
	}

}

ak::ui::widget::welcomeScreen::~welcomeScreen() { memFree(); }

QWidget * ak::ui::widget::welcomeScreen::widget(void) { return my_screen.Central.widget; }

void ak::ui::widget::welcomeScreen::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {

	assert(_colorStyle != nullptr); // Nullptr provided

	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sWidget));
	if (my_screen.Central.widget != nullptr) { my_screen.Central.widget->setStyleSheet(sheet); }
	if (my_screen.New.widget != nullptr) { my_screen.New.widget->setStyleSheet(sheet); }
	if (my_screen.Open.widget != nullptr) { my_screen.Open.widget->setStyleSheet(sheet); }
	if (my_screen.OpenNewCentral.widget != nullptr) { my_screen.OpenNewCentral.widget->setStyleSheet(sheet); }
	if (my_screen.Recents.widget != nullptr) { my_screen.Recents.widget->setStyleSheet(sheet); }

	sheet = "QListWidget{"
		"background-color:#00000000;"
		"border:0px;"
		"}\n"

		"QListWidget::item{"
		"background-color:#00000000;"
		"color:#";
	sheet.append(my_colorStyle->getControlsMainForecolor().toHexString());

	sheet.append(
		";}\n"
		"QListWidget::item:hover:!pressed:!selected{"
		"background-color:#00000000;"
		"color:#");
	sheet.append(my_colorStyle->getControlsFocusedColor().toHexString());

	sheet.append(
		";}\n"
		"QListWidget::item:pressed:hover{"
		"background-color:#00000000;"
		"color:#");
	sheet.append(my_colorStyle->getControlsPressedColor().toHexString());
	sheet.append(
		";}\n"
		"QListWidget::item:selected{"
		"background-color:#00000000;"
		"color:#");
	sheet.append(my_colorStyle->getControlsPressedColor().toHexString());

	sheet.append(
		";}\n"
	);

	if (my_new.List != nullptr) { my_new.List->setStyleSheet(sheet); }
	if (my_open.List != nullptr) { my_open.List->setStyleSheet(sheet); }
	sheet = my_colorStyle->getStylesheet(colorStyle::styleableObject::sListBorderless);
	if (sheet.length() == 0) {
		sheet = "QListWidget{"
			"background-color:#00000000;"
			"border:0px;"
			"}\n";
	}
	if (my_recents.List != nullptr) { my_recents.List->setStyleSheet(sheet); }

	sheet = "QLabel{color:#";
	sheet.append(my_colorStyle->getControlsFocusedColor().toHexString());
	sheet.append(";}\n");
	if (my_new.Label != nullptr) { my_new.Label->setStyleSheet(sheet); }
	if (my_open.Label != nullptr) { my_open.Label->setStyleSheet(sheet); }
	if (my_recents.Label != nullptr) { my_recents.Label->setStyleSheet(sheet); }

	sheet = "QScrollBar::add-line:vertical{border: 0px; background-color: #00000000; width: 0px; height: 0px;}"
		"QScrollBar::sub-line:vertical{border: 0px; background-color: #00000000; width: 0px; height: 0px;}";
	if (my_new.List != nullptr) { my_new.List->verticalScrollBar()->setStyleSheet(sheet); }
	if (my_open.List != nullptr) { my_open.List->verticalScrollBar()->setStyleSheet(sheet); }
	if (my_recents.List != nullptr) { my_recents.List->verticalScrollBar()->setStyleSheet(sheet); }


}

ak::ID ak::ui::widget::welcomeScreen::addItem(
	ak::ID							_group,
	const QString &					_text
) {
	switch (_group)
	{
	case MY_ID_RECENTS: return my_recents.List->AddItem(_text); break;
	case MY_ID_OPEN: return my_open.List->AddItem(_text); break;
	case MY_ID_NEW: return my_new.List->AddItem(_text); break;
	default:
		assert(0); // invalid group provided
		return ak::invalidID;
	}
}

ak::ID ak::ui::widget::welcomeScreen::addItem(
	ak::ID							_group,
	const QIcon &					_icon,
	const QString &					_text
) {
	switch (_group)
	{
	case MY_ID_RECENTS: return my_recents.List->AddItem(_icon, _text); break;
	case MY_ID_OPEN: return my_open.List->AddItem(_icon, _text); break;
	case MY_ID_NEW: return my_new.List->AddItem(_icon, _text); break;
	default:
		assert(0); // invalid group provided
		return ak::invalidID;
	}
}

ak::ID ak::ui::widget::welcomeScreen::addItemAtRecents(
	const QString &					_text
) { return my_recents.List->AddItem(_text); }

ak::ID ak::ui::widget::welcomeScreen::addItemAtRecents(
	const QIcon &					_icon,
	const QString &					_text
) { return my_recents.List->AddItem(_icon, _text); }

ak::ID ak::ui::widget::welcomeScreen::addItemAtOpen(
	const QString &					_text
) { return my_open.List->AddItem(_text); }

ak::ID ak::ui::widget::welcomeScreen::addItemAtOpen(
	const QIcon &					_icon,
	const QString &					_text
) { return my_open.List->AddItem(_icon, _text); }

ak::ID ak::ui::widget::welcomeScreen::addItemAtNew(
	const QString &					_text
) { return my_new.List->AddItem(_text); }

ak::ID ak::ui::widget::welcomeScreen::addItemAtNew(
	const QIcon &					_icon,
	const QString &					_text
) { return my_new.List->AddItem(_icon, _text); }

void ak::ui::widget::welcomeScreen::clear(void) {
	if (my_recents.List != nullptr) { my_recents.List->Clear(); }
	if (my_open.List != nullptr) { my_open.List->Clear(); }
	if (my_new.List != nullptr) { my_new.List->Clear(); }
}

void ak::ui::widget::welcomeScreen::clear(
	ak::ID							_group
) {
	switch (_group)
	{
	case MY_ID_RECENTS: if (my_recents.List != nullptr) { my_recents.List->Clear(); } break;
	case MY_ID_OPEN: if (my_open.List != nullptr) { my_open.List->Clear(); } break;
	case MY_ID_NEW: if (my_new.List != nullptr) { my_new.List->Clear(); } break;
	default:
		assert(0); // Invalid group provided
		break;
	}
}

QString ak::ui::widget::welcomeScreen::itemText(
	ak::ID							_group,
	ak::ID							_item
) {
	switch (_group)
	{
	case MY_ID_RECENTS: assert(my_recents.List != nullptr); return my_recents.List->itemText(_item); break;
	case MY_ID_OPEN: assert(my_open.List != nullptr); return my_open.List->itemText(_item); break;
	case MY_ID_NEW: assert(my_new.List != nullptr); return my_new.List->itemText(_item); break;
	default:
		assert(0); // Invalid group provided
		break;
	}
	return "";
}

QString ak::ui::widget::welcomeScreen::itemTextAtRecent(
	ak::ID							_item
) {
	assert(my_recents.List != nullptr);
	return my_recents.List->itemText(_item);
}

QString ak::ui::widget::welcomeScreen::itemTextAtOpen(
	ak::ID							_item
) {
	assert(my_open.List != nullptr);
	return my_open.List->itemText(_item);
}

QString ak::ui::widget::welcomeScreen::itemTextAtNew(
	ak::ID							_item
) {
	assert(my_new.List != nullptr);
	return my_new.List->itemText(_item);
}

void ak::ui::widget::welcomeScreen::setItemText(
	ak::ID							_group,
	ak::ID							_item,
	const QString &					_text
) {
	switch (_group)
	{
	case MY_ID_RECENTS: assert(my_recents.List != nullptr); my_recents.List->setItemText(_item, _text); break;
	case MY_ID_OPEN: assert(my_open.List != nullptr); my_open.List->setItemText(_item, _text); break;
	case MY_ID_NEW: assert(my_new.List != nullptr); my_new.List->setItemText(_item, _text); break;
	default:
		assert(0); // Invalid group provided
		break;
	}
}

void ak::ui::widget::welcomeScreen::setItemIcon(
	ak::ID							_group,
	ak::ID							_item,
	const QIcon &					_icon
) {
	switch (_group)
	{
	case MY_ID_RECENTS: assert(my_recents.List != nullptr); my_recents.List->setItemIcon(_item, _icon); break;
	case MY_ID_OPEN: assert(my_open.List != nullptr); my_open.List->setItemIcon(_item, _icon); break;
	case MY_ID_NEW: assert(my_new.List != nullptr); my_new.List->setItemIcon(_item, _icon); break;
	default:
		assert(0); // Invalid group provided
		break;
	}
}

QString ak::ui::widget::welcomeScreen::groupName(
	ak::ID							_group
) {
	switch (_group)
	{
	case MY_ID_NEW: return "New"; break;
	case MY_ID_OPEN: return "Open"; break;
	case MY_ID_RECENTS: return "Recent"; break;
	default:
		assert(0); // Invalid group ID
		return "";
	}
}

void ak::ui::widget::welcomeScreen::setObjectName(
	const QString &					_name
) {
	my_screen.Central.widget->setObjectName(_name);
}

QString ak::ui::widget::welcomeScreen::objectName(void) const {
	return my_screen.Central.widget->objectName();
}

// #############################################################################################################

				// Event handling

void ak::ui::widget::welcomeScreen::handleEvent(
	ak::UID							_group,
	ak::ID							_item,
	ak::core::eventType				_event
) {
	switch (_group)
	{
	case MY_ID_NEW:
		my_messenger->sendMessage(my_uid, _event, MY_ID_NEW, _item); break;
	case MY_ID_OPEN:
		my_messenger->sendMessage(my_uid, _event, MY_ID_OPEN, _item); break;
	case MY_ID_RECENTS:
		my_messenger->sendMessage(my_uid, _event, MY_ID_RECENTS, _item); break;
	default:
		assert(0); // Invalid group provided
		break;
	}
}

// ##############################################################################################
// Private functions

void ak::ui::widget::welcomeScreen::memFree(void) {

	// Destroy signal linker first to disconnect all signals
	if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }

	memFree(my_new);
	memFree(my_open);
	memFree(my_recents);

	memFree(my_screen.Central);
	memFree(my_screen.New);
	memFree(my_screen.Open);
	memFree(my_screen.OpenNewCentral);
	memFree(my_screen.Recents);
}

void ak::ui::widget::welcomeScreen::memFree(
	structLayoutWidget &		_layoutWidget
) {
	if (_layoutWidget.layout != nullptr) { delete _layoutWidget.layout; _layoutWidget.layout = nullptr; }
	if (_layoutWidget.widget != nullptr) { delete _layoutWidget.widget; _layoutWidget.widget = nullptr; }
}

void ak::ui::widget::welcomeScreen::memFree(
	structEntries &				_entry
) {
	if (_entry.List != nullptr) { _entry.List->Clear(); delete _entry.List; _entry.List = nullptr; }
	if (_entry.Label != nullptr) { delete _entry.Label; _entry.Label = nullptr; }
}

ak::ui::widget::welcomeScreen::structLayoutWidget ak::ui::widget::welcomeScreen::toLayoutWidgetCombo(
	QLayout *					_layout,
	QWidget *					_widget
) {
	structLayoutWidget ret;
	ret.layout = _layout;
	ret.widget = _widget;
	_widget->setLayout(_layout);
	return ret;
}

// ########################################################################################

// ****************************************************************************************

// ########################################################################################

ak::ui::welcomeScreenSignalLinker::welcomeScreenSignalLinker(
	widget::welcomeScreen *	_screen
) {
	assert(_screen != nullptr);
	my_screen = _screen;
}

ak::ui::welcomeScreenSignalLinker::~welcomeScreenSignalLinker() {
	for (auto itm : my_lists) {
		disconnect(itm, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *)));
		disconnect(itm, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotItemDoubleClicked(QListWidgetItem *)));
	}
}

void ak::ui::welcomeScreenSignalLinker::addLink(
	qt::list *						_object
) {
	assert(_object != nullptr);
	connect(_object, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *)));
	connect(_object, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotItemDoubleClicked(QListWidgetItem *)));
	my_lists.push_back(_object);
}

void ak::ui::welcomeScreenSignalLinker::slotItemClicked(QListWidgetItem * _item) {
	my_screen->handleEvent(castList(sender())->uid(), castListItem(_item)->id(), ak::core::eventType::eClicked);
}
void ak::ui::welcomeScreenSignalLinker::slotItemDoubleClicked(QListWidgetItem * _item) {
	my_screen->handleEvent(castList(sender())->uid(), castListItem(_item)->id(), ak::core::eventType::eDoubleClicked);
}

ak::ui::qt::list * ak::ui::welcomeScreenSignalLinker::castList(
	QObject *						_sender
) {
	assert(_sender != nullptr);
	qt::list * actualSender = nullptr;
	actualSender = dynamic_cast<qt::list *>(_sender);
	assert(actualSender != nullptr); // Cast failed
	return actualSender;
}

ak::ui::qt::listItem * ak::ui::welcomeScreenSignalLinker::castListItem(
	QListWidgetItem *				_item
) {
	assert(_item != nullptr);
	qt::listItem * item = nullptr;
	item = dynamic_cast<qt::listItem *>(_item);
	assert(item != nullptr); // Cast failed
	return item;
}