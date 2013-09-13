/* 
 * File:   storaclearrayobject.h
 * Author: dskaster
 *
 * Created on 14 de Janeiro de 2009, 18:15
 */

#ifndef _BYTEOIDARRAYOBJECT_H
#define	_BYTEOIDARRAYOBJECT_H

#include <string.h>

#include <gbdi/datatype/stTypes.h>
#include <gbdi/util/stException.h>
#include <gbdi/datatype/stObject.h>

template <class DataType>
class ByteOIDArrayObject : public stObject {
public:

    /**
     * Creates an empty new instance of this class.
     */
    ByteOIDArrayObject();

    /**
     * Creates a new instance of this class with n entries.
     *
     * @param dataNumEntries Number of entries.
     * @param data The initial array.
     * @param strOIDSize The size of String OID.
     * @param strOID The initial String OID.
     * @exception logic_error If any parameter is invalid.
     */
    ByteOIDArrayObject(stSize dataNumEntries, const DataType * data = NULL, stSize strOIDSize = 0, const stByte * strOID = NULL);

    /**
     * Disposes this instance and releases all associated resources.
     */
    virtual ~ByteOIDArrayObject() {
        if (TrueData != NULL)
            delete[] TrueData;
        TrueData = NULL;
    }//end ~stBasicArrayObject

    /**
     * Returns the number of the elements of this array.
     */
    stSize GetSize() {
        if (DataNumEntries != NULL)
            return *DataNumEntries;
        else
            return 0;
    }//end stSize

    /**
     * Sets the number of the elements of this array.
     *
     * @param dataNumEntries Number of entries.
     * @warning This operation destroy the previous array content.
     */
    void SetSize(stSize dataNumEntries, const DataType * data = NULL, stSize strOIDSize = 0, const stByte * strOID = NULL);

    /**
     * Copies the content of the array data to this object. The array pointed
     * by data must have at least GetSize() entries.
     *
     * @param data The array with the values.
     */
    void SetData(const DataType * data) {
        if (data != NULL) {
            memcpy(Data, data, (*DataNumEntries * sizeof (DataType)));
        }
    }


    /**
     * Gets the value of the element at a given position.
     *
     * @param idx The index of the desired entry.
     * @exception out_of_range If the index is out of range.
     */
    //DataType Get(stCount idx) {

    void * Get(stCount idx) {
        if (idx >= GetSize())
            throw out_of_range("Index out of range.");
        //return Data[idx];
        return &Data[idx];
    }

    /**
     * Sets the value of the element at a given position.
     *
     * @param idx The index of the desired entry.
     * @exception out_of_range If the index is out of range.
     */
    void Set(stCount idx, DataType value) {
        if (idx >= GetSize())
            throw out_of_range("Index out of range.");
        Data[idx] = value;
    }

    /**
     * Gets the String OID.
     */
    const stByte * GetStrOID() {
        return StrOID;
    }

    /**
     * Gets the size of the String OID.
     */
    stSize GetStrOIDSize() {
        return StrOIDSize;
    }

    /**
     * Sets the String OID.
     */
    void SetStrOID(stSize strOIDSize, const stByte * strOID) {
        if (strOID != NULL) {
            if (StrOIDSize != strOIDSize) {
                /* Save old data */
                DataType * oldData = new DataType[*DataNumEntries];
                memcpy(oldData, Data, (*DataNumEntries * sizeof (DataType)));

                /* Resize object, saving oldData and new strOID */
                SetSize(*DataNumEntries, oldData, strOIDSize, strOID);

                /* Free resources */
                delete[] oldData;
            } else {
                memcpy(StrOID, strOID, StrOIDSize);
            }
        }
    }

    /**
     * @copydoc stObject::Clone()
     */
    ByteOIDArrayObject * Clone() {
        ByteOIDArrayObject * tmp = new ByteOIDArrayObject(*DataNumEntries, Data, StrOIDSize, StrOID);
        return tmp;
    }//end Clone

    /**
     * @copydoc stObject::GetSerializedSize()
     */
    stSize GetSerializedSize() {
        return sizeof (stSize) + (*DataNumEntries * sizeof (DataType)) + StrOIDSize;
    }//end GetSerializedSize

    /**
     * @copydoc stObject::Serialize()
     */
    const stByte * Serialize() {
        return TrueData;
    }//end Serialize

    /**
     * @copydoc stObject::Unserialize()
     */
    void Unserialize(const stByte * data, stSize dataSize);

    /**
     * @copydoc stObject::IsEqual()
     */
    bool IsEqual(stObject * obj);

private:

    /**
     * True Data. Stores DataNumEntries, Data and StrOID contents.
     */
    stByte * TrueData;

    /**
     * Number of entries. It points to a position in the TrueData array.
     */
    stSize * DataNumEntries;

    /**
     * Data array. It points to a position in the TrueData array.
     */
    DataType * Data;

    /**
     * String OID. It points to a position in the TrueData array.
     */
    stByte * StrOID;

    /**
     * The size of String OID.
     */
    stSize StrOIDSize;

}; //end ByteOIDArrayObject

// Include implementation
#include "byteOIDArrayObject.cc"

#endif	/* _BYTEOIDARRAYOBJECT_H */
