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
 * This file defines a Class to extract a Normalized Histogram.
 *
 * @version 1.0
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @date 11-01-2010
 */
#ifndef NORMALIZEDHISTOGRAM_HPP
#define NORMALIZEDHISTOGRAM_HPP

#include <stdlib.h>
#include <gbdi/util/stException.h>
#include <gbdi/image/ImageBase.hpp>
#include <gbdi/extractor/Extractor.hpp>

/**
 * This is a Template-Class. A template-parameter numFeatures is used
 * to define the lenght of the histogram.
 *
 * <P> A Normalized Histogram is a simple sum of all pixel frequency
 * in the Image. In this case the image must be in gray scale.
 *
 * NOTE:
 * <ul>
 * <li>DataObjectType must satisfy the interface Image;</li>
 * <li>SignatureType must be an array object satisfying interface stObject
 * (e.g. stBasicArrayObject);</li>
 * <li>numFeatures is the number of bins of the histogram (defaults to 256);</li>
 * <li>normalizationRange is the maximum value of a bin in the normalization,
 * i.e., every bin is in the range [0, normalizationRange]. It defaults to 1.<br/>
 * The normalization in the range [0, 1] means that the sum of all bin values
 * is equal to 1. The normalization in the range [0, normalizationRange] first
 * normalize the bin values in the range [0, 1] and, thereafter, divides every
 * value is divided by the maximum value found, making the maximum equals to
 * normalizationRange.</li>
 * </ul>
 *
 * @brief Extractor of Normalized Histogram
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @see Image stBasicArrayObject
 * @version 1.0
 */
template< class SignatureType,
stWord numFeatures = 256,
stWord normalizationRange = 1,
class DataObjectType = Image >
class NormalizedHistogramExtractor : public Extractor<SignatureType, DataObjectType> {
public:

    /**
     * Empty class constructor.
     */
    NormalizedHistogramExtractor() {
    }

    /**
     * Class destructor.
     */
    virtual ~NormalizedHistogramExtractor() {
    }

    /**
     * Get the number of features.
     *
     * @return The number of features.
     */
    stWord getNumFeatures() {
        return numFeatures;
    }

    /**
     * Get the normalization range, i.e. the max bin value.
     *
     * @return The normalization range.
     */
    stWord getNormalizationRange() {
        return normalizationRange;
    }

    /**
     * Generates a Normalized Histogram from the image provided.
     *
     * @param image The image to be processed.
     * @param[out] sign The object to store the generated normalized histogram.
     * @return The resulting ExtReturnCode.
     */
    virtual ExtractorReturnCode generateSignature(const DataObjectType & image, SignatureType & sign);

}; //end class NormalizedHistogram

//include implementation (template class)
#include "NormalizedHistogram.cc"

#endif
