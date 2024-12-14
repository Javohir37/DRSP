#ifndef GETHOSPITALS_H
#define GETHOSPITALS_H

#include <mysql/mysql.h>

// Function declaration
char* getHospitals(MYSQL *conn, const char *region, const char *district);

#endif // GETHOSPITALS_H

