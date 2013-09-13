#ifndef EXTRACTORLIB_HPP
#define EXTRACTORLIB_HPP

#include <gbdi/image/ImageLib.hpp>
#include "Extractor.hpp"
#include "NormalizedHistogram.hpp"
#include "HaralickExtractor.hpp"
#include "ExtractorException.hpp"
#include "HaralickFeature.hpp"


/**
* Esse m�todo precisa ser otimizado
*
*/

Image* toImage(ObjectSignature *obj){
       
        JPGImage *i = new JPGImage();
     
        i->setWidth(obj->GetSize()+1);
        i->setHeight(obj->GetSize()+1);
        i->setSize(obj->GetSize()*obj->GetSize());
        i->setFilename("Signature");
        
        i->createPixelMatrix(i->getWidth(), i->getHeight());
        for (int x1 = 0; x1 < i->getWidth(); x1++){
            for (int y1 = 0; y1 < i->getHeight(); y1++){
                Pixel *p = new Pixel(0, 0, 0);
                i->setPixel(x1, y1, p);
                delete(p);
            }
        }        
     
        int x = 0;
        int y = 0;
     
        for (int a = 0; a < obj->GetSize(); a++){        
            float value = *((float *) obj->Get(a));
            y = (int)(obj->GetSize() - value);
            for (int ix = x; ix <= x+1; ix++){
                for(int iy = 0; iy <= y; iy++){
                        Pixel *p = new Pixel(255, 255, 255);
                        i->setPixel(ix, iy, p);
                        delete(p);  
                 }    
            }
            x++;
        }
        i->toGrayScale();
       i->setImageID(1);
       i->setChannels(0);
       i->setBitsPerPixel(8);
 
       return i;      
};

Image* equalizateImage(Image* img, int reductionParameter){
  
    Image* image = img->clone();
    float max, min;

    max = image->getPixel(0, 0).getGrayPixelValue();
    min = max;
    
    for (int y = 0; y < image->getHeight(); y++){
        for (int x = 0; x < image->getWidth(); x++){
            if (image->getPixel(x, y).getGrayPixelValue() > max)
                max = image->getPixel(x, y).getGrayPixelValue();
            if (image->getPixel(x, y).getGrayPixelValue() < min)
                min = image->getPixel(x, y).getGrayPixelValue();
        }
    }
        
    if (max != min){
        float dif = max - min;
        for(int y = 0; y < image->getHeight(); y++){
            for(int x = 0; x < image->getWidth(); x++){
                Pixel *p = new Pixel(image->getPixel(x, y));
                p->setGrayPixelValue((255*(image->getPixel(x, y).getGrayPixelValue()-min)/dif)/reductionParameter);
                image->setPixel(x, y, p);
                delete(p);
            }
        }
    }else{
        for(int y = 0; y < image->getHeight(); y++){
            for(int x = 0; x < image->getWidth(); x++){
                Pixel *p = new Pixel(image->getPixel(x, y));
                p->setGrayPixelValue(image->getPixel(x, y).getGrayPixelValue()/reductionParameter);
                image->setPixel(x, y, p);
                delete(p);
            }
        }
    }
    
    return image;
};

Image* windowing(Image *img, int window, int center){

            Image *dst_image = new Image();
   
            dst_image->setSize(img->getSize());
            dst_image->setFilename(img->getFilename());
            dst_image->setWidth(img->getWidth());
            dst_image->setHeight(img->getHeight());
            dst_image->setBitsPerPixel(img->getBitsPerPixel());
            dst_image->setChannels(img->getChannels());    
            dst_image->createPixelMatrix(img->getWidth(), img->getHeight());
            dst_image->setImageID(img->getImageID());

   
            int numBins = (int) pow(2.0, (double) dst_image->getBitsPerPixel());
            double *grayScale = new double[numBins];     
   
            double prop = 0;
            
            for(int i=0; i<numBins; i++){
                    if(i<(center-(window/2))){
                        grayScale[i] = 0;
                    } else if(i<numBins && i<(center+(window/2))){
                        grayScale[i] = (window*( (double)i / (double)numBins + 0.5) - center);
                        
                        if(grayScale[i]<0){
                            grayScale[i] = 0;
                        } else if(grayScale[i]>numBins){
                            grayScale[i] = numBins;
                        }
                    } else if(i<numBins){
                        grayScale[i] = numBins;
                    }
            }
   

            for(int x=0; x< dst_image->getWidth(); x++){
                    for(int y=0; y< dst_image->getHeight(); y++){
                            Pixel *p = new Pixel(img->getPixel(x, y));
                            int pixelValueInt = (int) p->getGrayPixelValue();
                            p->setGrayPixelValue(grayScale[pixelValueInt]);
                            dst_image->setPixel(x, y, p);
                            free(p);
                    }
            }
  
            delete(grayScale);
            
            return dst_image;
};

Image* medianFilter(Image *image){

       Image* filtered = image->clone();
       
  
       //apply median filter
       for(int y = 1; y < image->getHeight()-1; y++){
          for(int x = 1; x < image->getWidth()-1; x++){
              float val[9];
         
              val[0] = image->getPixel(x-1, y-1).getGrayPixelValue();
              val[1] = image->getPixel(x, y-1).getGrayPixelValue();
              val[2] = image->getPixel(x+1, y-1).getGrayPixelValue();
              val[3] = image->getPixel(x-1, y).getGrayPixelValue();
              val[4] = image->getPixel(x, y).getGrayPixelValue();
              val[5] = image->getPixel(x+1, y).getGrayPixelValue();
              val[6] = image->getPixel(x-1, y+1).getGrayPixelValue();
              val[7] = image->getPixel(x, y+1).getGrayPixelValue();
              val[8] = image->getPixel(x+1, y+1).getGrayPixelValue();
              //sort the vector with window parameters
              sort(val, val+9);
              Pixel *p = new Pixel(image->getPixel(x, y));
              p->setGrayPixelValue(val[4]);
              filtered->setPixel(x, y, p);
              delete(p);
         }      
       }
   
       return filtered;
};

Image* highPassFilter(Image *image){
       
       Image* filtered = image->clone();
  
       //filtro Passa Alta  ????? Confirmar com Joaquim !!!!!!!!!!!
       for(int y =1 ; y < image->getHeight()-1; y++){
           for(int x = 1; x < image->getWidth()-1; x++){
               int pixel = (int) (image->getPixel(x, y).getGrayPixelValue()*9
                                  - image->getPixel(x-1, y-1).getGrayPixelValue()
                                  - image->getPixel(x-1, y).getGrayPixelValue()
                                  - image->getPixel(x-1, y+1).getGrayPixelValue()
                                  - image->getPixel(x, y-1).getGrayPixelValue()
                                  - image->getPixel(x, y+1).getGrayPixelValue()
                                  - image->getPixel(x+1, y-1).getGrayPixelValue()
                                  - image->getPixel(x+1, y).getGrayPixelValue()
                                  - image->getPixel(x+1, y+1).getGrayPixelValue());
               if(pixel < 0) pixel = 0;
               if(pixel > 255) pixel = 255;
               Pixel *p = new Pixel(image->getPixel(x, y));
               p->setGrayPixelValue((float) pixel);
               filtered->setPixel(x, y, p);
               delete(p);
           }
       }

       filtered->setBitsPerPixel(8);
       return filtered;    
};

Image* resizeToMaxBins(Image* image, int numBins){
  
       Image* img = image->clone();
       
       float max = pow(2, (float) image->getBitsPerPixel());
       
       for (int x= 0; x < image->getWidth(); x++){
           for (int y = 0; y < image->getHeight(); y++){
               Pixel *p = new Pixel(image->getPixel(x, y));
               p->setGrayPixelValue((image->getPixel(x, y).getGrayPixelValue()*numBins)/max);
               img->setPixel(x, y, p);
               delete(p);
           }
       }
       
       //verficar necessidade de alterar bits por pixel. Acho q nao � necess�rio
       
       return img;
       
};

      
#endif
