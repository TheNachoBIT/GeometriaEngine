#pragma once
#include <string>
#include <vector>

struct StringAPI
{
	static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
	static std::string GetSubstringBetween(std::string text, std::string first, std::string last);
	static std::vector<std::string> SplitIntoVector(const std::string& s, const char* c);
	static bool StartsWith(std::string string, std::string starts);
};
