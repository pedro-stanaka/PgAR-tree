/* 
 * File:   logger.h
 * Author: dskaster
 *
 * Created on 12 de Janeiro de 2009, 11:27
 */

#ifndef _LOGGER_H
#define	_LOGGER_H

#include <fstream>
#include <time.h>

using namespace std;

class Logger {

public:
    Logger(string logFile);
    virtual ~Logger();
    void message(string msg);
    void warning(string msg);
    void error(string msg);

private:
    ofstream logFile;
    time_t currtime;
    struct tm *strtime;

    void write(string level, string msg);
};

#endif	/* _LOGGER_H */

