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
 * This file defines the basic interfaces for feature extractors.
 *
 * @version 1.0
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @date 11-01-2010
 */
#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#include <iostream>
#include <cstdlib>

#include <gbdi/datatype/stTypes.h>

/**
 * Return codes from extractor functions.
 */
enum ExtractorReturnCode {
    /* Codes from 0-999 are general to every extractor */
    EXTR_SUCCESS = 0,

    /* Codes from 1000-1999 refer to image extractors */
    EXTR_SIGNATURE_SIZE_MISMATCH = 1001, /* The ObjectSignature size did not match with the required feature vector size */
    EXTR_OUT_OF_BINS = 1002, /* The number of bins required for the histogram exceeded the number of colors of the image */
};

/**
 *
 * This class is the basic interface for feature extractors. It depends on
 * datatypes for handling source data (e.g. Image) and datatypes for
 * storing the extracted signature (e.g. stBasicArrayObject).
 *
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @version 1.0
 */
template< class SignatureType, class DataObjectType >
class Extractor {
public:

    /**
     * Builds a new uninitialized instance of this class.
     *
     * @note This constructor is required to fulfil this interface.
     */
    Extractor() {
    };

    /** 
     * Class destructor.
     */
    virtual ~Extractor() {
    };

    /**
     * This is a pure virtual method to trigger the signature generation of the
     * feature extractors in the library.
     *
     * @param dataObj The data object to be processed.
     * @param[out] sign The object to store the generated signature.
     * @return The resulting ExtReturnCode.
     */
    virtual ExtractorReturnCode generateSignature(const DataObjectType & dataObj, SignatureType & sign) = 0;

};

#endif

