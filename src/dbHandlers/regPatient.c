#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/regPatient.h"

// Function to register a patient and return the auto-incremented PatientID
// If a patient with the same Name and PhoneNumber exists, return "exists" instead
char* regPatient(MYSQL *conn, const char *name, const char *dob, const char *address, 
                 const char *phoneNumber, const char *email, const char *passportID, 
                 const char *citizenship, const char *password) {
    MYSQL_RES *res;

    // Escape user inputs to prevent SQL injection
    char escapedName[256], escapedDOB[256], escapedAddress[256], escapedPhoneNumber[256], 
         escapedEmail[256], escapedPassportID[256], escapedCitizenship[256], escapedPassword[256];
    
    mysql_real_escape_string(conn, escapedName, name, strlen(name));
    mysql_real_escape_string(conn, escapedDOB, dob, strlen(dob));
    mysql_real_escape_string(conn, escapedAddress, address, strlen(address));
    mysql_real_escape_string(conn, escapedPhoneNumber, phoneNumber, strlen(phoneNumber));
    mysql_real_escape_string(conn, escapedEmail, email, strlen(email));
    mysql_real_escape_string(conn, escapedPassportID, passportID, strlen(passportID));
    mysql_real_escape_string(conn, escapedCitizenship, citizenship, strlen(citizenship));
    mysql_real_escape_string(conn, escapedPassword, password, strlen(password));

    // Check if a patient with the same Name and PhoneNumber already exists
    char selectQuery[1024];
    snprintf(selectQuery, sizeof(selectQuery), 
             "SELECT 1 FROM patients WHERE Name = '%s' AND PhoneNumber = '%s'", 
             escapedName, escapedPhoneNumber);

    if (mysql_query(conn, selectQuery)) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return strdup("{\"error\":\"Database query failed\"}");
    }

    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return strdup("{\"error\":\"Failed to store query result\"}");
    }

    // Check if the row exists
    if (mysql_num_rows(res) > 0) {
        mysql_free_result(res);
        return strdup("{\"status\":\"exists\"}");
    }
    mysql_free_result(res); // Free the result from the SELECT query

    // Dynamically allocate a buffer for the INSERT query to handle large inputs
    size_t queryBufferSize = 4096;
    char *query = malloc(queryBufferSize);
    if (!query) {
        fprintf(stderr, "Memory allocation failed for query buffer\n");
        return strdup("{\"error\":\"Memory allocation failed\"}");
    }

    snprintf(query, queryBufferSize,
             "INSERT INTO patients (Name, DOB, Address, PhoneNumber, Email, PassportID, Citizenship, Password, LastLoginDate) "
             "VALUES ('%s', STR_TO_DATE('%s', '%%Y-%%m-%%d'), '%s', '%s', '%s', '%s', '%s', '%s', NOW())",
             escapedName, escapedDOB, escapedAddress, escapedPhoneNumber, escapedEmail,
             escapedPassportID, escapedCitizenship, escapedPassword);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT INTO patients failed: %s\n", mysql_error(conn));
        free(query);
        return strdup("{\"error\":\"Failed to insert patient\"}");
    }

    // Get the last inserted PatientID
    int patientID = (int)mysql_insert_id(conn);
    free(query); // Free the allocated query buffer

    // Allocate memory for the result and store the PatientID as a string
    static char result[128]; // Static to avoid returning a pointer to local memory
    snprintf(result, sizeof(result), "{\"status\":\"success\",\"PatientID\":%d}", patientID);
    return result; // Return the auto-incremented PatientID as a JSON string
}

