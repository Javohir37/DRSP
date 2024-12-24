#ifndef MINRTRADMINLOGIN_H
#define MINRTRADMINLOGIN_H

#include <mysql/mysql.h>

// Mini-router for adminLogin
void minRtrAdminLogin(const char *json_request, char *response_buffer, size_t buffer_size);

#endif

