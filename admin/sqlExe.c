#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "localmysql.h"

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
sqlResult sqlExe(const char *mysqlQuery) {
    sqlResult res;
    res.result = NULL;
    res.status = 0;

    // Establish a connection to the database
    MYSQL *conn = establish_connection();
    if (conn == NULL) {
        fprintf(stderr, "Database connection failed\n");
        res.status = -1;
        return res;
    }

    // Execute the SQL query
    if (mysql_query(conn, mysqlQuery)) {
        fprintf(stderr, "Database query failed: %s\n", mysql_error(conn));
        res.status = -1;
    } else {
        res.result = mysql_store_result(conn);
        if (res.result == NULL && mysql_field_count(conn) > 0) {
            fprintf(stderr, "Failed to retrieve query results: %s\n", mysql_error(conn));
            res.status = -1;
        }
    }

    // Clean up the database connection
    mysql_close(conn);
    return res;
}
