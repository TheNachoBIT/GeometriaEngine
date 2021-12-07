#pragma once

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#include "curl/curl.h"

class WebTools {
private:
	// Pro tip: don't repeat yourself.
	static std::string __escapeAll(std::regex regEx, std::string str);
public:
	// Encodes an string into URL format.
	static std::string EncodeURI(std::string uri);
	//static std::string DecodeURI(std::string uri);

	// Encodes an string into URI format.
	static std::string EncodeURIComponent(std::string uri);
	//static std::string DecodeURIComponent(std::string uri);

	//static std::string __unescapeAll(std::regex regEx, std::string str);
};

struct WebFormField {
	std::string fieldName;
	std::string value;

	WebFormField(std::string fieldName, std::string value) {
		this->fieldName = fieldName;
		this->value = value;
	}
};

struct WebForm {
	std::list <WebFormField> data;

	std::string __parser(std::string name, std::string value, bool toHeader = false) {
		if (toHeader)
			return name + ": " + WebTools::EncodeURIComponent(value);

		return WebTools::EncodeURIComponent(name) + "=" + WebTools::EncodeURIComponent(value);
	}

	// Adds a new field.
	void AddField(std::string fieldName, std::string value) {
		WebFormField newData = WebFormField(fieldName, value);

		data.push_front(newData);
	}

	// Parses all the fields to URI format.
	std::string Parse(bool isHeader = false) {
		std::list <WebFormField>::iterator it;
		std::string result = "";
		bool isFirst = true;

		for (it = data.begin(); it != data.end(); it++) {
			result += (isFirst ? "" : (isHeader ? "\n" : "&")) + __parser(it->fieldName, it->value, isHeader);

			if (isFirst) isFirst = false;
		}

		return result;
	}

	// Parses all the fields to a Curl list.
	void ParseToCurlList(curl_slist* list, bool isHeader = false) {
		std::list <WebFormField>::iterator it;

		for (it = data.begin(); it != data.end(); it++) {
			std::string strData = __parser(it->fieldName, it->value, isHeader);
			list = curl_slist_append(list, strData.c_str());
		}
	}
};

struct WebResponse {
	unsigned int code;
	float timeElapsed;
	double downloadProgress, uploadProgress;
	std::string body, headers, mime, url;
	bool isDone;
};

class WebRequest {
private:
	void __startRequest(WebForm *form, WebResponse *response) {
		// Create CURL request
		CURL *curl = curl_easy_init();

		if (curl) {
			// Start configuration
			std::string requestUrl = url;
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)maxRedirects);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
			curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, TRUE);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, TRUE);
			curl_easy_setopt(curl, CURLOPT_AUTOREFERER, TRUE);
			curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptEncoding.c_str());

			// Pass data
			std::string curlMethod = MethodToString();
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, (curlMethod == "CURL_DEFAULT" ? NULL : curlMethod.c_str()));

			if (cookies != "")
				curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());

			struct curl_slist *headersList = NULL;
			headers.ParseToCurlList(headersList, true);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersList);

			// Prepare URL
			if (form) {
				switch (method) {
					case HttpMethod::HTTP_GET:
						requestUrl += (requestUrl.find('?') != std::string::npos ? '&' : '?') + form->Parse();
						break;
					case HttpMethod::HTTP_POST:
					case HttpMethod::HTTP_PUT:
					case HttpMethod::HTTP_DELETE:
					case HttpMethod::CURL_DEFAULT:
						curl_easy_setopt(curl, CURLOPT_POSTFIELDS, form->Parse().c_str());
						break;
				}
			}
			curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());

			// Start CURL tracking and request
			__callCURL(curl, response);

			curl_slist_free_all(headersList);
			headersList = NULL;
		} else  __curlNullPointerResponse(response);
	}

	static void __startPrimitiveRequest(std::string url, std::string *postData, std::string *cookies, curl_slist *headers, WebResponse *response);
	static void __callCURL(CURL *curl, WebResponse *response);

	static void __curlNullPointerResponse(WebResponse *response);

	static int __curlProgressCallback(WebResponse *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static size_t __curlWriterCallback(char *buffer, size_t size, size_t nmemb, WebResponse *clientp);
	static size_t __curlHeaderCallback(char *buffer, size_t size, size_t nitems, WebResponse *clientp);
public:
	enum class HttpMethod { HTTP_GET, HTTP_HEAD, HTTP_POST, HTTP_PUT, HTTP_DELETE, CURL_DEFAULT };

	std::string url, cookies, acceptEncoding = "";
	HttpMethod method;
	WebForm headers;
	unsigned int status, maxRedirects = 10;
	unsigned long timeout = 60L;

	WebRequest() {
		this->url = "";
		method = HttpMethod::HTTP_GET;
	}

	WebRequest(std::string url) {
		this->url = url;
		method = HttpMethod::HTTP_GET;
	}

	WebRequest(std::string url, HttpMethod method) {
		this->url = url;
		this->method = method;
	}

	// Sets a new request header.
	void SetRequestHeader(std::string name, std::string value) {
		headers.AddField(name, value);
	}

	// Converts the current method to a string.
	std::string MethodToString() {
		switch (method) {
			case HttpMethod::HTTP_GET: return "GET";
			case HttpMethod::HTTP_HEAD: return "HEAD";
			case HttpMethod::HTTP_POST: return "POST";
			case HttpMethod::HTTP_PUT: return "PUT";
			case HttpMethod::HTTP_DELETE: return "DELETE";
			case HttpMethod::CURL_DEFAULT: return "CURL_DEFAULT";
		}

		return "GET";
	}

	// Starts a web request.
	void SendRequest(WebResponse* response) {
		this->__startRequest(NULL, response);
	}

	// Starts a web request.
	void SendRequest(WebResponse* response, WebForm form) {
		this->__startRequest(&form, response);
	}

	// Starts a web request using all defaults CURL configurations, ignoring all the WebRequest configuration.
	static void SendPrimitiveRequest(WebResponse *response, std::string url);
	
	// Starts a web request using all defaults CURL configurations, ignoring all the WebRequest configuration.
	static void SendPrimitiveRequest(WebResponse *response, std::string url, std::string postData);
	
	// Starts a web request using all defaults CURL configurations, ignoring all the WebRequest configuration.
	static void SendPrimitiveRequest(WebResponse *response, std::string url, std::string postData, std::string *cookies);
	
	// Starts a web request using all defaults CURL configurations, ignoring all the WebRequest configuration.
	static void SendPrimitiveRequest(WebResponse *response, std::string url, std::string postData, std::string *cookies, curl_slist *headers);
};
