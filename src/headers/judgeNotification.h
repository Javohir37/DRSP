#ifndef JUDGE_NOTIFICATION_H
#define JUDGE_NOTIFICATION_H

#include <mysql/mysql.h>

// Function to update the booking status in the appropriate tableName
// Parameters:
// - conn: MYSQL connection object
// - id: ID of the record as a string
// - status: New status for the record
// - tableName: Table name ('appointments' or 'waitlist')
// Returns: A success or error message
char* judgeNotification(MYSQL *conn, const char *id, const char *status, const char *tableName);

#endif // JUDGE_NOTIFICATION_H

