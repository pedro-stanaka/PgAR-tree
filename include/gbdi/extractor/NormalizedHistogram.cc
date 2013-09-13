/**
 * @file
 *
 * This file implements the Normalized Histogram class.
 */

#include "Extractor.hpp"


//------------------------------------------------------------------------------

template< class SignatureType, stWord numFeatures, stWord normalizationRange, class DataObjectType>
ExtractorReturnCode NormalizedHistogramExtractor<SignatureType, numFeatures, normalizationRange, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) {

    stCount i;
    stCount idxPixelValue;
    double normalizer;
    double * featureVector = NULL;

    //Get the rate to normalize each pixel value
    normalizer = pow(2, (float) image.getBitsPerPixel()) / getNumFeatures();

    //The number of bins exceed the number of colors?
    if (normalizer < 1)
        return EXTR_OUT_OF_BINS;

    featureVector = new double[numFeatures];
    for (i = 0; i < numFeatures; i++) {
        featureVector[i] = 0;
    }

    //Start extracting the histogram.
    //Read each pixel in the image, summing the color ocurrences.
    for (stCount x = 0; x < image.getWidth(); x++) {
        for (stCount y = 0; y < image.getHeight(); y++) {
            //Resize the pixel value according with the Histogram size.
            idxPixelValue = (stCount) ((image.getPixel(x, y).getGrayPixelValue() / normalizer));
            if (idxPixelValue >= numFeatures) {
                delete[] featureVector;
                throw out_of_range("NormalizedHistogram: Index out of range.");
            }
            featureVector[idxPixelValue]++;
        }
    }

    //Is the normalization range defined to [0, 1]?
    if (normalizationRange == 1) {
        //Normalize the histogram in the range [0, 1], making the sum of all bin
        //values equals to 1.
        for (i = 0; i < numFeatures; i++) {
            featureVector[i] = featureVector[i] / image.getSize();
        }
    }
    else {
        //No, the range is [0, normalizationRange].
        //Normalize the histogram in the range [0, 1] and identify the
        //maximum bin value.
        double maxValue = 0;
        for (i = 0; i < numFeatures; i++) {
            featureVector[i] = featureVector[i] / image.getSize();
            if (featureVector[i] > maxValue) {
                maxValue = featureVector[i];
            }
        }

        //Rescale the bin range to [0, normalizationRange]
        for (i = 0; i < numFeatures; i++) {
            featureVector[i] = (featureVector[i] * normalizationRange) / maxValue;
        }
    }

    //Is it necessary to adjust signature size?
    if (numFeatures != sign.GetSize()) {
        sign.SetSize(numFeatures);
    }

    //Populate the sign object with the features computed
    for (i = 0; i < numFeatures; i++) {
        sign.Set(i, featureVector[i]);
    }

    delete[] featureVector;

    return EXTR_SUCCESS;
}

//------------------------------------------------------------------------------