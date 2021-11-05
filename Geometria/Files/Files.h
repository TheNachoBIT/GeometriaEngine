#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Files
{
	static std::string Read(const char* url);
	static std::string Read(const char* url, bool isBinary);
	static std::string Write(const char* url, std::string content);
	static std::string Replace(const char* oldFile, const char* newFile, bool isBinary);
	static std::string Replace(const char* oldFile, const char* newFile);
	static std::string OpenImage(const char* url, int& width, int& height);
	static void OpenProgram(const wchar_t* url);
	static bool Remove(const char* url);
	static std::vector<unsigned char> GetImageData(const char* url, int& width, int& height);
	static bool SaveImage(std::string output, std::vector<unsigned char> data, int width, int height);
	static bool LoadScene(std::string file);
};