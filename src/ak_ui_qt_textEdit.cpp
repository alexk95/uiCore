/*
 * ak_ui_qt_textEdit.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_qt_textEdit.h>	// corresponding class
#include <ak_exception.h>		// error handling
#include <ak_ui_colorStyle.h>	// colorStyle

// Qt header
#include <qscrollbar.h>

ak::ui::qt::textEdit::textEdit(QWidget * _parent)
: QTextEdit(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(false) {}
ak::ui::qt::textEdit::textEdit(const QString & _text, QWidget * _parent)
: QTextEdit(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(false) {}

ak::ui::qt::textEdit::~textEdit() {}

// #######################################################################################################
// Event handling

void ak::ui::qt::textEdit::keyPressEvent(QKeyEvent *_event)
{
	QTextEdit::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::ui::qt::textEdit::keyReleaseEvent(QKeyEvent *_event) {
	QTextEdit::keyReleaseEvent(_event); emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::textEdit::widget(void) { return this; }

void ak::ui::qt::textEdit::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style", ak::Exception::exceptionType::Nullptr); }
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTextEdit));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::textEdit::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::textEdit::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::textEdit::setColorStyle()"); }
}

void ak::ui::qt::textEdit::addObjectSettingsToValue(
	rapidjson::Value &						_array,
	rapidjson::Document::AllocatorType &	_allocator
) {
	assert(_array.GetType() == rapidjson::Type::kArrayType); // Value is not an array type

	// Initialize object
	rapidjson::Value root;
	root.SetObject();

	// Add alias
	std::string str(my_alias.toStdString());
	rapidjson::Value nAlias(str.c_str(), _allocator);
	root.AddMember(RESTORABLE_NAME_ALIAS, nAlias, _allocator);

	// Add object type
	str = ak::ui::core::toQString(my_objectType).toStdString();
	rapidjson::Value nType(str.c_str(), _allocator);
	root.AddMember(RESTORABLE_NAME_TYPE, nType, _allocator);

	// Create settings
	rapidjson::Value settings;
	settings.SetObject();

	settings.AddMember(RESTORABLE_CFG_SIZE_X, width(), _allocator);
	settings.AddMember(RESTORABLE_CFG_SIZE_Y, height(), _allocator);

	// Add settings
	root.AddMember(RESTORABLE_NAME_SETTINGS, settings, _allocator);
	_array.PushBack(root, _allocator);
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