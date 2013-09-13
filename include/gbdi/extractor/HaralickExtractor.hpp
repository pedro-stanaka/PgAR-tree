/**********************************************************************
* GBDI Artemis - Copyright (c) 2009-2010 GBDI-ICMC-USP
*
*                                     Homepage: http://gbdi.icmc.usp.br
**********************************************************************/

/**
*
* The GBDI-ICMC-USP Software License Version 1.0
*
* Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
* Ci�ncias Matem�ticas e de Computa��o, University of S�o Paulo -
* Brazil (the Databases and Image Group - Intitute of Matematical and
* Computer Sciences).  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* 3. The end-user documentation included with the redistribution,
*    if any, must include the following acknowledgment:
*       "This product includes software developed by Grupo de Bases
*        de Dados e Imagens, Instituto de Ci�ncias Matem�ticas e de
*        Computa��o, University of S�o Paulo - Brazil (the Databases
*        and Image Group - Intitute of Matematical and Computer
*        Sciences)"
*
*    Alternately, this acknowledgment may appear in the software itself,
*    if and wherever such third-party acknowledgments normally appear.
*
* 4. The names of the research group, institute, university, authors
*    and collaborators must not be used to endorse or promote products
*    derived from this software without prior written permission.
*
* 5. The names of products derived from this software may not contain
*    the name of research group, institute or university, without prior
*    written permission of the authors of this software.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OF THIS SOFTWARE OR
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
*/

/**
* @file
*
* This file defines co-ocurrences Matrix for Haralick use.
*
* @version 1.0
* @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
* @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
* @date 18-08-2010
*/
#ifndef HARALICKEXTRACTOR_HPP
#define HARALICKEXTRACTOR_HPP


#include <iostream>
#include <cmath>

#include <gbdi/extractor/HaralickFeature.hpp>
#include <gbdi/extractor/Extractor.hpp>

/*
  Comments about the data structure:

                Co-ocurrence Matrix
                 x = numDistances
             ______________
            |_1|__|__|__|__|   y = numAngles 
            |__|__|__|__|__|
            |__|__|__|__|__|
            |__|__|__|__|__|
            |__|__|__|__|__|

               Where is writed 1 we have the matrix numBins x numBins associed
                 ...
            |__|__|__|__|__|
            |__|__|__|__|__|
            |__|__|__|__|__|
                 ...
*/

/**
* This is a Template-Class. A template-parameter numDistances is used
* to define the number of the distance what will be utilized on the
* co-ocurrence matrix. The template-parameter numAngles is utilized
* like numDistances. The template-parameter typeCharacteristics defines
* how will be created the signature. The others parameters typeHaralickExtractor
* and numBins defines the behavior of the extractor and the reescale for
* the gray-scaled image.
* <P>A haralick Extractor has six main features: Variance, Entropy, Uniformity,
* homogeinity, moment2thMatrix and Reverse Variance. </P>
*
* @brief Extractor of Haralick Features
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @see ImageBase
* @see ImageException
* @see HaralickFeatures.hpp
* @version 1.0
*/
using namespace std;

template< class SignatureType,
         stWord numDistances = 5,
         stWord numAngles = 4,
         stWord typeCharacteristics = 6,
         stWord typeHaralickExtractor = 0,
         stWord numBins = 16,
         class DataObjectType = Image >
class HaralickExtractor : public Extractor<SignatureType, DataObjectType> {

      private:
             float ****cm;
             HaralickFeaturesMatrix *varianceMatrix;
             HaralickFeaturesMatrix *entropyMatrix; 
             HaralickFeaturesMatrix *uniformityMatrix;
             HaralickFeaturesMatrix *homogeneityMatrix;
             HaralickFeaturesMatrix *moment3thMatrix;
             HaralickFeaturesMatrix *reverseVarianceMatrix;
      public:
             static const int VARIANCE = 0;
             static const int ENTROPY  = 1; 
             static const int UNIFORMITY = 2;
             static const int HOMOGEINITY = 3;
             static const int MOMENT_3TH = 4;
             static const int REVERSE_VARIANCE = 5;
             static const int ALL_CHARACTERISTICS = 6;
             static const int DISTANCE = 0;
             static const int ANGLE = 1;
             static const int ZERO_PI = 0;
             static const int PI_UNDER_FOUR = 1;
             static const int PI_UNDER_TWO = 2;
             static const int THREE_PI_UNDER_FOUR = 3;
      
      private:
             
             /**
             * Creates the big matrix what will be relationed with co-ocurrence matrix.
             *
             * @throw FullHeapException If the memory space are full.
             */
             void createCoocurrenceMatrix() throw (FullHeapException*){
             
                  try{
                      cm = new float***[getNumDistances()];
                      for (int i = 0; i < getNumDistances(); i++){
	                  cm[i] = new float**[getNumAngles()];
                          for (int j = 0; j < getNumAngles(); j++){
                              cm[i][j] = new float*[getNumBins()];
                              for (int k = 0; k < getNumBins(); k++){
                                   cm[i][j][k] = new float[getNumBins()];
                                   for (int l = 0; l < getNumBins(); l++){
                                       cm[i][j][k][l] = 0;
                                   }
                              }
                          }
                      }

                  }catch (bad_alloc&){
                     throw new FullHeapException(1, "Insuficient heap size to allocate Fetures Matrix", 0);
                  }catch (...){
                     throw new FullHeapException(1, "Error into allocate memory size", 0);
                  }
             }
             
             
             /**
             * Unlock the memory space utilized by Co-ocurrence Matrix.
             */
             void deleteCoocurrenceMatrix(){
        
                  if(cm != NULL)
                        delete(cm);
                  // Todo: is it enough to free the matrix?
             }
             
             /**
             * Sets the values on the big co-ocurrence matrix.
             *
             * @param distance    The distance of the pixels (First value of the four).
             * @param angle       The angle between the pixels (Second value of the four).
             * @param numBins_x   The value numBins_x of the co-ocurrence matrix (Third value of the four).
             * @param numBins_y   The value numBins_x of the co-ocurrence matrix (Fourthy value of the four).
             *
             * @throw If the position (distance, angle, numBins_x, numBins_y) in not valid.
             */
             void setCoocurrenceValue(int distance, int angle, int numBins_x, int numBins_y, float value) throw (OutOfBoundsException*){
              
                  if ((angle > getNumAngles()) || (distance > getNumDistances())
                     || (numBins_x > getNumBins()) || (numBins_y > getNumBins())
                     || (angle < 0) || (distance < 0) || (numBins_x < 0) || (numBins_y < 0))
                        throw new OutOfBoundsException(0, "The requested (distance, angle, x, y) is out of bounds", getNumDistances(), getNumAngles());
             
                     cm[distance][angle][numBins_x][numBins_y] = value;
             }
             
             /**
             * Normalize the image gray values.
             *
             * @param height   The height of the Image
             * @param width    The width of the Image
             */
             void normalizeCoocurrenceMatrix(int height, int width){
                  
                  double total;
                  // normaliza��o
                  for (int line = 0; line < getNumBins(); line++){
                      for (int column = 0; column < getNumBins(); column++){
                          for (int distance = 1; distance <= getNumDistances(); distance++){
                              // zero grau
                              total = 2*height*(width - distance);
                              cm[distance-1][0][line][column] /= total;
                              // 45 graus e 135 graus
                              total = 2*(height - distance)*(width - distance);
                              cm[distance-1][1][line][column] /= total;
                              cm[distance-1][3][line][column] /= total;
                              // 90 graus
                              total = 2*(height - distance)*width;
                              cm[distance-1][2][line][column] /= total;
                          }
                      }
                  }
            }
     

             
      public:
             
             /**
             * Construtor
             */
             HaralickExtractor(){
                  
                  varianceMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
                  entropyMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles()); 
                  uniformityMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
                  homogeneityMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
                  moment3thMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
                  reverseVarianceMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
             }
             
             /**
             * Destructor
             */
             ~HaralickExtractor() throw(){
             }
             
             /**
             * Gets the utilized number of distances.
             *
             * @return The number of the utilized distances.
             */
             stWord getNumDistances(){
             
                 return numDistances;
             }
             
             /**
             * Gets the utilized number of angles.
             *
             * @return The number of utilized angles.
             */
             stWord getNumAngles(){
             
                 return numAngles;
             }
             
             /**
             * Gets the number of the utilized characteristics utilized to compose the signature.
             *
             * @return The utilized characteristics.
             */
             stWord getTypeCharacteristics(){
               
                 return typeCharacteristics;  
             }
             
             /**
             * Gets the type of the extractor.
             *
             * @return The type of the extractor.
             */
             stWord getTypeHaralickExtractor(){
             
                 return typeHaralickExtractor;
             }
             
             /**
             * Gets the number of the utilized bins.
             *
             * @return The number of the utilized bins.
             */
             stWord getNumBins(){
             
                 return numBins;
             }
        
             /**
             * Gets the value on the big matrix.
             *
             * @param distance                The distance (First param of the matrix)
             * @param angle                   The angle (Second param of the matrix)
             * @param numBins_x               The numBins_x (Third param of the matrix)
             * @param numBins_y               The numBins_y (Fourthy param of the matrix)
             *
             * @throw OutOfBoundsException    If the (distance, angle, numBins_x, numBins_y) are not valid.
             *
             * @return The value on the big co-ocurrence matrix.
             */
             float getCoocurrenceValue(int distance, int angle, int numBins_x, int numBins_y) throw (OutOfBoundsException*){
              
                   if ((angle > getNumAngles()) || (distance > getNumDistances())
                      || (numBins_x > getNumBins()) || (numBins_y > getNumBins())
                      || (angle < 0) || (distance < 0) || (numBins_x < 0) || (numBins_y < 0))
                         throw new OutOfBoundsException(0, "The requested (distance, angle, x, y) is out of bounds", getNumDistances(), getNumAngles());
              
                   return cm[distance][angle][numBins_x][numBins_y];
             }
             
             /**
             * Load the co-ocurrence matrix
             *
             * @param *image The image that will be loaded into co-ocurrence matrix
             */
             void loadImageValues(const Image & image){

                //Build a clone image with gray value depth reduced
                Image img;
                img.createPixelMatrix(image.getWidth(), image.getHeight());

                float max = pow(2, (float) image.getBitsPerPixel());

                for (int x= 0; x < image.getWidth(); x++){
                    for (int y = 0; y < image.getHeight(); y++){
                        Pixel p(image.getPixel(x, y));
                        p.setGrayPixelValue((image.getPixel(x, y).getGrayPixelValue() * getNumBins()) / max, max/getNumBins());
                        img.setPixel(x, y, p);
                    }
                }

                //Compute the values of the co-ocurrence matrix
                for(int y = 0; y < img.getHeight(); y++){
                     for(int x = 0; x < img.getWidth(); x++){
                          for(int distance = 1; distance <= getNumDistances(); distance++){
                                if((x - distance) >= 0 && (x + distance) < img.getWidth()){
                                   setCoocurrenceValue(distance-1, 0, 
                                                           (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                           (int) (img.getPixel(x+distance, y).getGrayPixelValue()),
                                                           getCoocurrenceValue(distance-1, 0,  
                                                           (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                           (int) (img.getPixel(x+distance, y).getGrayPixelValue())) + 1);
                                   setCoocurrenceValue(distance-1, 0,
                                                           (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                           (int) (img.getPixel(x-distance, y).getGrayPixelValue()),
                                                           getCoocurrenceValue(distance-1, 0,  
                                                           (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                           (int) (img.getPixel(x-distance, y).getGrayPixelValue())) + 1);
                                }else{
                                   if((x - distance) < 0){
                                         setCoocurrenceValue(distance-1, 0,  
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x+distance, y).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 0,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x+distance, y).getGrayPixelValue())) + 1);
                                   }else{
                                         setCoocurrenceValue(distance-1, 0, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x-distance, y).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 0, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x-distance, y).getGrayPixelValue())) + 1);
                                   }
                                }// End 0

                                //Starts 90o
                                if(((y-distance) >= 0) && ((y+distance) < img.getHeight())){
                                   setCoocurrenceValue(distance-1, 2,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y+distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 2, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y+distance).getGrayPixelValue())) + 1);
                                   setCoocurrenceValue(distance-1, 2, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y-distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 2, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y-distance).getGrayPixelValue())) + 1);
                                }else{
                                   if((y-distance) < 0){
                                         setCoocurrenceValue(distance-1, 2, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y+distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 2,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y+distance).getGrayPixelValue())) + 1);
                                   }else{
                                         setCoocurrenceValue(distance-1, 2, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y-distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 2,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x, y-distance).getGrayPixelValue())) + 1);
                                   }
                                }//End 90 graus

                                // Start 135 graus
                                if(((x-distance) >= 0) && ((x+distance) < img.getWidth()) &&
                                   ((y-distance) >= 0) && ((y+distance) < img.getHeight())){
                                      setCoocurrenceValue(distance-1, 3,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x-distance, y-distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 3,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x-distance, y-distance).getGrayPixelValue())) + 1);
                                      setCoocurrenceValue(distance-1, 3, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x+distance, y+distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 3,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x+distance, y+distance).getGrayPixelValue())) + 1);
                                }else{
                                   if((((x-distance) < 0) && ((y+distance) < img.getHeight())) ||
                                      (((y-distance) < 0) && ((x+distance) < img.getWidth()))){
                                     setCoocurrenceValue(distance-1, 3,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x+distance, y+distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 3,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x+distance, y+distance).getGrayPixelValue())) + 1);
                                   }
                                   if((((x+distance) >= img.getWidth()) && ((y-distance) >= 0)) ||
                                      (((y+distance) >= img.getHeight()) && ((x-distance) >= 0))){
                                        setCoocurrenceValue(distance-1, 3,
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x-distance, y-distance).getGrayPixelValue()),
                                                                getCoocurrenceValue(distance-1, 3, 
                                                                (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (int) (img.getPixel(x-distance, y-distance).getGrayPixelValue())) + 1);
                                   }
                                }// End 135 graus

                                // Start 45 graus
                                if (((x-distance) >= 0) && ((x+distance) < img.getWidth()) &&
                                  ((y-distance) >= 0) && ((y+distance) < img.getHeight())){
                                     setCoocurrenceValue(distance-1, 1,
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x+distance, y-distance).getGrayPixelValue()),
                                                            getCoocurrenceValue(distance-1, 1, 
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x+distance, y-distance).getGrayPixelValue())) + 1);
                                     setCoocurrenceValue(distance-1, 1, 
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x-distance, y+distance).getGrayPixelValue()),
                                                            getCoocurrenceValue(distance-1, 1,
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x-distance, y+distance).getGrayPixelValue())) + 1);
                                }else{
                                      if((((x-distance) < 0) && ((y-distance) >= 0)) ||
                                         (((y+distance) >= img.getHeight()) && ((x+distance) < img.getWidth()))){
                                         setCoocurrenceValue(distance-1, 1,
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x+distance, y-distance).getGrayPixelValue()),
                                                            getCoocurrenceValue(distance-1, 1, 
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x+distance, y-distance).getGrayPixelValue())) + 1);
                                      }
                                      if((((x+distance) >= img.getWidth()) && ((y+distance) < img.getHeight())) ||
                                        (((y-distance) < 0) && ((x-distance) >= 0))){
                                           setCoocurrenceValue(distance-1, 1,
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x-distance, y+distance).getGrayPixelValue()),
                                                            getCoocurrenceValue(distance-1, 1,
                                                            (int) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (int) (img.getPixel(x-distance, y+distance).getGrayPixelValue())) + 1);
                                      }
                                }// End 45 graus
                          }// End for distance
                     }// End for largura
                 }// End for altura
          }
          
          /**
          * Gets the signature based on characteristic defined by the user: Entropy or/and variancy, etc....
          *
          * @return The selected signature.
          */
          float* getSelectedSignature(){
          
             float *result = NULL;
     
             if (getTypeCharacteristics() == VARIANCE){
                if (getTypeHaralickExtractor() == 0){
                   result = varianceMatrix->getDistanceMeanFeatures();
                } else{
                   result = varianceMatrix->getAngleMeanFeatures();
                }
             };
    
             if (getTypeCharacteristics() == ENTROPY){
                if (getTypeHaralickExtractor() == 0){
                   result = entropyMatrix->getDistanceMeanFeatures();
                }else{
                   result = entropyMatrix->getAngleMeanFeatures();
                }
             };
    
             if (getTypeCharacteristics() == UNIFORMITY){
                if (getTypeHaralickExtractor() == 0){
                   result = uniformityMatrix->getDistanceMeanFeatures();
                }else{
                   result = uniformityMatrix->getAngleMeanFeatures();
                }
             };
    
             if (getTypeCharacteristics() == HOMOGEINITY){
                if (getTypeHaralickExtractor() == 0){
                     result = homogeneityMatrix->getDistanceMeanFeatures();
                }else{
                     result = homogeneityMatrix->getAngleMeanFeatures();
                }
             };
    
             if (getTypeCharacteristics() == MOMENT_3TH){
                if (getTypeHaralickExtractor() == 0){
                     result = moment3thMatrix->getDistanceMeanFeatures();
                }else{
                     result = moment3thMatrix->getAngleMeanFeatures();
                }
             };
    
             if (getTypeCharacteristics() == REVERSE_VARIANCE){
                if (getTypeHaralickExtractor() == 0){
                      result = reverseVarianceMatrix->getDistanceMeanFeatures();
                }else{
                      result = reverseVarianceMatrix->getAngleMeanFeatures();
                }
             };
    
             if (getTypeCharacteristics() == ALL_CHARACTERISTICS){

                 if (getTypeHaralickExtractor() == 0){
                     int distanceMeanSize = varianceMatrix->getDistanceMeanSize();
                     int sizeVector = 6*distanceMeanSize;

                     result = new float[sizeVector];
                     float* variance = varianceMatrix->getDistanceMeanFeatures();
                     float* entropy = entropyMatrix->getDistanceMeanFeatures();
                     float* uniformity = uniformityMatrix->getDistanceMeanFeatures();
                     float* homogeneity = homogeneityMatrix->getDistanceMeanFeatures();
                     float* moment3th = moment3thMatrix->getDistanceMeanFeatures();
                     float* reverseVariance = reverseVarianceMatrix->getDistanceMeanFeatures();
            
                     for (int i = 0; i < distanceMeanSize; i++){
                         result[i+0*distanceMeanSize] = variance[i];
                         result[i+1*distanceMeanSize] = entropy[i];
                         result[i+2*distanceMeanSize] = uniformity[i];
                         result[i+3*distanceMeanSize] = homogeneity[i];
                         result[i+4*distanceMeanSize] = moment3th[i];
                         result[i+5*distanceMeanSize] = reverseVariance[i];
                     }
                 }else{
                     int angleMeanSize = varianceMatrix->getAngleMeanSize();
                     int sizeVector = 6*angleMeanSize;

                     result = new float[sizeVector];
                     float* variance = varianceMatrix->getAngleMeanFeatures();
                     float* entropy = entropyMatrix->getAngleMeanFeatures();
                     float* uniformity = uniformityMatrix->getAngleMeanFeatures();
                     float* homogeneity = homogeneityMatrix->getAngleMeanFeatures();
                     float* moment3th = moment3thMatrix->getAngleMeanFeatures();
                     float* reverseVariance = reverseVarianceMatrix->getAngleMeanFeatures();
            
                     for (int i = 0; i < angleMeanSize; i++){
                         result[i+0*angleMeanSize] = variance[i];
                         result[i+1*angleMeanSize] = entropy[i];
                         result[i+2*angleMeanSize] = uniformity[i];
                         result[i+3*angleMeanSize] = homogeneity[i];
                         result[i+4*angleMeanSize] = moment3th[i];
                         result[i+5*angleMeanSize] = reverseVariance[i];
                     }
                 }
             };
             return result;
             
          }
          
          
          /**
          * Calculus to obtain the signature, based on selected characteristics.
          *
          * @param *image                 The image what do you want extract the signature.
          *
          * @throw NotAvaliableException  If the image is not avaliable on gray scale.
          * @throw FullHeapException      If the memory space is not avaliable.
          *
          * @return The signature of the image.
          */
         /**
          * Generates the Haralick's descriptor defined in the template from the
          * image provided.
          *
          * @param image The image to be processed.
          * @param[out] sign The object to store the generated normalized histogram.
          * @return The resulting ExtReturnCode.
          */
          virtual ExtractorReturnCode generateSignature(const DataObjectType & image, SignatureType & sign) {

//          virtual ObjectSignature * getObjectSignature(Image & image) throw (NotAvaliableException*, FullHeapException*){
    
             try{
                 createCoocurrenceMatrix();
             } catch(...) {
                 throw new FullHeapException(1, "Error into allocate memory size", 0);
             }
    
             //
             // The original image are not affected
             //
             loadImageValues(image);

             for(int distance = 0; distance< getNumDistances(); distance++){
                   for(int angle = 0; angle < getNumAngles(); angle++){
                
                        // Sum without princ diag
                        for(int line = 0; line < getNumBins()-1; line++){
                              for(int column = (line+1); column < getNumBins(); column++){
                     
                                      varianceMatrix->setHaralickFeatureValue(distance, angle, 
                                                                              varianceMatrix->getHaralickFeatureValue(distance, angle) + 
                                                                              2*(cm[distance][angle][line][column]*(line-column)*(line-column)));   
                                      if(cm[distance][angle][line][column] != 0 ){
                                          entropyMatrix->setHaralickFeatureValue(distance, angle,
                                                                                 entropyMatrix->getHaralickFeatureValue(distance, angle) -
                                                                                 2*(cm[distance][angle][line][column]*log10(cm[distance][angle][line][column])));           
                                      }
                                      uniformityMatrix->setHaralickFeatureValue(distance, angle,
                                                                                uniformityMatrix->getHaralickFeatureValue(distance, angle) +   
                                                                                2*(cm[distance][angle][line][column]*cm[distance][angle][line][column]));      
                                      homogeneityMatrix->setHaralickFeatureValue(distance, angle,
                                                                                 homogeneityMatrix->getHaralickFeatureValue(distance, angle) + 
                                                                                 2*(cm[distance][angle][line][column]/(1+abs(line-column))));         
                                      moment3thMatrix->setHaralickFeatureValue(distance, angle,
                                                                               moment3thMatrix->getHaralickFeatureValue(distance, angle) + 
                                                                               2*(cm[distance][angle][line][column]*(line-column)*(line-column)*abs(line-column)));
                                      reverseVarianceMatrix->setHaralickFeatureValue(distance, angle,
                                                                                     reverseVarianceMatrix->getHaralickFeatureValue(distance, angle) + 
                                                                                     2*(cm[distance][angle][line][column]/((line-column)*(line-column))));
                              }                                                                   
                        }
            
                        // Sum diag princ
                        for(int line = 0; line < getNumBins(); line++){
                             if(cm[distance][angle][line][line] != 0 ){
                                     entropyMatrix->setHaralickFeatureValue(distance, angle,
                                                                            entropyMatrix->getHaralickFeatureValue(distance, angle) - 
                                                                            cm[distance][angle][line][line]*log10(cm[distance][angle][line][line])); 
                             }
                             uniformityMatrix->setHaralickFeatureValue(distance, angle,
                                                                       uniformityMatrix->getHaralickFeatureValue(distance, angle) +   
                                                                       cm[distance][angle][line][line]*cm[distance][angle][line][line]);        
                             homogeneityMatrix->setHaralickFeatureValue(distance, angle,
                                                                        homogeneityMatrix->getHaralickFeatureValue(distance, angle) + 
                                                                        cm[distance][angle][line][line]);         
                        }
                   }
             }
             
             int size = 0;
             if (getTypeHaralickExtractor() == 0){
                   size = varianceMatrix->getDistanceMeanSize();
             }else{
                   size = varianceMatrix->getAngleMeanSize();
             }
             if(getTypeCharacteristics() == ALL_CHARACTERISTICS){
                 size = 6*size;
             }

             //Is it necessary to adjust signature size?
             if (size != sign.GetSize()) {
                sign.SetSize(size);
             }

             //Populate the sign object with the features computed
             for (stCount i = 0; i < size; i++) {
                 sign.Set(i, getSelectedSignature()[i]);
             }

//             newSignature->SetData(getSelectedSignature());
             
             return EXTR_SUCCESS;
          };
          
};

#endif

