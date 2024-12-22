#ifndef MINRTRGETDOCS_H
#define MINRTRGETDOCS_H

#include <mysql/mysql.h>

// Mini-router function declaration
void minRtrGetDocs(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETDOCS_H

