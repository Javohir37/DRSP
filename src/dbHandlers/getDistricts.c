#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/getDistricts.h"

// Function to retrieve districts for a specific region and return a JSON-formatted string
char* getDistricts(MYSQL *conn, const char *regionName) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[1024];

    // Query to get distinct districts based on the specified region
    snprintf(query, sizeof(query), "SELECT DISTINCT District FROM hospitals WHERE Region = '%s'", regionName);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return NULL;
    }

    // Retrieve result
    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return NULL;
    }

    int row_count = mysql_num_rows(res);

    // If no rows found, return an empty JSON array
    if (row_count == 0) {
        mysql_free_result(res);
        return strdup("[]"); // Return empty JSON array
    }

    // Allocate buffer for JSON output
    size_t buffer_size = 1024; // Initial buffer size
    char *output = (char *)malloc(buffer_size);
    if (!output) {
        fprintf(stderr, "Memory allocation failed for output\n");
        mysql_free_result(res);
        return NULL;
    }

    // Initialize output with an empty JSON array
    snprintf(output, buffer_size, "[");

    // Fetch rows and append to output
    while ((row = mysql_fetch_row(res))) {
        size_t used_len = strlen(output);
        size_t district_len = row[0] ? strlen(row[0]) : 4; // "NULL" if no value

        // Reallocate buffer if needed
        if (used_len + district_len + 5 >= buffer_size) {
            buffer_size *= 2;
            output = realloc(output, buffer_size);
            if (!output) {
                fprintf(stderr, "Memory reallocation failed\n");
                mysql_free_result(res);
                return NULL;
            }
        }

        // Append district to JSON array
        if (used_len > 1) {
            strncat(output, ",", buffer_size - strlen(output) - 1);
        }
        if (row[0]) {
            strncat(output, "\"", buffer_size - strlen(output) - 1);
            strncat(output, row[0], buffer_size - strlen(output) - 1);
            strncat(output, "\"", buffer_size - strlen(output) - 1);
        } else {
            strncat(output, "NULL", buffer_size - strlen(output) - 1);
        }
    }

    // Close the JSON array
    strncat(output, "]", buffer_size - strlen(output) - 1);

    mysql_free_result(res);
    return output;
}

