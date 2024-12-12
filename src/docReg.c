#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "docReg.h"

// Function to register a doctor and return a JSON string containing the DoctorID
char* docReg(MYSQL *conn, const char *fullName, const char *spec, const char *role, int hospitalID, const char *phoneNumber, const char *email, const char *password) {
    char query[1024];

    // Construct the INSERT query with the new password field
    snprintf(query, sizeof(query), 
             "INSERT INTO doctors (FullName, Spec, Role, HospitalID, PhoneNumber, Email, password, LastLogin) "
             "VALUES ('%s', '%s', '%s', %d, '%s', '%s', '%s', NOW())", 
             fullName, spec, role, hospitalID, phoneNumber, email, password);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT INTO doctors failed: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default message on error
    }

    // Get the last inserted DoctorID
    int doctorID = (int)mysql_insert_id(conn);

    // Allocate memory and generate JSON containing the DoctorID
    size_t buffer_size = 128; // Buffer for JSON output
    char *json = (char *)malloc(buffer_size);
    if (!json) {
        fprintf(stderr, "Memory allocation failed for JSON\n");
        return "we did not get your output"; // Return default message on error
    }

    snprintf(json, buffer_size, "{\"DoctorID\": %d}", doctorID);

    return json; // Return the dynamically allocated JSON string
}

