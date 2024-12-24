#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "localmysql.h"  // Ensure this contains the prototype for establish_connection()

/**
 * Function to insert a new user join entry into the IP table with status 'sent'.
 *
 * @param ip The IP address of the user.
 * @param port The port number of the user.
 */
void userJoins(const char *ip, const char *port) {
    // Establish a connection to the database
    MYSQL *conn = establish_connection();
    if (conn == NULL) {
        fprintf(stderr, "Database connection failed\n");
        return;
    }

    // Prepare the SQL query to insert the new entry
    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO ip (IP, PortNum, Status) VALUES ('%s', '%s', 'sent')",
             ip, port);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Database query failed: %s\n", mysql_error(conn));
    } else {
        printf("User join entry inserted successfully\n");
    }

    // Close the database connection
    mysql_close(conn);
}
