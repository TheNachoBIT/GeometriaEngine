#include "WebRequest.h"
#include <regex>

#pragma region WebTools
std::string WebTools::__escapeAll(std::regex regEx, std::string str) {
	std::string encoded = "";

	for (std::string::size_type i = 0; i < str.size(); i++) {
		if (std::regex_match(str.substr(i, 1), regEx)) {
			encoded += str[i];
			continue;
		}

		std::ostringstream ostr;
		ostr << '%' << std::uppercase << std::hex << (0xff & str[i]);

		encoded += ostr.str();
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

int WebRequest::__curlProgressCallback(WebResponse* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
	if (dltotal != 0.0) clientp->downloadProgress = dlnow / dltotal;
	if (ultotal != 0.0) clientp->uploadProgress = ulnow / ultotal;

	return 0;
}

size_t WebRequest::__curlWriterCallback(char* buffer, size_t size, size_t nmemb, WebResponse* clientp) {
	clientp->body += buffer;

	return size * nmemb;
}

size_t WebRequest::__curlHeaderCallback(char* buffer, size_t size, size_t nitems, WebResponse* clientp) {
	clientp->headers += buffer;

	return size * nitems;
}

#pragma endregion

#pragma region CURL default calls
void WebRequest::__curlNullPointerResponse(WebResponse *response) {
	response->code = 10400;
	response->body = "CURL init unknown error.";
	response->mime = "text/plain";
	response->isDone = true;
}
void WebRequest::__callCURL(CURL *curl, WebResponse *response) {
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

	// Start CURL request
	CURLcode res = curl_easy_perform(curl);

	if (res == CURLE_OK) {
		// Tracking status data
		long *statusCode = 0L;
		float timeElapsed;
		char *responseURL, *responseMIME;

		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &timeElapsed);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &responseURL);
		curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &responseMIME);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, statusCode);

		response->code = (unsigned int)statusCode;
		response->timeElapsed = timeElapsed;
		if (responseMIME != NULL) response->mime = responseMIME;
		if (responseURL != NULL) response->url = responseURL;
		response->isDone = true;

		statusCode = NULL;
		responseMIME = NULL;
		responseURL = NULL;
	} else {
		std::string error = curl_easy_strerror(res);

		response->code = 10401;
		response->body = "CURL internal error: " + error;
		response->mime = "text/plain";
		response->isDone = true;
	}

	curl_easy_cleanup(curl);
	curl = NULL;
}
#pragma endregion

#pragma region Primitive WebRequest
void WebRequest::__startPrimitiveRequest(std::string url, std::string *postData, std::string *cookies, curl_slist *headers, WebResponse *response) {
	// Create CURL request
	CURL *curl = curl_easy_init();

	if (curl) {
		// Prepare URL
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		// Pass data
		if (cookies) curl_easy_setopt(curl, CURLOPT_COOKIE, cookies->c_str());
		if (headers) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		if (postData) curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData->c_str());

		// Start CURL tracking and request
		__callCURL(curl, response);

	} else __curlNullPointerResponse(response);
}

void WebRequest::SendPrimitiveRequest(WebResponse *response, std::string url) {
	__startPrimitiveRequest(url, NULL, NULL, NULL, response);
}
void WebRequest::SendPrimitiveRequest(WebResponse *response, std::string url, std::string postData) {
	__startPrimitiveRequest(url, &postData, NULL, NULL, response);
}
void WebRequest::SendPrimitiveRequest(WebResponse *response, std::string url, std::string postData, std::string cookies) {
	__startPrimitiveRequest(url, &postData, &cookies, NULL, response);
}
void WebRequest::SendPrimitiveRequest(WebResponse *response, std::string url, std::string postData, std::string cookies, curl_slist *headers) {
	__startPrimitiveRequest(url, &postData, &cookies, headers, response);
}
#pragma endregion

