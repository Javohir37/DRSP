#ifndef MINRTRREGPATIENT_H
#define MINRTRREGPATIENT_H

#include <stddef.h>

// Mini-router function for registering a patient
void minRtrRegPatient(const char *json_request, char *response_buffer, size_t buffer_size);

#endif // MINRTRREGPATIENT_H

