#ifndef MINRTR_PATIENT_LOGIN_H
#define MINRTR_PATIENT_LOGIN_H

#include <stddef.h>

// Mini-router for the `patientLogin` function
// Routes a JSON request to the `patientLogin` function
// Arguments:
// - json_request: JSON string containing the request
// - response_buffer: Buffer to store the JSON response
// - buffer_size: Size of the response buffer
void minRtrPatientLogin(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTR_PATIENT_LOGIN_H

