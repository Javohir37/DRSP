#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/docLogin.h"

// Function to validate doctor login using DoctorID and password
char* docLogin(MYSQL *conn, int doctorID, const char *password) {
    char query[1024];
    MYSQL_RES *result;
    MYSQL_ROW row;

	
	
    // Construct the SELECT query to fetch all columns except password
    snprintf(query, sizeof(query), 
             "SELECT DoctorID, FullName, Spec, Role, HospitalID, PhoneNumber, Email, LastLogin FROM doctors WHERE DoctorID = %d AND password = '%s'", 
             doctorID, password);
	
    // Execute the query
    if (mysql_query(conn, query)==1) {
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default message on error
    }

    // Store the query result
    result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Failed to store result: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default message on error
    }

    // Check if any rows were returned
    if ((row = mysql_fetch_row(result)) == NULL) {
        mysql_free_result(result); // Free result before returning
        return "False"; // Return False if no matching doctorID or password
    }

    // Successful login: Get the required data from the result
    int fetchedDoctorID = atoi(row[0]);
    const char *fullName = row[1];
    const char *spec = row[2];
    const char *role = row[3];
    const char *hospitalID = row[4];
    const char *phoneNumber = row[5];
    const char *email = row[6];
    const char *lastLogin = row[7]; // LastLogin is a string in the result set

    // Free the result
    mysql_free_result(result);

    // Update the LastLogin field to the current timestamp
    snprintf(query, sizeof(query),
             "UPDATE doctors SET LastLogin = NOW() WHERE DoctorID = %d", fetchedDoctorID);

    // Execute the update query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE query failed: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default message on error
    }

    // Allocate memory and generate JSON containing all the data except password
    size_t buffer_size = 512; // Buffer for JSON output (larger to accommodate multiple fields)
    char *json = (char *)malloc(buffer_size);
    if (!json) {
        fprintf(stderr, "Memory allocation failed for JSON\n");
        return "we did not get your output"; // Return default message on error
    }

    // Construct the JSON output
    snprintf(json, buffer_size, 
             "{\"DoctorID\": %d, \"FullName\": \"%s\", \"Spec\": \"%s\", \"Role\": \"%s\", \"HospitalID\": \"%s\", \"PhoneNumber\": \"%s\", \"Email\": \"%s\", \"LastLogin\": \"%s\"}", 
             fetchedDoctorID, fullName, spec, role, hospitalID, phoneNumber, email, lastLogin);

    return json; // Return the dynamically allocated JSON string
}

