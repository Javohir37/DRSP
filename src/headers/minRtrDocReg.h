#ifndef MINRTRDOCREG_H
#define MINRTRDOCREG_H

#include <stddef.h>

/**
 * Mini-router for doctor registration.
 * Parses the JSON request, calls docReg, and writes the response.
 *
 * @param json_request - JSON string containing the request
 * @param response_buffer - Buffer to store the response
 * @param buffer_size - Size of the response buffer
 */
void minRtrDocReg(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRDOCREG_H

