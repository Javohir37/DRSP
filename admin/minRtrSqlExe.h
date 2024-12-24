#ifndef MINRTRSQLEXE_H
#define MINRTRSQLEXE_H

#include <stddef.h>

/**
 * Mini-router function for handling the "sqlExe" functionality.
 *
 * @param json_request The JSON request string from the client.
 * @param response_buffer The buffer to store the JSON response.
 * @param buffer_size The size of the response buffer.
 */
void minRtrSqlExe(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRSQLEXE_H
