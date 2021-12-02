#pragma once

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#include "curl/curl.h"

class Web
{
public:
	static std::string Get(const char* urlLink, bool printProgress);
	static std::string Post(std::string url, const char* params);
};