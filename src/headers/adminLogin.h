#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <mysql/mysql.h>

char* adminLogin(MYSQL *conn, int adminID, const char *password);

#endif

