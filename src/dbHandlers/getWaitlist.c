#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/getWaitlist.h"

char* getWaitlist(MYSQL *conn, const char *date, int doctorID) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[1024];
    char *json = NULL;
    size_t json_size = 1024; // Initial size of the JSON buffer
    size_t pos = 0;

    // Construct the SQL query to fetch waitlist entries with patient names
    snprintf(query, sizeof(query),
         "SELECT w.WaitlistID, p.Name, w.PatientID "
         "FROM waitlist AS w "
         "JOIN patients AS p ON w.PatientID = p.PatientID "
         "WHERE w.Date = '%s' AND w.DoctorID = %d "
         "ORDER BY w.WaitlistID",
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
        const char *waitlistID = row[0] ? row[0] : "null";
        const char *fullName = row[1] ? row[1] : "null";
        const char *patientID = row[2] ? row[2] : "null";

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
                       "{\"WaitlistID\": %s, \"FullName\": \"%s\", \"PatientID\": %s},",
                       waitlistID, fullName, patientID);
    }

    // Remove trailing comma and close JSON array
    if (pos > 1 && json[pos - 1] == ',') {
        pos--;
    }
    pos += snprintf(json + pos, json_size - pos, "]");

    mysql_free_result(res);
    return json;
}

