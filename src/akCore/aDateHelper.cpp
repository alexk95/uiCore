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