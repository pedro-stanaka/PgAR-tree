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
#ifndef HARALICKFEATURE_HPP
#define HARALICKFEATURE_HPP


#include <iostream>
#include <gbdi/util/ArtemisException.hpp>
#include <gbdi/image/ImageBase.hpp>

using namespace std;
using namespace artemis;


/**
* This is single class that describes a coocurrence matrix with
* Haralick features. For each pair (distance, angle) six Haralick
* characteristics are extracted.
*
* <P> The co-ocurrence matrix are based o texture parameters. </P>
*
* @brief Generic co-ocurrence Matrix
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @see ImageBase
* @see ImageException
* @see HaralickFeature.cpp
* @version 1.0
*/
class HaralickFeaturesMatrix {

    private:
        int numDistances;
        int numAngles;
        
        float **featuresMatrix; //[NUM_ANGULOS][NUM_DISTANCIA];
        float *distanceMeanFeatures;
        float *angleMeanFeatures;
    
    private:
        void createFeaturesMatrix() throw (FullHeapException*);
        void createDistanceMeanVector() throw (FullHeapException*);
        void createAngleMeanVector() throw (FullHeapException*);
        
        void deleteFeaturesMatrix();
        void deleteDistanceMeanVector();
        void deleteAngleMeanVector();
        
    public:
        HaralickFeaturesMatrix(int numDistances = 5, int numAngles = 4){
            setNumAngles(numAngles);
            setNumDistances(numDistances);
            createFeaturesMatrix();
            createDistanceMeanVector();
            createAngleMeanVector();             
        }
        
        ~HaralickFeaturesMatrix() throw();
        
        void setHaralickFeatureValue(int distance, int angle, float value) throw (OutOfBoundsException*);
        void setDistanceMeanValue(int pos, float value) throw (OutOfBoundsException*);
        void setAngleMeanValue(int pos, float value) throw (OutOfBoundsException*);
        void setNumAngles(int numAngles);
        void setNumDistances(int numDistances);
        
        float getHaralickFeatureValue(int distance, int angle) throw (OutOfBoundsException*);
        float getDistanceMeanValue(int pos) throw (OutOfBoundsException*);
        float getAngleMeanValue(int pos) throw (OutOfBoundsException*);
        int getNumAngles();
        int getNumDistances();
        int getDistanceMeanSize();
        int getAngleMeanSize();
        
        float* getDistanceMeanFeatures();
        float* getAngleMeanFeatures();

        HaralickFeaturesMatrix* clone();
        bool isEqual(HaralickFeaturesMatrix *h);
};

#endif
