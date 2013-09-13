/**
 * @file
 *
 * This file contains the implementation of the Pixel class.
 */

#include <gbdi/image/Pixel.hpp>

//------------------------------------------------------------------------------

Pixel::Pixel() {
    r = 0;
    g = 0;
    b = 0;
    alpha = 0;
    grayPixel = 0;
    bw = 0;
}

//------------------------------------------------------------------------------

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b) {
    setRGBPixelValue(r, g, b);
    alpha = 0;
}

//------------------------------------------------------------------------------

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha) {
    setRGBPixelValue(r, g, b);
    this->alpha = alpha;
}

//------------------------------------------------------------------------------

Pixel::Pixel(float grayValue, unsigned short grayLevels) {
    setGrayPixelValue(grayValue, grayLevels);
    alpha = 0;
}

//------------------------------------------------------------------------------

Pixel::Pixel(const Pixel & pixel) {
    r = pixel.getRedPixelValue();
    g = pixel.getGreenPixelValue();
    b = pixel.getBluePixelValue();
    grayPixel = pixel.getGrayPixelValue();
    bw = pixel.getBlackAndWhitePixelValue();
    alpha = pixel.getAlphaPixelValue();
}

//------------------------------------------------------------------------------

Pixel::~Pixel() {

}

//------------------------------------------------------------------------------

void Pixel::setRedPixelValue(unsigned char r) {
    this->r = r;
    toGrayScale();
}

//------------------------------------------------------------------------------

void Pixel::setGreenPixelValue(unsigned char g) {
    this->g = g;
    toGrayScale();
}

//------------------------------------------------------------------------------

void Pixel::setBluePixelValue(unsigned char b) {
    this->b = b;
    toGrayScale();
}

//------------------------------------------------------------------------------

void Pixel::setRGBPixelValue(unsigned char r, unsigned char g, unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
    toGrayScale();
}

//------------------------------------------------------------------------------

void Pixel::setGrayPixelValue(float grayValue, unsigned short grayLevels) {
    this->grayPixel = grayValue;
    if (grayLevels > 256) {
        r = g = b = (unsigned char) (grayValue / (grayLevels / 256));
    } else {
        r = g = b = (unsigned char) grayValue;
    }
    toBlackAndWhite(grayLevels);
}

//------------------------------------------------------------------------------

void Pixel::setAlphaPixelValue(unsigned char alpha) {

    this->alpha = alpha;
}

//------------------------------------------------------------------------------

void Pixel::setBlackAndWhitePixelValue(unsigned char bw) {
    if ((bw == 0) || (bw == 255)) {
        this->bw = bw;
    }
}

//------------------------------------------------------------------------------

void Pixel::toGrayScale() {
    /* Into 256 gray scale levels */
    setGrayPixelValue((this->getBluePixelValue()*0.114) + (this->getGreenPixelValue()*0.587) + (this->getRedPixelValue()*0.299), 256);
    toBlackAndWhite(256);
}

//------------------------------------------------------------------------------

void Pixel::toBlackAndWhite(unsigned short grayLevels) {
    if (getGrayPixelValue() > (grayLevels / 2))
        setBlackAndWhitePixelValue(255);
    else
        setBlackAndWhitePixelValue(0);
}

//------------------------------------------------------------------------------

float Pixel::getGrayPixelValue() const {

    return grayPixel;
}

//------------------------------------------------------------------------------

unsigned char Pixel::getRedPixelValue() const {

    return r;
}

//------------------------------------------------------------------------------

unsigned char Pixel::getGreenPixelValue() const {

    return g;
}

//------------------------------------------------------------------------------

unsigned char Pixel::getBluePixelValue() const {

    return b;
}

//------------------------------------------------------------------------------

unsigned char Pixel::getAlphaPixelValue() const {

    return alpha;
}

//------------------------------------------------------------------------------

unsigned char Pixel::getBlackAndWhitePixelValue() const {

    return bw;
}

//------------------------------------------------------------------------------

Pixel * Pixel::clone() const {
    return new Pixel(*this);
}

//------------------------------------------------------------------------------

bool Pixel::isEqual(const Pixel & p) const {
    return ((p.getRedPixelValue() == r) &&
            (p.getGreenPixelValue() == g) &&
            (p.getBluePixelValue() == b) &&
            (p.getGrayPixelValue() == grayPixel) &&
            (p.getBlackAndWhitePixelValue() == bw) &&
            (p.getAlphaPixelValue() == alpha));
}

//------------------------------------------------------------------------------