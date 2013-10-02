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
#include "rtreepg.h"
#include "utils.h"
#include "postgres.h"


// R-tree declarations
typedef stEuclideanMetricEvaluator < float > myBasicMetricEvaluator;
typedef stBasicArrayObject <float, int> myBasicArrayObjectRTree;
typedef stResult < myBasicArrayObjectRTree > myResultRTree;
typedef stRTree < float, int > myRTree; // the same types as myBasicArrayObjectRTree


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

double _createRtree(){
    //    QString query = "SELECT c.id, c.ctid, ST_AsText(c.geoloc), sum(sa.sale_total)  ";
    //    query += "FROM clients c JOIN sales sa ON c.id = sa.client_id, ibge_sectors i  ";
    //    query += "WHERE ST_Contains(i.area_polyg, c.geoloc)  ";
    //    query += "GROUP BY c.id LIMIT 100000;";

    QString query = "SELECT c.id, ST_AsText(c.geoloc)  ";
    query += "FROM clients c  ";
    query += "WHERE geoloc IS NOT NULL;";


    logger.message("METHOD _createRtree");

    logger.message(query.toStdString().c_str());

    int counter = 0;

    QSqlDatabase db = openConnection();

    QSqlQuery qsq;
    qsq.prepare(query);

    if (qsq.exec()) {
        logger.message("Adding nodes to the rtree");
        // Rtree Instatiation
        myBasicMetricEvaluator *me = new myBasicMetricEvaluator();
        stDiskPageManager *pmR = new stDiskPageManager(INDEX_FILE, 1024);
        myRTree *rtree = new myRTree(pmR);
        rtree->SetQueryMetricEvaluator(me);
        rtree->SetSplitMethod(myRTree::smQUADRATIC); // smLINEAR, smQUADRATIC, smEXPONENTIAL

        myBasicArrayObjectRTree *aux = new myBasicArrayObjectRTree(2);

        while (qsq.next() && qsq.isValid()) {
            QString oid = qsq.value(0).toString();
            QString point = qsq.value(1).toString();
            point.replace(QString("POINT("), QString(""));
            point.replace(QString(")"), QString(""));
            QStringList listLatLon = point.split(" ");
            aux->Set(0, listLatLon.at(0).toFloat());
            aux->Set(1, listLatLon.at(1).toFloat());
            aux->SetOID(oid.toInt());
            rtree->Add(aux);
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

    myBasicArrayObjectRTree *storeObj = new myBasicArrayObjectRTree(2, latlon);
    storeObj->SetOID(oid.toInt());

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

    myBasicArrayObjectRTree *storeObj = new myBasicArrayObjectRTree(2, latlon);
    storeObj->SetOID(oid.toInt());

    logger.message("Started range query");

    myResultRTree * resultRTree = rtree->RangeQuery(storeObj, radius);

    logger.message("Finished range query");



    QString retRows = QString::number(resultRTree->GetNumOfEntries());

    logger.message("Num of rows " + retRows.toStdString());

    delete resultRTree;
    delete rtree;
    delete pmR;
    delete me;

    logger.message("END # METHOD _getCliFromStore");
}
