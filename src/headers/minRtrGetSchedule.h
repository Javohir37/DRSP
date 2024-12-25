#ifndef MINRTR_GET_SCHEDULE_H
#define MINRTR_GET_SCHEDULE_H

#include <stddef.h>

// Mini-router for the `getSchedule` function
// Routes a JSON request to the `getSchedule` function
// Arguments:
// - json_request: JSON string containing the request
// - response_buffer: Buffer to store the JSON response
// - buffer_size: Size of the response buffer
void minRtrGetSchedule(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTR_GET_SCHEDULE_H

