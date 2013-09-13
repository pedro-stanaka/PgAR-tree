/**
 * @file
 *
 * This file contains the implementation of the Image class.
 */

#include <gbdi/image/ImageBase.hpp>

//------------------------------------------------------------------------------

Image::Image() {

    width = 0;
    height = 0;
    filename = "NO_FILE";
    pixel = NULL;
    imageID = 0;
    nchannels = 0;
    bitsPerPixel = 0;
}

//------------------------------------------------------------------------------

Image::Image(const Image & i) {

    pixel = NULL;
    createPixelMatrix(i.getWidth(), i.getHeight());

    for (unsigned short x = 0; x < i.getWidth(); x++) {
        for (unsigned short y = 0; y < i.getHeight(); y++) {
            Pixel p(i.getPixel(x, y));
            setPixel(x, y, p);
        }
    }

    setFilename(i.getFilename());
    setImageID(i.getImageID());
    setChannels(i.getChannels());
    setBitsPerPixel(i.getBitsPerPixel());
}

//------------------------------------------------------------------------------

Image::~Image() {

    deletePixelMatrix();
}

//------------------------------------------------------------------------------

void Image::setFilename(string filename) {

    this->filename = filename;
}

//------------------------------------------------------------------------------

void Image::setImageID(long int imageID) {

    this->imageID = imageID;
}

//------------------------------------------------------------------------------

void Image::setChannels(unsigned short nchannels) {

    this->nchannels = nchannels;
}

//------------------------------------------------------------------------------

void Image::setPixel(unsigned short x, unsigned short y, const Pixel & pixel) {

    if (this->pixel != NULL) {
        if ((x >= 0) && (x < getWidth()) && (y >= 0) && (y < getHeight())) {
            /* Create a copy of pixel and store it in the pixel matrix */
            Pixel p(pixel);
            this->pixel[x][y] = p;
        } else {
            throw out_of_range("The requested pixel is out of the image bounds");
        }
    } else {
        /**
         * @todo: The exception below shold be a NullPointerException and not an OutOfBoundsException
         */
        throw logic_error("Attempted to access a pixel from a NULL pixel matrix");
    }
}

//------------------------------------------------------------------------------

void Image::setBitsPerPixel(unsigned short bitsPerPixel) {

    this->bitsPerPixel = bitsPerPixel;
}

unsigned int Image::getSize() const {

    return (width * height);
}

//------------------------------------------------------------------------------

string Image::getFilename() const {

    return filename;
}

//------------------------------------------------------------------------------

unsigned short Image::getWidth() const {

    return width;
}

//------------------------------------------------------------------------------

unsigned short Image::getHeight() const {

    return height;
}

//------------------------------------------------------------------------------

long int Image::getImageID() const {

    return imageID;
}

//------------------------------------------------------------------------------

unsigned short Image::getChannels() const {

    return nchannels;
}

//------------------------------------------------------------------------------

const Pixel & Image::getPixel(unsigned short x, unsigned short y) const {

    if (this->pixel != NULL) {
        if ((x >= 0) && (x < getWidth()) && (y >= 0) && (y < getHeight()))
            return this->pixel[x][y];
        else {
            throw out_of_range("The requested pixel is out of the image bounds");
        }
    } else {
        /**
         * @todo: The exception below shold be a NullPointerException and not an OutOfBoundsException
         */
        throw logic_error("Attempted to access a pixel from a NULL pixel matrix");
    }
}

//------------------------------------------------------------------------------

unsigned short Image::getBitsPerPixel() const {

    return bitsPerPixel;
}

//------------------------------------------------------------------------------

void Image::createPixelMatrix(unsigned short width, unsigned short height) {

    deletePixelMatrix();
    //Build the pixel matrix with dynamic values
    pixel = new Pixel*[width];
    for (unsigned short i = 0; i < width; i++)
        pixel[i] = new Pixel[height];
    this->width = width;
    this->height = height;
}

//------------------------------------------------------------------------------

void Image::deletePixelMatrix() {
    if (pixel != NULL) {
        for (unsigned short i = 0; i < getWidth(); i++) {
            delete[] pixel[i];
        }
        delete[] pixel;
        pixel = NULL;
        width = 0;
        height = 0;
    }
}

//------------------------------------------------------------------------------

Image * Image::clone() const {
    return new Image(*this);
}

//------------------------------------------------------------------------------

bool Image::isEqual(const Image & i) const {

    if (i.getHeight() != this->getHeight()) {
        return false;
    }

    if (i.getWidth() != this->getWidth()) {
        return false;
    }

    for (unsigned short x = 0; x < i.getWidth(); x++) {
        for (unsigned short y = 0; y < i.getHeight(); y++) {
            if (!(i.getPixel(x, y).isEqual(this->getPixel(x, y)))) {
                return false;
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------