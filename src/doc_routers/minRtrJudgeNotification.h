#ifndef MINRTRJUDGENOTIFICATION_H
#define MINRTRJUDGENOTIFICATION_H

#include <mysql/mysql.h>

/**
 * Mini-router for judgeNotification.
 * Parses the JSON request, calls judgeNotification, and writes the response.
 *
 * @param conn - MySQL connection object
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrJudgeNotification(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRJUDGENOTIFICATION_H

