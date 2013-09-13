#ifndef METADATAEXTRACTOR_HPP
#define METADATAEXTRACTOR_HPP

#include <stdlib.h>
#include <gbdi/util/stException.h>
#include <gbdi/image/ImageBase.hpp>
#include <gbdi/extractor/Extractor.hpp>

template< class SignatureType,
stWord numMetadata = 15,
class DataObjectType = DCMImage >
class MetadataExtractor : public Extractor<SignatureType, DataObjectType> {
public:

    /**
     * Empty class constructor.
     */
    MetadataExtractor() {
    }

    /**
     * Class destructor.
     */
    virtual ~MetadataExtractor() {
    }

    /**
     * Get the number of metadata.
     *
     * @return The number of metadata.
     */
    stWord getNumMetadata() {
        return numMetadata;
    }

  

    virtual ExtractorReturnCode generateSignature(const DataObjectType & image, SignatureType & sign);

}; 

//include implementation (template class)
#include "MetadataExtractor.cc"

#endif

