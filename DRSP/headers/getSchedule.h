#ifndef GET_SCHEDULE_H
#define GET_SCHEDULE_H

#include <mysql/mysql.h>

// Function declaration for getSchedule
char* getSchedule(MYSQL *conn, const char *date, int doctorID);

#endif // GET_SCHEDULE_H

