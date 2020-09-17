/*
 * ak_ui_color.cpp
 *
 *  Created on: April 02, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_color.h"		// corresponding class
#include "ak_exception.h"		// error handling
#include <string>				// C++ string
#include <sstream>				// string stream

ak::ui::color::color()
:	my_a(255),
	my_r(0),
	my_g(0),
	my_b(0),
	my_wasChanged(false)
{}

ak::ui::color::color(
	int				_r,
	int				_g,
	int				_b,
	int				_a
) : my_wasChanged(false) {
	try {
		if (_a < 0 || _a > 255) { throw ak::Exception("Value out of range", "Check alpha channel value"); }
		if (_r < 0 || _r > 255) { throw ak::Exception("Value out of range", "Check red channel value"); }
		if (_g < 0 || _g > 255) { throw ak::Exception("Value out of range", "Check green channel value"); }
		if (_b < 0 || _b > 255) { throw ak::Exception("Value out of range", "Check blue channel value"); }
		my_a = _a;
		my_r = _r;
		my_g = _g;
		my_b = _b;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::color::color()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::color::color()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::color::color()"); }
}

ak::ui::color::color(
	const ak::ui::color & _other
)
	: my_a(_other.a()),
	my_b(_other.b()),
	my_g(_other.g()),
	my_r(_other.r()),
	my_wasChanged(false)
{}

ak::ui::color::color(
	QColor &			_other
)
	: my_a(_other.alpha()),
	my_b(_other.blue()),
	my_g(_other.green()),
	my_r(_other.red()),
	my_wasChanged(false)
{}

ak::ui::color & ak::ui::color::operator = (
	const ak::ui::color & _other
) {
	my_a = _other.a();
	my_r = _other.r();
	my_g = _other.g();
	my_b = _other.b();
	my_wasChanged = false;
	return *this;
}

ak::ui::color ak::ui::color::operator + (
	const ak::ui::color & _other
) {
	ak::ui::color out;
	int a = my_a + _other.a();
	if (a > 0) { a /= 2; }
	int r = my_r + _other.r();
	if (r > 0) { r /= 2; }
	int g = my_g + _other.g();
	if (g > 0) { g /= 2; }
	int b = my_b + _other.b();
	if (b > 0) { b /= 2; }
	return ak::ui::color(r, g, b, a);
}

bool ak::ui::color::operator == (const ak::ui::color & _other) const {
	if (my_a != _other.a()) { return false; }
	if (my_r != _other.r()) { return false; }
	if (my_g != _other.g()) { return false; }
	if (my_b != _other.b()) { return false; }
	return true;
}

bool ak::ui::color::operator != (const ak::ui::color & _other) const { return !(*this == _other); }

ak::ui::color::~color(){}

// Set color

void ak::ui::color::setA(
	int					_a
) {
	try {
		if (_a < 0 || _a > 255) { throw ak::Exception("Value out of range", "Check alpha channel value"); }
		my_a = _a;
		my_wasChanged = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::color::setA()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::color::setA()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::color::setA()"); }
}

void ak::ui::color::setR(int
	_r
) {
	try {
		if (_r < 0 || _r > 255) { throw ak::Exception("Value out of range", "Check red channel value"); }
		my_r = _r;
		my_wasChanged = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::color::setR()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::color::setR()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::color::setR()"); }
}

void ak::ui::color::setG(
	int					_g
) {
	try {
		if (_g < 0 || _g > 255) { throw ak::Exception("Value out of range", "Check green channel value"); }
		my_g = _g;
		my_wasChanged = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::color::setG()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::color::setG()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::color::setG()"); }
}

void ak::ui::color::setB(int
	_b
) {
	try {
		if (_b < 0 || _b > 255) { throw ak::Exception("Value out of range", "Check blue channel value"); }
		my_b = _b;
		my_wasChanged = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::color::setB()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::color::setB()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::color::setB()"); }
}

void ak::ui::color::setRGBA(
	int				_r,
	int				_g,
	int				_b,
	int				_a
) {
	try {
		if (_a < 0 || _a > 255) { throw ak::Exception("Value out of range", "Check alpha channel value"); }
		if (_r < 0 || _r > 255) { throw ak::Exception("Value out of range", "Check red channel value"); }
		if (_g < 0 || _g > 255) { throw ak::Exception("Value out of range", "Check green channel value"); }
		if (_b < 0 || _b > 255) { throw ak::Exception("Value out of range", "Check blue channel value"); }
		my_a = _a;
		my_r = _r;
		my_g = _g;
		my_b = _b;
		my_wasChanged = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::color::setRGBA()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::color::setRGBA()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::color::setRGBA()"); }
}

// Get color

int ak::ui::color::a(void) const { return my_a; }

int ak::ui::color::r(void) const { return my_r; }

int ak::ui::color::g(void) const { return my_g; }

int ak::ui::color::b(void) const { return my_b; }

// Get color string

QString ak::ui::color::toHexString(
	bool				_includeA,
	const QString &		_prefix
) const {
	QString out = _prefix;
	QString v = "";
	if (_includeA) {
		v = ak::ui::toHexString(my_a, 2);
		out.append(v);
	}
	v = ak::ui::toHexString(my_r, 2);
	out.append(v);
	v = ak::ui::toHexString(my_g, 2);
	out.append(v);
	v = ak::ui::toHexString(my_b, 2);
	out.append(v);
	return out;
}

QString ak::ui::color::toRGBString(
	const QString &		_delimiter
) const {
	QString out = QString::number(my_r);
	out.append(_delimiter);
	out.append(QString::number(my_g));
	out.append(_delimiter);
	out.append(QString::number(my_b));
	return out;
}

QString ak::ui::color::toARGBString(
	const QString &		_delimiter
) const {
	QString out = QString::number(my_a);
	out.append(_delimiter);
	out.append(QString::number(my_r));
	out.append(_delimiter);
	out.append(QString::number(my_g));
	out.append(_delimiter);
	out.append(QString::number(my_b));
	return out;
}

QColor ak::ui::color::toQColor(void) const
{ return QColor(my_r, my_g, my_b, my_a); }

bool ak::ui::color::wasChanged(void) const { return my_wasChanged; }

QString ak::ui::toHexString(
	int					_value,
	int					_minimumLength
) {
	std::stringstream stream;
	stream << std::hex << _value;
	QString out = "";
	std::string str = stream.str();
	for (int i = str.length(); i < _minimumLength; i++) { out.append("0"); }
	return QString(out.append(str.c_str()));
}
