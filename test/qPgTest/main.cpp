/*
 * File:   main.cpp
 * Author: pedro
 *
 * Created on 9 de Agosto de 2013, 21:41
 */

// Includes from Qt libraries to database communication
#include <QtSql/QtSql>
#include <QtGui/QApplication>

//--- arboretum src include to avoid having to build the library ---
#include <util/stUtil.cpp>
#include <arboretum/stTreeInformation.cpp>
#include <arboretum/storage/stPage.cpp>
#include <arboretum/storage/stDiskPageManager.h>

////--------------------- arboretum header include ----------------------------
#include <gbdi/datatype/stBasicObjects.h>
#include <gbdi/datatype/byteOIDArrayObject.h>

#include <gbdi/distance/stMetricEvaluators.h>
#include <gbdi/arboretum/slimTree/stSlimTree.h>
#include <gbdi/arboretum/rTree/stRTree.h>


// includes for stPlainDiskPageManager
#include <arboretum/storage/stPlainDiskPageManager.cpp>
#include <QtCore/qglobal.h>

//--------------------- Slim and R type declarations ------------------------
typedef stEuclideanMetricEvaluator < float > myBasicMetricEvaluator;

//--------------------- R type declarations ---------------------------------
typedef stBasicArrayObject <float, int> myBasicArrayObjectRTree;
typedef stResult < myBasicArrayObjectRTree > myResultRTree;
typedef stRTree < float, int > myRTree; // the same types as myBasicArrayObjectRTree

int main(void) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    QString host = "localhost";
    QString dbName = "georeferencer";
    QString userName = "sinetic";
    QString password = "sinetic123";
    int numberOfRows = 20;
    if (db.isValid()) {
        db.setHostName(host);
        db.setDatabaseName(dbName);
        db.setUserName(userName);
        db.setPassword(password);

        if (!db.open()) {
            qDebug() << "Could not connect to database. Exiting";
            exit(1);
        }

        QString query = "SELECT id, ST_AsText(geoloc) AS geometry FROM clients LIMIT :limit;";


        QSqlQuery qsq;
        qsq.prepare(query);
        qsq.bindValue("limit", numberOfRows);

        if (qsq.exec()) {
            int counter = 0;
            qDebug() << "Inserting rows in the R-tree...";
            myBasicArrayObjectRTree **allCities;
            while (qsq.next() && qsq.isValid()) {
                allCities = new myBasicArrayObjectRTree*[qsq.numRowsAffected()];
                qDebug() << "OID: " << qsq.value(0).toString() << " -- GEOM: " << qsq.value(1).toString();
                QString oid = qsq.value(0).toString();
                QString point = qsq.value(1).toString();
                float *latlon = new float[2];
                point.replace(QString("POINT("), QString(""));
                point.replace(QString(")"), QString(""));
                QStringList listLatLon = point.split(" ");
                latlon[0] = listLatLon.at(0).toFloat();
                latlon[1] = listLatLon.at(1).toFloat();

                allCities[counter] = new myBasicArrayObjectRTree(2, latlon);
                allCities[counter++]->SetOID(oid.toInt());
            }
            myBasicMetricEvaluator *me = new myBasicMetricEvaluator();
            stDiskPageManager *pmR = new stDiskPageManager("r.dat", 1024);
            myRTree *rtree = new myRTree(pmR);
            rtree->SetQueryMetricEvaluator(me);
            rtree->SetSplitMethod(myRTree::smQUADRATIC); // smLINEAR, smQUADRATIC, smEXPONENTIAL

            for (int i = 0; i < qsq.numRowsAffected(); i++) {
                rtree->Add(allCities[i]);
            }

            qDebug() << "number of nodes=" << rtree->GetNodeCount() << " height=" << rtree->GetHeight() << endl;

            // reset statistics
            rtree->GetQueryMetricEvaluator()->ResetStatistics();
            rtree->GetPageManager()->ResetStatistics();

            srand(time(NULL));
            int querySubjectIdx = rand() % qsq.numRowsAffected();
            qsq.seek(querySubjectIdx, false); //Seek the record at position querySubjectIdx
            
            // range query
            qDebug() << "Query objectId=" << qsq.value(0).toString() << " lat=" << allCities[querySubjectIdx]->Get(0) << " long=" << allCities[querySubjectIdx]->Get(1) << endl;
            myResultRTree * resultRTree = rtree->RangeQuery(allCities[querySubjectIdx], 0.3);
            qDebug() << "Range query result: " << endl;
            qDebug() << "distancecount=" << rtree->GetQueryMetricEvaluator()->GetDistanceCount() << " diskaccesses=" << rtree->GetPageManager()->GetAccessCount() << endl;
            int i = 0;
            for (myResultRTree::tItePairs it = resultRTree->beginPairs(); it != resultRTree->endPairs(); it++) {
                myBasicArrayObjectRTree *tmpobj = (myBasicArrayObjectRTree *) (*it)->GetObject();
                qDebug() << "[" << ++i << "] (" << *((float *) tmpobj->Get(0)) << ", " << *((float *) tmpobj->Get(1)) << ") " << tmpobj->GetOID() << " (L2=" << (stDistance) (*it)->GetDistance() << ")" << endl;
            }
            delete resultRTree;
        } else {
            qDebug() << "An error has occured.\nMessage:" << qsq.lastError().databaseText();
            exit(1);
        }


    }

}


//
//int main(void) {
//    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
//    QString host = "localhost";
//    QString dbName = "georeferencer";
//    QString userName = "sinetic";
//    QString password = "sinetic123";
//
//    if(db.isValid()){
//        db.setHostName(host);
//        db.setDatabaseName(dbName);
//        db.setUserName(userName);
//        db.setPassword(password);
//
//        if(!db.open()){
//            qDebug() << "Could not connect to database. Exiting";
//            exit(1);
//        }
//
//        //QString query = "SELECT id, rua, ST_AsText(geoloc) AS localizacao FROM clients WHERE rua LIKE :rua;";
//        
//        QString query = "SELECT c.id, SUM(sale_total) AS total_sales, s.sale_date ";
//        query        += "FROM clients c JOIN sales s ON c.id = s.client_id ";
//        query        += "WHERE s.sale_total IS NOT NULL ";
//        query        += "GROUP BY c.id, s.sale_date ";
//        query        += "ORDER BY total_sales DESC; ";
//
//        QSqlQuery qsq;
//        qsq.prepare(query);
//        //qsq.bindValue("rua","%amb%");
//
//        if(qsq.exec()){
//            qDebug() << "Num of rows: " << qsq.numRowsAffected() << ".";
//            qDebug() << "ID --\t\t   SUM \t ---\t DATE ";
//            while(qsq.next() && qsq.isValid()){
//                qDebug() << qsq.value(0).toString() << " ~~ " << qsq.value(1).toString() << " ~~ " << qsq.value(2).toString();
//            }
//        }else{
//            qDebug() << "FALSE";
//            exit(1);
//        }
//
//        query =  "SELECT column_name,data_type ";
//        query += "FROM information_schema.columns ";
//        query += "WHERE table_name = :tablename;";
//        qsq.prepare(query);
//        qsq.bindValue("tablename", "clients");
//
//        if(qsq.exec()){
//            qDebug() << "# Rows: " << qsq.numRowsAffected();
//            while(qsq.next() && qsq.isValid()){
//                qDebug() << "Column:" << qsq.value(0).toString() << "Column type: " << qsq.value(1).toString();
//                if(qsq.value(1).toString() == "USER-DEFINED" ){
//                    qDebug() << "geom";
//                }
//            }
//            qDebug() << "END";
//        }
//
//
//    }
//
//}
