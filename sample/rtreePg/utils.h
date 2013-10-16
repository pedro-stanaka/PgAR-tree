#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include "rtreePg_global.h"

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

extern "C" void freeCompositeArray(compositeArray* aux);

void freeCompositeArray(compositeArray * aux){
    if(aux != NULL){
        for(int i = 0; i < aux->size; i++){
            delete[] aux->data[i];
        }
        delete[] aux->data;
        delete aux;
    }

}

#endif // UTILS_H
