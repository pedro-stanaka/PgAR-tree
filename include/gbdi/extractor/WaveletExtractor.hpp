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
* This file defines the extractor for Daubechies and Haar Wavelects Extractor.
*
* @version 1.0
* @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
* @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
* @date 18-08-2010
*/
#ifndef WAVELETEXTRACTOR_HPP
#define WAVELETEXTRACTOR_HPP

#include <iostream>
#include <vector>
#include <gbdi/extractor/Extractor.hpp>

using namespace std;
using namespace artemis;

/**
* This is a Template-Class. A template-parameter levels defines the levels
* utilized on the Wavelet. The template parameter vectorComposition defines
* a type of signature what will be utilized.
*
* @brief Basic of Wavelet Extractor
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @see ImageBase
* @see ImageException
* @version 1.0
*/
template<class ObjectSignature, int levels = 1, int vectorComposition = 4>
class WaveletExtractor : public Extractor<ObjectSignature>{

    private:
        vector<float> mean;
        vector<float> entropy;
        vector<float> energy;
        vector<float> variance;
        
        Image *multiSquare;

    protected:

        /**
        * Mount the full signature based on wanted characteristics.
        *
        * @param *image The image trated by Wavelets
        *
        * @return The wanted signature.
        */
        ObjectSignature *extractSignature(Image *image){
            int sumPixels;
            int x0, x1, y0, y1;
            int i = 0;

            for(int level = 1; level <= levels; level++){
                for(int width = 1; width <= 3; width++){
                    switch(width){
                    case 1 :
                        y0 = 0;
                        y1 = (image->getHeight()/(2*level));
                        x0 = (image->getWidth()/(2*level));
                        x1 = (image->getWidth()/level);
                        break;
                    case 2 :
                        y0 = (image->getHeight()/(2*level));
                        y1 = (image->getHeight()/(level));
                        x0 = (image->getWidth()/(2*level));
                        x1 = (image->getWidth()/level);
                        break;
                    case 3 :
                        y0 = (image->getHeight()/(2*level));
                        y1 = (image->getHeight()/(level));
                        x0 = 0;
                        x1 = (image->getWidth()/(2*level));
                        break;
                    }

                    sumPixels = 0;
                    mean.push_back(0);
                    entropy.push_back(0);
                    energy.push_back(0);
                    variance.push_back(0);

                    for(int y = y0; y < y1; y++){
                        for(int x = x0; x < x1; x++){
                            if(image->getPixel(x, y).getGrayPixelValue()!= 0){
                                entropy[i] = entropy[i] + (image->getPixel(x, y).getGrayPixelValue())*log(image->getPixel(x, y).getGrayPixelValue())/log(2);
                            }
                            energy[i] = energy[i] + image->getPixel(x, y).getGrayPixelValue()*image->getPixel(x, y).getGrayPixelValue();
                            mean[i] = mean[i] + image->getPixel(x, y).getGrayPixelValue();
                            variance[i] = variance[i] + (image->getPixel(x, y).getGrayPixelValue()-mean[i])*(image->getPixel(x, y).getGrayPixelValue()-mean[i]);
                            sumPixels++;
                        }
                    }

                    mean[i] = mean[i]/sumPixels;
                    entropy[i] = entropy[i]/sumPixels;
                    energy[i] = energy[i]/sumPixels;
                    variance[i] = variance[i]/sumPixels;

                    i++;
                }

                if (level == (levels)){
                    y0 = 0;
                    y1 = (image->getHeight()/(2*level));
                    x0 = 0;
                    x1 = (image->getWidth()/(2*level));

                    sumPixels = 0;
                    mean.push_back(0);
                    entropy.push_back(0);
                    energy.push_back(0);
                    variance.push_back(0);
                    for(int y = y0; y < y1; y++){
                        for(int x = x0; x < x1; x++){
                            if(image->getPixel(x, y).getGrayPixelValue() != 0){
                                entropy[i] = entropy[i] + (image->getPixel(x, y).getGrayPixelValue())*log(image->getPixel(x, y).getGrayPixelValue())/log(2);
                            }
                            energy[i] = energy[i] + image->getPixel(x, y).getGrayPixelValue()*image->getPixel(x, y).getGrayPixelValue();
                            mean[i] = mean[i] + image->getPixel(x, y).getGrayPixelValue();
                            variance[i] = variance[i] + (image->getPixel(x, y).getGrayPixelValue()-mean[i])*(image->getPixel(x, y).getGrayPixelValue()-mean[i]);
                            sumPixels++;
                        }
                    }
                    mean[i] = mean[i]/sumPixels;
                    entropy[i] = entropy[i]/sumPixels;
                    energy[i] = energy[i]/sumPixels;
                    variance[i] = variance[i]/sumPixels;
                    i++;

                }
            }
            float *result = new float[(mean.size() + entropy.size() + energy.size() + variance.size())];

            for (int j = 0; j < mean.size(); j++){
                result[j] = mean[j];
            }

            for (int j = 0; j < entropy.size(); j++){
                result[(j + mean.size())] = entropy[j];
            }

            for (int j = 0; j < energy.size(); j++){
                result[(j + mean.size() + entropy.size())] = energy[j];
            }

            for (int j = 0; j < variance.size(); j++){
                result[(j + mean.size() + entropy.size() + energy.size() )] = variance[j];
            }

            delete(image);

            ObjectSignature *newSignature = new ObjectSignature(mean.size() + entropy.size() + energy.size() + variance.size(), NULL);
            newSignature->SetOID(image->getImageID());
            newSignature->SetData(result);

            mean.clear();
            entropy.clear();
            energy.clear();
            variance.clear();

            return newSignature;
       }

    public:

        /**
        * Constructor.
        */
        WaveletExtractor(){

            mean.clear();
            entropy.clear();
            energy.clear();
            variance.clear();
        }

        /**
        * Destructor.
        */
        ~WaveletExtractor(){
            clearFeatures();
        }

        /**
        * Clear the characteristics vector.
        */
        void clearFeatures(){

            mean.clear();
            entropy.clear();
            energy.clear();
            variance.clear();
        }

        /**
        * Normalize the imagens between 0 and 1.
        * It's needed for each new square, delimited by (x0, y0) and (x1, y1)
        *
        * @param image: A pointer to divided image
        * @param x0: Start position x of the square
        * @param x1: End position x of the square
        * @param y0: Start position y of the square
        * @param y1: End position y of the square
        * @throw OutOfBoundsException: If the coordinates (x0, y0) e (x1, y1) are incorrects.
        */
        void NormalizeImage(Image *image, int x0, int x1, int y0, int y1) throw (artemis::OutOfBoundsException*){

            if ((x0 < 0) || (x1 >= image->getWidth()) || (y0 < 0) || (y1 >= image->getHeight())){
                throw new artemis::OutOfBoundsException(4, "Incorret Values", x0, y0);
            }

            float maxValue = 0;
            float minValue = 99999999999.0;

            for(int y = y0; y < y1; y++){
                for(int x = x0; x < x1; x++){
                    if(maxValue < image->getPixel(x, y).getGrayPixelValue()){
                        maxValue = image->getPixel(x, y).getGrayPixelValue();
                    }
                    if(minValue > image->getPixel(x, y).getGrayPixelValue()){
                        minValue = image->getPixel(x, y).getGrayPixelValue();
                    }
                }
            }

            float dif = maxValue - minValue;

            //Is another scale depending of the max and the min values of the square. The difference between
            //the max and the min creates the rule.
            for(int y = y0; y < y1; y++){
                for(int x = x0; x < x1; x++){
                    image->setPixel(x, y, abs(255*fabs((image->getPixel(x, y).getGrayPixelValue() - minValue))/dif));
                }
            }
        }
        
        /**
        * Sets a image what will be processed.
        *
        * @param *image  The image what will be cloned.
        */
        void setMultiSquareImage(Image *image){
        
            multiSquare = image->clone();
        }

        /**
        * Gets a MeanFeatures vector.
        *
        * @return The mean features vector.
        */
        vector<float> getMeanFeatures(){

            return mean;
        }

        /**
        * Gets a EntropyFeatures vector.
        *
        * @return The entropy features vector.
        */
        vector<float> getEntropyFeatures(){

            return entropy;
        }

        /**
        * Gets a EnergyFeatures vector.
        *
        * @return The energy features vector.
        */
        vector<float> getEnergyFeatures(){

            return energy;
        }

        /**
        * Gets a VarianceFeatures vector.
        *
        * @return The variance features vector.
        */
        vector<float> getVarianceFeatures(){

            return variance;
        }
        
        /**
        * Gets the multi squared image.
        *
        * @return The processed image.
        */
        Image* getMultiSquareImage(){
        
            return multiSquare;
        }

        /**
        * Gets the number of levels.
        *
        * @return The number of levels.
        */
        int getLevels(){

            return levels;
        }

        /**
        * Gets the composition of the signature.
        *
        * @return The composition of the signature.
        */
        int getComposition(){

            return vectorComposition;
        }
};


/**
* This is a Template-Class. A template-parameter levels defines the levels
* utilized on the Wavelet. The template parameter vectorComposition defines
* a type of signature what will be utilized.
*
* @brief Implementation of Haar Wavelet Extractor
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @see ImageBase
* @see ImageException
* @version 1.0
*/
template<class ObjectSignature, int levels = 1, int vectorComposition = 4>
class HaarWaveletExtractor : public WaveletExtractor<ObjectSignature, levels, vectorComposition>{

    public:
        /**
        * Constructor.
        */
        HaarWaveletExtractor(){
        }

        /**
        * Destructor.
        */
        ~HaarWaveletExtractor(){
        }

        /**
        * Extract a signature based on Haar Wavelet Theory of the image.
        *
        * @param image The image what you wants a signature.
        * @return The wanted signature.
        */
        virtual ObjectSignature *getObjectSignature(Image *img)throw (Exception*){

            Image* image = img->clone();
            Image* aux = image->clone();
            Image* aux2 = image->clone();

            int widthFull, heightFull;
            int width  = image->getWidth();
            int height   = image->getHeight();

            Pixel *p;
            for(int level = 0; level < levels; level++){

                widthFull = width;
                heightFull  = height;
                width  = (int) width/2;
                height   = (int) height/2;

                //
                // Makes the the addition or subtration in the y axis.
                // Use the Haar theory to generate Pixel Gray Scale Value
                //
                for(int y = 0; y < heightFull; y++){
                    for(int x = 0, low = 0, high = width; x < widthFull; x = x+2, low++, high++){
                        if(x != (widthFull-1)){
                            p = new Pixel();
                            p->setGrayPixelValue(((image->getPixel(x, y).getGrayPixelValue() +
                                                    image->getPixel(x+1, y).getGrayPixelValue())*sqrt(2)/2));
                            aux->setPixel(low, y, p);
                            delete(p);

                            p = new Pixel();
                            p->setGrayPixelValue((image->getPixel(x, y).getGrayPixelValue() -
                                                     image->getPixel(x+1, y).getGrayPixelValue())*sqrt(2)/2);
                            aux->setPixel(high, y, p);
                            delete(p);
                        }else{
                            p = new Pixel;
                            p->setGrayPixelValue((image->getPixel(x, y).getGrayPixelValue() -
                                                     image->getPixel(x, y).getGrayPixelValue())*sqrt(2)/2);
                            aux->setPixel(high, y, p);
                            delete(p);
                        }
                    }
                }

                //
                // Makes the the addition or subtration in the x axis.
                // Use the Haar theory to generate Pixel Gray Scale Value
                //
                for(int x = 0; x < widthFull; x++){
                    for(int y = 0, low = 0, high = height; y < heightFull; y = y+2, low++, high++){
                        if(y != (heightFull-1)){
                            p = new Pixel();
                            p->setGrayPixelValue(((aux->getPixel(x, y).getGrayPixelValue() +
                                                     aux->getPixel(x, y+1).getGrayPixelValue())*sqrt(2)/2));
                            aux2->setPixel(x, low, p);
                            delete(p);

                            p = new Pixel();
                            p->setGrayPixelValue(((aux->getPixel(x, y).getGrayPixelValue() -
                                                   aux->getPixel(x, y+1).getGrayPixelValue())*sqrt(2)/2));
                            aux2->setPixel(x, high, p);
                            delete(p);

                        }else{
                            p = new Pixel();
                            p->setGrayPixelValue(((aux->getPixel(x, y).getGrayPixelValue() - aux->getPixel(x, y).getGrayPixelValue())*sqrt(2)/2));
                            image->setPixel(x, high, p);
                            delete(p);

                        }
                    }
                }
            }

            delete(aux);
            delete(aux2);
            return this->extractSignature(image);
        }
};

/**
* This is a Template-Class. A template-parameter levels defines the levels
* utilized on the Wavelet. The template parameter vectorComposition defines
* a type of signature what will be utilized.
*
* @brief Implementation of Daubechies Wavelet Extractor
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @see ImageBase
* @see ImageException
* @version 1.0
*/
template<class ObjectSignature, int levels = 1, int vectorComposition = 4>
class DaubechiesWaveletExtractor: public WaveletExtractor<ObjectSignature, levels, vectorComposition>{

    public:
        /**
        * Constructor.
        */
        DaubechiesWaveletExtractor(){
        }

        /**
        * Destructor.
        */
        ~DaubechiesWaveletExtractor(){
        }

        /**
        * Extract a signature based on Daubechies Wavelet Theory of the image.
        *
        * @param image The image what you wants a signature.
        * @return The wanted signature.
        */
        virtual ObjectSignature *getObjectSignature(Image *img) throw (Exception*){

            Image* image = img->clone();
            Image* aux = image->clone();
            Image* aux2 = image->clone();

            int widthFull, heightFull;
            int width  = image->getWidth();
            int height   = image->getHeight();

            for(int level = 0; level < levels; level++){
                widthFull = width;
                heightFull  = height;
                width  = (int)width/2;
                height   = (int)height/2;

                Pixel *p = new Pixel();

                //
                // Makes the the addition or subtration in the x axis.
                // Use the Daubechies theory to generate Pixel Gray Scale Value
                //
                for(int y = 0; y < heightFull; y++){
                    p = new Pixel();
                    p->setGrayPixelValue((image->getPixel(widthFull-2, y).getGrayPixelValue()*(1+sqrt(3)) +
                                         image->getPixel(widthFull-1, y).getGrayPixelValue()*(3+sqrt(3)) +
                                         image->getPixel(0, y).getGrayPixelValue()*(3-sqrt(3)) +
                                         image->getPixel(1, y).getGrayPixelValue()*(1-sqrt(3)))*sqrt(2)/8);
                    aux->setPixel(0, y, p);
                    delete(p);

                    p = new Pixel();
                    p->setGrayPixelValue((image->getPixel(widthFull-2, y).getGrayPixelValue()*(1-sqrt(3)) +
                                          image->getPixel(widthFull-1, y).getGrayPixelValue()*(sqrt(3)-3) +
                                          image->getPixel(0, y).getGrayPixelValue()*(3+sqrt(3))          -
                                          image->getPixel(1, y).getGrayPixelValue()*(1+sqrt(3)))*sqrt(2)/8);
                    aux->setPixel(width, y, p);
                    delete(p);

                    for(int x = 2, low = 1, high = width+1; x < widthFull;
                        x=x+2, low++, high++){
                        if(x != (widthFull-1)){

                            p = new Pixel();
                            p->setGrayPixelValue(((image->getPixel(x-2, y).getGrayPixelValue()*(1+sqrt(3)) +
                                                   image->getPixel(x-1, y).getGrayPixelValue()*(3+sqrt(3)) +
                                                   image->getPixel(x, y).getGrayPixelValue()*(3-sqrt(3))   +
                                                   image->getPixel(x+1, y).getGrayPixelValue()*(1-sqrt(3)))*sqrt(2)/8));
                            aux->setPixel(low, y, p);
                            delete(p);

                            p = new Pixel();
                            p->setGrayPixelValue(((image->getPixel(x-2, y).getGrayPixelValue()*(1-sqrt(3)) +
                                                   image->getPixel(x-1, y).getGrayPixelValue()*(sqrt(3)-3) +
                                                   image->getPixel(x, y).getGrayPixelValue()*(3+sqrt(3))   -
                                                   image->getPixel(x+1, y).getGrayPixelValue()*(1+sqrt(3)))*sqrt(2)/(8) ));
                            aux->setPixel(high, y, p);
                            delete(p);

                        }else{

                            p = new Pixel();
                            p->setGrayPixelValue(((image->getPixel(x-2, y).getGrayPixelValue()*(1-sqrt(3)) +
                                                   image->getPixel(x-1, y).getGrayPixelValue()*(sqrt(3)-3) +
                                                   image->getPixel(x, y).getGrayPixelValue()*(3+sqrt(3))   -
                                                   image->getPixel(0, y).getGrayPixelValue()*(1+sqrt(3)))*sqrt(2)/(8)));
                            aux->setPixel(high, y, p);
                            delete(p);
                        }
                    }
                }

                for(int x=0; x<widthFull; x++){

                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, heightFull-2).getGrayPixelValue()*(1+sqrt(3)) +
                                           aux->getPixel(x, heightFull-1).getGrayPixelValue()*(3+sqrt(3)) +
                                           aux->getPixel(x,0).getGrayPixelValue()*(3-sqrt(3))         +
                                           aux->getPixel(x, 1).getGrayPixelValue()*(1-sqrt(3)))*sqrt(2)/8));
                    image->setPixel(x, 0, p);
                    delete(p);


                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, heightFull-2).getGrayPixelValue()*(1-sqrt(3)) +
                                           aux->getPixel(x, heightFull-1).getGrayPixelValue()*(sqrt(3)-3) +
                                           aux->getPixel(x, 0).getGrayPixelValue()*(3+sqrt(3))         -
                                           aux->getPixel(x, 1).getGrayPixelValue()*(1+sqrt(3)))*sqrt(2)/8));
                    image->setPixel(x, height, p);
                    delete(p);

                    for (int y = 2, low = 1, high = height+1; y < heightFull; y=y+2, low++, high++){
                        if(y != (heightFull-1)){

                            p = new Pixel();
                            p->setGrayPixelValue(((aux->getPixel(x, y-2).getGrayPixelValue()*(1+sqrt(3)) +
                                                   aux->getPixel(x, y-1).getGrayPixelValue()*(3+sqrt(3)) +
                                                   aux->getPixel(x, y).getGrayPixelValue()*(3-sqrt(3))   +
                                                   aux->getPixel(x, y+1).getGrayPixelValue()*(1-sqrt(3)))*sqrt(2)/8));
                            image->setPixel(x, low, p);
                            delete(p);

                            p = new Pixel();
                            p->setGrayPixelValue(((aux->getPixel(x, y-2).getGrayPixelValue()*(1-sqrt(3)) +
                                                   aux->getPixel(x, y-1).getGrayPixelValue()*(sqrt(3)-3) +
                                                   aux->getPixel(x, y).getGrayPixelValue()*(3+sqrt(3))   -
                                                   aux->getPixel(x,y+1).getGrayPixelValue()*(1+sqrt(3)))*sqrt(2)/8));
                            image->setPixel(x, high, p);
                            delete(p);

                        }else{

                            p = new Pixel();
                            p->setGrayPixelValue(((aux->getPixel(x, y-2).getGrayPixelValue()*(1-sqrt(3)) +
                                                   aux->getPixel(x, y-1).getGrayPixelValue()*(sqrt(3)-3) +
                                                   aux->getPixel(x, y).getGrayPixelValue()*(3+sqrt(3))   -
                                                   aux->getPixel(x, 0).getGrayPixelValue()*(1+sqrt(3)))*sqrt(2)/8));
                            image->setPixel(x, high, p);
                            delete(p);
                        }
                    }
                }
            }
            delete(aux);
            delete(aux2);
            return this->extractSignature(image);
        }
};

#endif
