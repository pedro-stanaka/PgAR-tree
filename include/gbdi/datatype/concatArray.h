
#ifndef _CONCATARRAY_H
#define	_CONCATARRAY_H

#include <string.h>
#include <gbdi/datatype/stTypes.h>
/**
 * Deve armazenar um vetor dinâmico serializável de objetos serializados
 */
class ConcatArray {
public:
	
	ConcatArray() {
		TrueData = NULL;
	}
	
	ConcatArray(stSize objectSize, stCount numEntries) {
		TrueData = new stByte[sizeof(stCount) + sizeof(stSize) + (objectSize * numEntries)];
		
		DataNumEntries = (stCount *) TrueData;
		*DataNumEntries = numEntries;
		
		ObjectSize = (stSize *) (TrueData + sizeof(stCount));
		*ObjectSize = objectSize;
	}
	
	void setObject(stCount idx, const stByte * data) {
		memcpy((void *) (TrueData + sizeof(stCount) + sizeof(stSize) + (idx * (*ObjectSize))), (void *) data, *ObjectSize);
	}
	
	const stByte * getObject(stCount idx) {
		return (TrueData + sizeof(stCount) + sizeof(stSize) + (idx * (*ObjectSize)));
	}
	
	stSize getObjectSize() {
		return *ObjectSize;
	}
	
	stSize GetSize() {
		return *DataNumEntries;
	}
	
	const stByte * Serialize() {
		return TrueData;
	}
	
	stSize GetSerializedSize() {
		return (sizeof(stCount) + sizeof(stSize) + ((*ObjectSize) * (*DataNumEntries)));
	}
	
	void Unserialize(const stByte * data, stSize dataSize) {
		if (TrueData != NULL) {
			delete[] TrueData;		
		}
		
		TrueData = new stByte[dataSize];
		
		DataNumEntries = (stCount *) TrueData;
		
		ObjectSize = (stSize *) (TrueData + sizeof(stCount));

		memcpy((void *) TrueData, (void *) data, dataSize);
	}
	
private:
	
	/**
     * True Data. Stores DataNumEntries and obj.
     */
    stByte * TrueData;

    /**
     * Number of entries. It points to a position in the TrueData array.
     */
    stSize * DataNumEntries;

    /**
     * Number of entries. It points to a position in the TrueData array.
     */
    stSize * ObjectSize;	

};
#endif	/* _CONCATARRAY_H */