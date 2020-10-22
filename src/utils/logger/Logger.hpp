#pragma once
#define LOGGER_H

#include <string>

class Logger
{
private:
    static Logger* instance;
    int m_verbosityLevel;
    Logger();
    ~Logger();

public:
    static Logger *getInstance();
    void AddLog(std::string message, int verbosityLevel);
    void SetVerbosity(int verbosityLevel);
};


