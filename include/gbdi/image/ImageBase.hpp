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
 * This file defines a base class to manipulate images.
 *
 * @version 1.0
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @date 11-01-2010
 */
#ifndef IMAGEBASE_HPP
#define IMAGEBASE_HPP

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>

#include <gbdi/util/ArtemisException.hpp>
#include <gbdi/util/stException.h>
#include <gbdi/image/Pixel.hpp>

using namespace std;
using namespace artemis;

/**
 * This is a base class to manipulate images.
 * Its structures are also used by the feature extractors provided by the
 * Image Extractors library.
 *
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @version 1.0
 * @see Pixel
 */
class Image {
private:
    /**
     * Image file name.
     */
    string filename;

    /**
     * Image width.
     */
    unsigned short width; //lg x

    /**
     * Image height.
     */
    unsigned short height; //alt y

    /**
     * Number of channels.
     */
    unsigned short nchannels;

    /**
     * Image identificator.
     */
    long int imageID;

    /**
     * Pixel matrix.
     */
    Pixel **pixel;

    /**
     * Image depth.
     */
    unsigned short bitsPerPixel;

public:
    /**
     * Default class constructor.
     */
    Image();

    /**
     * Copy class constructor.
     *
     * @param i The Image object to be copied.
     */
    Image(const Image & i);

    /**
     * Class destructor.
     */
    virtual ~Image();

    /**
     * Sets the image file name.
     *
     * @param filename Name of the associated file.
     */
    void setFilename(string filename);

    /**
     * Sets the identificator of the image.
     *
     * @param imageID The ID of the image.
     */
    void setImageID(long int imageID);

    /**
     * Sets the number of channels.
     *
     * @param nchannels The number of channels.
     */
    void setChannels(unsigned short nchannels);

    /**
     * Sets the values of a pixel in the pixel matrix copying them from a Pixel object.
     *
     * @param x Position x of the pixel.
     * @param y Position y of the pixel.
     * @param pixel The Pixel object from which the values will be copied.
     * @exception out_of_range If the position (x, y) is out of the image bounds.
     * @exception logic_error If the pixel matrix is NULL.
     */
    void setPixel(unsigned short x, unsigned short y, const Pixel & pixel);

    /**
     * Sets the image depth.
     *
     * @param bitsPerPixel The number of bits to describe the image pixels.
     */
    void setBitsPerPixel(unsigned short bitsPerPixel);

    /**
     * Gets the image size.
     *
     * @return The image size.
     * @see createPixelMatrix
     */
    unsigned int getSize() const;

    /**
     * Gets the name of the associed file.
     *
     * @return The name of the associated file.
     */
    string getFilename() const;

    /**
     * Gets the image width.
     *
     * @return The image width.
     * @see createPixelMatrix
     */
    unsigned short getWidth() const;

    /**
     * Gets the image height.
     *
     * @return The image height.
     * @see createPixelMatrix
     */
    unsigned short getHeight() const;

    /**
     * Gets the image identificator.
     *
     * @return The ID of the Image.
     */
    long int getImageID() const;

    /**
     * Gets the number of channels.
     *
     * @return The number of channels.
     */
    unsigned short getChannels() const;

    /**
     * Gets a reference to the pixel in the position x, y in the pixel matrix.
     * NOTE: The returned pixel cannot be modified. If it is desired to
     * modify the pixel values, it is necessary to clone it, perform the
     * modification in the cloned one an update the image pixel using the
     * setPixel(x, y, clonedPixel) method.
     *
     * @param x The position x of the pixel.
     * @param y The position y of the pixel.
     * @exception out_of_range If the position (x, y) is out of the image bounds.
     * @exception logic_error If the pixel matrix is NULL.
     */
    const Pixel & getPixel(unsigned short x, unsigned short y) const;

    /**
     * Gets the image depth.
     *
     * @return The number of bits to describe the image pixels.
     */
    unsigned short getBitsPerPixel() const;

    /**
     * Creates a dynamic matrix of pixels.
     * NOTE: The matrix starts on (0, 0).
     *
     * @param width The width of the matrix.
     * @param height The height of the matrix.
     * @throw bad_alloc Error allocating memory.
     */
    void createPixelMatrix(unsigned short width, unsigned short height);

    /**
     * Frees the allocated memory for the pixel matrix.
     */
    void deletePixelMatrix();

    /**
     * Clones the current Image.
     *
     * @return An image clone.
     * @warning The caller of this method is responsible to dispose the returned object.
     */
    Image * clone() const;

    /**
     * Compares if two images are equal.
     *
     * @param i The image to be compared to this instance.
     * @return True if the images have the same dimensions and if all corresponding
     * pixels are equal; or false otherwise.
     */
    bool isEqual(const Image & i) const;
};//end class Image


#endif



