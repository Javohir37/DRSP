#ifndef MINRTR_GET_NOTIF_H
#define MINRTR_GET_NOTIF_H

#include <stddef.h>

/**
 * Mini-router for `getNotif`.
 * Parses the JSON request, calls the `getNotif` function, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrGetNotif(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTR_GET_NOTIF_H

