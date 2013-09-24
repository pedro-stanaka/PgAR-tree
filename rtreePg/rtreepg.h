#ifndef RTREEPG_H
#define RTREEPG_H

#include "rtreePg_global.h"

#include "logger.h"
Logger logger("/var/lib/postgresql/pgrtree.log");

extern "C"{

    #include "postgres.h"
    #include "fmgr.h"
    #include "funcapi.h"
    #include "executor/executor.h"  /* for GetAttributeByName() */
    #include <stdlib.h>

    #ifdef PG_MODULE_MAGIC
        PG_MODULE_MAGIC;
    #endif

    PG_FUNCTION_INFO_V1(createRtree);
    PG_FUNCTION_INFO_V1(getClientsFromStore);
    PG_FUNCTION_INFO_V1(c_overpaid);

    double _createRtree();
    void _getClientsFromStore(const char *storeGeom, double radius);

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
    *CREATE FUNCTION c_overpaid(emp, integer) RETURNS boolean
    *   AS 'DIRECTORY/funcs', 'c_overpaid'
    *   LANGUAGE C STRICT;
    */


    Datum createRtree(PG_FUNCTION_ARGS){
        elog(NOTICE, "Creating rtree... \n");
        int8 tmp = 0;
        tmp =(int8) _createRtree();
        elog(NOTICE, "DONE! Creating rtree... \n");
    }



    void getClientsFromStore(PG_FUNCTION_ARGS){

        // Get Text field
        text* storeGeom = PG_GETARG_TEXT_PP(0);
        char * stGeom = palloc(VARSIZE(storeGeom));
        stGeom =(char *) VARDATA(storeGeom);

        float8 radius = PG_GETARG_FLOAT8(1);

        if(!PG_ARGISNULL(2)){
            int32 storeId = PG_GETARG_INT32(0);
        }

//        const char* ret =
//           _getClientsFromStore((int) storeId, (double) radius);
           _getClientsFromStore(stGeom , (double) radius);

          pfree((void *) stGeom);
//        const char * notice = strcat("Number of rows returned ", ret);

//        elog(NOTICE, notice);

    }

}

#endif // RTREEPG_H
