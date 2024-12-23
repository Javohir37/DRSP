// minRtrPationLogin.h

#ifndef MIN_RTR_PATIENT_LOGIN_H
#define MIN_RTR_PATIENT_LOGIN_H

#include <mysql/mysql.h>

// Function to handle patient login request and generate a response
char* handlePatientLoginRequest(MYSQL *conn, const char *requestJSON);

#endif // MIN_RTR_PATIENT_LOGIN_H
