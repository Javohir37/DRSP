#ifndef MINRTRGETDISTRICTS_H
#define MINRTRGETDISTRICTS_H

#include <mysql/mysql.h>

void minRtrGetDistricts(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETDISTRICTS_H

