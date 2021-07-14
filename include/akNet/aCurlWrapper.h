/*
 *	File:		aCurlWrapper.h
 *	Package:	akNet
 *
 *  Created on: July 14, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// uiCore header
#include <akCore/globalDataTypes.h>

// Qt header
#include <qstring.h>

// C++ header
#include <string>
#include <list>

namespace ak {

	class UICORE_API_EXPORT aCurlWrapper {
	public:


		// ######################################################################################

		// Static functions

		static bool sendMail(
			const std::string &				_mailserver,
			const std::string &				_senderMail,
			const std::list<std::string> &	_receiverMails,
			const std::list<std::string> &	_ccMails,
			const std::string &				_subject,
			const std::string &				_messageBody,
			std::string						_date,
			std::string &					_response,
			int								_timeout = 3000
		);

	private:

	};

}