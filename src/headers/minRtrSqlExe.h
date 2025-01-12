#ifndef MINRTR_SQLEXE_H
#define MINRTR_SQLEXE_H

#include <stddef.h>

/**
 * Mini-router function for the sqlExe feature.
 *
 * @param json_request The incoming JSON request as a string.
 * @param response_buffer The buffer to store the response JSON.
 * @param buffer_size The size of the response buffer.
 */
void minRtrSqlExe(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTR_SQLEXE_H

