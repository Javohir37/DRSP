#ifndef GET_REGIONS_H
#define GET_REGIONS_H

#include <mysql/mysql.h>

// Function declarations
char ***get_regions(MYSQL *conn, MYSQL_FIELD **fields, int *row_count, int *num_fields);
void free_regions(char ***all_rows, int row_count, int num_fields);

#endif

