#ifndef MINRTRGETHOSPITALS_H
#define MINRTRGETHOSPITALS_H

#include <stddef.h>

/**
 * Mini-router for retrieving hospital details.
 * Parses the JSON request, calls getHospitals, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrGetHospitals(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRGETHOSPITALS_H

