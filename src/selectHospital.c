#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "selectHospital.h"

// Function to select a hospital by Name, Region, District, and Address, and return HospitalID and Address as a string
char* selectHospital(MYSQL *conn, const char *name, const char *region, const char *district, const char *address) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[1024];
    static char result[512];  // Static buffer for holding the result as a string

    // Construct the SELECT query to find HospitalID and Address based on Name, Region, District, and Address
    snprintf(query, sizeof(query), 
             "SELECT HospitalID, Address FROM hospitals WHERE Name = '%s' AND Region = '%s' AND District = '%s' AND Address = '%s'", 
             name, region, district, address);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default error message
    }

    // Retrieve result
    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default error message
    }

    // If no row is found, return the default message
    if (mysql_num_rows(res) == 0) {
        mysql_free_result(res);
        return "we did not get your output";
    }

    // Get the first row (there should only be one result)
    row = mysql_fetch_row(res);
    int hospitalID = atoi(row[0]); // Convert HospitalID from string to int
    const char *fetchedAddress = row[1];  // Get the address

    // Prepare the result as a string
    snprintf(result, sizeof(result), "{\"HospitalID\": %d, \"Address\": \"%s\"}", hospitalID, fetchedAddress);

    mysql_free_result(res);

    return result; // Return the result as a string
}
