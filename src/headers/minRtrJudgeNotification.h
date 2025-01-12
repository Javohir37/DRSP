#ifndef MINRTR_JUDGE_NOTIFICATION_H
#define MINRTR_JUDGE_NOTIFICATION_H

#include <stddef.h>

// Mini-router for the `judgeNotification` function
// Routes a JSON request to the `judgeNotification` function
// Arguments:
// - json_request: JSON string containing the request
// - response_buffer: Buffer to store the JSON response
// - buffer_size: Size of the response buffer
void minRtrJudgeNotification(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTR_JUDGE_NOTIFICATION_H

