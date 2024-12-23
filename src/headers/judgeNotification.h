#ifndef JUDGE_NOTIFICATION_H
#define JUDGE_NOTIFICATION_H

#include <mysql/mysql.h>

// Function declaration for judgeNotification
char* judgeNotification(MYSQL *conn, int id, const char *status, const char *tableName);

#endif // JUDGE_NOTIFICATION_H

