// minRtrGetSpecs.h
#ifndef MINRTRGETSPECS_H
#define MINRTRGETSPECS_H

#include <stddef.h>

/**
 * Mini-router for fetching specializations.
 * Parses the JSON request, calls getSpecs, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrGetSpecs(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETSPECS_H

