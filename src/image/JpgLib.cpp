#include <gbdi/image/JpgLib.hpp>

/**
* Load the JPG Image in the Pixel Matrix.
* A new matrix of Pixels are allocated and the
* JPG File are loaded in.
* NOTE: OpenCv use.
*
* @exception FullHeapException Insufficient space for memory allocation.
*/
void JPGImage::loadPixelMatrix() throw (FullHeapException*){


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
          Pixel *aux = new Pixel((const char) s.val[2], (const char) s.val[1], (const char) s.val[0]);
          setPixel(x, y, aux);
          free(aux);
      }
    }

    //Destroy the OpenCv structure
    cvReleaseImage(&image);
}

/**
* Constructor of JPG Image Class.
* It's advised when you want to create
* a new JPG File.
* NOTE: For default all JPG Files are opened
* in colored mode with 8 bits per pixel.
*/
JPGImage::JPGImage(){

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
* Constructor of JPG Image Class.
*
* @param filename: The name of the file.
*/
JPGImage::JPGImage(string filename){

    openImage(filename);
    setGrayScaleAvaliable(false);
    setBlackAndWhiteAvaliable(false);

    setBitsPerPixel(8);
}

/**
* Destructor of the JPG Image Class.
*/
JPGImage::~JPGImage(){

    deletePixelMatrix();
}

/**
* Open the JPG images.
* If file exists, the image are loaded in the pixel matrix.
*
* @param filename: Name of the Image to open.
* @exception FileException Throwed when the file does not exists.
* @exception FullHeapException Insufficient space for memory allocation.
* @see Pixel.
*/
void JPGImage::openImage(string filename) throw (artemis::FileException*, artemis::FullHeapException*){

    IplImage *image;
    setFilename(filename);
    image = cvLoadImage(getFilename().c_str(), 3);
    if (!image)
       throw new FileException(0, "The image file cannot be opened or the file does not exists", filename);


    //JPG is a multi channel colored image
    setChannels(image->nChannels);
    setImageID(0);

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
* Converts the RGB Image to GrayScale Image.
*/
void JPGImage::toGrayScale(){

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
* Converts the RGB Image to BlackAndWhite Scale.
*/
void JPGImage::toBlackAndWhiteScale(){

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
* Save the current JPG into a file.
*
* @param filename The name of the file.
*
* @throw FileException If the image cannot be saved.
*/
void JPGImage::saveToFile(string filename) throw (artemis::FileException*){

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

    filename.append(".jpg");

    if (!cvSaveImage(filename.c_str(), image)){
        throw new artemis::FileException(0, "This file cannot been created!", filename);
    }
}

/**
* Clones a current Image.
* If, for any reason the Image cannot be copied,
* then NULL is returned.
*/
JPGImage* JPGImage::clone(){

      //Create a new instance of JPG Image and copy
      //the features of the current instance to the new instance.
      JPGImage *j = new JPGImage();

      j->setWidth(getWidth());
      j->setHeight(getHeight());
      j->setSize(getSize());
      j->setFilename(getFilename());

      try{
          j->createPixelMatrix(j->getWidth(), j->getHeight());
      } catch (...) {
          return NULL;
      }

      for(int x = 0; x < j->getWidth(); x++){
              for (int y = 0; y < j->getHeight(); y++){
                  Pixel *p = new Pixel(getPixel(x, y));
                  j->setPixel(x, y, p);
                  delete(p);
              }
      }

      j->setImageID(getImageID());
      j->setChannels(getChannels());

          j->setGrayScaleAvaliable(isGrayScaleAvaliable());
          j->setBlackAndWhiteAvaliable(isBlackAndWhiteAvaliable());
      j->setBitsPerPixel(getBitsPerPixel());

      return j;
}

/**
* Compares two JPG Images. If ALL pixels, width, height and size are equals
* return true else return false.
*
* @param j A JPG Image to be compared.
* @return If ALL pixels, width, height and size are equals return true else return false.
*/
bool JPGImage::isEqual(JPGImage *j){

    if (j->getHeight() != this->getHeight()){
        return false;
     }

     if (j->getWidth() != this->getWidth()){
        return false;
     }

     for (int x = 0; x < j->getWidth(); x++){
         for (int y = 0; y < j->getHeight(); y++){
             if (!(j->getPixel(x, y).isEqual(&this->getPixel(x, y)))){
                return false;
             }
         }
     }

     return true;
 }

