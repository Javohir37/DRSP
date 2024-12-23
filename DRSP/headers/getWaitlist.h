#ifndef GET_WAITLIST_H
#define GET_WAITLIST_H

#include <mysql/mysql.h>

// Function to retrieve the waitlist for a specific date and doctor
char* getWaitlist(MYSQL *conn, const char *date, int doctorID);

#endif // GET_WAITLIST_H

