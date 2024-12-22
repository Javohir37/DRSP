#ifndef MINRTRGETSCHEDULE_H
#define MINRTRGETSCHEDULE_H

#include <stddef.h>

/**
 * Mini-router for retrieving schedule details.
 * Parses the JSON request, calls getSchedule, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrGetSchedule(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETSCHEDULE_H

