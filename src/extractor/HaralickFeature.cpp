
#include <gbdi/extractor/HaralickFeature.hpp>

/**
 Explanation:
             
          HaralickFeatureMatrix
                    |       Distance Vector
                    |             |
              x = distances       |
              ______________      __
  y = angles |__|__|__|__|__|    |__|
             |__|__|__|__|__|    |__|
             |__|__|__|__|__|    |__|
             |__|__|__|__|__|    |__|

              ______________
             |__|__|__|__|__| -- Angle Vector

  Use (x , y) = (distance, angle) to set and get value.
  The "default" values to (x, y) are (5, 4).
  The direct access to the vectores are not allowed.
*/

/**
* Creates the Co-ocurrence Matrix.
*
* @throw FullHeapException: If the memory are full.
*/
void HaralickFeaturesMatrix::createFeaturesMatrix() throw (FullHeapException*){

     try{
        featuresMatrix = new float*[getNumDistances()];
        for (int i = 0; i < getNumDistances(); i++)
	        featuresMatrix[i] = new float[getNumAngles()];
	        
        for(int x = 0; x < getNumDistances(); x++){
            for(int y = 0; y< getNumAngles(); y++){
                 setHaralickFeatureValue(x, y, 0);
            }
        }
    }catch (bad_alloc&){
        throw new FullHeapException(1, "Insuficient heap size to allocate Fetures Matrix", 0);
    }catch (...){
        throw new FullHeapException(1, "Error into allocate memory size", 0);
    }     
}

/**
* Create the characteristics vector of mean by distance.
*
* @throw FullHeapException If the memory size is not sufficient.
*/
void HaralickFeaturesMatrix::createDistanceMeanVector() throw (FullHeapException*){

     try{
        distanceMeanFeatures = new float[getNumAngles()];
        for(int y = 0; y< getNumAngles(); y++){
            setDistanceMeanValue(y, 0);   
        }
     }catch (bad_alloc&){
        throw new FullHeapException(1, "Insuficient heap size to allocate distance feature vector", 0);
     }catch (...){
        throw new FullHeapException(1, "Error into allocate memory size", 0);
     } 
     
}

/**
* Create the characteristics vector of mean by angle.
*
* @throw FullHeapException If the memory size is not sufficient.
*/
void HaralickFeaturesMatrix::createAngleMeanVector() throw (FullHeapException*){
     
     try{
        angleMeanFeatures = new float[getNumDistances()];
        for(int y = 0; y< getNumDistances(); y++){
                setAngleMeanValue(y, 0);   
        }
     }catch (bad_alloc&){
        throw new FullHeapException(1, "Insuficient heap size to allocate distance feature vector", 0);
     }catch (...){
        throw new FullHeapException(1, "Error into allocate memory size", 0);
     } 
}

/**
* Unlock memory space utilized by FeaturesMatrix.
*/
void HaralickFeaturesMatrix::deleteFeaturesMatrix(){

     if(featuresMatrix != NULL){
        delete(featuresMatrix);                
     }
}

/**
* Unlock memory space utilized by DistanceMeanVector.
*/
void HaralickFeaturesMatrix::deleteDistanceMeanVector(){

     if(distanceMeanFeatures != NULL){
        delete(distanceMeanFeatures);
    }
}

/**
* Unlock memory space utilized by AngleMeanVector.
*/
void HaralickFeaturesMatrix::deleteAngleMeanVector(){
     
     if(angleMeanFeatures != NULL){
        delete(angleMeanFeatures);
    }
}

/**
* Destructor method.
*/
HaralickFeaturesMatrix::~HaralickFeaturesMatrix() throw(){

        deleteFeaturesMatrix();
        deleteDistanceMeanVector();
        deleteAngleMeanVector();
}

/**
* Sets the value of the position on co-ocurrence matrix.
*
* @param distance The distance (X Axis) of the value.
* @param angle    The angle (Y axis) of the value.
* @param value    The value to be inserted on co-ocurrence matrix.
* @throw OutOfBoundsException If the pair (distance, angle) is not a valid position in the co-ocurrence matrix.
*/
void HaralickFeaturesMatrix::setHaralickFeatureValue(int distance, int angle, float value) throw (OutOfBoundsException*){

     if ((distance >= getNumDistances()) || (distance < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());
     if ((angle >= getNumAngles()) || (angle < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());
     
     featuresMatrix[distance][angle] = value;
     //cout << " HARL" << featuresMatrix[distance][angle] << endl;
}

/**
* Sets a value in a valid position of the de DistanceMeanVector
*
* @param pos    The position to be inserted
* @param value  The value what will be inserted
*
* @throw OutOfBoundsException If the pos is a not valid position in the DistanceMean vector.
*/
void HaralickFeaturesMatrix::setDistanceMeanValue(int pos, float value) throw (OutOfBoundsException*){

     if ((pos >= getNumAngles()) || (pos < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());

     distanceMeanFeatures[pos] = value;
}

/**
* Sets a value in a valid position of the de DistanceMeanVector
*
* @param pos    The position to be inserted
* @param value  The value what will be inserted
*
* @throw OutOfBoundsException If the pos is a not valid position in the AngleMean vector.
*/
void HaralickFeaturesMatrix::setAngleMeanValue(int pos, float value) throw (OutOfBoundsException*){

     if ((pos >= getNumDistances()) || (pos < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());

     angleMeanFeatures[pos] = value;
}

/**
* Sets the number of angles utilized.
*
* @param numAngles The number of the angles utilized.
*/
void HaralickFeaturesMatrix::setNumAngles(int numAngles){

     this->numAngles = numAngles;
}

/**
* Sets the number of the distances utilized.
*
* @param numDistances The number of the distances utilized.
*/
void HaralickFeaturesMatrix::setNumDistances(int numDistances){

     this->numDistances = numDistances;
}
        
/**
* Gets a value by pair (distance, angle) of the co-ocurrence matrix.
*
* @param distance    The distance value (X axis).
* @param angle       The angle value (Y axis).
* @return The value of the Haralick feature on the position (distance, angle) of the co-ocurrence matrix.
*
* @throw OutOfBoundsException If the pair (distance, angle) is not a valid position.
*/
float HaralickFeaturesMatrix::getHaralickFeatureValue(int distance, int angle) throw (OutOfBoundsException*){

     if ((distance >= getNumDistances()) || (distance < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());
     if ((angle >= getNumAngles()) || (angle < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());
     
     return featuresMatrix[distance][angle];
}

/**
* Gets a value of the DistanceMean vector.
*
* @param pos The position what will be recovered.
*
* @throw OutOfBoundsException If the pos is not a valid position.
*
* @return A value of the Distance Mean vector on the position pos.
*/
float HaralickFeaturesMatrix::getDistanceMeanValue(int pos) throw (OutOfBoundsException*){

      if ((pos >= getNumAngles()) || (pos < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());

      return distanceMeanFeatures[pos];
}

/**
* Gets a value of the AngleMean vector.
*
* @param pos The position what will be recovered.
*
* @throw OutOfBoundsException If the pos is not a valid position.
*
* @return A value of the Angle Mean vector on the position pos.
*/
float HaralickFeaturesMatrix::getAngleMeanValue(int pos) throw (OutOfBoundsException*){

      if ((pos >= getNumDistances()) || (pos < 0))
        throw new OutOfBoundsException(0, "The requested (distance, angle) is out of bounds", getNumDistances(), getNumAngles());

      return angleMeanFeatures[pos];
}

/**
* Gets the utilized number of angles.
*
* @return The utilized number of angles.
*/
int HaralickFeaturesMatrix::getNumAngles(){

      return numAngles;
}

/**
* Gets the utilized number of distances.
*
* @return The utilized number of distances.
*/
int HaralickFeaturesMatrix::getNumDistances(){

      return numDistances;
}

/**
* Gets the size of the DistanceMean vector.
*
* @return The size of the DistanceMean.
*/
int HaralickFeaturesMatrix::getDistanceMeanSize(){

    return getNumAngles();
}

/**
* Gets the size of the AngleMean vector.
*
* @return The size of the AngleMean.
*/
int HaralickFeaturesMatrix::getAngleMeanSize(){

    return getNumDistances();
}
        
/**
* This method catch a "nature" identification of the co-ocurrence matrix based on distance.
*
* @return The characteristics of the co-ocurrence matrix.
*/
float* HaralickFeaturesMatrix::getDistanceMeanFeatures(){

     for(int y = 0; y< getNumAngles(); y++){
          setDistanceMeanValue(y, 0);   
     };
     
     for(int y = 0; y< getNumAngles(); y++){
        for(int x=0; x < getNumDistances(); x++){
            setDistanceMeanValue(y, (getDistanceMeanValue(y) + getHaralickFeatureValue(x, y)));
        }
        setDistanceMeanValue(y, (getDistanceMeanValue(y)/getNumDistances()));
     } 
    
     return distanceMeanFeatures;  
}

/**
* This method catch a "nature" identification of the co-ocurrence matrix based on angle.
*
* @return The characteristics of the co-ocurrence matrix.
*/
float* HaralickFeaturesMatrix::getAngleMeanFeatures(){

     for(int y = 0; y< getNumDistances(); y++){
          setAngleMeanValue(y, 0);   
     };
     
     for(int x=0; x < getNumDistances(); x++){
        for(int y = 0; y< getNumAngles(); y++){
            setAngleMeanValue(x, (getAngleMeanValue(x) + getHaralickFeatureValue(x, y)));
        }
        setAngleMeanValue(x, (getAngleMeanValue(x)/getNumAngles()));
     }
     
     return angleMeanFeatures;
}

/**
* Clone the co-ocurrence matrix.
*
* @return A cloned co-ocurrence matrix.
*/
HaralickFeaturesMatrix* HaralickFeaturesMatrix::clone(){

     HaralickFeaturesMatrix *h = new HaralickFeaturesMatrix(this->getNumDistances(), 
                                                            this->getNumAngles());
     
     for(int x=0; x < getNumDistances(); x++){
        for(int y = 0; y< getNumAngles(); y++){
              h->setHaralickFeatureValue(x, y, this->getHaralickFeatureValue(x, y)); 
        }
        h->setAngleMeanValue(x, this->getAngleMeanValue(x));
     }
     
     for(int y = 0; y< getNumAngles(); y++){
             h->setDistanceMeanValue(y, this->getDistanceMeanValue(y));
     }
     
     return h;
}

/**
* Compare two co-ocurrence matrix.
*
* @return True if the matrix are equal or false if are distinct.
*/
bool HaralickFeaturesMatrix::isEqual(HaralickFeaturesMatrix *h){

     if (this->getNumDistances() != h->getNumDistances())
        return false;
     if (this->getNumAngles() != h->getNumAngles())
        return false;
     
     for(int x=0; x < getNumDistances(); x++){
        for(int y = 0; y< getNumAngles(); y++){
              if (h->getHaralickFeatureValue(x, y) != this->getHaralickFeatureValue(x, y)){
                  return false;
              } 
        }
        if (h->getAngleMeanValue(x) != this->getAngleMeanValue(x))
           return false;
     }
     
     for(int y = 0; y< getNumAngles(); y++){
             if (h->getDistanceMeanValue(y) != this->getDistanceMeanValue(y))
                return false;
     }
     
     return true;
}
