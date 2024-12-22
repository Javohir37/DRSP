#ifndef GET_DOCS_H
#define GET_DOCS_H

#include <mysql/mysql.h>

char* getDocs(MYSQL *conn, int hospitalID, const char *spec);

#endif

