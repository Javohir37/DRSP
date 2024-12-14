#ifndef GETDISTRICTS_H
#define GETDISTRICTS_H

#include <mysql/mysql.h>

char* getDistricts(MYSQL *conn, const char *regionName); // Ensure this matches your implementation

#endif

