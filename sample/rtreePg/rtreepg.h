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
    #include <stdlib.h>

    #ifdef PG_MODULE_MAGIC
    PG_MODULE_MAGIC;
    #endif

    PG_FUNCTION_INFO_V1(createRtree);
    PG_FUNCTION_INFO_V1(getClientsFromStore);
    PG_FUNCTION_INFO_V1(c_overpaid);

    double _createRtree();
    void _getClientsFromStore(const char *storeGeom, double radius, int storeId);
    compositeArray* _getCliFromStore(const char *storeGeom, double radius);



    /**
        *CREATE FUNCTION c_overpaid(emp, integer) RETURNS boolean
        *   AS 'DIRECTORY/funcs', 'c_overpaid'
        *   LANGUAGE C STRICT;
        *
        * Example got from PG docs:
        *   http://www.postgresql.org/docs/9.1/static/sql-createfunction.html
        */

    Datum
    c_overpaid(PG_FUNCTION_ARGS)
    {
        HeapTupleHeader  t = PG_GETARG_HEAPTUPLEHEADER(0);
        int32            limit = PG_GETARG_INT32(1);
        bool isnull;
        Datum salary;

        salary = GetAttributeByName(t, "salary", &isnull);
        if (isnull) {
            PG_RETURN_BOOL(false);
        }
        /**
             * Alternatively, we might prefer to do PG_RETURN_NULL() for null salary.
             **/

        PG_RETURN_BOOL(DatumGetInt32(salary) > limit);
    }


    /**
            CREATE OR REPLACE FUNCTION createRtree()
                RETURNS INT AS
                '/usr/lib/librtreePg.so', 'createRtree'
                LANGUAGE C STRICT;
          */

    Datum createRtree(PG_FUNCTION_ARGS){
        elog(NOTICE, "Creating rtree... \n");
        int8 tmp = 0;
        tmp =(int8) _createRtree();
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
      *
      Return ctid's from this function

        ### PG Statement creation
        CREATE FUNCTION getCliFromStore (st, DOUBLE PRECISION)
        RETURNS TABLE(ctid tid) AS
        '/usr/lib/librtreePg', 'getCliFromStore'
        LANGUAGE C STRICT;
      */

    Datum getCliFromStore(PG_FUNCTION_ARGS){
        FuncCallContext  *funcctx;
        Datum             result;
        AttInMetadata    *attinmeta;
        HeapTupleHeader t = PG_GETARG_HEAPTUPLEHEADER(0);
        float8 radius = PG_GETARG_FLOAT8(1);

        compositeArray*    allClients;
        TupleDesc         tpdesc;


        // This block does the setup of the function, since to assemble the
        // record set this function are going to be called while it has tuples
        // to insert into this set.
        if (SRF_IS_FIRSTCALL()){// If is the first call of the function

            bool isnull = true;

            MemoryContext oldcontext;
            funcctx = SRF_FIRSTCALL_INIT();
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


            text* storeGeom = DatumGetTextP(GetAttributeByName(t, "geoloc", &isnull));
            if(!isnull){
                char * stGeom = (char *) palloc(VARSIZE(storeGeom));
                stGeom =(char *) VARDATA(storeGeom);
                allClients = _getCliFromStore(stGeom , (double) radius);
                funcctx->user_fctx = (void *)  allClients;
            }else{
                ereport(ERROR,
                        (errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
                         errmsg("Null value not allowed for geom")));
            }



            // Assemble tuple descriptor, use TupleDesc or AttInMetadata

            if (get_call_result_type(fcinfo, NULL, &tpdesc) != TYPEFUNC_COMPOSITE) {
                ereport(ERROR,
                        (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                         errmsg("function returning record called in context that cannot accept type record")));
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


        /* this is just one way we might test whether we are done: */
        if (funcctx->call_cntr < allClients->size) //
        {
            /* Here we want to return another item: */
            // Put here the code to return each position of the array
            HeapTuple tuple;
            bool nulls[2]={false,false};



            char* value = allClients->data[funcctx->call_cntr];
            char **val;
            val = (char **) palloc (2 * sizeof *val);
            val[0] = (char *) CStringGetTextDatum(value);

            /* build a tuple */
            tuple = heap_form_tuple(tpdesc, (Datum*)*val, nulls);

            /* make the tuple into a datum */
            tuple = BuildTupleFromCStrings(funcctx->attinmeta, val);
            result = HeapTupleGetDatum(tuple);

            /* clean up (this is not really necessary) */
            SRF_RETURN_NEXT(funcctx, result);
        }
        else
        {
            /* Here we are done returning items and just need to clean up: */
            // Delete pointers
            SRF_RETURN_DONE(funcctx);
        }


    }

}

#endif // RTREEPG_H
