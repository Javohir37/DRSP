#ifndef GETSPECS_H
#define GETSPECS_H

#include <mysql/mysql.h>

// Function declaration: Returns JSON string of all distinct specializations in a hospital
char* getSpecs(MYSQL *conn, int hospitalID);

#endif

