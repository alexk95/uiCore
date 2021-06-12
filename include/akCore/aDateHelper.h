#pragma once

#include <qdatetime.h>
#include <akCore/globalDataTypes.h>

namespace ak {

	class UICORE_API_EXPORT aDateHelper {
	public:
		static QTime addSeconds(const QTime & _orig, int _sec, bool & _overflow);

	};

}
