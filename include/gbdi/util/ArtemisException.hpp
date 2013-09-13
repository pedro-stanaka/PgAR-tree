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
* This file defines all kind of exceptions throwd by Artemis.
*
* @version 1.0
* @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
* @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
* @date 11-03-2010
*/


#ifndef ARTEMISEXCEPTION_HPP
#define ARTEMISEXCEPTION_HPP

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>

using namespace std;

/**
* Namespace to work with Artemis data structures.
*
* @brief Namespace with basic data exceptions and constants.
* @author Marcos Vinicius Naves Bedo
* @author Marcelo Ponciano da Silva
* @version 1.0
*/
namespace artemis{

    /**
    * Base Class to exception of the image extractor.
    *
    * @brief Base class of exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */
    class Exception: public exception{

        private:
              int id;
              string origin;

        public:
             Exception(){
                setID(0);
                setOrigin("NO_REASON_FOUNDED");
             };

             Exception(int id, string origin){
                  setID(id);
                  setOrigin(origin);
             };

             ~Exception() throw(){

             };

             string what() throw(){

                     return "An exception ocurred: " + getOrigin()+".";
             };

             void setID(int id){

                this->id = id;
             };

             void setOrigin(string origin){

                 this->origin = origin;
             };

             int getID(){

                 return this->id;
             };

             string getOrigin(){

                 return this->origin;
             };
    };

    /**
    * File Class to exception of the image extractor.
    *
    * @brief File class of exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */
    class FileException: public Exception{

          private:
              string filename;

          public:
             FileException(){
                setID(1);
                setOrigin("NO_REASON_FOUND");
                setFilename("NO_FILE");
             };

             FileException(int id, string origin, string filename){
                setID(1);
                setOrigin(origin);
                setFilename(filename);
             };

             ~FileException() throw(){

             };

             void setFilename(string filename){
                this->filename = filename;
             };

             string getFilename(){
                return this->filename;
             };
    };

    /**
    * File Class to exception of directories.
    *
    * @brief File class of exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */
    class DirectoryException: public Exception{

        private:
            string dir;

        public:
            DirectoryException(){

                setID(8);
                setOrigin("NO_REASON_FOUND");
                setDirectory(".");
            };

            DirectoryException(int id, string origin, string dir){

                setID(8);
                setOrigin(origin);
                setDirectory(dir);
            };

            ~DirectoryException() throw(){
            };

            void setDirectory(string dir){

                this->dir = dir;
            };

            string getDirectory(){

                return dir;
            }
    };

    /**
    * Memory Control Class to exception of the image extractor.
    *
    * @brief Memory control class of exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */
    class FullHeapException: public Exception{

          private:
              int size;

          public:
             FullHeapException(){
                 setID(2);
                 setOrigin("NO_REASON_FOUND");
                 setSize(0);
             };

             FullHeapException(int id, string origin, int size){

                setID(id);
                setOrigin(origin);
                setSize(size);
             };

             ~FullHeapException() throw(){

             };

             void setSize(int size){

                this->size = size;
             };

             int getSize(){

                 return this->size;
             };
    };

    /**
    * General Class to exception of the image extractor.
    *
    * @brief General class of exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */
    class ImageException: public Exception{

          public:
             ImageException(){
                setID(3);
                setOrigin("NO_REASON_FOUND");
             };

             ImageException(int id, string origin){

                setID(id);
                setOrigin(origin);
             };

             ~ImageException() throw(){

             };
    };

    /**
    * Class of borders exception.
    *
    * @brief Class of borders exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */
    class OutOfBoundsException: public ImageException{

          private:
              int width, height;

          public:
             OutOfBoundsException(){

                setID(4);
                setOrigin("NO_REASON_FOUND");
                setWidth(0);
                setHeight(0);
             };

             OutOfBoundsException(int id, string origin, int w, int h){

                setID(id);
                setOrigin(origin);
                setWidth(w);
                setHeight(h);
             };

             ~OutOfBoundsException() throw(){

             };

             void setWidth(int w){

                 this->width = w;
             };

             void setHeight(int h){

                 this->height = h;
             };

             int getWidth(){

                 return this->width;
             };

             int getHeight(){

                 return this->height;
             };
    };

    /**
    * Validate Class to exception of the image extractor.
    *
    * @brief Validate class of exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */
    class NotAvaliableException: public ImageException{

          public:
             NotAvaliableException(){
                setID(5);
                setOrigin("NO_REASON_FOUND");
             };

             NotAvaliableException(int id, string origin){
                setID(6);
                setOrigin("NO_REASON_FOUND");
             };

             ~NotAvaliableException() throw(){

             };
    };

    /**
    * Validate Class to exception of the TAGS Dicom.
    *
    * @brief Validate class of exception.
    * @author Marcos Vinicius Naves Bedo.
    * @version 1.0.
    */

    class TagNotFoundException: public ImageException{

          private:
             string tag_hexa;
             string tag_name;

          public:
             TagNotFoundException(){
                setID(7);
                setOrigin("TAG_NOT_FOUND");
                setTagHexaValue("");
                setTagName("");
             };

             TagNotFoundException(int id, string origin, string tag_hexa, string tag_name){
                setID(id);
                setOrigin(origin);
                setTagHexaValue(tag_hexa);
                setTagName(tag_name);

             };

             ~TagNotFoundException() throw(){

             };

             void setTagHexaValue(string tag_hexa){

                 this->tag_hexa = tag_hexa;
             };

             void setTagName(string tag_name){

                 this->tag_name = tag_name;
             };

             string getTagHexaValue(){

                 return this->tag_hexa;
             };

             string getTagName(){

                 return this->tag_name;
             };
    };

};

#endif // ARTEMISEXCEPTION_HPP
