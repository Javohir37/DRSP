#ifndef MINRTRGETCASEHISTORY_H
#define MINRTRGETCASEHISTORY_H

#include <mysql/mysql.h>

void minRtrGetCaseHistory(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETCASEHISTORY_H

