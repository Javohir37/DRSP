#ifndef MINRTRMODIFYAPP_H
#define MINRTRMODIFYAPP_H

#include <stddef.h>

/**
 * Mini-router for modifying appointment status.
 * Parses the JSON request, calls modifyApp, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrModifyApp(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRMODIFYAPP_H

