#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "regPatient.h"

// Function to register a patient and return the auto-incremented PatientID
// If the patient with the same Name and PhoneNumber exists, return "exists" instead 
// Done by Davlatbek Kobiljonov
char* regPatient(MYSQL *conn, const char *name, const char *dob, const char *address, 
                 const char *phoneNumber, const char *email, const char *passportID, 
                 const char *citizenship, const char *password) {
    char query[2048];
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Escape user inputs to prevent SQL injection
    char escapedName[256], escapedPhoneNumber[256];
    mysql_real_escape_string(conn, escapedName, name, strlen(name));
    mysql_real_escape_string(conn, escapedPhoneNumber, phoneNumber, strlen(phoneNumber));

    // Check if a patient with the same Name and PhoneNumber already exists
    snprintf(query, sizeof(query), 
             "SELECT 1 FROM patients WHERE Name = '%s' AND PhoneNumber = '%s'", 
             escapedName, escapedPhoneNumber);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return "error"; // Return error message on query failure
    }

    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return "error"; // Return error message if the result is null
    }

    // Check if the row exists
    if (mysql_num_rows(res) > 0) {
        mysql_free_result(res);
        return "exists"; // Return "exists" if a patient with the same Name and PhoneNumber exists
    }
    mysql_free_result(res); // Free the result from the SELECT query

    // Construct the INSERT query if the patient doesn't exist
    if (email && strlen(email) > 0) {
        // Email is provided
        snprintf(query, sizeof(query),
                 "INSERT INTO patients (Name, DOB, Address, PhoneNumber, Email, PassportID, Citizenship, Password, LastLoginDate) "
                 "VALUES ('%s', STR_TO_DATE('%s', '%%d/%%m/%%Y'), '%s', '%s', '%s', '%s', '%s', '%s', NOW())",
                 escapedName, dob, address, escapedPhoneNumber, email, passportID, citizenship, password);
    } else {
        // Email is not provided
        snprintf(query, sizeof(query),
                 "INSERT INTO patients (Name, DOB, Address, PhoneNumber, Email, PassportID, Citizenship, Password, LastLoginDate) "
                 "VALUES ('%s', STR_TO_DATE('%s', '%%d/%%m/%%Y'), '%s', '%s', NULL, '%s', '%s', '%s', NOW())",
                 escapedName, dob, address, escapedPhoneNumber, passportID, citizenship, password);
    }

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT INTO patients failed: %s\n", mysql_error(conn));
        return "error"; // Return error message on query failure
    }

    // Get the last inserted PatientID
    int patientID = (int)mysql_insert_id(conn);
    
    // Allocate memory for the result and store the PatientID as a string
    static char result[64]; // Static to avoid returning a pointer to local memory
    snprintf(result, sizeof(result), "%d", patientID);
    return result; // Return the auto-incremented PatientID as a string
}

