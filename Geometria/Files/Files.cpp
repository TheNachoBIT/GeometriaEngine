#include "Files.h"
#include "Image/lodepng.h"
#include "Scene/SceneFile.h"

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
