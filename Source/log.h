#pragma once

#include <string>

void log(const char file[], int line, const char* format, ...);
std::string getLogBuffer();
bool hasNewLogs();