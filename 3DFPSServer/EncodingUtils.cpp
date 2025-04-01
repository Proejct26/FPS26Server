
#include "EncodingUtils.h"
#include <windows.h>

std::wstring Utf8ToWString(const std::string& utf8Str)
{
    if (utf8Str.empty())
        return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    if (size_needed == 0)
        return std::wstring();

    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &result[0], size_needed);

    // Remove null terminator
    result.pop_back();
    return result;
}

std::string WStringToUtf8(const std::wstring& wideStr)
{
    if (wideStr.empty())
        return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (size_needed == 0)
        return std::string();

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);

    // Remove null terminator
    result.pop_back();
    return result;
}
