/**
 * @file
 *
 * This file contains a simple example of manipulation of DICOM images using the
 * GBDI/Artemis library.
 *
 * @version 1.0
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @date 01-02-2011
 */


#include <stdlib.h>
#include <iostream>
#include <string>

/* Artemis includes */
#include <gbdi/image/DcmLib.hpp> //Handle DICOM images
#include <gbdi/extractor/NormalizedHistogram.hpp> //Normalized histogram color image extractor
#include <gbdi/extractor/HaralickExtractor.hpp> //Haralick texture image extractor
#include <gbdi/datatype/byteOIDArrayObject.h> //Object signature (n-dimensional array)

/* Include the required cpp files to also compile them */
#include <image/DcmLib.cpp>
#include <image/ImageBase.cpp>
#include <image/Pixel.cpp>
#include <extractor/HaralickExtractor.cpp>
#include <extractor/HaralickFeature.cpp>

typedef ByteOIDArrayObject<float> ObjectSignature;

/**
 * DCMImage test.
 *
 * NOTE: It is necessary to provide the DICOM data dictionary, which is operation
 * system dependent.
 *
 * On GNU/Linux this can be done:
 * a) setting the variable DCM_DICT_DEFAULT_PATH=/path/to/dcmtk/lib/dicom.dic in
 * the DCMTK file /path/to/dcmtk/include/dcmtk/config/cfunix.h
 * 
 * b) setting the environment variable as follows (bash syntax):
 * $ export DCMDICTPATH=/path/to/dcmtk/lib/dicom.dic
 */
int main(int argc, char** argv) {

    DCMImage img;
    DCMImage * clone = NULL;
    string newTagValue = "";
    NormalizedHistogramExtractor<ObjectSignature, 256, 256> histoExt;
    HaralickExtractor<ObjectSignature> haralickExt;
    ObjectSignature obj;

    try {
        img.openImage("1.dcm");
    } catch (artemis::FileException * eFile) {
        cout << eFile->what() << endl;
    }

    try {
        cout << "Testing tag read..." << endl;
        cout << "StudyInstanceUID: " << img.getStringTag(DCMImage::DICOM_StudyInstanceUID) << endl;
        cout << "SeriesDescription: " << img.getStringTag(DCMImage::DICOM_SeriesDescription) << endl;
        cout << "AdmittingDiagnoses: " << img.getStringTag(DCMImage::DICOM_AdmittingDiagnosesDescription) << endl << endl;

        cout << "Testing tag update..." << endl;
        clone = img.clone(); //save img status
        cout << "Provide a new value for the SeriesDescription tag (this update will not be saved in the original image): ";
        getline(cin, newTagValue);
//
        img.setStringTag(DCMImage::DCMImage::DICOM_SeriesDescription, newTagValue);
        cout << "Old SeriesDescription: " << clone->getStringTag(DCMImage::DICOM_SeriesDescription) << endl;
        cout << "New SeriesDescription: " << img.getStringTag(DCMImage::DICOM_SeriesDescription) << endl << endl;
        delete clone;

        cout << "Testing feature extraction..." << endl;
        ExtractorReturnCode status;

        //Normalized histogram
        status = histoExt.generateSignature(img, obj);
        if (status != EXTR_SUCCESS) {
            cout << "Erro: " << status << endl;
            return(EXIT_FAILURE);
        }
        cout << "Histogram: (";
        for (unsigned short i = 0; i < (obj.GetSize() - 1); i++) {
            cout << *((float *) obj.Get(i)) << ", ";
        }
        cout << *((float *) obj.Get(obj.GetSize() - 1)) << ")" << endl << endl;

        //Haralick
        status = haralickExt.generateSignature(img, obj);
        if (status != EXTR_SUCCESS) {
            cout << "Erro: " << status << endl;
            return(EXIT_FAILURE);
        }
        cout << "Haralick: (";
        for (unsigned short i = 0; i < (obj.GetSize() - 1); i++) {
            cout << *((float *) obj.Get(i)) << ", ";
        }
        cout << *((float *) obj.Get(obj.GetSize() - 1)) << ")" << endl << endl;

        //Done
        cout << "Finished." << endl;
    return (EXIT_SUCCESS);
}

