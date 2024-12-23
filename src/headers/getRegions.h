#ifndef GET_REGIONS_H
#define GET_REGIONS_H

#include <mysql/mysql.h>

// Function to retrieve regions and return a JSON-formatted string
char* get_regions(MYSQL *conn);

#endif // GET_REGIONS_H

