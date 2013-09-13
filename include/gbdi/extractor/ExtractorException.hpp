#ifndef EXTRACTOREXCEPTION_HPP
#define EXTRACTOREXCEPTION_HPP

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "../util/ArtemisException.hpp"

using namespace std;
using namespace artemis;

/**
* Base Class to exception of the image extractor
* @brief Base class of exception
* @author Marcos Vinicius Naves Bêdo
* @version 1.0
* @data 11/01/2010
*/

/*
class Exception: public exception{

      private:
              int id;
              string origin;
      
      public:
             Exception();
             Exception(int id, string origin);
             ~Exception() throw();


             string what() throw();
             
             void setID(int id);
             void setOrigin(string origin);
             
             int getID();
             string getOrigin();
};
*/

class OutOfBinsException : public Exception{
    
    private:
        int numMaxBins;
    
    public:
        OutOfBinsException();
        OutOfBinsException(int id, string origin, int numMaxBins);
        ~OutOfBinsException() throw();
        
        void setNumMaxBins(int numMaxBins);
        int getNumMaxBins();
};

#endif
