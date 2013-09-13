PgAR-tree
=========

This project is a implementation of a structure which uses a R-tree and the PostgreSQL DBMS to index geographic data associated with pre-aggregated data. The R-tree access method is implemented by the [Arboretum](http://www.gbdi.icmc.usp.br/old/arboretum/) library which implements a gamma of access methods in C++, but the R-tree is going to be modified in order to do less HDD accesses.

Along with Arboretum this project uses the [QtLibrary](http://qt-project.org/doc/qt-5.1/qtdoc/reference-overview.html) to get full database abstraction.
In order to access PostgreSQL data types the libraries from the source of the DBMS were included as well.

This project is the result of a Conclusion Course work presented to the State University of Londrina. The link to the article that describes the work done will be available here upon completion of it.

![alt text](http://www.uel.br/cce/dc/site2/wp-content/uploads/header_dc_uel3.jpg "State University of Londrina & Computer Departament")

