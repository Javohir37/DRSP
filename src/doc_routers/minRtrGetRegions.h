#ifndef MINRTRGETREGIONS_H
#define MINRTRGETREGIONS_H

#include <mysql/mysql.h>

void minRtrGetRegions(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETREGIONS_H

