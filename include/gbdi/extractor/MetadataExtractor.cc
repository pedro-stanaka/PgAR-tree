
#include "Extractor.hpp"


//------------------------------------------------------------------------------

template< class SignatureType, stWord numMetadata, class DataObjectType>
ExtractorReturnCode MetadataExtractor<SignatureType, numMetadata, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) {


    //DCMImage img = (DCMImage)image;

    //Is it necessary to adjust signature size?
    if (numMetadata != sign.GetSize()) {
        sign.SetSize(numMetadata);
    }

    //Populate the sign object with the features computed
    stCount i = 0;

    //0 - DICOM_PatientAge  = 0x00101010
    string value;
	try {
    value = image.getStringTag(DCMImage::DICOM_PatientAge);
	value.replace(3, 3, ""); //retrieve "Y" of the date
    
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
	i++;

    //1 - DICOM_PatientWeight  = 0x00101030
	try {
		value = image.getStringTag(DCMImage::DICOM_PatientWeight);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;

    //2 - DICOM_SliceThickness  = 0x00180050
    
	try {
		value = image.getStringTag(DCMImage::DICOM_SliceThickness);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;
	
    //3 - DICOM_RepetitionTime  = 0x00180080
    try {
		value = image.getStringTag(DCMImage::DICOM_RepetitionTime);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;


    //4 - DICOM_EchoTime
	try {
		value = image.getStringTag(DCMImage::DICOM_EchoTime);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;
 

    //5 - DICOM_InversionTime = 0x00180082
	//try {
	//	value = image.getStringTag(DCMImage::DICOM_InversionTime);
	//}catch (exception &e) {
     //   value = "0";
    //}
	//if(value.compare("") == 0){
	//	value = "0";
	//}
	//sign.Set(i, (double) atof(value.c_str()));
    //i++;

    //6 - DICOM_NumberOfAverages = 0x00180083
	try {
		value = image.getStringTag(DCMImage::DICOM_NumberOfAverages);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;

    //7 - DICOM_ImagingFrequency = 0x00180084
	try {
		value = image.getStringTag(DCMImage::DICOM_ImagingFrequency);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
	i++;

    //8 - DICOM_EchoNumbers = 0x00180086,
	try {
		value = image.getStringTag(DCMImage::DICOM_EchoNumbers);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;

    //9 - DICOM_MagneticFieldStrength = 0x00180087,
	try {
		value = image.getStringTag(DCMImage::DICOM_MagneticFieldStrength);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;

    //10 - DICOM_SpacingBetweenSlices = 0x00180088,
	try {
		value = image.getStringTag(DCMImage::DICOM_SpacingBetweenSlices);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;

    //11 - DICOM_EchoTrainLength = 0x00180091,
	try {
		value = image.getStringTag(DCMImage::DICOM_EchoTrainLength);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;

    //12 - DICOM_PercentSampling = 0x00180093,
	try {
		value = image.getStringTag(DCMImage::DICOM_PercentSampling);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;

    //13 - DICOM_PercentPhaseFieldOfView = 0x00180094,
	try {
		value = image.getStringTag(DCMImage::DICOM_PercentPhaseFieldOfView);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;
    
    //14 - DICOM_ACR_NEMA_Location = 0x00200050,,
	try {
		value = image.getStringTag(DCMImage::DICOM_ACR_NEMA_Location);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;
            
    //15 - DICOM_SliceLocation = 0x00201041,,
	try {
		value = image.getStringTag(DCMImage::DICOM_SliceLocation);
	}catch (exception &e) {
        value = "0";
    }
	if(value.compare("") == 0){
		value = "0";
	}
	sign.Set(i, (double) atof(value.c_str()));
    i++;




    //sign.Set(i, image.getStringTag(DCMImage::DICOM_SeriesDescription));i++;
    //             
    //sign.Set(i, "DICOM_AdmittingDiagnosesDescription");i++;
    //sign.Set(i, valueSemanticNetwork(image.getStringTag(DCMImage::DICOM_AdmittingDiagnosesDescription));i++;
    //             newSignature->SetData(getSelectedSignature());


    return EXTR_SUCCESS;
}

//------------------------------------------------------------------------------