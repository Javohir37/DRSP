#ifndef MINRTRGETDOCS_H
#define MINRTRGETDOCS_H

#include <stddef.h>

/**
 * Mini-router for handling getDocs requests.
 * Parses the JSON request, calls getDocs, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrGetDocs(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETDOCS_H

