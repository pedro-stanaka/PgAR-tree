/* 
 * File:   byteOIDArrayObject.cc
 * Author: dskaster
 * 
 * Created on 14 de Janeiro de 2009, 18:15
 */

#ifdef _BYTEOIDARRAYOBJECT_H

#include <fstream>

#include "byteOIDArrayObject.h"

template <class DataType>
ByteOIDArrayObject< DataType >::ByteOIDArrayObject() {
    TrueData = NULL;
    Data = NULL;
    StrOID = NULL;
    DataNumEntries = NULL;
    StrOIDSize = 0;
}//en stBasicStrUIDArrayObject

template <class DataType>
ByteOIDArrayObject< DataType >::ByteOIDArrayObject(stSize dataNumEntries, const DataType * data, stSize strOIDSize, const stByte * strOID) {

//    if ((dataNumEntries == 0) || (data == NULL) || (strOIDSize == 0) || (strOID == NULL)) {
//        throw logic_error("Wrong StrOIDArrayObject initialization.");
//    }
//
//    //StrOIDSize = strlen(strOID) + 1; // +1 to include the end-of-string (\0) using memcpy
//    StrOIDSize = strOIDSize;
//
//    // Allocate data
//    TrueData = new stByte[sizeof (stSize) + (sizeof (DataType) * dataNumEntries) + StrOIDSize];
//
//    DataNumEntries = (stSize *) TrueData;
//    *DataNumEntries = dataNumEntries;
//
//    Data = (DataType *) (TrueData + sizeof (stSize));
//    memcpy(Data, data, (*DataNumEntries * sizeof (DataType)));
//
//    StrOID = (stByte *) (TrueData + sizeof (stSize) + (*DataNumEntries * sizeof (DataType)));
//    memcpy(StrOID, strOID, StrOIDSize);
    TrueData = NULL;
    Data = NULL;
    StrOID = NULL;
    DataNumEntries = NULL;
    StrOIDSize = 0;

    SetSize(dataNumEntries, data, strOIDSize, strOID);
} //end stBasicStrUIDArrayObject

template <class DataType>
void ByteOIDArrayObject< DataType >::SetSize(stSize dataNumEntries, const DataType * data, stSize strOIDSize, const stByte * strOID) {

    if ((dataNumEntries == 0) || ((DataNumEntries != NULL) && (dataNumEntries == *DataNumEntries) && (StrOIDSize == strOIDSize)))  {
        return;
    }

    StrOIDSize = strOIDSize;

    // Allocate data
    if (TrueData != NULL)
      delete[] TrueData;
    TrueData = new stByte[sizeof (stSize) + (sizeof (DataType) * dataNumEntries) + StrOIDSize];

    DataNumEntries = (stSize *) TrueData;
    *DataNumEntries = dataNumEntries;

    Data = (DataType *) (TrueData + sizeof (stSize));
    SetData(data);

    StrOID = (stByte *) (TrueData + sizeof (stSize) + (*DataNumEntries * sizeof (DataType)));
    SetStrOID(strOIDSize, strOID);

} //end SetSize

template <class DataType>
void ByteOIDArrayObject< DataType >::Unserialize(const stByte * data, stSize dataSize) {

    if (TrueData != NULL)
      delete[] TrueData;

    TrueData = new stByte[dataSize];
    memcpy(TrueData, data, dataSize);

    DataNumEntries = (stSize *) TrueData;

    Data = (DataType *) (TrueData + sizeof (stSize));

    StrOIDSize = dataSize - sizeof (stSize) - (*DataNumEntries * sizeof (DataType));

    StrOID = (stByte *) (TrueData + (dataSize - StrOIDSize));

}//end Unserialize

template <class DataType>
bool ByteOIDArrayObject< DataType >::IsEqual(stObject * obj) {
    if (*DataNumEntries != ((ByteOIDArrayObject*) obj)->GetSize()) {
        return false;
    }//end if

    // Scanning...
    bool equal = true;
    stCount i = 0;
    while (i < *DataNumEntries) {
        if (Data[i] != ((ByteOIDArrayObject*) obj)->Data[i]) {
            equal = false;
            break;
        }
        i++;
    }//end while

    return equal;
} //end IsEqual

#endif
