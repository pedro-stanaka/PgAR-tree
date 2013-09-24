#ifndef UTILS_H
#define UTILS_H

#include <fstream>

bool fileExists (const std::string& name) {
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

#endif // UTILS_H
