#ifndef DOC_LOGIN_H
#define DOC_LOGIN_H

#include <mysql/mysql.h>

// Function to validate doctor login using DoctorID and password
char* docLogin(MYSQL *conn, int doctorID, const char *password);

#endif // DOC_LOGIN_H

