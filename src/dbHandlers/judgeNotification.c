#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/judgeNotification.h"

// Function to update the booking status in the appropriate tableName (appointments or waitlist)
char* judgeNotification(MYSQL *conn, int id, const char *status, const char *tableName) {
    char query[1024];
    static char result[512]; // Static buffer for the result message

    // Validate the tableName name
    if (strcmp(tableName, "appointments") != 0 && strcmp(tableName, "waitlist") != 0) {
        return "Invalid tableName name. Use 'appointments' or 'waitlist'.";
    }

    // Construct the UPDATE query
    snprintf(query, sizeof(query),
             "UPDATE %s SET Status = '%s', ModifiedDateTime = NOW() WHERE %sID = %d",
             tableName, status, (strcmp(tableName, "appointments") == 0 ? "Appointment" : "Waitlist"), id);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE query failed: %s\n", mysql_error(conn));
        return "Failed to update the booking status.";
    }

    // Check if any rows were affected
    if (mysql_affected_rows(conn) == 0) {
        return "No matching record found.";
    }

    // Prepare the success message
    snprintf(result, sizeof(result), "Booking status updated successfully in %s tableName for ID %d.", tableName, id);

    return result;
}

