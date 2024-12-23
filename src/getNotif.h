#ifndef GET_NOTIF_H
#define GET_NOTIF_H

#include <mysql/mysql.h>

char* getNotif(MYSQL *conn, int patientID, const char* lastLoginDate);

#endif

