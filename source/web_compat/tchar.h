#pragma once

#ifndef GGN_WEB_COMPAT_TCHAR_H
#define GGN_WEB_COMPAT_TCHAR_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fstream>
#include <string>

#ifdef __EMSCRIPTEN__

typedef char16_t TCHAR;
typedef char16_t _TCHAR;
typedef const TCHAR* LPCTSTR;
typedef TCHAR* LPTSTR;
typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef int BOOL;
typedef bool boolean;
typedef long LONG;
typedef long long INT64;
typedef void* HWND;
typedef void* HANDLE;
typedef void* HINSTANCE;
typedef void* LPVOID;
typedef char* LPSTR;
typedef int errno_t;

inline errno_t strcpy_s(char* destination, size_t destinationCount, const char* source)
{
    if(destination == NULL || destinationCount == 0) return EINVAL;
    if(source == NULL)
    {
        destination[0] = 0;
        return EINVAL;
    }
    size_t i = 0;
    for(; i + 1 < destinationCount && source[i] != 0; ++i)
    {
        destination[i] = source[i];
    }
    destination[i] = 0;
    return source[i] != 0 ? ERANGE : 0;
}

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef FW_REGULAR
#define FW_REGULAR 400
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef _T
#define __GGN_TEXT2(x) u##x
#define __GGN_TEXT(x) __GGN_TEXT2(x)
#define _T(x) __GGN_TEXT(x)
#endif

#ifndef TEXT
#define TEXT(x) _T(x)
#endif

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

inline size_t _tcslen(const TCHAR* value)
{
    if(value == NULL) return 0;
    const TCHAR* cursor = value;
    while(*cursor != 0) ++cursor;
    return (size_t)(cursor - value);
}

inline int _tcscmp(const TCHAR* left, const TCHAR* right)
{
    if(left == right) return 0;
    if(left == NULL) return -1;
    if(right == NULL) return 1;
    while(*left != 0 && *left == *right)
    {
        ++left;
        ++right;
    }
    return (int)(*left) - (int)(*right);
}

inline TCHAR* _tcscpy(TCHAR* destination, const TCHAR* source)
{
    TCHAR* start = destination;
    if(destination == NULL) return destination;
    if(source == NULL)
    {
        *destination = 0;
        return start;
    }
    while((*destination++ = *source++) != 0) {}
    return start;
}

inline errno_t _tcscpy_s(TCHAR* destination, size_t destinationCount, const TCHAR* source)
{
    if(destination == NULL || destinationCount == 0) return EINVAL;
    size_t i = 0;
    if(source != NULL)
    {
        for(; i + 1 < destinationCount && source[i] != 0; ++i)
        {
            destination[i] = source[i];
        }
    }
    destination[i] = 0;
    return (source != NULL && source[i] != 0) ? ERANGE : 0;
}

inline void ggn_append_utf8_codepoint(std::string& output, unsigned long code)
{
    if(code <= 0x7f)
    {
        output.push_back((char)code);
    }
    else if(code <= 0x7ff)
    {
        output.push_back((char)(0xc0 | ((code >> 6) & 0x1f)));
        output.push_back((char)(0x80 | (code & 0x3f)));
    }
    else
    {
        output.push_back((char)(0xe0 | ((code >> 12) & 0x0f)));
        output.push_back((char)(0x80 | ((code >> 6) & 0x3f)));
        output.push_back((char)(0x80 | (code & 0x3f)));
    }
}

inline std::string ggn_tchar_to_utf8(const TCHAR* value)
{
    std::string output;
    if(value == NULL) return output;
    for(size_t i = 0; value[i] != 0; ++i)
    {
        ggn_append_utf8_codepoint(output, (unsigned long)value[i]);
    }
    return output;
}

inline std::string ggn_tchar_to_utf8(const std::basic_string<TCHAR>& value)
{
    return ggn_tchar_to_utf8(value.c_str());
}

inline unsigned long ggn_read_utf8_codepoint(const char* value, size_t& index)
{
    unsigned char first = (unsigned char)value[index++];
    if(first < 0x80) return first;
    if((first & 0xe0) == 0xc0)
    {
        unsigned char second = (unsigned char)value[index++];
        return ((unsigned long)(first & 0x1f) << 6) | (unsigned long)(second & 0x3f);
    }
    if((first & 0xf0) == 0xe0)
    {
        unsigned char second = (unsigned char)value[index++];
        unsigned char third = (unsigned char)value[index++];
        return ((unsigned long)(first & 0x0f) << 12) |
            ((unsigned long)(second & 0x3f) << 6) |
            (unsigned long)(third & 0x3f);
    }
    return '?';
}

inline errno_t mbstowcs_s(size_t* returnValue, TCHAR* destination, size_t destinationCount, const char* source, size_t count)
{
    if(destination == NULL || destinationCount == 0) return EINVAL;
    if(source == NULL)
    {
        destination[0] = 0;
        if(returnValue != NULL) *returnValue = 0;
        return EINVAL;
    }

    size_t input = 0;
    size_t output = 0;
    size_t limit = count == (size_t)-1 ? (size_t)-1 : count;
    while(source[input] != 0 && input < limit && output + 1 < destinationCount)
    {
        size_t before = input;
        unsigned long code = ggn_read_utf8_codepoint(source, input);
        if(input > limit)
        {
            input = before;
            break;
        }
        if(code > 0xffff) code = '?';
        destination[output++] = (TCHAR)code;
    }
    destination[output] = 0;
    if(returnValue != NULL) *returnValue = output + 1;
    return (source[input] != 0 && output + 1 >= destinationCount) ? ERANGE : 0;
}

inline errno_t wcstombs_s(size_t* returnValue, char* destination, size_t destinationCount, const TCHAR* source, size_t count)
{
    if(destination == NULL || destinationCount == 0) return EINVAL;
    std::string converted = ggn_tchar_to_utf8(source);
    size_t maxCopy = count == (size_t)-1 ? converted.size() : count;
    if(maxCopy > converted.size()) maxCopy = converted.size();
    size_t copyCount = maxCopy;
    if(copyCount >= destinationCount) copyCount = destinationCount - 1;
    memcpy(destination, converted.data(), copyCount);
    destination[copyCount] = 0;
    if(returnValue != NULL) *returnValue = copyCount + 1;
    return copyCount < maxCopy ? ERANGE : 0;
}

inline std::string ggn_tchar_to_path(const TCHAR* value)
{
    std::string output = ggn_tchar_to_utf8(value);
    for(size_t i = 0; i < output.size(); ++i)
    {
        if(output[i] == '\\') output[i] = '/';
    }
    return output;
}

inline std::string ggn_tchar_to_path(const std::basic_string<TCHAR>& value)
{
    return ggn_tchar_to_path(value.c_str());
}

inline void ggn_append_ascii(std::basic_string<TCHAR>& output, const char* value)
{
    if(value == NULL) return;
    while(*value != 0)
    {
        output.push_back((unsigned char)*value);
        ++value;
    }
}

inline void ggn_append_tchars(std::basic_string<TCHAR>& output, const TCHAR* value)
{
    if(value == NULL) value = _T("(null)");
    while(*value != 0)
    {
        output.push_back(*value);
        ++value;
    }
}

inline void ggn_append_padded_tchars(std::basic_string<TCHAR>& output, const TCHAR* value, int width, bool leftAlign)
{
    std::basic_string<TCHAR> text;
    ggn_append_tchars(text, value);
    int padding = width - (int)text.size();
    if(padding < 0) padding = 0;
    if(!leftAlign)
    {
        for(int i = 0; i < padding; ++i) output.push_back(_T(' '));
    }
    output += text;
    if(leftAlign)
    {
        for(int i = 0; i < padding; ++i) output.push_back(_T(' '));
    }
}

inline int ggn_copy_format_result(TCHAR* buffer, size_t bufferCount, const std::basic_string<TCHAR>& value)
{
    if(buffer == NULL) return -1;
    size_t copyCount = value.size();
    if(bufferCount != (size_t)-1)
    {
        if(bufferCount == 0) return -1;
        if(copyCount >= bufferCount) copyCount = bufferCount - 1;
    }
    for(size_t i = 0; i < copyCount; ++i)
    {
        buffer[i] = value[i];
    }
    buffer[copyCount] = 0;
    return (int)value.size();
}

inline void ggn_format_numeric(std::basic_string<TCHAR>& output, const char* format, va_list& args, char conversion, bool longArg, bool longLongArg)
{
    char temp[256];
    temp[0] = 0;
    switch(conversion)
    {
    case 'd':
    case 'i':
        if(longLongArg) snprintf(temp, sizeof(temp), format, va_arg(args, long long));
        else if(longArg) snprintf(temp, sizeof(temp), format, va_arg(args, long));
        else snprintf(temp, sizeof(temp), format, va_arg(args, int));
        break;
    case 'u':
    case 'x':
    case 'X':
    case 'o':
        if(longLongArg) snprintf(temp, sizeof(temp), format, va_arg(args, unsigned long long));
        else if(longArg) snprintf(temp, sizeof(temp), format, va_arg(args, unsigned long));
        else snprintf(temp, sizeof(temp), format, va_arg(args, unsigned int));
        break;
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
        snprintf(temp, sizeof(temp), format, va_arg(args, double));
        break;
    case 'p':
        snprintf(temp, sizeof(temp), format, va_arg(args, void*));
        break;
    default:
        temp[0] = '%';
        temp[1] = conversion;
        temp[2] = 0;
        break;
    }
    ggn_append_ascii(output, temp);
}

inline int ggn_vstprintf(TCHAR* buffer, size_t bufferCount, const TCHAR* format, va_list args)
{
    if(format == NULL) return ggn_copy_format_result(buffer, bufferCount, std::basic_string<TCHAR>());

    std::basic_string<TCHAR> output;
    for(const TCHAR* cursor = format; *cursor != 0; )
    {
        if(*cursor != _T('%'))
        {
            output.push_back(*cursor++);
            continue;
        }

        ++cursor;
        if(*cursor == _T('%'))
        {
            output.push_back(_T('%'));
            ++cursor;
            continue;
        }

        char narrowFormat[64];
        size_t fmtLen = 0;
        narrowFormat[fmtLen++] = '%';
        bool leftAlign = false;
        bool longArg = false;
        bool longLongArg = false;
        int width = 0;

        while(*cursor == _T('-') || *cursor == _T('+') || *cursor == _T(' ') || *cursor == _T('#') || *cursor == _T('0'))
        {
            if(*cursor == _T('-')) leftAlign = true;
            if(fmtLen + 1 < sizeof(narrowFormat)) narrowFormat[fmtLen++] = (char)*cursor;
            ++cursor;
        }

        while(*cursor >= _T('0') && *cursor <= _T('9'))
        {
            width = width * 10 + (int)(*cursor - _T('0'));
            if(fmtLen + 1 < sizeof(narrowFormat)) narrowFormat[fmtLen++] = (char)*cursor;
            ++cursor;
        }

        if(*cursor == _T('.'))
        {
            if(fmtLen + 1 < sizeof(narrowFormat)) narrowFormat[fmtLen++] = '.';
            ++cursor;
            while(*cursor >= _T('0') && *cursor <= _T('9'))
            {
                if(fmtLen + 1 < sizeof(narrowFormat)) narrowFormat[fmtLen++] = (char)*cursor;
                ++cursor;
            }
        }

        if(*cursor == _T('l'))
        {
            longArg = true;
            if(fmtLen + 1 < sizeof(narrowFormat)) narrowFormat[fmtLen++] = 'l';
            ++cursor;
            if(*cursor == _T('l'))
            {
                longLongArg = true;
                if(fmtLen + 1 < sizeof(narrowFormat)) narrowFormat[fmtLen++] = 'l';
                ++cursor;
            }
        }

        char conversion = (char)*cursor;
        if(conversion == 0) break;
        ++cursor;

        if(conversion == 's')
        {
            const TCHAR* stringArg = va_arg(args, const TCHAR*);
            ggn_append_padded_tchars(output, stringArg, width, leftAlign);
            continue;
        }
        if(conversion == 'c')
        {
            output.push_back((TCHAR)va_arg(args, int));
            continue;
        }

        if(fmtLen + 1 < sizeof(narrowFormat)) narrowFormat[fmtLen++] = conversion;
        narrowFormat[fmtLen] = 0;
        ggn_format_numeric(output, narrowFormat, args, conversion, longArg, longLongArg);
    }

    return ggn_copy_format_result(buffer, bufferCount, output);
}

inline int _stprintf(TCHAR* buffer, const TCHAR* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = ggn_vstprintf(buffer, (size_t)-1, format, args);
    va_end(args);
    return result;
}

inline int _stprintf_s(TCHAR* buffer, size_t bufferCount, const TCHAR* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = ggn_vstprintf(buffer, bufferCount, format, args);
    va_end(args);
    return result;
}

inline int _sntprintf(TCHAR* buffer, size_t bufferCount, const TCHAR* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = ggn_vstprintf(buffer, bufferCount, format, args);
    va_end(args);
    return result;
}

inline int _vstprintf_s(TCHAR* buffer, size_t bufferCount, const TCHAR* format, va_list args)
{
    return ggn_vstprintf(buffer, bufferCount, format, args);
}

inline int wsprintf(TCHAR* buffer, const TCHAR* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = ggn_vstprintf(buffer, (size_t)-1, format, args);
    va_end(args);
    return result;
}

inline long _tcstol(const TCHAR* value, TCHAR** end, int base)
{
    std::string narrow = ggn_tchar_to_utf8(value);
    char* narrowEnd = NULL;
    long result = strtol(narrow.c_str(), &narrowEnd, base);
    if(end != NULL) *end = const_cast<TCHAR*>(value + (narrowEnd - narrow.c_str()));
    return result;
}

inline unsigned long _tcstoul(const TCHAR* value, TCHAR** end, int base)
{
    std::string narrow = ggn_tchar_to_utf8(value);
    char* narrowEnd = NULL;
    unsigned long result = strtoul(narrow.c_str(), &narrowEnd, base);
    if(end != NULL) *end = const_cast<TCHAR*>(value + (narrowEnd - narrow.c_str()));
    return result;
}

inline double _tcstod(const TCHAR* value, TCHAR** end)
{
    std::string narrow = ggn_tchar_to_utf8(value);
    char* narrowEnd = NULL;
    double result = strtod(narrow.c_str(), &narrowEnd);
    if(end != NULL) *end = const_cast<TCHAR*>(value + (narrowEnd - narrow.c_str()));
    return result;
}

inline size_t _tcsftime(TCHAR* buffer, size_t bufferCount, const TCHAR* format, const struct tm* timeptr)
{
    std::string narrowFormat = ggn_tchar_to_utf8(format);
    char temp[256];
    size_t result = strftime(temp, sizeof(temp), narrowFormat.c_str(), timeptr);
    if(result == 0)
    {
        if(buffer != NULL && bufferCount > 0) buffer[0] = 0;
        return 0;
    }
    std::basic_string<TCHAR> converted;
    ggn_append_ascii(converted, temp);
    ggn_copy_format_result(buffer, bufferCount, converted);
    return converted.size();
}

inline errno_t _tfopen_s(FILE** file, const TCHAR* filename, const TCHAR* mode)
{
    if(file == NULL) return EINVAL;
    std::string path = ggn_tchar_to_path(filename);
    std::string narrowMode = ggn_tchar_to_utf8(mode);
    *file = fopen(path.c_str(), narrowMode.c_str());
    return *file == NULL ? errno : 0;
}

inline int _tstoi(const TCHAR* value)
{
    return atoi(ggn_tchar_to_utf8(value).c_str());
}

inline double _tstof(const TCHAR* value)
{
    return atof(ggn_tchar_to_utf8(value).c_str());
}

inline void OutputDebugString(const TCHAR* value)
{
    (void)value;
}

inline void OutputDebugString(const char* value)
{
    (void)value;
}

inline std::ostream& operator<<(std::ostream& stream, const std::basic_string<TCHAR>& value)
{
    stream << ggn_tchar_to_utf8(value);
    return stream;
}

class ggn_tofstream
{
public:
    ggn_tofstream()
    {
    }

    explicit ggn_tofstream(const TCHAR* filename)
        : stream_(ggn_tchar_to_path(filename).c_str())
    {
    }

    ggn_tofstream(const TCHAR* filename, std::ios_base::openmode mode)
        : stream_(ggn_tchar_to_path(filename).c_str(), mode)
    {
    }

    operator bool() const
    {
        return stream_.good();
    }

    void close()
    {
        stream_.close();
    }

    template<typename T>
    ggn_tofstream& operator<<(const T& value)
    {
        stream_ << value;
        return *this;
    }

    ggn_tofstream& operator<<(const TCHAR* value)
    {
        stream_ << ggn_tchar_to_utf8(value);
        return *this;
    }

    ggn_tofstream& operator<<(const std::basic_string<TCHAR>& value)
    {
        stream_ << ggn_tchar_to_utf8(value);
        return *this;
    }

private:
    std::ofstream stream_;
};

class ggn_tifstream
{
public:
    ggn_tifstream()
    {
    }

    explicit ggn_tifstream(const TCHAR* filename)
        : stream_(ggn_tchar_to_path(filename).c_str())
    {
    }

    operator bool() const
    {
        return stream_.good();
    }

    void close()
    {
        stream_.close();
    }

private:
    std::ifstream stream_;
};

#endif

#endif

