#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "getSchedule.h"

// Function to retrieve the schedule for a specific date and doctor
char* getSchedule(MYSQL *conn, const char *date, int doctorID) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[1024];
    char *json = NULL;
    size_t json_size = 1024; // Initial size of the JSON buffer
    size_t pos = 0;

    // Construct the SQL query to fetch appointments for the given date and doctorID
    snprintf(query, sizeof(query),
             "SELECT DISTINCT a.AppointmentID, a.DateTime, a.Status "
             "FROM appointments AS a "
             "WHERE DATE(a.DateTime) = '%s' AND a.DoctorID = %d",
             date, doctorID);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return NULL;
    }

    int row_count = mysql_num_rows(res);
    if (row_count == 0) {
        mysql_free_result(res);
        return strdup("[]"); // Return empty JSON array
    }

    json = (char *)malloc(json_size);
    if (!json) {
        fprintf(stderr, "Memory allocation failed for JSON\n");
        mysql_free_result(res);
        return NULL;
    }

    // Start JSON array
    pos += snprintf(json + pos, json_size - pos, "[");

    while ((row = mysql_fetch_row(res))) {
        // Extract data from the row
        const char *appointmentID = row[0] ? row[0] : "null";
        const char *dateTime = row[1] ? row[1] : "null";
        const char *status = row[2];

        // Expand JSON buffer if needed
        if (pos + 256 >= json_size) {
            json_size *= 2;
            char *temp = realloc(json, json_size);
            if (!temp) {
                fprintf(stderr, "Memory reallocation failed for JSON\n");
                free(json);
                mysql_free_result(res);
                return NULL;
            }
            json = temp;
        }

        // Append JSON object for this row
        pos += snprintf(json + pos, json_size - pos,
                        "{\"AppointmentID\": %s, \"DateTime\": \"%s\", \"Status\": \"%s\"},",
                        appointmentID, dateTime, status);
    }

    // Remove trailing comma and close JSON array
    if (pos > 1 && json[pos - 1] == ',') {
        pos--;
    }
    pos += snprintf(json + pos, json_size - pos, "]");

    mysql_free_result(res);
    return json;
}
