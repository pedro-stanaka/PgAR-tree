#include <gbdi/image/KrlLib.hpp>

/**
* Empty Constructor.
* It's advised when you want to create
* a new KRL File.
* NOTE: For default all KRL Files are opened
* in gray scale mode with 12 bits per pixel.
*/
KRLImage::KRLImage(){
                
    setFilename("NO_FILE");
    setSize(0);
    setWidth(0);
    setHeight(0);
    setChannels(0);
    setImageID(0);
    setBitsPerPixel(12);
}

/**
* Constructor.
*
* @param filename The KRL file what will be opened.
*/
KRLImage::KRLImage(string filename){

    setFilename(filename);
    openImage(filename);
}

/**
* Destructor.
*/
KRLImage::~KRLImage(){

}

/**
* Open a KRL file.
*
* @param fileName The name of the KRL file what will be opened.
* @exception FileException Throwed if the file does not exists.
* @exception FullHeapException Throwed if the memory are insufficient.
*/
void KRLImage::openImage(string filename) throw (artemis::FileException*, artemis::FullHeapException*){
    
    KrlHeaderType header;
     
    try{
        FileHandler * img = new FileHandler(filename, img->FILE_BINARY);
        
        //==================== CABEÇALHO DA IMAGEM ================================
        
        header.headerType = *((unsigned short *)img->readFile(2));
        img->endianSwap(header.headerType);
    
        for(int i=0; i<32; i++)
            header.filename[i] = *(img->readFile(1));
    
    
        header.width = *((unsigned short *) img->readFile(2));
        img->endianSwap(header.width);
        this->setWidth(header.width);
    
        header.height = *((unsigned short *) img->readFile(2));
        img->endianSwap(header.height);
        this->setHeight(header.height);
     
        header.pixBytes = *(img->readFile(1));
        header.pixBits  = *(img->readFile(1));
        header.OS       = *(img->readFile(1));
        header.compress = *(img->readFile(1));

        for(int i=0;i<5;i++){     
           header.compParas[i] = *((unsigned short *) img->readFile(2));
           img->endianSwap(header.compParas[i]);
        }
    
    for(int i=0; i<32; i++)
         header.hdtime[i] = *(img->readFile(1));
     
     header.imgType = *(img->readFile(1));
     header.black   = *(img->readFile(1));
     
     header.pixSize = *((unsigned short*) img->readFile(2));
     img->endianSwap(header.pixSize);
     
     header.sampType = *((unsigned short*)img->readFile(2));
     img->endianSwap(header.sampType);
     
     header.optDensity = *(img->readFile(1));
    
     for(int i=0; i<13; i++)
         header.dayNo[i] =  *(img->readFile(1));
     
     header.medHistNo = *((long*) img->readFile(4));
     
     for(int i=0; i<10; i++)
         header.studyType[i]   = *(img->readFile(1));
     for(int i=0; i<50; i++)
         header.patientName[i] = *(img->readFile(1));
     for(int i=0; i<216; i++)
         header.special[i]     = *(img->readFile(1));
     for(int i=0; i<128; i++)
         header.comment[i]     = *(img->readFile(1));   
     
     //========================= FIM CABEÇALHO =================================
     //======================== CORPO DA IMAGEM ================================ 
   
     unsigned short tmpValue;
     
     try{
        createPixelMatrix(header.width, header.height);
     } catch (FullHeapException *he) {
        throw he;
     }
     
     for(int x=0; x< header.width; x++){ 
         for(int y=0; y<header.height; y++){
             tmpValue = *((unsigned short *) img->readFile(2));
             img->endianSwap(tmpValue);   
             Pixel *aux = new Pixel(0, 0, 0);
             aux->setGrayPixelValue((float)tmpValue);
             setPixel(x,y,aux);
	         delete(aux);
         }
     }
    setWidth(header.width);
    setHeight(header.height);
    setSize(header.width*header.height);
    setFilename(filename);
    setImageID(2);
    setChannels(0);
	setBlackAndWhiteAvaliable(false);
	setGrayScaleAvaliable(true);
    setBitsPerPixel(header.pixBits - 0);
         
     } catch (FileException *f) {
         throw f;
    }
}

/**
* Clones a current Image.
* If, for any reason the Image cannot be copied,
* then NULL is returned.
*/
KRLImage* KRLImage::clone(){

     KRLImage *k = new KRLImage();
     try{
         k->openImage(getFilename());
     } catch(...) {
         return NULL;
     }
     
     return k;
}

/**
* Compares two KRL Images. If ALL pixels, width, height and size are equals
* return true else return false.
*
* @param k A KRL Image to be compared.
* @return If ALL pixels, width, height and size are equals return true else return false.
*/
bool KRLImage::isEqual(KRLImage *k){

     if (k->getHeight() != this->getHeight()){
        return false;
     }
     
     if (k->getWidth() != this->getWidth()){
        return false;
     }
     
     for (int x = 0; x < k->getWidth(); x++){
         for (int y = 0; y < k->getHeight(); y++){
             if (!(k->getPixel(x, y).isEqual(&this->getPixel(x, y)))){
                return false;
             }
         }
     }
     
     return true;
}



