#ifndef PATIENT_LOGIN_H
#define PATIENT_LOGIN_H

#include <mysql/mysql.h>

char* patientLogin(MYSQL *conn, int patientID, const char *password);

#endif // PATIENT_LOGIN_H

