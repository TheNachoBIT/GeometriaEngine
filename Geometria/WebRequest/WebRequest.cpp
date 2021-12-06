#include "WebRequest.h"
#include <regex>

#define RSPrint(x) std::cout << x << std::endl

#pragma region WebTools
std::string WebTools::__escapeAll(std::regex regEx, std::string str) {
	std::string encoded = "";

	for (std::string::size_type i = 0; i < str.size(); i++) {
		if (std::regex_match(str.substr(i, 1), regEx)) {
			encoded += str[i];
			continue;
		}

		encoded += "%" + int((unsigned char)str[i]);
	}

	return encoded;
}

std::string WebTools::EncodeURI(std::string uri) {
	std::regex regEx("[A-Za-z0-9;,\\/?:@&=+$\\-\\_.!~*'()#]");
	std::string encoded = __escapeAll(regEx, uri);

	return encoded;
}

std::string WebTools::EncodeURIComponent(std::string uri) {
	std::regex regEx("[A-Za-z0-9\\-\\_.!~*'()]");
	std::string encoded = __escapeAll(regEx, uri);

	return encoded;
}
#pragma endregion

#pragma region Curl Callbacks

int __curlProgressCallback(WebResponse* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
	if (dltotal == 0.0) return 0;

	clientp->progress = (float)dlnow / (float)dltotal;

	RSPrint(clientp->progress);

	return 0;
}

size_t __curlWriterCallback(char* buffer, size_t size, size_t nmemb, WebResponse* clientp) {
	clientp->body += buffer;

	return size * nmemb;
}

size_t __curlHeaderCallback(char* buffer, size_t size, size_t nitems, WebResponse* clientp) {
	clientp->headers += buffer;

	return size * nitems;
}

#pragma endregion

void WebRequest::__startRequest(WebForm* form, WebResponse* response) {
	// Create CURL request
	CURL* curl = curl_easy_init();

	if (curl) {
		// Start configuration
		std::string requestUrl = url;
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)maxRedirects);
		curl_easy_setopt(curl, CURLOPT_MAXLIFETIME_CONN, (long)maxTime);
		curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, TRUE);

		// Pass data
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, MethodToString().c_str());
		if (cookies != "") {
			curl_easy_setopt(curl, CURLOPT_COOKIE, TRUE);
			headers.AddField("Cookie", cookies);
		}
		struct curl_slist* headersList = NULL;
		headers.ParseToCurlList(headersList, true);
		curl_easy_setopt(curl, CURLOPT_HEADER, headersList);

		// Prepare URL
		if (form) {
			std::regex regEx("\\?");

			switch (method) {
			case HttpMethod::HTTP_GET:
				if (std::regex_match(requestUrl, regEx))
					requestUrl += "&" + form->Parse();
				else
					requestUrl += "?" + form->Parse();
				break;
			case HttpMethod::HTTP_POST:
			case HttpMethod::HTTP_PUT:
			case HttpMethod::HTTP_DELETE:
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, form->Parse().c_str());
				break;
			}
		}
		curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());

		// Tracking progress
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, __curlProgressCallback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, response);

		// Tracking body data
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, __curlWriterCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

		// Tracking headers data
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, __curlHeaderCallback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, response);

		// Tracking status data
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, response->code);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, response->timeElapsed);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, response->url);
		curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, response->mime);

		// Start CURL request
		CURLcode res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			std::string error = curl_easy_strerror(res);

			response->code = 10401;
			response->body = "CURL internal error: " + error;
			response->mime = "text/plain";
			response->progress = 1;
		}

		curl_easy_cleanup(curl);

		curl = NULL;
	}
	else {
		response->code = 10400;
		response->body = "CURL init unknown error.";
		response->mime = "text/plain";
		response->progress = 1;
		response->timeElapsed = 0.f;
	}
}
