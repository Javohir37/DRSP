#ifndef MINRTRGETIP_H
#define MINRTRGETIP_H

#include <stddef.h>

/**
 * Mini-router function for handling the "getIP" functionality.
 *
 * @param json_request The JSON request string from the client.
 * @param response_buffer The buffer to store the JSON response.
 * @param buffer_size The size of the response buffer.
 */
void minRtrGetIP(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETIP_H
