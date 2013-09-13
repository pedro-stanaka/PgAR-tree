#include <gbdi/extractor/MetricHistogram.hpp>

/**
* Constructor.
*/
UnitNondimensional::UnitNondimensional(){

    setGray(0);
    setValue(0);
}

/**
* Destructor.
*/
UnitNondimensional::~UnitNondimensional(){

}

/**
* Sets a gray value.
*
* @param gray The gray value what will be set.
*/
void UnitNondimensional::setGray(int gray){

    this->gray = gray;
}

/**
* Sets a value.
*
* @param value The value what will be set.
*/
void UnitNondimensional::setValue(float value){

    this->value = value;
}

/**
* Sets a gray and a value into a nondimensional unit.
*
* @param gray The gray value what will be set.
* @param value The value what will be set.
*/
void UnitNondimensional::setXYAxis(int gray, float value){

    setGray(gray);
    setValue(value);
}

/**
* Gets a gray value.
*
* @return Gets a gray value.
*/
int UnitNondimensional::getGray(){

    return gray;
}

/**
* Gets a value.
*
* @return Gets value.
*/
float UnitNondimensional::getValue(){

    return value;
}

/**
* Constructor.
*/
NondimensionalHistogram::NondimensionalHistogram(){

    nondimHistogram.clear();
}

/**
* Destructor.
*/
NondimensionalHistogram::~NondimensionalHistogram(){

    nondimHistogram.clear();
}

/**
* Sets a unit nondimensional into a histogram.
*
* @param unit The unit what will be set.
* @param pos The pos in the nondimensional histogram.
*/
void NondimensionalHistogram::setUnitNondimensional(UnitNondimensional unit, int pos){

    UnitNondimensional ut;
    ut.setGray(0);
    ut.setValue(0);
    if (nondimHistogram.size() <= pos){
        for (int i = nondimHistogram.size(); i <= pos; i++)
            nondimHistogram.push_back(ut);
    }
    nondimHistogram[pos] = unit;
}

/**
* Gets a size of the nondimensional histogram.
*
* @return Gets the size of the non dimensional histogram.
*/
int NondimensionalHistogram::getSize(){

    return nondimHistogram.size();
}

/**
* Gets a unit nondimensional.
*
* @param pos The position what will be recovered
* @return The nondimensional unit.
* @throw OutOfBoundsException If pos is not a valid position.
*/
UnitNondimensional NondimensionalHistogram::getUnitNondimensional(int pos) throw (artemis::OutOfBoundsException*){

    try{
        return nondimHistogram[pos];
    }catch(...){
        throw new OutOfBoundsException();
    }
}

/**
* Clear the non dimensional histogram.
*/
void NondimensionalHistogram::clearHistogram(){

    nondimHistogram.clear();
}

