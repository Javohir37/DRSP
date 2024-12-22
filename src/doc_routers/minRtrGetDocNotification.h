#ifndef MINRTRGETDOCNOTIFICATION_H
#define MINRTRGETDOCNOTIFICATION_H

#include <stddef.h>

/**
 * Mini-router for retrieving doctor notifications.
 * Parses the JSON request, calls getDocNotification, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrGetDocNotification(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETDOCNOTIFICATION_H

