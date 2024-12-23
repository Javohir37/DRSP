#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/getHospitals.h"

// Function to retrieve hospital details based on Region and District
char* getHospitals(MYSQL *conn, const char *region, const char *district) {
    char query[1024];
    MYSQL_RES *result;
    MYSQL_ROW row;

    // Construct the SELECT query to fetch hospital details based on Region and District
    snprintf(query, sizeof(query),
             "SELECT HospitalID, Name, Region, District, Address FROM hospitals WHERE Region = '%s' AND District = '%s'",
             region, district);

    // Execute the query
    if (mysql_query(conn, query)) {
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
        return "No hospitals found"; // Return message if no matching hospitals
    }

    // Allocate memory for the JSON response
    size_t buffer_size = 1024; // Buffer for JSON output
    char *json = (char *)malloc(buffer_size);
    if (!json) {
        fprintf(stderr, "Memory allocation failed for JSON\n");
        return "we did not get your output"; // Return default message on error
    }

    // Start building the JSON response
    snprintf(json, buffer_size, "[");

    // Loop through the result set and construct JSON for each hospital
    int first_row = 1;  // Flag to check if it's the first hospital
    do {
        if (!first_row) {
            strncat(json, ",", buffer_size); // Add comma between records
        }
        first_row = 0;

        // Get the hospital data
        //const char *hospitalID = row[0];
        const char *name = row[1];
        //const char *region = row[2];
        //const char *district = row[3];
        //const char *address = row[4];

        // Append the current hospital data to the JSON string
        snprintf(json + strlen(json), buffer_size - strlen(json), 
                 "{\"Name\": \"%s\"}",
                 name);

    } while ((row = mysql_fetch_row(result)) != NULL);

    // Close the JSON array
    snprintf(json + strlen(json), buffer_size - strlen(json), "]");

    // Free the result
    mysql_free_result(result);

    return json; // Return the dynamically allocated JSON string
}

