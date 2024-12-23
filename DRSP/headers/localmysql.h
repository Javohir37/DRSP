#ifndef LOCALMYSQL_H
#define LOCALMYSQL_H

#include <mysql/mysql.h>

/**
 * Establish a connection to the MySQL database.
 *
 * @return MYSQL* - Pointer to the MySQL connection object
 */
MYSQL* establish_connection();

/**
 * Log MySQL error and terminate the program.
 *
 * @param con - MYSQL connection object
 */
void finish_with_error(MYSQL *con);

#endif // LOCALMYSQL_H

