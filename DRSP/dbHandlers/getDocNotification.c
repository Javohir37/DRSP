#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/getDocNotification.h"

// Function to retrieve notifications for a doctor
char* getDocNotification(MYSQL *conn, int doctorID, const char *lastLogin) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[1024];
    char *json = NULL;
    size_t json_size = 1024; // Initial size of the JSON buffer
    size_t pos = 0;

    // Construct the SQL query
    snprintf(query, sizeof(query),
             "SELECT a.AppointmentID, a.PatientID, a.DateTime, a.Status, p.Name "
             "FROM appointments AS a "
             "JOIN patients AS p ON a.PatientID = p.PatientID "
             "WHERE a.DoctorID = %d "
             "AND (a.Status = 'Waiting' OR (a.Status = 'Cancelled' AND a.ModifiedDateTime > '%s'))",
             doctorID, lastLogin);

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
        int appointmentID = atoi(row[0]);
        int patientID = atoi(row[1]);
        const char *dateTime = row[2];
        const char *status = row[3];
        const char *fullName = row[4];

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
                        "{\"AppointmentID\": %d, \"PatientID\": %d, \"DateTime\": \"%s\", \"Status\": \"%s\", \"FullName\": \"%s\"},",
                        appointmentID, patientID, dateTime, status, fullName);
    }

    // Remove trailing comma and close JSON array
    if (pos > 1 && json[pos - 1] == ',') {
        pos--;
    }
    pos += snprintf(json + pos, json_size - pos, "]");

    mysql_free_result(res);
    return json;
}

