#ifndef GETIP_H
#define GETIP_H

#include <mysql/mysql.h>

/**
 * Function to retrieve IP addresses and port numbers from the database based on status.
 *
 * @param conn The MySQL connection object.
 * @param status The status to filter IP addresses.
 * @return A JSON-formatted string with IP addresses and port numbers.
 */
char* getIP(MYSQL *conn, const char *status);

#endif // GETIP_H
