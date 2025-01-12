#ifndef MINRTR_REG_PATIENT_H
#define MINRTR_REG_PATIENT_H

#include <stddef.h>

// Mini-router function declaration for `regPatient`
// Routes a JSON request to the `regPatient` function
// Arguments:
// - json_request: JSON string containing the request
// - response_buffer: Buffer to store the JSON response
// - buffer_size: Size of the response buffer
void minRtrRegPatient(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTR_REG_PATIENT_H

