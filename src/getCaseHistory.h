// getCaseHistory.h

#ifndef GET_CASE_HISTORY_H
#define GET_CASE_HISTORY_H

#include <mysql/mysql.h>

// Function to retrieve case history for a specific patient
char* getCaseHistory(MYSQL *conn, int patientID);

#endif // GET_CASE_HISTORY_H
