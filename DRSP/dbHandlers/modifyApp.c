#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/modifyApp.h"

// Function to modify the status of an appointment
char* modifyApp(MYSQL *conn, int appointmentID, const char *status) {
    char query[1024];
    static char result[512]; // Static buffer for the result message

    // Construct the UPDATE query
    snprintf(query, sizeof(query),
             "UPDATE appointments SET Status = '%s', ModifiedDateTime = NOW() WHERE AppointmentID = %d",
             status, appointmentID);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE query failed: %s\n", mysql_error(conn));
        return "Failed to update the appointment status.";
    }

    // Check if any rows were affected
    if (mysql_affected_rows(conn) == 0) {
        return "No matching appointment found.";
    }

    // Prepare the success message
    snprintf(result, sizeof(result), "Appointment status updated successfully for AppointmentID %d.", appointmentID);

    return result;
}

