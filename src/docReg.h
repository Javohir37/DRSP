#ifndef DOC_REG_H
#define DOC_REG_H

#include <mysql/mysql.h>

// Function to register a doctor and return the DoctorID as a JSON string
char* docReg(MYSQL *conn, const char *fullName, const char *spec, const char *role, int hospitalID, const char *phoneNumber, const char *email, const char *password);

#endif // DOC_REG_H

