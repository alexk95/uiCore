#include <akCore/aDateHelper.h>

QTime ak::aDateHelper::addSeconds(const QTime & _orig, int _sec, bool & _overflow) {
	int h = _orig.hour();
	int m = _orig.minute();
	int s = _orig.second();
	int ms = _orig.msec();

	_overflow = false;

	s += _sec;

	while (s < 0) {
		s += 60;
		m--;
	}
	while (s > 59) {
		s -= 60;
		m++;
	}

	while (m < 0) {
		m += 60;
		h--;
	}
	while (m > 59) {
		m -= 60;
		h++;
	}

	while (h < 0) {
		_overflow = true;
		h += 24;
	}
	
	while (h > 23) {
		_overflow = true;
		h -= 24;
	}

	return QTime(h, m, s, ms);

}

QTime ak::aDateHelper::addTime(const QTime & _orig, int _h, int _m, int _s, bool & _overflow) {
	return addTime(_orig, _h, _m, _s, 0, _overflow);
}

QTime ak::aDateHelper::addTime(const QTime & _orig, int _h, int _m, int _s, int _ms, bool & _overflow) {
	int h = _orig.hour();
	int m = _orig.minute();
	int s = _orig.second();
	int ms = _orig.msec();

	_overflow = false;

	ms += _ms;
	s += _s;
	m += _m;
	h += _h;

	while (s < 0) {
		s += 60;
		m--;
	}
	while (s > 59) {
		s -= 60;
		m++;
	}

	while (m < 0) {
		m += 60;
		h--;
	}
	while (m > 59) {
		m -= 60;
		h++;
	}

	while (h < 0) {
		_overflow = true;
		h += 24;
	}

	while (h > 23) {
		_overflow = true;
		h -= 24;
	}

	return QTime(h, m, s, ms);
}

QTime ak::aDateHelper::addTime(const QTime & _orig, const QTime & _other, bool & _overflow) {
	return addTime(_orig, _other.hour(), _other.minute(), _other.second(), _other.msec(), _overflow);
}

QTime ak::aDateHelper::subTime(const QTime & _orig, const QTime & _other, bool & _overflow) {
	return addTime(_orig, _other.hour() * (-1), _other.minute() * (-1), _other.second() * (-1), _other.msec() * (-1), _overflow);
}