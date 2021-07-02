/*
 *	File:		aColor.cpp
 *	Package:	akGui
 *
 *  Created on: April 02, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aColor.h>
#include <akCore/aException.h>

// C++ header
#include <string>				// C++ string
#include <sstream>				// string stream

ak::aColor::aColor()
:	my_a(255),
	my_r(0),
	my_g(0),
	my_b(0),
	my_wasChanged(false)
{}

ak::aColor::aColor(
	int				_r,
	int				_g,
	int				_b,
	int				_a
) : my_wasChanged(false) {
	try {
		if (_a < 0 || _a > 255) { assert(0); throw aException("Value out of range", "Check alpha channel value"); }
		if (_r < 0 || _r > 255) { assert(0); throw aException("Value out of range", "Check red channel value"); }
		if (_g < 0 || _g > 255) { assert(0); throw aException("Value out of range", "Check green channel value"); }
		if (_b < 0 || _b > 255) { assert(0); throw aException("Value out of range", "Check blue channel value"); }
		my_a = _a;
		my_r = _r;
		my_g = _g;
		my_b = _b;
	}
	catch (const aException & e) { throw aException(e, "ak::aColor::aColor()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aColor::aColor()"); }
	catch (...) { throw aException("Unknown error", "ak::aColor::aColor()"); }
}

ak::aColor::aColor(
	const ak::aColor & _other
)
	: my_a(_other.a()),
	my_b(_other.b()),
	my_g(_other.g()),
	my_r(_other.r()),
	my_wasChanged(false)
{}

ak::aColor::aColor(
	QColor &			_other
)
	: my_a(_other.alpha()),
	my_b(_other.blue()),
	my_g(_other.green()),
	my_r(_other.red()),
	my_wasChanged(false)
{}

ak::aColor & ak::aColor::operator = (
	const ak::aColor & _other
) {
	my_a = _other.a();
	my_r = _other.r();
	my_g = _other.g();
	my_b = _other.b();
	my_wasChanged = false;
	return *this;
}

ak::aColor & ak::aColor::operator = (
	const QColor & _other
	) {
	my_a = _other.alpha();
	my_r = _other.red();
	my_g = _other.green();
	my_b = _other.blue();
	my_wasChanged = false;
	return *this;
}

ak::aColor ak::aColor::operator + (
	const ak::aColor & _other
) {
	ak::aColor out;
	int a = my_a + _other.a();
	if (a > 0) { a /= 2; }
	int r = my_r + _other.r();
	if (r > 0) { r /= 2; }
	int g = my_g + _other.g();
	if (g > 0) { g /= 2; }
	int b = my_b + _other.b();
	if (b > 0) { b /= 2; }
	return aColor(r, g, b, a);
}

bool ak::aColor::operator == (const ak::aColor & _other) const {
	if (my_a != _other.a()) { return false; }
	if (my_r != _other.r()) { return false; }
	if (my_g != _other.g()) { return false; }
	if (my_b != _other.b()) { return false; }
	return true;
}

bool ak::aColor::operator != (const aColor & _other) const { return !(*this == _other); }

ak::aColor::~aColor(){}

// Set aColor

void ak::aColor::setA(
	int					_a
) {
	try {
		if (_a < 0 || _a > 255) { assert(0); throw aException("Value out of range", "Check alpha channel value"); }
		my_a = _a;
		my_wasChanged = true;
	}
	catch (const aException & e) { throw aException(e, "ak::aColor::setA()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aColor::setA()"); }
	catch (...) { throw aException("Unknown error", "ak::aColor::setA()"); }
}

void ak::aColor::setR(int
	_r
) {
	try {
		if (_r < 0 || _r > 255) { assert(0); throw aException("Value out of range", "Check red channel value"); }
		my_r = _r;
		my_wasChanged = true;
	}
	catch (const aException & e) { throw aException(e, "ak::aColor::setR()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aColor::setR()"); }
	catch (...) { throw aException("Unknown error", "ak::aColor::setR()"); }
}

void ak::aColor::setG(
	int					_g
) {
	try {
		if (_g < 0 || _g > 255) { assert(0); throw aException("Value out of range", "Check green channel value"); }
		my_g = _g;
		my_wasChanged = true;
	}
	catch (const aException & e) { throw aException(e, "ak::aColor::setG()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aColor::setG()"); }
	catch (...) { throw aException("Unknown error", "ak::aColor::setG()"); }
}

void ak::aColor::setB(int
	_b
) {
	try {
		if (_b < 0 || _b > 255) { assert(0); throw aException("Value out of range", "Check blue channel value"); }
		my_b = _b;
		my_wasChanged = true;
	}
	catch (const aException & e) { throw aException(e, "ak::aColor::setB()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aColor::setB()"); }
	catch (...) { throw aException("Unknown error", "ak::aColor::setB()"); }
}

void ak::aColor::setRGBA(
	int				_r,
	int				_g,
	int				_b,
	int				_a
) {
	try {
		if (_a < 0 || _a > 255) { assert(0); throw aException("Value out of range", "Check alpha channel value"); }
		if (_r < 0 || _r > 255) { assert(0); throw aException("Value out of range", "Check red channel value"); }
		if (_g < 0 || _g > 255) { assert(0); throw aException("Value out of range", "Check green channel value"); }
		if (_b < 0 || _b > 255) { assert(0); throw aException("Value out of range", "Check blue channel value"); }
		my_a = _a;
		my_r = _r;
		my_g = _g;
		my_b = _b;
		my_wasChanged = true;
	}
	catch (const aException & e) { throw aException(e, "ak::aColor::setRGBA()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aColor::setRGBA()"); }
	catch (...) { throw aException("Unknown error", "ak::aColor::setRGBA()"); }
}

// Get aColor

int ak::aColor::a(void) const { return my_a; }

int ak::aColor::r(void) const { return my_r; }

int ak::aColor::g(void) const { return my_g; }

int ak::aColor::b(void) const { return my_b; }

// Get aColor string

QString ak::aColor::toHexString(
	bool				_includeA,
	const QString &		_prefix
) const {
	return toHexString(*this, _includeA, _prefix);
}

QString ak::aColor::toHexString(
	const aColor &		_color,
	bool				_includeA,
	const QString &		_prefix
) {
	QString out = _prefix;
	QString v = "";
	if (_includeA) {
		v = ak::toHexString(_color.my_a, 2);
		out.append(v);
	}
	v = ak::toHexString(_color.my_r, 2);
	out.append(v);
	v = ak::toHexString(_color.my_g, 2);
	out.append(v);
	v = ak::toHexString(_color.my_b, 2);
	out.append(v);
	return out;
}

QString ak::aColor::toHexString(
	const QColor &		_color,
	bool				_includeA,
	const QString &		_prefix
) {
	QString out = _prefix;
	QString v = "";
	if (_includeA) {
		v = ak::toHexString(_color.alpha(), 2);
		out.append(v);
	}
	v = ak::toHexString(_color.red(), 2);
	out.append(v);
	v = ak::toHexString(_color.green(), 2);
	out.append(v);
	v = ak::toHexString(_color.blue(), 2);
	out.append(v);
	return out;
}

QString ak::aColor::toRGBString(
	const QString &		_delimiter
) const {
	QString out = QString::number(my_r);
	out.append(_delimiter);
	out.append(QString::number(my_g));
	out.append(_delimiter);
	out.append(QString::number(my_b));
	return out;
}

QString ak::aColor::toARGBString(
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

QColor ak::aColor::toQColor(void) const
{ return QColor(my_r, my_g, my_b, my_a); }

bool ak::aColor::wasChanged(void) const { return my_wasChanged; }

QString ak::toHexString(
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
