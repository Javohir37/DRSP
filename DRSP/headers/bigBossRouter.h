#ifndef BIGBOSSROUTER_H
#define BIGBOSSROUTER_H

#include <mysql/mysql.h>
#include "localmysql.h"
/**
 * Big Boss Router: Routes the JSON request and writes the response into the provided buffer.
 *
 * @param conn - MySQL connection object
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void bigBossRouter(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // BIGBOSSROUTER_H

