// minRtrGetCaseHistory.h

#ifndef MIN_RTR_GET_CASE_HISTORY_H
#define MIN_RTR_GET_CASE_HISTORY_H

#include <mysql/mysql.h>

// Function to handle JSON request for retrieving patient case history
char* handleGetCaseHistoryRequest(MYSQL *conn, const char *requestJSON);

#endif // MIN_RTR_GET_CASE_HISTORY_H

