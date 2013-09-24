/* 
 * File:   logger.cpp
 * Author: dskaster
 * 
 * Created on 12 de Janeiro de 2009, 11:27
 */

#include "logger.h"

Logger::Logger(string logFileName) {
    logFile.open(logFileName.c_str(), ios::out | ios::app);
}

Logger::~Logger() {
    logFile.close();
}

void Logger::write(string level, string msg) {
    if (logFile.is_open()) {
        time(&currtime);
        strtime = localtime(&currtime);
        logFile << strtime->tm_mday << "/" << (strtime->tm_mon + 1) << "/" << (strtime->tm_year + 1900) << " " << strtime->tm_hour << ":" << strtime->tm_min << ":" << strtime->tm_sec << " " << level << " " << msg << endl;
    }
}

void Logger::message(string msg) {
    write("[MSG]", msg);
}

void Logger::warning(string msg) {
    write("[WARNING]", msg);
}

void Logger::error(string msg) {
    write("[ERROR]", msg);
}