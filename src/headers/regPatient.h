#ifndef REG_PATIENT_H
#define REG_PATIENT_H

#include <mysql/mysql.h>

// Function to register a patient and return the result in JSON format
// Parameters:
// - conn: A pointer to the MYSQL connection object
// - name: The patient's full name
// - dob: The patient's date of birth in 'YYYY-MM-DD' format
// - address: The patient's address
// - phoneNumber: The patient's phone number
// - email: The patient's email address (optional, can be NULL or empty)
// - passportID: The patient's passport ID
// - citizenship: The patient's citizenship
// - password: The patient's password
//
// Returns:
// - A JSON string indicating success, failure, or if the patient already exists
char* regPatient(MYSQL *conn, const char *name, const char *dob, const char *address, 
                 const char *phoneNumber, const char *email, const char *passportID, 
                 const char *citizenship, const char *password);

#endif // REG_PATIENT_H

