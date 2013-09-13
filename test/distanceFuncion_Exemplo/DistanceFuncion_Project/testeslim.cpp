/*
 * This example can be compiled in GNU g++ using the following command line:
 * gbdiLibs/test/distanceFuncion_Exemplo/DistanceFuncion_Project$ g++ -o testeslim.exe -I../../../include -I../../../src testeslim.cpp 
 */

//--------------------- c/c++ include ---------------------------------------
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <fstream>

//--- arboretum src include to avoid having to build the library ---
#include <util/stUtil.cpp>
#include <arboretum/stTreeInformation.cpp>
#include <arboretum/storage/stPage.cpp>

// includes for stPlainDiskPageManager
#include <arboretum/storage/stPlainDiskPageManager.cpp>

// includes for stDiskPageManager ==> page manager with memory cache
#include <arboretum/storage/stDiskPageManager.cpp>
#include <arboretum/storage/CStorage.cpp>

// includes for stMemoryPageManager
#include <arboretum/storage/stMemoryPageManager.cpp>

// includes for slimTree
#include <arboretum/slimTree/stSlimNode.cpp>

// includes for dummyTree
//#include <stDummyNode.cpp>

// includes for MTree
//#include <stMNode.cpp>

//--------------------- arboretum header include ----------------------------
//#include <gbdi/datatype/stBasicObjects.h>
#include <gbdi/datatype/byteOIDArrayObject.h>

//#include <arboretum/stUserLayerUtil.h>
#include <gbdi/distance/stMetricEvaluators.h>
#include <gbdi/arboretum/slimTree/stSlimTree.h>
#include <gbdi/arboretum/rTree/stRTree.h>

// Page managers
#include <gbdi/arboretum/storage/stPlainDiskPageManager.h>
#include <gbdi/arboretum/storage/stDiskPageManager.h>
#include <gbdi/arboretum/storage/stMemoryPageManager.h>

//--------------------- Slim and R type declarations ------------------------
typedef stEuclideanMetricEvaluator < float > myBasicMetricEvaluator;

//--------------------- Slim type declarations ------------------------------
typedef ByteOIDArrayObject <float> myBasicArrayObject;
typedef stResult < myBasicArrayObject > myResult;
typedef stSlimTree < myBasicArrayObject, myBasicMetricEvaluator > mySlimTree;

//--------------------- R type declarations ---------------------------------
typedef stBasicArrayObject <float, int> myBasicArrayObjectRTree;
typedef stResult < myBasicArrayObjectRTree > myResultRTree;
typedef stRTree < float, int > myRTree;  // the same types as myBasicArrayObjectRTree

//--------------------- main ------------------------------------------------
int main(int argc, char** argv) {
    // read data
    int objectCount = 0;
    char *cityName;
    float *latlon = new float[2];
    char **cityNames = new char*[5507];
    myBasicArrayObject **allCities = new myBasicArrayObject*[5507];
    ifstream in("cities.txt");
    if (!in.is_open()) {
        cout << "Cannot open data file!" << endl;
        return -1;
    }
    else {
        cityName = new char[200];
        while (in.getline(cityName, 200, '\t')){
            in >> latlon[0];
            in >> latlon[1];
            in.ignore();
            cityNames[objectCount] = cityName;
            allCities[objectCount] = new myBasicArrayObject(2, latlon, strlen(cityName)+1, (stByte *) cityName);
            cityName = new char[200];
            objectCount++;
        }
        in.close();
    }

    // Slim-tree test
    cout << "Building Slim-tree... " << endl;
    //stPlainDiskPageManager *pmSlim = new stPlainDiskPageManager("slim.dat", 1024);
    stDiskPageManager *pmSlim = new stDiskPageManager("slim.dat", 1024);
    //stMemoryPageManager *pmSlim = new stMemoryPageManager(1024); // <== Nao funcionou o slim->Add com o stMemoryPageManager; nao encontrou o header no SlimNode::CreateNode
    mySlimTree *slim = new mySlimTree(pmSlim);

    for (int i = 0; i < objectCount; i++) {
        slim->Add(allCities[i]);
    }
    cout << "number of nodes=" << slim->GetNodeCount() << " height=" << slim->GetHeight() << endl;

    int queryobjectid = 3183; //Sao Carlos-SP

    // reset statistics
    slim->GetMetricEvaluator()->ResetStatistics();
    slim->GetPageManager()->ResetStatistics();

    // range query
    cout << "Query object=" << cityNames[queryobjectid] << " lat=" << *((float *) allCities[queryobjectid]->Get(0)) << " long=" << *((float *) allCities[queryobjectid]->Get(1)) << endl;
    myResult *result = slim->RangeQuery(allCities[queryobjectid], 0.3);
    cout << "Range query result: " << endl;
    cout << "distancecount=" << slim->GetMetricEvaluator()->GetDistanceCount() << " diskaccesses=" << slim->GetPageManager()->GetAccessCount() << endl;
    int i=0;
    for (myResult::tItePairs it = result->beginPairs(); it != result->endPairs(); it++){
         myBasicArrayObject *tmpobj = (myBasicArrayObject *)(*it)->GetObject();
         cout << "[" << ++i << "] (" << *((float *) tmpobj->Get(0)) << ", " << *((float *) tmpobj->Get(1)) << ") " << (char *) tmpobj->GetStrOID() << " (L2=" << (stDistance)(*it)->GetDistance() << ")" << endl;
    }
    delete result;

    // reset statistics
    slim->GetMetricEvaluator()->ResetStatistics();
    slim->GetPageManager()->ResetStatistics();

    // knn query
    cout << "Query object=" << cityNames[queryobjectid] << " lat=" << *((float *) allCities[queryobjectid]->Get(0)) << " long=" << *((float *) allCities[queryobjectid]->Get(1)) << endl;
    result = slim->NearestQuery(allCities[queryobjectid], 10);
    cout << "kNN query result: " << endl;
    cout << "distancecount=" << slim->GetMetricEvaluator()->GetDistanceCount() << " diskaccesses=" << slim->GetPageManager()->GetAccessCount() << endl;
    i=0;
    for (myResult::tItePairs it = result->beginPairs(); it != result->endPairs(); it++){
         myBasicArrayObject *tmpobj = (myBasicArrayObject *)(*it)->GetObject();
         cout << "[" << ++i << "] (" << *((float *) tmpobj->Get(0)) << ", " << *((float *) tmpobj->Get(1)) << ") " << (char *) tmpobj->GetStrOID() << " (L2=" << (stDistance)(*it)->GetDistance() << ")" << endl;
    }
    delete result;

    // delete tree
    delete slim;
    delete pmSlim;

    // R-tree test    
    /* int */ objectCount = 0;
    myBasicArrayObjectRTree **allCitiesRTree = new myBasicArrayObjectRTree*[5507];
    /* ifstream */ in.open("cities.txt");
    if (!in.is_open()) {
        cout << "Cannot open data file!" << endl;
        return -1;
    }
    else {
        cityName = new char[200];
        while (in.getline(cityName, 200, '\t')){
            in >> latlon[0];
            in >> latlon[1];
            in.ignore();
            //cityNames[objectCount] = cityName;
            allCitiesRTree[objectCount] = new myBasicArrayObjectRTree(2, latlon);
            allCitiesRTree[objectCount]->SetOID(objectCount);
            cityName = new char[200];
            objectCount++;
        }
        in.close();
    }
    
    cout << endl << "Building R-tree... " << endl;
    myBasicMetricEvaluator *me = new myBasicMetricEvaluator();
    stDiskPageManager *pmR = new stDiskPageManager("r.dat", 1024);
    myRTree *rtree = new myRTree(pmR);
    rtree->SetQueryMetricEvaluator(me);
    rtree->SetSplitMethod(myRTree::smQUADRATIC); // smLINEAR, smQUADRATIC, smEXPONENTIAL

    for (int i = 0; i < objectCount; i++) {
        rtree->Add(allCitiesRTree[i]);
    }
    cout << "number of nodes=" << rtree->GetNodeCount() << " height=" << rtree->GetHeight() << endl;

    // reset statistics
    rtree->GetQueryMetricEvaluator()->ResetStatistics();
    rtree->GetPageManager()->ResetStatistics();

    // range query
    cout << "Query object=" << cityNames[queryobjectid] << " lat=" << *((float *) allCities[queryobjectid]->Get(0)) << " long=" << *((float *) allCities[queryobjectid]->Get(1)) << endl;
    myResultRTree * resultRTree = rtree->RangeQuery(allCitiesRTree[queryobjectid], 0.3);
    cout << "Range query result: " << endl;
    cout << "distancecount=" << rtree->GetQueryMetricEvaluator()->GetDistanceCount() << " diskaccesses=" << rtree->GetPageManager()->GetAccessCount() << endl;
    /* int */ i=0;
    for (myResultRTree::tItePairs it = resultRTree->beginPairs(); it != resultRTree->endPairs(); it++){
         myBasicArrayObjectRTree *tmpobj = (myBasicArrayObjectRTree *)(*it)->GetObject();
         cout << "[" << ++i << "] (" << *((float *) tmpobj->Get(0)) << ", " << *((float *) tmpobj->Get(1)) << ") " << (char *) cityNames[tmpobj->GetOID()] << " (L2=" << (stDistance)(*it)->GetDistance() << ")" << endl;
    }
    delete resultRTree;
        
    // reset statistics
    rtree->GetQueryMetricEvaluator()->ResetStatistics();
    rtree->GetPageManager()->ResetStatistics();

    // knn query
    cout << "Query object=" << cityNames[queryobjectid] << " lat=" << *((float *) allCities[queryobjectid]->Get(0)) << " long=" << *((float *) allCities[queryobjectid]->Get(1)) << endl;
    resultRTree = rtree->NearestQuery(allCitiesRTree[queryobjectid], 10);
    cout << "kNN query result: " << endl;
    cout << "distancecount=" << rtree->GetQueryMetricEvaluator()->GetDistanceCount() << " diskaccesses=" << rtree->GetPageManager()->GetAccessCount() << endl;
    i=0;
    for (myResultRTree::tItePairs it = resultRTree->beginPairs(); it != resultRTree->endPairs(); it++){
         myBasicArrayObjectRTree *tmpobj = (myBasicArrayObjectRTree *)(*it)->GetObject();
         cout << "[" << ++i << "] (" << *((float *) tmpobj->Get(0)) << ", " << *((float *) tmpobj->Get(1)) << ") " << (char *) cityNames[tmpobj->GetOID()] << " (L2=" << (stDistance)(*it)->GetDistance() << ")" << endl;
    }
    delete resultRTree;    
    
    // delete tree
    delete rtree;
    delete pmR;

    return (EXIT_SUCCESS);
}
