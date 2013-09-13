/**
*
* The GBDI-ICMC-USP Software License Version 1.0
*
* Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
* Ciï¿½ncias Matemï¿½ticas e de Computaï¿½ï¿½o, University of Sï¿½o Paulo -
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
*        de Dados e Imagens, Instituto de Ciï¿½ncias Matemï¿½ticas e de
*        Computaï¿½ï¿½o, University of Sï¿½o Paulo - Brazil (the Databases
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
* NOTE: THIS FILE JUST PROVED A SIMPLE VIEW!!!
*/

#ifndef IMAGEVIEW_HPP
#define IMAGEVIEW_HPP

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <gbdi/image/ImageBase.hpp>

using namespace std;

/**
* @brief Component General Class for view
* @author Marcos Vinícius Naves Bêdo
* @version 1.0
* @date 11/01/2010 
*/
     
class Component{
      
      private:
            string name;
      
      public:
            Component();
            Component(string name);
            ~Component();
            
            void setName(string name);
            string getName();      
};

/**
* @brief Bar to increase/decrease scale
* @author Marcos Vinícius Naves Bêdo
* @version 1.0
* @date 11/01/2010 
*/
class Trackbar : public Component{

      private:
              int min;
              int max;
             
      public:
             Trackbar();
             Trackbar(string name, int min, int max);
             ~Trackbar();
             
             void setMinimumSize(int min);
             void setMaximumSize(int max);
            
             int getMinimumSize();
             int getMaximumSize();
};

/**
* @brief Bar to increase/decrease brightness
* @author Marcos Vinícius Naves Bêdo
* @version 1.0
* @date 11/01/2010 
*/
class BrightnessTrackbar : public Trackbar{
      
      public:
              int current_b;

      public:
            BrightnessTrackbar();
            BrightnessTrackbar(string name, int min, int max, int current_b);
            ~BrightnessTrackbar();
            
            void setCurrentBrightness(int current_b);
            int getCurrentBrightness();                 
};

/**
* @brief Bar to increase/decrease contrast
* @author Marcos Vinícius Naves Bêdo
* @version 1.0
* @date 11/01/2010 
*/
class ContrastTrackbar : public Trackbar{
      
      public:
              int current_c;

      public:
            ContrastTrackbar();
            ContrastTrackbar(string name, int min, int max, int current_c);
            ~ContrastTrackbar();
            
            void setCurrentContrast(int current_c);
            int getCurrentContrast();                 
};

/**
* @brief Master class for visualization
* @author Marcos Vinícius Naves Bêdo
* @version 1.0
* @date 11/01/2010 
*/
class Window{

      private:
              string windowname;
              string windowtitle;
              int qtd_components;
              int height;
              int width;
              vector<Image*> img;
      
      private:
             void showManyImages(string title, int nchannels) throw (OutOfBoundsException*);
      
      public:
             Window();
             Window(string name, string title, int qtd_components, int height, int width, Image *img);
             ~Window();
             
             void setWindowName(string windowname);
             void setWindowTitle(string windowtitle);
             void setSumOfComponents(int qtd_components);
             void setHeight(int height);
             void setWidth(int width);

             void addImage(Image *img);
             void addComponent(Component *c);
             void addBrightnessTrackbar(BrightnessTrackbar *bt);
             void addContrastTrackbar(ContrastTrackbar *ct);
             
             void removeImage(Image *img);
             void removeComponent();
             void removeBrightnessTrackbar(BrightnessTrackbar *bt);
             void removeContrastTrackbar(ContrastTrackbar *ct);
             
             void showColoredImage() throw (OutOfBoundsException*);
             void showGrayScaleImage() throw (OutOfBoundsException*);
             void showBlackAndWhiteImage() throw (OutOfBoundsException*);
             
     
             string getWindowName();
             string getWindowTitle();
             int getSumOfComponents();
             int getHeight();
             int getWidth();
};


#endif

