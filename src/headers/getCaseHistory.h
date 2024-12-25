#ifndef GET_CASE_HISTORY_H
#define GET_CASE_HISTORY_H

#include <mysql/mysql.h>

// Function to retrieve case history as a JSON array for a given patient ID
char* getCaseHistory(MYSQL *conn, int patientID);

#endif // GET_CASE_HISTORY_H

