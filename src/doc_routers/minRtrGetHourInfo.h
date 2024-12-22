#ifndef MINRTRGETHOURINFO_H
#define MINRTRGETHOURINFO_H

#include <mysql/mysql.h>

/**
 * Mini-router for getHourInfo.
 * Parses the JSON request, calls getHourInfo, and writes the response.
 *
 * @param conn - MySQL connection object
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrGetHourInfo(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETHOURINFO_H

