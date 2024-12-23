#ifndef GET_DOC_NOTIFICATION_H
#define GET_DOC_NOTIFICATION_H

#include <mysql/mysql.h>

// Function declaration for getDocNotification
char* getDocNotification(MYSQL *conn, int doctorID, const char *lastLogin);

#endif // GET_DOC_NOTIFICATION_H

