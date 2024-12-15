#include "log.h" 
#include "Globals.h"
#include <atomic>
#include <cstdarg> 
#ifdef _WIN32
#include <Windows.h>
#endif

std::string logBuffer;
std::atomic<bool> newLogs(false);

void log(const char file[], int line, const char* format, ...) {
    static char tmp_string[4096];
    static char tmp_string2[4096];
    va_list ap;

    // Extract the filename
    const char* filename = strrchr(file, '\\');
    if (filename) {
        filename++;
    }
    else {
        filename = file;
    }

    // Construct the log message
    va_start(ap, format);
    vsprintf_s(tmp_string, 4096, format, ap);
    va_end(ap);
    sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", filename, line, tmp_string);

#ifdef _WIN32
    OutputDebugString(tmp_string2);
#endif
    // Update log buffer and newLogs flag
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