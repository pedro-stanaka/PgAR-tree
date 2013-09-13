/* 
 * File:   main.cpp
 * Author: pedro
 *
 * Created on 7 de Agosto de 2013, 09:58
 */

#include <cstdlib>
#include <postgresql/libpq-fe.h>
#include <string>
#include <qt4/QtSql/QSqlDatabase>

using namespace std;



/*
 * Simple function that connects to the database just for test
 */
int main(int argc, char** argv) {
    string pg_host = "localhost";
    string pg_port = "5432";
    string pg_database = "georeferencer";
    string pg_user = "postgres";
    string pg_pwd = "pg123";
    PGconn connection = PQsetdbLogin(pg_host.c_str(),
                 pg_port.c_str(),
                 NULL,
                 NULL,
                 pg_database.c_str(),
                 pg_user.c_str(),
                 pg_pwd.c_str());
    
    return 0;
}

