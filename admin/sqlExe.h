#ifndef SQLEXE_H
#define SQLEXE_H

#include <mysql/mysql.h>

// Define a structure to hold the SQL result
typedef struct {
    MYSQL_RES *result;
    int status;
} sqlResult;

/**
 * Executes a given SQL query and returns the result.
 *
 * @param mysqlQuery The SQL query string to execute.
 * @return sqlResult The result of the SQL query execution.
 */
sqlResult sqlExe(const char *mysqlQuery);

#endif // SQLEXE_H
