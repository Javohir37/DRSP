#ifndef MODIFY_APP_H
#define MODIFY_APP_H

#include <mysql/mysql.h>

// Function declaration for modifyApp
char* modifyApp(MYSQL *conn, int appointmentID, const char *status);

#endif // MODIFY_APP_H

