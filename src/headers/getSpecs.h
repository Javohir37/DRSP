#ifndef GET_SPECS_H
#define GET_SPECS_H

#include <mysql/mysql.h>

// Fetch all distinct specializations from a hospital and return as JSON.
char* getSpecs(MYSQL *conn, const char *hospitalID);

#endif // GET_SPECS_H

