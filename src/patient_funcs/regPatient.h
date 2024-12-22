// regPatient.h

#ifndef REG_PATIENT_H
#define REG_PATIENT_H

#include <mysql/mysql.h>

// Function to register a patient and return the auto-incremented PatientID as a string
// If the patient with the same Name and PhoneNumber exists, return "exists" instead
char* regPatient(MYSQL *conn, const char *name, const char *dob, const char *address,
                 const char *phoneNumber, const char *email, const char *passportID,
                 const char *citizenship, const char *password);

#endif // REG_PATIENT_H

