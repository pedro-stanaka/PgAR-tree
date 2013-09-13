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
* Ciï¿½ncias Matemï¿½ticas e de Computaï¿½ï¿½o, University of Sï¿½o Paulo -
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
*        de Dados e Imagens, Instituto de Ciï¿½ncias Matemï¿½ticas e de
*        Computaï¿½ï¿½o, University of Sï¿½o Paulo - Brazil (the Databases
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
#ifndef METRICHISTOGRAM_HPP
#define METRICHISTOGRAM_HPP

#include <cstdlib>
#include <cmath>
#include <gbdi/image/ImageBase.hpp>
#include <gbdi/extractor/Extractor.hpp>
#include <vector>


using namespace std;
using namespace artemis;

/**
* Define a component used to build a Metric Histogram.
* It works like a connect list.
*
* @brief Defines a component of Metric Histogram
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @version 1.0
*/
class UnitNondimensional {

    private:
        int gray;    //X Axis
        float value; //Y Axis

    public:

        UnitNondimensional();
        ~UnitNondimensional();

        void setGray(int gray);
        void setValue(float value);
        void setXYAxis(int gray, float value);

        int getGray();
        float getValue();
};

/**
* Define a structure what contain a Metric Histogram.
* Its a association of the nondimensional units.
*
* @brief Defines a component of Metric Histogram
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @version 1.0
*/
class NondimensionalHistogram{

    private:
        vector<UnitNondimensional> nondimHistogram;

    public:
        NondimensionalHistogram();
        ~NondimensionalHistogram();

        void setUnitNondimensional(UnitNondimensional unit, int pos);

        int getSize();
        UnitNondimensional getUnitNondimensional(int pos) throw (artemis::OutOfBoundsException*);

        void clearHistogram();
};

/**
* This is a template class. The template parameter numFeatures define the deepth of the image.
* The template threshTangent and threshZero define the thresholds to valide variations on traditional
* histogram. The template parameter bucketLenght define the max size of the buckets.
*
* @brief Defines the correct behavior of Metric Histogram
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @version 1.0
*/
template<class ObjectSignature, int numFeatures = 256, int threshTangent = 2, int threshZero = 1, int bucketLenght = 3>
class MetricHistogram : public Extractor<ObjectSignature> {
    private:
        vector<float> tradHistogram;

    public:
        static const float PI = 3.14;

    public:

        /**
        * Constructor.
        */
        MetricHistogram(){

            tradHistogram.clear();
        }

        /**
        * Destructor.
        */
        ~MetricHistogram(){

            tradHistogram.clear();
        }

        /**
        * Verify if the direction of the vector is bigger than a threshold.
        *
        * @param value The new value of the direction.
        *
        * @return A int what defines the direction of the vector.
        */
        int verifyDirection(float value){
            if (fabsl(value) < (threshZero*0.001)){
                return 0;
            }else if (value < 0){
                return -1;
            }else{
                return 1;
            }
        }

        /**
        * Verify if the inclination of the vector between two points is bigger than a threshold.
        *
        * @param angle 1     The angle of the first point.
        * @param angle 2     The angle of the second point.
        *
        * @return True if the difference between two angles are bigger than a threshold or false.
        */
        bool verifyTangent(float angle1, float angle2){

            double a = fabsl(atan(angle1) - atan(angle2));

            if (a > PI){
                a = 2 * PI - a ;
            }

            return (a < threshTangent);
        }

        /**
        * Gets a metric histogram.
        *
        * @param *image A image what will be extracted.
        *
        * @return A signature of the image.
        */
        virtual ObjectSignature *getObjectSignature(Image *image)throw (Exception*){

            tradHistogram.clear();

            //Reescale the image
            Image *aux = image->clone();
            for (int x = 0; x < aux->getWidth(); x++){
                for(int y = 0; y < aux->getHeight(); y++){
                    int reescaled = (((int) aux->getPixel(x, y).getGrayPixelValue())*(numFeatures/(int) pow(2,(float)aux->getBitsPerPixel())));
                    Pixel *p = new Pixel(reescaled);
                    aux->setPixel(x, y, p);
                    delete(p);
                }
            }

            //Starts the sum of the traditional histogram
            for (int i = 0; i < numFeatures; i++){
                tradHistogram.push_back(0);
            }

            //Calculates the Traditional Histogram
            for (int x = 0; x < aux->getWidth(); x++){
                for (int y = 0; y < aux->getHeight(); y++){
                    tradHistogram[(int) aux->getPixel(x, y).getGrayPixelValue()]++;
                }
            }

            //Normalizes the Histogram
            int max = 0;
            for (int i = 0; i < tradHistogram.size(); i++){
                if (tradHistogram[i] > max)
                    max = tradHistogram[i];
            }
            for (int i = 0; i < tradHistogram.size(); i++){
                tradHistogram[i] = tradHistogram[i]/max;
            }

            //Calculates Metric Histogram

            NondimensionalHistogram metricHistogram; // Temp bin vector
            int numBins = 0;                         // n Number of bins in mhisto
            int brigthness = 0;                      // b Brightness value
            double ll;                               // Left limit of h'(x)
            double rl;                               // Right limit of h'(x)
            double threshold = 5E-5;                 // Threshold number to eliminate little variations
            int dir;                                 // m (derivate)
            bool accept;                             // Accept bin

            // Add First entry if it is bigger than the th value
            while (tradHistogram[numBins] < threshold) brigthness++;

            UnitNondimensional unit;
            unit.setXYAxis(0 ,tradHistogram[brigthness]);
            metricHistogram.setUnitNondimensional(unit, numBins);
            numBins++;

            // Process entries
            for (int w = brigthness + 1 ; w < numFeatures-2; w++){
                // w < numFeatures-2 to ignore the last bin of normalized histogram
                accept = false;
                if (tradHistogram[w] < threshold){
                    brigthness++;
                }else{
                    // Left and right limits
                    ll = tradHistogram[w] - tradHistogram[w - 1];
                    rl = tradHistogram[w + 1] - tradHistogram[w];

                    // Get direction
                    dir = verifyDirection(ll);

                    // Lets evaluate the point
                    if (dir != verifyDirection(rl)){
                        // Inversion of direction - Local min or max
                        accept = true;
                    }else if (dir != 0) {
                        // Same direction. Test angent limit
                        accept = verifyTangent(ll, rl);
                    }//end if

                    if (accept){
                        // Add bin
                        unit.setXYAxis(w - brigthness ,tradHistogram[w]);
                        metricHistogram.setUnitNondimensional(unit, numBins);
                        numBins++;
                    }else{
                        if (verifyDirection(metricHistogram.getUnitNondimensional(numBins -1).getValue() -
                                            tradHistogram[w]) != 0){
                            if (((w - brigthness) - metricHistogram.getUnitNondimensional(numBins - 1).getGray()) > bucketLenght){
                                // Add bin because bucket len is too large
                                unit.setXYAxis(w - brigthness ,tradHistogram[w]);
                                metricHistogram.setUnitNondimensional(unit, numBins);
                                numBins++;
                            }//end if
                        }//end if
                    }//end if
                }//end if
            }//end for

            float *result = new float[(metricHistogram.getSize()*2)];

            for (int i = 0; i < metricHistogram.getSize(); i++){
                result[i] = metricHistogram.getUnitNondimensional(i).getGray();
                result[i + metricHistogram.getSize()] = metricHistogram.getUnitNondimensional(i).getValue();
            }

            ObjectSignature *newSignature = new ObjectSignature((metricHistogram.getSize()*2), NULL);
            newSignature->SetOID(image->getImageID());
            /*
            * @todo Verificar se o conteúdo do ponteiro é copiado no SetData da assinatura.
            */
            newSignature->SetData(result);
            delete[] result;

            return newSignature;

        }
};

#endif // METRICHISTOGRAM_HPP
