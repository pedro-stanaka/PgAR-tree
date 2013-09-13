#include <gbdi/image/PngLib.hpp>

/**
* Load the PNG Image in the Pixel Matrix.
* A new matrix of Pixels are allocated and the
* PNG File are loaded in.
* NOTE: OpenCv use.
*
* @exception FullHeapException Insufficient space for memory allocation.
*/
void PNGImage::loadPixelMatrix() throw (FullHeapException*){


    //Structures of OpenCv
    CvScalar s;
    IplImage *image;

    image = cvLoadImage(getFilename().c_str(), -1);

    try{
        createPixelMatrix(getWidth(), getHeight());
    } catch (FullHeapException *he) {
        throw he;
    }


    //Converts the IpLImage OpenCVStructure to Pixel Matrix
    for(int x = 0; x < getWidth(); x++){
      for(int y = 0; y < getHeight(); y++){
          //Catch the RGB from opened Image and tranfer to the matrix of pixels
          s = cvGet2D(image, y, x);
          Pixel *aux = new Pixel((const char)s.val[2], (const char)s.val[1], (const char)s.val[0]);
          aux->setAlphaPixelValue((const char) s.val[3]);
          setPixel(x, y, aux);
          delete(aux);
      }
    }

    //Destroy the OPenCv structure
    cvReleaseImage(&image);

}

/**
* Empty constructor of PNGImage Class.
* It's advised when you want to create
* a new PNG File.
* NOTE: For default all PNG Files are opened
* in colored mode with Alpha Transparency
* and 8 bits per pixel.
*/
PNGImage::PNGImage(){

    setFilename("NO_FILE");
    setSize(0);
    setWidth(0);
    setHeight(0);
    setChannels(0);
    setImageID(0);

    setGrayScaleAvaliable(false);
    setBlackAndWhiteAvaliable(false);

    setBitsPerPixel(1);
}

/**
* Constructor of PNGImage Class.
* It's advised when you want to open
* a PNG File.
* NOTE: For default all PNG Files are opened
* in colored mode with 8 bits per pixel.
*
* @param filename: The name of the file.
*/
PNGImage::PNGImage(string filename){

    openImage(filename);
    setGrayScaleAvaliable(false);
    setBlackAndWhiteAvaliable(false);

    setBitsPerPixel(8);
}

/**
* Destructor of PNGImage Class
*/
PNGImage::~PNGImage(){

    deletePixelMatrix();
}

/**
* Open the PNG images.
* If file exists, the image are loaded in the pixel matrix.
*
* @param filename: Name of the Image to open.
* @exception FileException Throwed when the file does not exists.
* @exception FullHeapException Insufficient space for memory allocation.
* @see Pixel.
*/
void PNGImage::openImage(string filename) throw (artemis::FileException*, artemis::FullHeapException*){

    IplImage *image;

    setFilename(filename);

    image = cvLoadImage(this->getFilename().c_str(), -1);
    if (!image)
       throw new FileException(0, "The image file cannot be opened or the file does not exists", filename);


    setImageID(0);

    //PNG has alpha transparency
    setChannels(image->nChannels);
    //Properties of the Object

    //Size is the pixel area
    //Here is the calculus
    setSize((image->height)*(image->width));
    setWidth(image->width);
    setHeight(image->height);
    //Transfer RGB from IplImage to Matrix of Pixels
    //The Matrix of Pixels can be accessed now
    //Full Load of the jpg image
    //This function load the JPEG image by OpenCV
    try{
        loadPixelMatrix();
    }catch(FullHeapException he){
        throw he;
    }
    cvReleaseImage(&image);
}

/**
* Converts the RGB Image in to a Gray Scale Image.
*/
void PNGImage::toGrayScale(){

     setGrayScaleAvaliable(true);

   //The news values of Gray Pixel are copied to the Pixel Matrix
   for(int x = 0; x < getWidth(); x++){
      for(int y = 0; y < getHeight(); y++){
          Pixel *p = new Pixel(getPixel(x, y));
          p->toGrayScale();
          setPixel(x, y, p);
          delete(p);
      }
   }
}

/**
* Converts the RGB Image in to a Black And White Image .
*/
void PNGImage::toBlackAndWhiteScale(){

   setGrayScaleAvaliable(true);
   setBlackAndWhiteAvaliable(true);
   toGrayScale();

   for(int x = 0; x < getWidth(); x++){
      for(int y = 0; y < getHeight(); y++){
          Pixel *p = new Pixel(getPixel(x, y));
          p->toGrayScale();
          p->toBlackAndWhite();
          setPixel(x, y, p);
          delete(p);
      }
   }
}

/**
* Save the current PNG into a file.
*
* @param filename The name of the file.
*
* @throw FileException If the image cannot be saved.
*/
void PNGImage::saveToFile(string filename) throw (artemis::FileException*){

    CvScalar s;
    IplImage *image = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_8U, 0);

    int div;
    if ((getBitsPerPixel() > 8) && (getBitsPerPixel() <= 16))
        div = 16;
    else
        div = 1;

    for(int x = 0; x < getWidth(); x++){
        for(int y = 0; y < getHeight(); y++){
            s.val[0] = (getPixel(x, y).getGrayPixelValue()/div);
            cvSet2D(image, y, x, s);
        }
    }

    filename.append(".png");

    if (!cvSaveImage(filename.c_str(), image)){
        throw new artemis::FileException(0, "This file cannot been created!", filename);
    }
}

/**
* Clones a current Image.
* If, for any reason the Image cannot be copied,
* then NULL is returned.
*/
PNGImage* PNGImage::clone(){

    //Create a new instance of BMP Image and copy
      //the features of the current instance to the new instance.
      PNGImage *p = new PNGImage();

      p->setWidth(this->getWidth());
      p->setHeight(this->getHeight());
      p->setSize(this->getSize());
      p->setFilename(this->getFilename());

      try{
          p->createPixelMatrix(p->getWidth(), p->getHeight());
      } catch (...) {
          return NULL;
      }

      for(int x = 0; x < p->getWidth(); x++){
              for (int y = 0; y < p->getHeight(); y++){
                  Pixel *t = new Pixel(this->getPixel(x, y));
                  p->setPixel(x, y, t);
                  delete(t);
              }
      }

      p->setImageID(this->getImageID());
      p->setChannels(this->getChannels());

          p->setGrayScaleAvaliable(this->isGrayScaleAvaliable());
          p->setBlackAndWhiteAvaliable(this->isBlackAndWhiteAvaliable());
      p->setBitsPerPixel(this->getBitsPerPixel());

      return p;
}

/**
* Compares two PNG Images. If ALL pixels, width, height and size are equals
* return true else return false.
*
* @param p A PNG Image to be compared.
* @return If ALL pixels, width, height and size are equals return true else return false.
*/
bool PNGImage::isEqual(PNGImage *p){

    if (p->getHeight() != this->getHeight()){
        return false;
     }

     if (p->getWidth() != this->getWidth()){
        return false;
     }

     for (int x = 0; x < p->getWidth(); x++){
         for (int y = 0; y < p->getHeight(); y++){
             if (!(p->getPixel(x, y).isEqual(&this->getPixel(x, y)))){
                return false;
             }
         }
     }

     return true;
}
