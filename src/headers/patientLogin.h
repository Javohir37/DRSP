#ifndef PATIENT_LOGIN_H
#define PATIENT_LOGIN_H

#include <mysql/mysql.h>

// Function to validate patient login and return a JSON response
char* patientLogin(MYSQL *conn, const char *patientID, const char *password);

#endif // PATIENT_LOGIN_H

