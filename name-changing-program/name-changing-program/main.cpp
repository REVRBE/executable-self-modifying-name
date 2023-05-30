#include <iostream>
#include <windows.h>

// Function to generate a random string of specified length
std::wstring GenerateRandomString(size_t length) {
    const wchar_t* characters = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::wstring result;

    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (size_t i = 0; i < length; ++i) {
        // Append a random character from the character set
        result += characters[std::rand() % wcslen(characters)];
    }
    return result;
}

// Function to generate a new name for the current file
int main() {
    wchar_t currentFileName[MAX_PATH];

    // Get the current file name
    DWORD length = GetModuleFileNameW(NULL, currentFileName, MAX_PATH);
    if (length == 0) {
        std::cerr << "Error getting current file name." << std::endl;
        return 1;
    }

    // Create a new file name based on the current file name
    std::wstring newFileName(currentFileName);

    // Find the last slash or backslash in the path
    size_t lastSlashIndex = newFileName.find_last_of(L"\\/");
    if (lastSlashIndex != std::wstring::npos) {
        // Remove the file name portion, keeping only the directory path
        newFileName = newFileName.substr(0, lastSlashIndex + 1);
    }

    // Generate a random string and append it to the new file name
    newFileName += GenerateRandomString(13) + L".exe"; // Change the file extension here if its not an executable file

    // Rename the file using the MoveFile function
    if (MoveFileW(currentFileName, newFileName.c_str())) {
        std::wcout << "File renamed using MoveFile function." << std::endl;
    }
    else {
        // Error occurred while renaming
        DWORD error = GetLastError();
        std::cerr << "Error renaming file: " << error << std::endl;
    }

    // Rename the file using the MoveFileEx function with additional options
    if (MoveFileExW(currentFileName, newFileName.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)) {
        std::wcout << "File renamed using MoveFileEx function." << std::endl;
    }
    else {
        // Error occurred while renaming
        DWORD error = GetLastError();
        std::cerr << "Error renaming file: " << error << std::endl;
    }

    return 0;
}
