#include <gbdi/extractor/ExtractorException.hpp>

OutOfBinsException::OutOfBinsException(){

    setID(4);
    setOrigin("Number of Bins Exceeded");
    setNumMaxBins(0);
};

OutOfBinsException::OutOfBinsException(int id, string origin, int numMaxBins){

    setID(id);
    setOrigin(origin);
    setNumMaxBins(numMaxBins);
};

OutOfBinsException::~OutOfBinsException() throw(){};
        
void OutOfBinsException::setNumMaxBins(int numMaxBins){

    this->numMaxBins = numMaxBins;
};

int OutOfBinsException::getNumMaxBins(){
    
    return numMaxBins;
};
