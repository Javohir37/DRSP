#ifndef SELECT_HOSPITAL_H
#define SELECT_HOSPITAL_H

#include <mysql/mysql.h>

// Function declaration to select HospitalID and Address based on Name, Region, District, and Address
char* selectHospital(MYSQL *conn, const char *name, const char *region, const char *district, const char *address);

#endif // SELECT_HOSPITAL_H

