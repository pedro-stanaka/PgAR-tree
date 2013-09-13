#include <gbdi/image/ImageView.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>


/**
* Component Constructor
*
*/
Component::Component(){
  
    this->name = "NO_IDENTIFIED";                     
};

/**
* Component Constructor
* @param name of the Component
*/
Component::Component(string name){

    this->setName(name);                            
};

/**
* Component Destructor
*
*/
Component::~Component(){
                        
};

/**
* Sets the name of the component
* @param name the new name of the component 
*/
void Component::setName(string name){
     
    this->name = name;
};

/**
* Get's the component name
* @return The component name
*/
string Component::getName(){
       
    return this->name;
};

/**
* Trackbar Constructor
*
*/
Trackbar::Trackbar(){
  
    setName("NO_IDENTIFIED");
    setMinimumSize(0);
    setMaximumSize(0);                   
};

/**
* Trackbar Constructor
* @param name The name of the Trackbar
* @param min The minimun value of the trackbar scale
* @param max The maximum value of the trackbar scale
*/
Trackbar::Trackbar(string name, int min, int max){
    
    setName(name);
    setMinimumSize(min);
    setMaximumSize(max);                        
};

/**
* Trackbar Destructor
*
*/
Trackbar::~Trackbar(){
                      
};
             
/**
* Sets the new minimum value of the Trackbar
* @param min The new minimum value
*/
void Trackbar::setMinimumSize(int min){

     this->min = min;
};

/**
* Sets the new maximum value of the Trackbar
* @param max The new maximum value of the Trackbar
*/
void Trackbar::setMaximumSize(int max){
  
     this->max = max;   
};
            
/**
* Gets the Trackbar minimum values
* @return The minimum value of the Trackbar
*/
int Trackbar::getMinimumSize(){
  
     return this->min;  
};

/**
* Gets the Trackbar maxi9mum values
* @return The maximum value of the Trackbar
*/
int Trackbar::getMaximumSize(){
  
     return this->max;  
};

/**
* Constructor
*/
BrightnessTrackbar::BrightnessTrackbar(){
  
     setName("NO_IDENTIFIED");
     setMinimumSize(0);
     setMaximumSize(0);
     setCurrentBrightness(0);                                       
};

/**
* Constructor
* @param name The name of the Brightness Trackbar
* @param min The minimum value of the Brightness Trackbar
* @param max The maximum value of the Brightness Trackbar
* @param current_b The start value of the Brightness Trackbar
*/
BrightnessTrackbar::BrightnessTrackbar(string name, int min, int max, int current_b){
     
     setName(name);
     setMinimumSize(min);
     setMaximumSize(max);
     setCurrentBrightness(current_b);                                       
};

/**
* Destructor
*/
BrightnessTrackbar::~BrightnessTrackbar(){

};

/**
* Sets the new value of the current brightness
* @param current_b The new value of the current brightness
*/
void BrightnessTrackbar::setCurrentBrightness(int current_b){

     this->current_b = current_b;
};

/**
* Get the current brightness
* @return The current brightness
*/
int BrightnessTrackbar::getCurrentBrightness(){
    
    return current_b;
};            

/**
* Constructor
*/
ContrastTrackbar::ContrastTrackbar(){
     setName("NO_IDENTIFIED");
     setMinimumSize(0);
     setMaximumSize(0);
     setCurrentContrast(0); 
};

/**
* Constructor
* @param name The name of the Contrast Trackbar
* @param min The minimum value of the Contrast Trackbar
* @param max The maximum value of the Contrast Trackbar
* @param current_c The start value of contrast
*/
ContrastTrackbar::ContrastTrackbar(string name, int min, int max, int current_c){

     setName(name);
     setMinimumSize(min);
     setMaximumSize(max);
     setCurrentContrast(current_c);                                           
};

/**
* Destructor
*/
ContrastTrackbar::~ContrastTrackbar(){
                                       
};

/**
* Sets a new contrast value
* @param current_c A new current value of contrast
*/
void ContrastTrackbar::setCurrentContrast(int current_c){

     this->current_c = current_c;
};

/**
* Gets the current value of Contrast
* @return The current value of contrast
*/
int ContrastTrackbar::getCurrentContrast(){
  
     return current_c;  
};

/**
* Constructor
*/
Window::Window(){

    setWindowName("NO_IMAGE_FILE_SELECTED");
    setWindowTitle("NO_IMAGE_FILE_SELECTED");
    setSumOfComponents(0);
    setHeight(0);
    setWidth(0);
    addImage(NULL); 
    
    cvNamedWindow(getWindowTitle().c_str(), 1);                 
};

/**
* Constructor
* @param name The name of the window
* @param title The title waht will be displayed
* @param qtd_components The number of components
* @param height The height of the Window
* @param width The width of the Window
* @param Image A first image to be added in the Window
*/
Window::Window(string name, string title, int qtd_components, int height, int width, Image *img){

    setWindowName(name);
    setWindowTitle(title);
    setSumOfComponents(qtd_components);
    setHeight(height);
    setWidth(width);
    addImage(img);    
    cvNamedWindow(getWindowTitle().c_str(), 1);                   
};

/**
* Destructor
*/
Window::~Window(){
  
    this->img.clear();               
};
            
/**
* Sets a Window Name
* @param windowname The new Window Name
*/ 
void Window::setWindowName(string windowname){
  
     this->windowname = windowname;   
};

/**
* Sets a Window Title
* @param title A new title
*/
void Window::setWindowTitle(string windowtitle){
  
     this->windowtitle = windowtitle;   
};

/**
* Sets a sum of components
* @param qtd_components The number of components
*/
void Window::setSumOfComponents(int qtd_components){
  
     this->qtd_components = qtd_components;   
};

/**
* Sets the height
* @param height The new height
*/
void Window::setHeight(int height){

     this->height = height;     
};

/**
* Sets the width
* @param height The new width
*/
void Window::setWidth(int width){

     this->width = width;     
};

/**
* Add a image what will be displayed
* BE CAREFUL:
*     - A added image should be not modified
*     - Only 12 images are allowed           
* @param img A pointer to Image what will be added in the window
*/
void Window::addImage(Image *img){
  
     if (img != NULL)
        this->img.push_back(img);
};

/**
* Add a simple component in the window
* @param c A new component what will be added
*/         
void Window::addComponent(Component *c){
     
     //adiciona componente 
     setSumOfComponents(getSumOfComponents() + 1);
};

/**
* Add a Brightness Track Bar in the Window
* BE CAREFUL:
*      - Only one brightness toolbar are supported by the window
* @param bt A BrightnessTrackBar what will be added
*/
void Window::addBrightnessTrackbar(BrightnessTrackbar *bt){
  
     cvCreateTrackbar(bt->getName().c_str(), getWindowTitle().c_str(), &bt->current_b, 
     bt->getMaximumSize(), NULL);  
     
     setSumOfComponents(getSumOfComponents() + 1);;
};

/**
* Add a Contrast Track Bar in the Window
* BE CAREFUL:
*      - Only one contrast toolbar are supported by the window
* @param ct A ContrastTrackBar what will be added
*/
void Window::addContrastTrackbar(ContrastTrackbar *ct){
  
     cvCreateTrackbar(ct->getName().c_str(), getWindowTitle().c_str(), &ct->current_c, 
     ct->getMaximumSize(), NULL);  
     setSumOfComponents(getSumOfComponents() + 10); 
};

/**
* Remove a added Image, if the image was not added nothing happened
* @param img The image what will be removed
*/
void Window::removeImage(Image *img){

     for (int i = 0; i < this->img.size(); i++){
         if (this->img[i]->getImageID() == img->getImageID())
            this->img.erase(this->img.begin() + i);
     }
};

/**
* Remove a added Component, if the component was not added nothing happened
*/
void Window::removeComponent(){

};

/**
* Remove a added Component, if the component was not added nothing happened
* @param ct A BrightnessTrackbar what will be removed
*/
void Window::removeBrightnessTrackbar(BrightnessTrackbar *bt){

};

/**
* Remove a added Component, if the component was not added nothing happened
* @param ct A ContrastTrackbar what will be removed
*/
void Window::removeContrastTrackbar(ContrastTrackbar *ct){

};

/**
*
* Special thanks to Parameswaran
* Parameswaran, 
* Chennai, India.
*
* cegparamesh[at]gmail[dot]com  
* 
* This function does not stretch the image... 
* It resizes the image without modifying the width/height ratio.
* @param title The title of the Window
* @param nchannels The nchannels of the images
* @throw Exceed the allowed number of Images
*
*/
void Window::showManyImages(string title, int nchannels) throw (OutOfBoundsException*) {

    vector<IplImage*> display;
    CvScalar s;
      
    if (nchannels == 0){
          int bpp = 1;
          for (int i = 0; i < this->img.size(); i++){
             IplImage *ex = cvCreateImage(cvSize(img[i]->getWidth(), img[i]->getHeight()),IPL_DEPTH_8U, 0);
             for(int x = 0; x < img[i]->getWidth(); x++){
                     if ((img[i]->getBitsPerPixel() > 0) && (img[i]->getBitsPerPixel() <= 8))
                        bpp = 1;
                     if ((img[i]->getBitsPerPixel() > 8) && (img[i]->getBitsPerPixel() <= 16))
                        bpp = 16;
                     for (int y = 0; y < img[i]->getHeight(); y++){
                              s.val[0] = (img[i]->getPixel(x, y).getGrayPixelValue())/bpp;
                              s.val[1] = 0;
                              s.val[2] = 0;
                              s.val[3] = 0;
                              cvSet2D(ex, y, x, s);
                     }        
             }
             display.push_back(ex);
          }           
    } else {
          for (int i = 0; i < this->img.size(); i++){
             IplImage *ex = cvCreateImage(cvSize(img[i]->getWidth(), img[i]->getHeight()),IPL_DEPTH_8U, 3);
             for(int x = 0; x < img[i]->getWidth(); x++){
                    for (int y = 0; y < img[i]->getHeight(); y++){
                              s.val[2] = img[i]->getPixel(x, y).getRedPixelValue();
	                          s.val[1] = img[i]->getPixel(x, y).getGreenPixelValue();
	                          s.val[0] = img[i]->getPixel(x, y).getBluePixelValue();
                              cvSet2D(ex, y, x, s);
                     }        
             }
             display.push_back(ex);
          } 
    };
    
    // img - Used for getting the arguments 
    IplImage *img;

    // DispImage - the image in which input images are to be copied
    IplImage *DispImage;

    int size;
    int i;
    int m, n;
    int x, y;

    // w - Maximum number of images in a row 
    // h - Maximum number of images in a column 
    int w, h;

    // scale - How much we have to resize the image
    float scale;
    int max;

    // If the number of arguments is lesser than 0 or greater than 12
    // return without displaying 
    if(display.size() <= 0) {
        throw new OutOfBoundsException(2, "Theres no added images", 0, 12);
    }
    else if(display.size() > 12) {
        throw new OutOfBoundsException(2, "The number of the allowed images are 12", 0, 12);
    }
    // Determine the size of the image, 
    // and the number of rows/cols 
    // from number of arguments 
    else if (display.size() == 1) {
        w = h = 1;
        size = 300;
    }
    else if (display.size() == 2) {
        w = 2; h = 1;
        size = 300;
    }
    else if (display.size() == 3 || display.size() == 4) {
        w = 2; h = 2;
        size = 300;
    }
    else if (display.size() == 5 || display.size() == 6) {
        w = 3; h = 2;
        size = 200;
    }
    else if (display.size() == 7 || display.size() == 8) {
        w = 4; h = 2;
        size = 200;
    }
    else {
        w = 4; h = 3;
        size = 150;
    }

    DispImage = cvCreateImage( cvSize(100 + size*w, 60 + size*h), 8, nchannels );


    // Loop for images
    for (i = 0, m = 20, n = 20; i < display.size(); i++, m += (20 + size)) {

        // Get the Pointer to the IplImage
        img = display[i];

        // Check whether it is NULL or not
        // If it is NULL, release the image, and return
        if(img == 0) {
            throw new OutOfBoundsException(2, "Invalid format", 0, 12);
        }

        // Find the width and height of the image
        x = img->width;
        y = img->height;

        // Find whether height or width is greater in order to resize the image
        max = (x > y)? x: y;

        // Find the scaling factor to resize the image
        scale = (float) ( (float) max / size );

        // Used to Align the images
        if( i % w == 0 && m!= 20) {
            m = 20;
            n+= 20 + size;
        }

        // Set the image ROI to display the current image
        cvSetImageROI(DispImage, cvRect(m+27, n, (int)( x/scale ), (int)( y/scale )));

        // Resize the input image and copy the it to the Single Big Image
        cvResize(img, DispImage);

        // Reset the ROI in order to display the next image
        cvResetImageROI(DispImage);
    }
    

    // Create a new window, and show the Single Big Image
    cvNamedWindow(title.c_str(), 1 );
    cvShowImage(title.c_str(), DispImage);

    cvWaitKey();
    cvDestroyWindow(title.c_str());

    // Release the Image Memory
    cvReleaseImage(&DispImage);
    display.clear();
};      

/**
* Display all the colored added images
* @throw Exceed the allowed number of Images
*/ 
void Window::showColoredImage() throw (OutOfBoundsException*){
     
     try{
         showManyImages(getWindowTitle(), 3);
     }catch (OutOfBoundsException *ob){
         throw ob;
     }
};

/**
* Display all the gray-scale added images
* @throw Exceed the allowed number of Images
*/ 
void Window::showGrayScaleImage() throw (OutOfBoundsException*){
     
     try{
         showManyImages(getWindowTitle(), 0);
     }catch (OutOfBoundsException *ob){
         throw ob;
     }
};

/**
* Display all the clack and white added images
* @throw Exceed the allowed number of Images
*/ 
void Window::showBlackAndWhiteImage() throw (OutOfBoundsException*){

     try{
         showManyImages(getWindowTitle(), 0);
     }catch (OutOfBoundsException *ob){
         throw ob;
     }
};

/**
* Gets the Window name
* @return The Window name
*/      
string Window::getWindowName(){

       return this->windowname;       
};

/**
* Gets the Window Title
* @return The Window Title
*/
string Window::getWindowTitle(){
       
       return this->windowtitle;
};

/**
* Gets the total of Components
* @return The total of Components
*/
int Window::getSumOfComponents(){
    
       return this->qtd_components;
};

/**
* Gets the height of the window
* @return The height of the window
*/
int Window::getHeight(){

    return this->height;    
};

/**
* Gets the width of the window
* @return The width of the window
*/
int Window::getWidth(){

    return this->width;    
};




