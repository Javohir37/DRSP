#ifndef GET_DOC_NOTIFICATION_H
#define GET_DOC_NOTIFICATION_H

#include <mysql/mysql.h>

// Function to retrieve notifications for a doctor
// Returns a JSON array string
char* getDocNotification(MYSQL *conn, const char *doctorID, const char *lastLogin);

#endif // GET_DOC_NOTIFICATION_H

