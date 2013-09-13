#include <gbdi/image/BmpLib.hpp>

/**
* Load the BMP Image in the Pixel Matrix.
* A new matrix of Pixels are allocated and the
* BMP File are loaded in.
* NOTE: OpenCv use.
*
* @exception FullHeapException Insufficient space for memory allocation.
*/
void BMPImage::loadPixelMatrix() throw (FullHeapException*){

    //A OpenCv type to recovery a Pixel
    CvScalar s;
    //A OpenCv structure to load a image file
    IplImage *image;

    image = cvLoadImage(getFilename().c_str(), 3);

    //Try allocate pixel matrix in the memory
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
          setPixel(x, y, aux);
          delete(aux);
      }
    }

    //Destroy the OpenCv structure
    cvReleaseImage(&image);
}

/**
* Empty constructor of BMPImage Class.
* It's advised when you want to create
* a new BMP File.
* NOTE: For default all BMP Files are opened
* in colored mode with 8 bits per pixel.
*/
BMPImage::BMPImage(){

    setFilename("NO_FILE");
    setSize(0);
    setWidth(0);
    setHeight(0);
    setChannels(0);
    setImageID(0);
    setGrayScaleAvaliable(false);
    setBlackAndWhiteAvaliable(false);
    setBitsPerPixel(8);
}

/**
* Constructor of BMPImage Class.
* It's advised when you want to open
* a BMP File.
* NOTE: For default all BMP Files are opened
* in colored mode with 8 bits per pixel.
*
* @param filename: The name of the file.
*/
BMPImage::BMPImage(string filename){

    openImage(filename);
    setGrayScaleAvaliable(false);
    setBlackAndWhiteAvaliable(false);
    setBitsPerPixel(8);
}


/**
* Destructor of BMPImage Class.
*/
BMPImage::~BMPImage(){

    deletePixelMatrix();
}

/**
* Open the BMP images.
* If file exists, the image are loaded in the pixel matrix.
*
* @param filename: Name of the Image to open.
* @exception FileException Throwed when the file does not exists.
* @exception FullHeapException Insufficient space for memory allocation.
* @see Pixel.
*/
void BMPImage::openImage(string filename) throw (artemis::FileException*, artemis::FullHeapException*){


    IplImage *image;

    setFilename(filename);
    image = cvLoadImage(this->getFilename().c_str(), 3);

    if (!image)
       throw new FileException(0, "The image file cannot be opened or the file does not exists", filename);

    setImageID(0);

    //JPG is a 3 channel colored image
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
void BMPImage::toGrayScale(){

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
* Converts the RGB Image in to a Black And White Image.
*/
void BMPImage::toBlackAndWhiteScale(){

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
* Save the current BMP into a file.
*
* @param filename The name of the file.
*
* @throw FileException If the image cannot be saved.
*/
void BMPImage::saveToFile(string filename) throw (artemis::FileException*){

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

    filename.append(".bmp");

    if (!cvSaveImage(filename.c_str(), image)){
        throw new artemis::FileException(0, "This file cannot been created!", filename);
    }
}

/**
* Clones a current Image.
* If, for any reason the Image cannot be copied,
* then NULL is returned.
*/
BMPImage* BMPImage::clone(){

      //Create a new instance of BMP Image and copy
      //the features of the current instance to the new instance.
      BMPImage *b = new BMPImage();

      b->setWidth(this->getWidth());
      b->setHeight(this->getHeight());
      b->setSize(this->getSize());
      b->setFilename(this->getFilename());

      try{
          b->createPixelMatrix(b->getWidth(), b->getHeight());
      } catch (...) {
          return NULL;
      }

      for(int x = 0; x < b->getWidth(); x++){
              for (int y = 0; y < b->getHeight(); y++){
                  Pixel *p = new Pixel(this->getPixel(x, y));
                  b->setPixel(x, y, p);
                  delete(p);
              }
      }

      b->setImageID(this->getImageID());
      b->setChannels(this->getChannels());

          b->setGrayScaleAvaliable(this->isGrayScaleAvaliable());
          b->setBlackAndWhiteAvaliable(this->isBlackAndWhiteAvaliable());
      b->setBitsPerPixel(this->getBitsPerPixel());

      return b;

}

/**
* Compares two BMP Images. If ALL pixels, width, height and size are equals
* return true else return false.
*
* @param b A BMP Image to be compared.
* @return If ALL pixels, width, height and size are equals return true else return false.
*/
bool BMPImage::isEqual(BMPImage *b){

     if (b->getHeight() != this->getHeight()){
        return false;
     }

     if (b->getWidth() != this->getWidth()){
        return false;
     }

     for (int x = 0; x < b->getWidth(); x++){
         for (int y = 0; y < b->getHeight(); y++){
             if (!(b->getPixel(x, y).isEqual(&this->getPixel(x, y)))){
                return false;
             }
         }
     }

     return true;

}
