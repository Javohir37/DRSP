#ifndef MINRTR_GET_DOC_NOTIFICATION_H
#define MINRTR_GET_DOC_NOTIFICATION_H

#include <stddef.h>

// Mini-router function for `getDocNotification`
// Arguments:
// - json_request: JSON string containing the request
// - response_buffer: Buffer to store the JSON response
// - buffer_size: Size of the response buffer
void minRtrGetDocNotification(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTR_GET_DOC_NOTIFICATION_H

