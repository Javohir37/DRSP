#ifndef GET_CASE_HISTORY_H
#define GET_CASE_HISTORY_H

#include <mysql/mysql.h>

// Function declaration to get the case history for a given PatientID and return it as a string
char* getCaseHistory(MYSQL *conn, int patientID);

#endif // GET_CASE_HISTORY_H

