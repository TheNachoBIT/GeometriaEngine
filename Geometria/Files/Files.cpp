#include "Files.h"
#include "Image/lodepng.h"
#include "Scene/SceneFile.h"
#include "nlohmann/json.hpp"
#include <experimental/filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

std::string Files::Read(const char* url)
{
    return Files::Read(url, false);
}

std::string Files::Read(const char* url, bool isBinary)
{
    std::ifstream f;

    if (isBinary)
        f = std::ifstream(url, std::ios::binary);
    else
        f = std::ifstream(url);

    std::string content;

    std::ostringstream ss;
    ss << f.rdbuf();
    content = ss.str();

    return content;
}

std::string Files::Write(const char* url, std::string content)
{
    std::ofstream file(url);
    file << content;
    file.close();

    return content;
}

std::string Files::Replace(const char* oldFile, const char* newFile)
{
    return Files::Replace(oldFile, newFile, false);
}

std::string Files::Replace(const char* oldFile, const char* newFile, bool isBinary)
{
    std::string oldFileContent = Files::Read(oldFile, isBinary);
    std::string newFileContent = Files::Read(newFile, isBinary);

    //std::cout << newFileContent << std::endl;

    //Files::Remove(oldFile);
    return Files::Write(oldFile, newFileContent);
}

std::string Files::OpenImage(const char* url, int& width, int& height)
{
    std::vector<unsigned char> png;
    std::vector<unsigned char> image;
    lodepng::State setState;
    unsigned int w = 0, h = 0;
    
    unsigned state = lodepng::load_file(png, url);
    if (!state)
    {
        state = lodepng::decode(image, w, h, setState, png);
    }

    width = w;
    height = h;

    return std::string(image.begin(), image.end());
}

void Files::OpenProgram(const wchar_t* url)
{
#ifdef _WIN32
    LPCTSTR lpApplicationName = url;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcess(lpApplicationName,   // the path
        NULL,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#endif
}

bool Files::Remove(const char* url)
{
    std::remove(url);

    bool failed = !std::ifstream(url);
    //if (failed) { std::perror("Error opening deleted file"); }
    return failed;
}

std::vector<unsigned char> Files::GetImageData(const char* url, int& width, int& height)
{
    std::vector<unsigned char> png;
    std::vector<unsigned char> image;
    lodepng::State setState;
    unsigned int w = 0, h = 0;

    unsigned state = lodepng::load_file(png, url);
    if (!state)
    {
        state = lodepng::decode(image, w, h, setState, png);
    }

    width = w;
    height = h;

    return image;
}

bool Files::SaveImage(std::string output, std::vector<unsigned char> data, int width, int height)
{
    unsigned error = lodepng::encode(output, data, width, height);
    return error;
}

bool Files::LoadScene(std::string file)
{
    

    return true;
}

std::string Files::GetDirectoryOf(const char* file)
{
    std::string fileToString = file;
    size_t found = fileToString.find_last_of("/\\");
    return(fileToString.substr(0, found));
}

std::vector<std::string> Files::OpenTexturePack(const char* gtxp)
{
    std::string txpContent = Files::Read(gtxp, true);
    auto jsonBlocks = nlohmann::json::parse(txpContent);
    auto entriesArray = jsonBlocks["TexturePack"];

    int total = 0;
    for (auto i : entriesArray)
    {
        std::string n = i["ID"];
        int currentNumb = std::stoi(n);

        if (total < currentNumb)
            total = currentNumb;
    }

    std::vector<std::string> texturePackLinks(total + 1);
    std::cout << texturePackLinks.size() << std::endl;

    std::string directory = Files::GetDirectoryOf(gtxp);

    for (auto i : entriesArray)
    {
        std::string n = i["ID"];
        int numb = std::stoi(n);
        std::string imgName = i["Image"];

        texturePackLinks[numb] += directory;
        texturePackLinks[numb] += "/";
        texturePackLinks[numb] += imgName;
        texturePackLinks[numb] += ".png";
    }

    return texturePackLinks;
}
