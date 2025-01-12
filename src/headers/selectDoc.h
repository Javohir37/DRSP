#ifndef SELECT_DOC_H
#define SELECT_DOC_H

#include <mysql/mysql.h>

char* selectDoc(MYSQL *conn, int hospitalID, const char *fullName);

#endif

