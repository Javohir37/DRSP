#ifndef MINRTRDOCLOGIN_H
#define MINRTRDOCLOGIN_H

#include <mysql/mysql.h>

void minRtrDocLogin(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRDOCLOGIN_H

