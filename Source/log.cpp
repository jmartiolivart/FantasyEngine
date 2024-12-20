#include "log.h"
#include "Globals.h"
#include <atomic>
#include <cstdarg>
#ifdef _WIN32
#include <Windows.h>
#endif

// Buffers i flags per als logs generals i tinyGLTF
std::string logBuffer;
std::atomic<bool> newLogs(false);

std::string tinyGLTFLogBuffer;
std::atomic<bool> newTinyGLTFLogs(false);

void log(const char file[], int line, const char* format, ...) {
    static char tmp_string[4096];
    static char tmp_string2[4096];
    va_list ap;

    const char* filename = strrchr(file, '\\');
    if (filename) {
        filename++;
    }
    else {
        filename = file;
    }

    va_start(ap, format);
    vsprintf_s(tmp_string, 4096, format, ap);
    va_end(ap);
    sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", filename, line, tmp_string);

#ifdef _WIN32
    OutputDebugString(tmp_string2);
#endif

    logBuffer += tmp_string2;
    newLogs.store(true);
}

std::string getLogBuffer() {
    std::string temp = logBuffer;
    logBuffer.clear();
    newLogs.store(false);
    return temp;
}

bool hasNewLogs() {
    return newLogs.load();
}

void logTinyGLTF(const char file[], int line, const char* format, ...) {
    static char tmp_string[4096];
    static char tmp_string2[4096];
    va_list ap;

    const char* filename = strrchr(file, '\\');
    if (filename) {
        filename++;
    }
    else {
        filename = file;
    }

    va_start(ap, format);
    vsprintf_s(tmp_string, 4096, format, ap);
    va_end(ap);
    sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", filename, line, tmp_string);

#ifdef _WIN32
    OutputDebugString(tmp_string2);
#endif

    tinyGLTFLogBuffer += tmp_string2;
    newTinyGLTFLogs.store(true);
}

std::string getTinyGLTFLogBuffer() {
    std::string temp = tinyGLTFLogBuffer;
    tinyGLTFLogBuffer.clear();
    newTinyGLTFLogs.store(false);
    return temp;
}

bool hasNewTinyGLTFLogs() {
    return newTinyGLTFLogs.load();
}
