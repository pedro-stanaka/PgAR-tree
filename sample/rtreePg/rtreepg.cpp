// Includes from Qt libraries to database communication
#include <QtSql/QtSql>
#include <QtGui/QApplication>

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


//--------------------- arboretum header include ----------------------------
//#include <gbdi/datatype/stBasicObjects.h>
#include <gbdi/datatype/byteOIDArrayObject.h>

//#include <arboretum/stUserLayerUtil.h>
#include <gbdi/distance/stMetricEvaluators.h>
#include <gbdi/arboretum/rTree/stRTree.h>

// Page managers
#include <gbdi/arboretum/storage/stPlainDiskPageManager.h>
#include <gbdi/arboretum/storage/stDiskPageManager.h>
#include <gbdi/arboretum/storage/stMemoryPageManager.h>



// Headers includes
#include "string"
#include "rtreepg.h"
#include "utils.h"
#include <stdio.h>
#include <sstream>


// R-tree declarations
typedef ByteOIDArrayObject< float > myOIDArrayObjectRTree;
typedef stEuclideanMetricEvaluator < float > myBasicMetricEvaluator;
typedef stManhatanMetricEvaluator< float > myBasicMetricEvaluator2;
typedef stResult < myOIDArrayObjectRTree > myResultRTree;
typedef stRTree < float > myRTree; // the same types as myBasicArrayObjectRTree



void loginfo(const char * info){
    logger.message(info);
}


QSqlDatabase openConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    QString host = "localhost";
    QString dbName = "georeferencer";
    QString userName = "sinetic";
    QString password = "sinetic123";

    if (db.isValid()) {
        db.setHostName(host);
        db.setDatabaseName(dbName);
        db.setUserName(userName);
        db.setPassword(password);

        if (!db.open()) {
            logger.error("Could not connect to database. Exiting...");
            logger.error(db.lastError().text().toStdString().c_str());
        }else{
        }
    }else{
        logger.error(db.lastError().text().toStdString().c_str());
    }
    return db;
}


bool closeConnection( QSqlDatabase db ){
    db.close();
}


/**
  * This function is a C++ wrapper to access R*-Tree from Arboretum Libraries.
  * For more info:
  * @see createRtree
  */
double _createRtree(){

    QString query = "SELECT c.ctid, ST_AsText(c.geoloc)  ";
    query += "FROM clients c;";
    float *latlon = new float[2];


    logger.message("METHOD _createRtree");

    logger.message(query.toStdString().c_str());

    int counter = 0;

    QSqlDatabase db = openConnection();

    QSqlQuery qsq;
    qsq.prepare(query);

    if (qsq.exec()) {

        // Rtree Instatiation
        myBasicMetricEvaluator *me = new myBasicMetricEvaluator();
        stDiskPageManager *pmR = new stDiskPageManager(INDEX_FILE, PAGE_SIZE);
        myRTree *rtree = new myRTree(pmR);
        rtree->SetQueryMetricEvaluator(me);
        rtree->SetSplitMethod(myRTree::smQUADRATIC); // smLINEAR, smQUADRATIC, smEXPONENTIAL

        logger.message("Number of entries: " + QString::number(qsq.numRowsAffected()).toStdString());
        myOIDArrayObjectRTree * aux;
        logger.message("Adding nodes to the rtree");
        int i = 0;
        while (qsq.next() && qsq.isValid()) {

            QString oid = qsq.value(0).toString();
            QString point = qsq.value(1).toString();
            point.replace(QString("POINT("), QString(""));
            point.replace(QString(")"), QString(""));
            QStringList listLatLon = point.split(" ");
            latlon[0] = listLatLon.at(0).toFloat();
            latlon[1] = listLatLon.at(1).toFloat();
            aux = new myOIDArrayObjectRTree(2, latlon, oid.length()+1, (stByte *) oid.toStdString().c_str());
            rtree->Add(aux);

            if( ++i % (qsq.numRowsAffected()/10) == 0){
                logger.message("Processed " + QString::number(i).toStdString() + " entries.");
            }
        }
        logger.message("DONE! Adding nodes to the rtree");



        closeConnection(db);
        delete rtree;
        delete pmR;
        delete me;
        delete aux;
        //        delete[] rows;
    }else{
        logger.error(db.lastError().text().toStdString().c_str());
    }

    return (double) counter;
}


void _getClientsFromStore(const char* storeGeom, double radius, int storeId = NULL){

    logger.message("METHOD getClientsFromStore");

    QString oid = QString::number(storeId);
    QString point = QString(storeGeom);
    float *latlon = new float[2];
    point.replace(QString("POINT("), QString(""));
    point.replace(QString(")"), QString(""));
    QStringList listLatLon = point.split(" ");
    latlon[0] = listLatLon.at(0).toFloat();
    latlon[1] = listLatLon.at(1).toFloat();

    logger.message("Assembled query subject object");
    myBasicMetricEvaluator *me = new myBasicMetricEvaluator();
    stDiskPageManager *pmR = new stDiskPageManager(INDEX_FILE);
    myRTree *rtree = new myRTree(pmR);
    rtree->SetQueryMetricEvaluator(me);
    rtree->GetQueryMetricEvaluator()->ResetStatistics();
    rtree->GetPageManager()->ResetStatistics();

    myOIDArrayObjectRTree *storeObj = new myOIDArrayObjectRTree(2, latlon); //, 5, (stByte *) "store"

    logger.message("Started range query");

    myResultRTree * resultRTree = rtree->RangeQuery(storeObj, radius);

    logger.message("Finished range query");



    QString retRows = QString::number(resultRTree->GetNumOfEntries());

    logger.message("Num of rows " + retRows.toStdString());

    delete resultRTree;
    delete rtree;
    delete pmR;
    delete me;
}

compositeArray* _getCliFromStore(const char *storeGeom, double radius){
    logger.message("METHOD getClientsFromStore");

    QString point = QString(storeGeom);
    float *latlon = new float[2];
    point.replace(QString("POINT("), QString(""));
    point.replace(QString(")"), QString(""));
    QStringList listLatLon = point.split(" ");
    latlon[0] = listLatLon.at(0).toFloat();
    latlon[1] = listLatLon.at(1).toFloat();

    myBasicMetricEvaluator *me = new myBasicMetricEvaluator();
    stDiskPageManager *pmR = new stDiskPageManager(INDEX_FILE);
    myRTree *rtree = new myRTree(pmR);
    rtree->SetQueryMetricEvaluator(me);
    rtree->GetQueryMetricEvaluator()->ResetStatistics();
    rtree->GetPageManager()->ResetStatistics();

    myOIDArrayObjectRTree *storeObj = new myOIDArrayObjectRTree(2, latlon, 5, (stByte *) "store");

    logger.message("Started range query");


    myResultRTree * resultRTree = rtree->RangeQuery(storeObj, radius);


    logger.message("Number of entries retrieved: " + QString::number(resultRTree->GetNumOfEntries()).toStdString());

    int resultBytesSize = 0;
    myOIDArrayObjectRTree *tmpobj;
    for (myResultRTree::tItePairs it = resultRTree->beginPairs(); it != resultRTree->endPairs(); ++it){
         tmpobj = (myOIDArrayObjectRTree *) (*it)->GetObject();
        resultBytesSize += tmpobj->GetStrOIDSize();
    }

    compositeArray *resultArray = new compositeArray();
    resultArray->size = resultRTree->GetNumOfEntries();
    resultArray->data = new char*[resultArray->size];


    int i = 0;
    for (myResultRTree::tItePairs it = resultRTree->beginPairs(); it != resultRTree->endPairs(); ++it){
        tmpobj = (myOIDArrayObjectRTree *) (*it)->GetObject();
        resultArray->data[i] = new char[tmpobj->GetStrOIDSize()];
        memcpy(resultArray->data[i], tmpobj->GetStrOID(), tmpobj->GetStrOIDSize());
        logger.message("Distance #" + QString::number(i).toStdString()+ ":  " + QString::number((*it)->GetKey(), 'g', 15).toStdString());
        i++;
    }

    logger.message("Finished range query");


    delete resultRTree;
    delete rtree;
    delete pmR;
    delete me;

    logger.message("END # METHOD _getCliFromStore");

    return resultArray;
} // End -- _getCliFromStore
