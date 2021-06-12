#pragma once

#include <qdatetime.h>

namespace ak {

	class aDateHelper {

		static QTime addSeconds(const QTime & _orig, int _sec, bool & _overflow);

	};

}
