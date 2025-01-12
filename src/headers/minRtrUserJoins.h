#ifndef MINRTRUSERJOINS_H
#define MINRTRUSERJOINS_H

#include <stddef.h>

/**
 * Mini-router function for handling the "userJoins" functionality.
 *
 * @param json_request The JSON request string from the client.
 * @param response_buffer The buffer to store the JSON response.
 * @param buffer_size The size of the response buffer.
 */
void minRtrUserJoins(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRUSERJOINS_H
