#ifndef GET_HOUR_INFO_H
#define GET_HOUR_INFO_H

#include <mysql/mysql.h>

// Function declaration
char* getHourInfo(MYSQL *conn, int doctorID, const char *dateTime);

#endif

