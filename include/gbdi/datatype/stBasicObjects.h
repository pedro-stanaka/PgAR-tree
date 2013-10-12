/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
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
 * ====================================================================
 *                                            http://gbdi.icmc.usp.br/
 */
/**
* @file
*
* This file defines a generic array object that implements all methods required
* by the stObject interface. This object may be used in combination with the
* metric evaluators defined in the file stBasicMetricEvaluator.h.
*
* @version 2.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
// Copyright (c) 2002-2003 GBDI-ICMC-USP
#ifndef __STBASICOBJECTS_H
#define __STBASICOBJECTS_H

#include <stdlib.h>

#include <gbdi/datatype/stTypes.h>
#include <gbdi/util/stException.h>
#include <gbdi/datatype/stObject.h>

//==============================================================================
//  class stBasicArrayObject
//------------------------------------------------------------------------------
/**
* This class template implements an array object that can be indexed by all
* metric trees in this library.
*
* <P>This object holds an array of some type and its OID. The OID may be used to
* hold an additional information about the object (like the primary key in a
* database) and will never be used by the distance functions.
*
* <P>The template parameter <b>DataType</b> defines the data type of each unit
* of the array. It may be any C/C++ standard numeric types (integers, characters
* and floating points). The template parameter <b>OIDType</b> defines the type
* of the OID to be used. As <b>DataType</b>, it may assume any numeric type. By
* default, it will always be and <b>int</b> value.
*
* <P>The Serialized version of this class is organized as follows:<br>
* I'll provide the figure later...
*
* <P>This class template was designed to work with the metric evaluators
* defined in stBasicMetricEvaluators.h.
*
* @version 1.1
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @see stObject
* @todo Documentation review.
* @todo It needs optimizations.
* @ingroup user
*/
template <class DataType, class OIDType = int>
class stBasicArrayObject : public stObject{
   public:
      /**
      * Default constructor. It creates an array with 0 elements. Use SetSize()
      * to change the number of entries before use this instance.
      *
      * <P>This constructor is required by the stObject interface.
      */
      stBasicArrayObject(){
         Size = 0;
         TrueData = NULL;
         Data = NULL;
         OID = NULL;
      }//end stBasicArrayObject

      /**
      * Creates a new instance of this class with n entries. If the parameter
      * data is not NULL, the content pointed by it will be used to initializate
      * this instance.
      *
      * @param n Number of entries.
      * @param data The initial values of NULL (default).
      */
      stBasicArrayObject(stSize n, const DataType * data = NULL){

         // Allocate data
         Size = 0;
         TrueData = NULL;
         Data = NULL;
         OID = NULL;
         SetSize(n);

         // Initialize values
         if (data != NULL){
            SetData(data);
         }//end if
      }//end stBasicArrayObject

      /**
      * Disposes this instance and releases all associated resources.
      */
      virtual ~stBasicArrayObject(){

         if (TrueData != NULL){
            delete[] TrueData;
         }//end if
      }//end ~stBasicArrayObject

      /**
      * Copies the content of the array data to this object. The array pointed
      * by data must have at least GetSize() entries.
      *
      * @param data The array with the values.
      */
      void SetData(const DataType * data){

         memcpy(Data, data, sizeof(DataType) * Size);
      }//end SetData

      /**
      * Returns the pointer to the data array.
      */
      DataType * GetData(){
          return Data;
      }//end GetData

      /**
      * Sets a value
      *
      * @param pos The position in the vector
      * @param value The value
      */
      void Set(int pos, DataType value){
          Data[pos] = value;
      }//end SetData

      /**
      * Returns the number of the elements of this array.
      */
      stSize GetSize(){
         return Size;
      }//end stSize

      /**
      * Sets the size of this array. Previous values are maintained, truncating
      * the array to the new size. That is, if the new size is less than the old
      * size, the oversized array positions will be lost.
      *
      * @param n The new number of objects of this array.
      */
      void SetSize(stSize n){
         if (Size != n){
            if (TrueData != NULL){
               stByte * oldTrueData = NULL;
               stSize oldSerializedSize = 0;

               // Save old data.
               oldTrueData = TrueData;
               oldSerializedSize = GetSerializedSize();

               // Create array.
               TrueData = new stByte[(sizeof(DataType) * n) + sizeof(OIDType)];
               Size = n;
               OID = (OIDType *)TrueData;
               Data = (DataType *) (TrueData + sizeof(OIDType));

               // Restore old data, truncating the old array, if necessary.
               memcpy(TrueData, oldTrueData, GetSerializedSize() <= oldSerializedSize ? GetSerializedSize() : oldSerializedSize);

               delete[] oldTrueData;
            }//end if
            else {
               // Create array.
               TrueData = new stByte[(sizeof(DataType) * n) + sizeof(OIDType)];
               Size = n;
               OID = (OIDType *)TrueData;
               Data = (DataType *) (TrueData + sizeof(OIDType));
//               *OID = 0; // Set OID to 0.
            }//end else
         }//end if
      }//end SetSize

      /**
      * This operator can be used to access all positions of the array for
      * reading or writing.
      *
      * @param idx The index of the element.
      * @warning This operator does not check the range of idx.
      */
      DataType & operator [] (int idx){
         return Data[idx];
      }//end operator []

      /**
      * Gets the value of the element at a given position.
      *
      * @param idx The index of the desired entry.
      * @exception out_of_range If the index is out of range.
      */
      //DataType Get(stCount idx);
      void * Get(stCount idx);

      /**
      * Returns the OID associated with this instance.
      *
      * @warning This method returns nothing if the object size is 0.
      */
      long GetOID(){
         if (OID == NULL){
            return 0;
         }else{
            return *OID;
         }//end if
      }//end GetOID

      /**
      * Sets the OID associated with this instance.
      *
      * @param oid The new OID value.
      * @warning This method does nothing if the object size is 0.
      */
      void SetOID(OIDType oid){
         if (OID != NULL){
            *OID = oid;
         }//end if
      }//end SetOID

      // stObject interface
      /**
      * @copydoc stObject::Clone()
      */
      stBasicArrayObject * Clone(){
         stBasicArrayObject * tmp = new stBasicArrayObject(Size, Data);
         tmp->SetOID(this->GetOID());
         return tmp;
      }//end Clone

      /**
      * @copydoc stObject::GetSerializedSize()
      */
      stSize GetSerializedSize(){
         return sizeof(DataType) * Size + sizeof(OIDType);
      }//end GetSerializedSize

      /**
      * @copydoc stObject::Serialize()
      */
      const stByte * Serialize(){
         return TrueData;
      }//end Serialize

      /**
      * @copydoc stObject::Unserialize()
      */
      void Unserialize(const stByte * data, stSize dataSize){

         // Resize data
         SetSize((dataSize - sizeof(OIDType)) / sizeof(DataType));

         memcpy(TrueData, data, dataSize);
      }//end Unserialize

      /**
      * @copydoc stObject::IsEqual()
      */
      bool IsEqual(stObject * obj);
      
   private:
   
      /**
      * Data array. It points to a position in the TrueData array.
      */
      DataType * Data;

      /**
      * OID Type. It points to a position in the TrueData array.
      */
      OIDType * OID;

      /**
      * This is the true data array. Both OID and Data points
      * to positions in this array.
      */
      stByte * TrueData;

      /**
      * Number of entries.
      */
      stSize Size;

};//end stBasicArrayObject

//==============================================================================
//  class stBasicPairArrayObject
//------------------------------------------------------------------------------
template <class DataTypeOne, class DataTypeTwo, class OIDType = int>
class stBasicPairArrayObject : public stObject{
   public:
      /**
      * Default constructor. It creates an array with 0 elements. Use SetSize()
      * to change the number of entries before use this instance.
      *
      * <P>This constructor is required by the stObject interface.
      */
      stBasicPairArrayObject(){
         Size = 0;
         OID = 0;
         DataOne = NULL;
         DataTwo = NULL;
      }//end stBasicArrayObject

      /**
      * Creates a new instance of this class with n entries. If the parameter
      * data is not NULL, the content pointed by it will be used to initializate
      * this instance.
      *
      * @param n Number of entries.
      * @param data The initial values of NULL (default).
      */
      stBasicPairArrayObject(stSize n){
         Size = n;
         OID = 0;
         DataOne = new DataTypeOne[n];
         DataTwo = new DataTypeTwo[n];
      }//end stBasicArrayObject

      /**
      * Disposes this instance and releases all associated resources.
      */
      virtual ~stBasicPairArrayObject(){

         if (DataOne != NULL){
            delete[] DataOne;
         }//end if
         if (DataTwo != NULL){
            delete[] DataTwo;
         }//end if
      }//end ~stBasicArrayObject

      /**
      * Returns the number of the elements of this array.
      */
      stSize GetSize(){
         return Size;
      }//end stSize

      /**
      * Gets the value of the element one at a given position.
      *
      * @param idx The index of the desired entry.
      * @exception out_of_range If the index is out of range.
      */
      DataTypeOne GetDataOne(stCount idx){
         if (idx >= Size){
            throw out_of_range("Index out of range.");
         }else{
            return DataOne[idx];
         }//end if
      }//end GetDataOne
      
      /**
      * Sets the value of the element one at a given position.
      *
      * @param idx The index of the desired entry.
      * @exception out_of_range If the index is out of range.
      */
      void SetDataOne(stCount idx, DataTypeOne val){
         if (idx >= Size){
            throw out_of_range("Index out of range.");
         }else{
            DataOne[idx] = val;
         }//end if
      }//end GetDataOne

      
      /**
      * Gets the value of the element two at a given position.
      *
      * @param idx The index of the desired entry.
      * @exception out_of_range If the index is out of range.
      */
      DataTypeTwo GetDataTwo(stCount idx){
         if (idx >= Size){
            throw out_of_range("Index out of range.");
         }else{
            return DataTwo[idx];
         }//end if
      }//end GetDataTwo
      
      /**
      * Sets the value of the element one at a given position.
      *
      * @param idx The index of the desired entry.
      * @exception out_of_range If the index is out of range.
      */
      void SetDataTwo(stCount idx, DataTypeTwo val){
         if (idx >= Size){
            throw out_of_range("Index out of range.");
         }else{
            DataTwo[idx] = val;
         }//end if
      }//end SetDataTwo

      /**
      * Returns the OID associated with this instance.
      *
      * @warning This method returns nothing if the object size is 0.
      */
      long GetOID(){
         return OID;
      }//end GetOID

      /**
      * Sets the OID associated with this instance.
      *
      * @param oid The new OID value.
      * @warning This method does nothing if the object size is 0.
      */
      void SetOID(OIDType oid){
         OID = oid;
      }//end SetOID

      // stObject interface
      /**
      * @copydoc stObject::Clone()
      */
      stBasicPairArrayObject * Clone(){
         int i;
         stBasicPairArrayObject * tmp = new stBasicPairArrayObject(Size);
         //copy data one and two
         for(i=0; i<Size; i++){
            tmp->SetDataOne(i, this->GetDataOne(i));
            tmp->SetDataTwo(i, this->GetDataTwo(i));
         }//endfor
         //set oid
         tmp->SetOID(this->GetOID());
         return tmp;
      }//end Clone

      /**
      * @copydoc stObject::GetSerializedSize()
      */
      stSize GetSerializedSize(){
         return sizeof(OIDType) + sizeof(stSize) + (sizeof(DataTypeOne) * Size) + (sizeof(DataTypeTwo) * Size);
      }//end GetSerializedSize

      /**
      * @copydoc stObject::Serialize()
      */
      const stByte * Serialize(){
          //serializing
          stByte * serial = new stByte[this->GetSerializedSize()];
          OIDType * serOID = (OIDType *) serial;
          serOID[0] = OID;
          stSize * serSize = (stSize *) (serial + sizeof(OIDType));
          serSize[0] = Size;
          DataTypeOne * serDataOne = (DataTypeOne *) (serial + sizeof(OIDType) + 
                  sizeof(stSize));
          memcpy(serDataOne, DataOne, Size*sizeof(DataTypeOne));
          DataTypeTwo * serDataTwo = (DataTypeTwo *) (serial + sizeof(OIDType) + 
                  sizeof(stSize) + Size*sizeof(DataTypeOne));
          memcpy(serDataTwo, DataTwo, Size*sizeof(DataTypeTwo));
          return serial;
      }//end Serialize

      /**
      * @copydoc stObject::Unserialize()
      */
      void Unserialize(const stByte * data, stSize dataSize){
          //clean
          if (DataOne != NULL){
             delete[] DataOne;
          }//end if
          if (DataTwo != NULL){
             delete[] DataTwo;
          }//end if
          OIDType * serOID = (OIDType *) data;
          OID = serOID[0];
          stSize * serSize = (stSize *) (data + sizeof(OIDType));
          Size = serSize[0];
          if(Size>0){
             DataOne = new DataTypeOne[Size];
             DataTwo = new DataTypeTwo[Size];
             DataTypeOne * serDataOne = (DataTypeOne *) (data + sizeof(OIDType) + 
                  sizeof(stSize));
             memcpy(DataOne, serDataOne, Size*sizeof(DataTypeOne));
             DataTypeTwo * serDataTwo = (DataTypeTwo *) (data + sizeof(OIDType) + 
                  sizeof(stSize) + Size*sizeof(DataTypeOne));
             memcpy(DataTwo, serDataTwo, Size*sizeof(DataTypeTwo));
          }//endif
      }//end Unserialize

      /**
      * @copydoc stObject::IsEqual()
      */
      bool IsEqual(stObject * obj){
         int i = 0;
         if(((stBasicPairArrayObject *)obj)->GetSize() == this->GetSize()){
            //verify
            while((i<this->GetSize()) &&
                    (((stBasicPairArrayObject *)obj)->GetDataOne(i)==this->GetDataOne(i))&&
                    (((stBasicPairArrayObject *)obj)->GetDataTwo(i)==this->GetDataTwo(i))){
                i++;
            }//endwhile
            if(i == this->GetSize()){
                return true;
            }//endif
         }//endif
         return false;
      }
      
   private:
   
      /**
      * Data array. It points to a position in the TrueData array.
      */
      DataTypeOne * DataOne;
      DataTypeTwo * DataTwo;

      /**
      * OID Type. It points to a position in the TrueData array.
      */
      OIDType OID;

      /**
      * Number of entries.
      */
      stSize Size;

};//end stBasicPairArrayObject


//==============================================================================
//  class stBasicStringObject
//------------------------------------------------------------------------------
/**
* This class template implements an string object that can be indexed by all
* metric trees in this library.
*
* <P>This object holds a null terminated string of some type and a OID. The OID
* may be used to hold an additional information about the object (like the
* primary key in a database) and will never be used by the distance functions.
*
* <P>The template parameter <b>DataType</b> defines the data type of each unit
* of the array. It may be any C/C++ standard numeric types (integers, characters
* and floating points). The template parameter <b>OIDType</b> defines the type
* of the OID to be used. As <b>DataType</b>, it may assume any numeric type. By
* default, it will always be and <b>int</b> value.
*
* <P>The Serialized version of this class is organized as follows:<br>
* <CODE>
*           +---------+------+<BR>
*           | OID[10] | Data |<BR>
*           +---------+------+<BR>
* </CODE>
*
* Since Data has a variable size (associated with the string object), the
* serialized form will also have a variable number of bytes.
*
* <P>This class template was designed to work with the metric evaluators
* defined in stBasicMetricEvaluators.h.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @see stObject
* @ingroup user
*/
template <class OIDType = int>
//class stBasicStringObject : public stObject{
class stBasicStringObject {
   public:
      /**
      * Default constructor. It creates an array with 0 elements. Use SetSize()
      * to change the number of entries before use this instance.
      *
      * <P>This constructor is required by the stObject interface.
      */
      stBasicStringObject(){
          serial = 0;
         Data = NULL;
         Len = 0;
         OID = 0;
      }//end stBasicStringObject

      /**
      * Creates a new instance of this class and initializes it with a given
      * null terminated string.
      *
      * @param str A null terminated string.
      */
      stBasicStringObject(const char * str){
          serial =0;
         Data = NULL;
         Len = 0;
         OID = 0;
         SetString(str);
      }//end stBasicStringObject

      /**
      * Creates a new instance of this class and initializes it with a given
      * null terminated string and a oid.
      *
      * @param str A null terminated string.
      */
      stBasicStringObject(const char * str, const OIDType id){
          serial = 0;
         Data = NULL;
         Len = 0;
         SetString(str);
         SetOID(id);
      }//end stBasicStringObject

      /**
      * Creates a new instance of this class and initializes it with a given
      * null terminated string.
      *
      * @param obj A null terminated string.
      */
      stBasicStringObject(stBasicStringObject * obj){
          serial = 0;
         Data = NULL;
         Len = 0;
         OID = 0;
         // Initializing...
         this->SetString(obj->GetString());
         this->SetOID(obj->GetOID());
      }//end stBasicStringObject

      /**
      * Disposes this instance and releases all associated resources.
      */
      virtual ~stBasicStringObject(){
         if (Data != NULL){
            delete[] Data;
            Data = NULL;
            Len = 0;
            OID = 0;
         }//end if
         if(serial != 0){
             delete[] serial;
             serial = 0;
         }
      }//end ~stBasicStringObject

      /**
      * Returns the length of this string. It is a synonym to GetLength().
      * This name is used by the basic metric evaluators templates.
      */
      stSize GetSize(){
         return Len;
      }//end GetSize

      /**
      * This operator can be used to access all positions of the array for
      * reading or writing.
      *
      * @param idx The index of the element.
      * @warning This operator does not check the range of idx.
      */
      char & operator [] (int idx){
          if ((Data==NULL)&&((idx < 0) || (idx >= Len))){
             throw out_of_range("Index out of range.");
          }else{
             return Data[idx];
          }
      }//end operator []

      /**
      * Gets the value of the element at a given position.
      *
      * @param idx The index of the desired entry.
      * @exception out_of_range If the index is out of range.
      */
      char Get(int idx){
          if ((Data==NULL)&&((idx < 0) || (idx >= Len))){
             throw out_of_range("Index out of range.");
          }else{
             return Data[idx];
          }
      }//end Get

      /**
      * Returns the OID associated with this instance.
      *
      * @warning This method returns nothing if the object size is 0.
      */
      OIDType GetOID(){
         return OID;
      }//end GetOID

      /**
      * Sets the OID associated with this instance.
      *
      * @param OID The new OID value.
      * @warning This method does nothing if the object size is 0.
      */
      void SetOID(OIDType oid){
         OID = oid;
      }//end SetOID

      /**
      * Copies a null terminated string to this instance.
      *
      * @param str A null terminates string.
      * @see GetString()
      */
      void SetString(const char * str){
         if(Data != NULL){
             delete [] Data;
         }//end if
         Data = NULL;
         Len = 0;
         if(str!=NULL){
            Len = strlen(str);
            Data = new char[Len+1];
            strcpy(Data, str);
         }//endif
      }//end SetString

      /**
      * Returns the contents of this object as a null terminated string.
      *
      * @warning This method is meaningless if the contents of this instance is
      * not a null terminated string.
      * @see SetString()
      */
      const char * GetString(){
         return Data;
      }//end GetString

      // stObject interface
      /**
      * @copydoc stObject::Clone()
      */
      stBasicStringObject * Clone(){
         stBasicStringObject * tmp = new stBasicStringObject(this->GetString(), this->GetOID());
         return tmp;
      }//end Clone

      /**
      * @copydoc stObject::GetSerializedSize()
      */
      stSize GetSerializedSize(){
         if((Len>0)&&(Data!=NULL)){
            return (10 + Len);
         }else{
            return 10;
         }//endif
      }//end GetSerializedSize

      /**
      * @copydoc stObject::Serialize()
      */
      const stByte * Serialize(){
         //allocation
         if(serial == 0){
             serial = new stByte[GetSerializedSize()+1];
             //oid
             char chrOID[11];
             sprintf(chrOID, "%10d", OID);
             chrOID[10]='\0';
             memcpy(serial, chrOID, 10);
             //String
             if(Data!=NULL){
                memcpy((serial+10), Data, Len);
             }//endif
             serial[GetSerializedSize()] = '\0';
         }
         return serial;
      }//end Serialize

      /**
      * @copydoc stObject::Unserialize()
      */
      void Unserialize(const stByte * data, unsigned size){
         char chrOID[11];
         if((data!=NULL)&&(size>0)){
            // length
            Len = size-10;
            // oid
            strncpy(chrOID, (char*)data, 10);
            chrOID[10]='\0';
            OID = atoi(chrOID);
            //string
            if(Data != NULL){
                delete [] Data;
            }//end if
            if(Len>0){
               Data = new char[Len+1];
               strncpy(Data, (char*)(data+10), Len);
               Data[Len] = '\0';
            }//endif
         }else{
            Len = 0;
            OID = 0;
            Data = NULL;
         }
         if(serial != 0){
             delete[] serial;
             serial = 0;
         }

      }//end Unserialize

      /**
      * @copydoc stObject::IsEqual()
      */
      bool IsEqual(stBasicStringObject * obj){
          return (strcmp(GetString(), obj->GetString()) == 0);
      }// end IsEqual
      

   private:
      stByte* serial;

      /**
      * This is the data array. 
      */
      char * Data;

      /**
      * String length of the Data used.
      */
      stSize Len;

      /**
      * String length of the Data used.
      */
      OIDType OID;
      
};//end stBasicStringObject

// Include implementation
#include "stBasicObjects.cc"

#endif //__STBASICOBJECTS_H
