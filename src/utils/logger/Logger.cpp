#include "Logger.hpp"
#include "iostream"

Logger *Logger::instance = 0;

Logger::Logger(/* args */)
{
    m_verbosityLevel = 0;
}

Logger::~Logger()
{
}

Logger* Logger::getInstance(){
    if (instance==NULL){
        instance = new Logger;
    }
    return instance;
}

void Logger::AddLog(std::string message, int verbosityLevel){
    if (verbosityLevel <= m_verbosityLevel){
        std::cout << message << std::endl;
    }
}

void Logger::SetVerbosity(int verbosityLevel){
    m_verbosityLevel = verbosityLevel;
}