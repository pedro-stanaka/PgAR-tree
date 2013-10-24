#ifndef RTREEPG_H
#define RTREEPG_H

#include "rtreePg_global.h"

#include "logger.h"
Logger logger("/var/lib/postgresql/pgrtree.log");



extern "C"{

    #include "postgres.h"
    #include "fmgr.h"
    #include "funcapi.h"
    #include "storage/itemptr.h"
    #include "executor/executor.h"  /* for GetAttributeByName() */
    #include "utils/builtins.h"
    #include "storage/itemptr.h"
    #include "utils.h"
    #include <stdlib.h>
    #include <stdio.h>

    #ifdef PG_MODULE_MAGIC
    PG_MODULE_MAGIC;
    #endif






    /**
          * C++ Function prototypes
          */
    double _createRtree();
    void _getClientsFromStore(const char *storeGeom, double radius, int storeId);
    compositeArray* _getCliFromStore(const char *storeGeom, double radius);

    // Logger C bridge
    void loginfo(const char *);

    /**
      -- CREATE STATEMENT
    CREATE OR REPLACE FUNCTION createRtree()
        RETURNS VOID AS
        '/usr/lib/librtreePg.so', 'createRtree'
        LANGUAGE C STRICT;
              */
    PG_FUNCTION_INFO_V1(createRtree);

    Datum createRtree(PG_FUNCTION_ARGS){
        elog(NOTICE, "Creating rtree... \n");
        _createRtree();
        elog(NOTICE, "DONE! Creating rtree... \n");
    }




    /**
             *
                CREATE OR REPLACE FUNCTION getClientsFromStore(INT, double precision, INT DEFAULT NULL)
                    RETURNS VOID AS
                    '/usr/lib/librtreePg', 'getClientsFromStore'
                    LANGUAGE C STRICT;
             *
             */
    PG_FUNCTION_INFO_V1(getClientsFromStore);
    void getClientsFromStore(PG_FUNCTION_ARGS){

        // Get Text field
        text* storeGeom = PG_GETARG_TEXT_PP(0);
        char * stGeom = (char *) palloc(VARSIZE(storeGeom));
        stGeom =(char *) VARDATA(storeGeom);

        float8 radius = PG_GETARG_FLOAT8(1);
        int32 storeId;
        if(!PG_ARGISNULL(2)){
            storeId = PG_GETARG_INT32(0);
        }

        _getClientsFromStore(stGeom , (double) radius, (int) storeId);

        pfree((void *) stGeom);
    }



    /**

      Return ctid's from this function

        CREATE TYPE __myTid AS (mytid tid);

        -- ### PG Statement creation
        CREATE OR REPLACE FUNCTION getCliFromStore (TEXT, DOUBLE PRECISION)
        RETURNS SETOF __myTid AS
        '/usr/lib/librtreePg', 'getCliFromStore'
        LANGUAGE C STRICT;



        -- Get result from function:
        SELECT *
        FROM clients
        WHERE ctid IN (
            SELECT *
            FROM getCliFromStore((SELECT ST_AsText(geom) FROM stores WHERE id = 472), 1.2)
        );
          */

    PG_FUNCTION_INFO_V1(getCliFromStore);
    Datum getCliFromStore(PG_FUNCTION_ARGS){

        FuncCallContext  *funcctx;
        Datum             result;
        AttInMetadata    *attinmeta;
        bool isnull = true;


        isnull = PG_ARGISNULL(0);


        if(isnull){
            ereport(ERROR,
                    (errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
                     errmsg("Null value not allowed for geom")));
        }
        float8 radius = PG_GETARG_FLOAT8(1);

        text * storeGeom = PG_GETARG_TEXT_P(0);
        compositeArray    *allClients;
        TupleDesc          tpdesc;

        // This block does the setup of the function, since to assemble the
        // record set this function are going to be called while it has tuples
        // to insert into this set.
        if (SRF_IS_FIRSTCALL()){// If is the first call of the function
            loginfo("METHOD getClifromSt = NO WRAPPER");
            MemoryContext oldcontext;
            funcctx = SRF_FIRSTCALL_INIT();
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


            // Assemble tuple descriptor, use TupleDesc or AttInMetadata
            if (get_call_result_type(fcinfo, NULL, &tpdesc) != TYPEFUNC_COMPOSITE) {
                ereport(ERROR,
                        (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                         errmsg("Function returning record called in context that cannot accept type record")));
            }

            if(storeGeom != NULL){
                char * stGeom = (char *) palloc(VARSIZE(storeGeom));
                stGeom =(char *) VARDATA(storeGeom);
                allClients = _getCliFromStore(stGeom , (double) radius);

                funcctx->user_fctx = (void *) allClients;
                //TODO: Pesquisar se o allclients não vai ser liberado para re-uso de memória por outro processo
            }else{
                ereport(ERROR,
                        (errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
                         errmsg("Null value not allowed for geom")));
            }

            /*
                 * generate attribute metadata needed later to produce tuples from raw
                 * C strings
                 */
            attinmeta = TupleDescGetAttInMetadata(tpdesc);
            funcctx->attinmeta = attinmeta;


            MemoryContextSwitchTo(oldcontext);


        } // end -- if isfirstcall



        // Each call setup
        funcctx = SRF_PERCALL_SETUP();
        allClients = (compositeArray*) funcctx->user_fctx;
        attinmeta = funcctx->attinmeta;

        char** val;

            /* this is just one way we might test whether we are done: */
            if (funcctx->call_cntr <  allClients->size){ //allClients->size-1

                /* Here we want to return another item: */
                // Put here the code to return each position of the array
                HeapTuple tuple;

                val = (char **) palloc(sizeof(char *));
                val[0] = (char *) palloc(strlen(allClients->data[funcctx->call_cntr]) * sizeof(char) + 1);

                memcpy(val[0], allClients->data[funcctx->call_cntr], strlen(allClients->data[funcctx->call_cntr])+1);
                tuple = BuildTupleFromCStrings(attinmeta, val);

                pfree(val[0]);
                pfree(val);

                result = HeapTupleGetDatum(tuple);

                SRF_RETURN_NEXT(funcctx, result);
            }
            else
            {
                /* Here we are done returning items and just need to clean up: */
                loginfo("Cleaning variables");
                freeCompositeArray(allClients);
                loginfo("END PG_METHOD GETCLI");
                loginfo("          ");
                SRF_RETURN_DONE(funcctx);
            }

    } // End -- getCliFromStore

}// EXTERN "C"

#endif // RTREEPG_H
