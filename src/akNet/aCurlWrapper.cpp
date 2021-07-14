/*
 *	File:		aCurlWrapper.cpp
 *	Package:	akNet
 *
 *  Created on: July 14, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// uiCore header
#include <akNet/aCurlWrapper.h>

// Curl header
#include <curl/curl.h>

// C++ header
#include <mutex>

static std::mutex g_mutex;

// #####################################################################################

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

bool ak::aCurlWrapper::sendMail(
	const std::string &				_mailserver,
	const std::string &				_senderMail,
	const std::list<std::string> &	_receiverMails,
	const std::list<std::string> &	_ccMails,
	const std::string &				_subject,
	const std::string &				_messageBody,
	std::string						_date,
	std::string&					_response,
	int								_timeout
) {
	g_mutex.lock();

	std::string curlMessage;

	auto curl = curl_easy_init();
	if (!curl)
	{
		g_mutex.unlock();
		return false;
	}

	curl_easy_setopt(curl, CURLOPT_URL, _mailserver.c_str());
	curl_easy_setopt(curl, CURLOPT_MAIL_FROM, _senderMail.c_str());

	// Add receiver
	struct curl_slist * recipients{ nullptr };
	for (auto receiver : _receiverMails) {
		recipients = curl_slist_append(recipients, receiver.c_str());
	}
	for (auto receiver : _ccMails) {
		recipients = curl_slist_append(recipients, receiver.c_str());
	}
	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, _messageBody);

	std::string header_string;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_response);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

	// Send the message
	auto res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		_response = curl_easy_strerror(res);
	}

	// Clean up the memory
	curl_slist_free_all(recipients);
	curl_easy_cleanup(curl);

	g_mutex.unlock();

	return res == CURLE_OK;
}