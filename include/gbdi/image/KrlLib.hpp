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
* This file contains a class to manipulate KRL Images. KRL images are
* scanned medical images with a little header.
*
* This header is represented here by a struct, but your attributes are
* relationed only with the image.
*
* @version 1.0
* @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
* @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
* @date 11-01-2010
*/
#ifndef KRLLIB_HPP
#define KRLLIB_HPP

#include <gbdi/image/ImageBase.hpp>
#include <gbdi/util/FileHandler.hpp>
#include <gbdi/util/ArtemisException.hpp>

/**
* This struct is a header for KRL images.
*
* @brief Header for KRL Images.
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @version 1.0
*/
struct KrlHeaderType {
       
    unsigned short headerType;
    char filename[32];
    unsigned short width;
    unsigned short height;
    char pixBytes;
    char pixBits;
    char OS;
    char compress;
    unsigned short compParas[5];
    char hdtime[32];
    char imgType;
    char black;
    unsigned short pixSize;
    unsigned short sampType;
    char optDensity;
    char dayNo[13];
    long medHistNo;
    char studyType[10];
    char patientName[50];
    char special[216];
    char comment[128];
};

/**
* @brief Handler Class to KRL Images
*
* @author Marcelo Ponciano da Silva
* @author Marcos Vinicius Naves Bedo
* @see ImageBase
* @see ImageException
* @see krlsample.cpp
* @version 1.0
*/
class KRLImage : public Image{

    private:            
        KrlHeaderType header;
        
    public:
        KRLImage();
        KRLImage(string filename);
        ~KRLImage();
        
        void openImage(string filename) throw (artemis::FileException*, artemis::FullHeapException*);
        
        KRLImage* clone();
        bool isEqual(KRLImage *k);
};

#endif
