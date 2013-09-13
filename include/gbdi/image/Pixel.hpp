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
 * This file contains the definition of the Pixel class.
 *
 * @version 1.0
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @date 11-01-2010
 */
#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>

using namespace std;

/**
 * Pixel is a basic structure for processing and visualization of the used Images.
 * The class Pixel  * supports Black And White scale, Gray Scale, RGB Scale and
 * Alpha Transparency.
 *
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @version 1.0.
 */
class Pixel {
private:
    /**
     * RGB values.
     */
    unsigned char r, g, b;

    /**
     * Alpha transparency.
     */
    unsigned char alpha;

    /**
     * Gray scale value.
     */
    float grayPixel;

    /**
     * Black and white scale value.
     */
    unsigned char bw;

public:

    /**
     * Default class constructor.
     */
    Pixel();

    /**
     * Class constructor.
     *
     * @param r: The pixel red color on RGB scale.
     * @param g: The pixel green color on RGB scale.
     * @param b: The pixel blue color on RGB scale.
     */
    Pixel(unsigned char r, unsigned char g, unsigned char b);

    /**
     * Class constructor.
     *
     * @param r: The pixel red color on RGB scale.
     * @param g: The pixel green color on RGB scale.
     * @param b: The pixel blue color on RGB scale.
     * @param alpha: The alpha-transparency value.
     */
    Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);

    /**
     * Class constructor.
     *
     * @param grayValue: The value in gray scale.
     * @param grayLevels: The number of levels in the grayscale employed.
     * @see setGrayPixelValue()
     */
    Pixel(float grayValue, unsigned short grayLevels);

    /**
     * Copy class constructor.
     *
     * @param pixel The Pixel object to be copied.
     */
    Pixel(const Pixel & pixel);

    /**
     * Class destructor.
     */
    virtual ~Pixel();

    /**
     * Sets the Red value of the pixel.
     *
     * @param r: The pixel red color in RGB scale.
     */
    void setRedPixelValue(unsigned char r);

    /**
     * Sets the Green value of the pixel.
     *
     * @param g: The pixel green color in RGB scale.
     */
    void setGreenPixelValue(unsigned char g);

    /**
     * Sets the Blue value of the pixel.
     *
     * @param b: The pixel blue color in RGB scale.
     */
    void setBluePixelValue(unsigned char b);

    /**
     * Sets the RGB values of the pixel.
     *
     * @param r: The pixel red color in RGB scale.
     * @param g: The pixel green color in RGB scale.
     * @param b: The pixel blue color in RGB scale.
     */
    void setRGBPixelValue(unsigned char r, unsigned char g, unsigned char b);

    /**
     * Sets the value of the pixel in gray scale.
     * NOTE: picture images have 8 bits per pixel depth (256 gray levels).
     * However, DICOM images usually have 12 bits per pixel (4096 gray levels),
     * thus, it is necessary to reduce the grayValue to 8 bits, using the
     * provided number of gray levels, to correctly set the RGB values.
     *
     * @param grayValue: The value in gray scale.
     * @param grayLevels: The number of levels in the grayscale employed.
     */
    void setGrayPixelValue(float grayValue, unsigned short grayLevels);

    /**
     * Sets the alpha of the Pixel.
     *
     * @param alpha The alpha transparency value of the pixel.
     */
    void setAlphaPixelValue(unsigned char alpha);

    /**
     * Sets the black and white scale value of the pixel.
     *
     * @param bw: The The value in black and white scale (0 - black; 255 - white).
     */
    void setBlackAndWhitePixelValue(unsigned char bw);

    /**
     * Sets the gray scale value of pixel based on its RGB values.
     */
    void toGrayScale();

    /**
     * Sets the black and white scale value of pixel based on its gray scale value.
     */
    void toBlackAndWhite(unsigned short grayLevels);

    /**
     * Gets the pixel on Gray Scale Value.
     *
     * @return The Gray Scale value of the Pixel.
     */
    float getGrayPixelValue() const;

    /**
     * Gets the pixel red color on RGB scale.
     *
     * @return The pixel red color on RGB scale.
     */
    unsigned char getRedPixelValue() const;

    /**
     * Gets the pixel green color on RGB scale.
     *
     * @return The pixel green color on RGB scale.
     */
    unsigned char getGreenPixelValue() const;

    /**
     * Gets the pixel blue color on RGB scale.
     *
     * @return The pixel blue color on RGB scale.
     */
    unsigned char getBluePixelValue() const;

    /**
     * Gets the Alpha Transparency of the Pixel.
     *
     * @return The alpha value of the Pixel.
     */
    unsigned char getAlphaPixelValue() const;

    /**
     * Gets the BAW value of the pixel.
     *
     * @return The BAW value of the pixel.
     */
    unsigned char getBlackAndWhitePixelValue() const;

    /**
     * Clones the current pixel.
     *
     * @return A pixel clone.
     * @warning The caller of this method is responsible to dispose the returned object.
     */
    Pixel * clone() const;

    /**
     * Compares if two pixels are equal.
     *
     * @param p The pixel to be compared to this instance.
     * @return True if all atributtes are equal, or false otherwise.
     */
    bool isEqual(const Pixel & p) const;
};//end class Pixel

#endif



