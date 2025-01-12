#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../headers/modifAppoint.h"

char* modifAppoint(MYSQL *conn, int patientID, int appointmentID, const char *status) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    char query[512];

    // Update the appointment status
    snprintf(query, sizeof(query),
             "UPDATE appointments SET Status = '%s', ModifiedDateTime = NOW() "
             "WHERE AppointmentID = %d AND PatientID = %d",
             status, appointmentID, patientID);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database update failed\"}");
    }

    if (mysql_affected_rows(conn) == 0) {
        return strdup("{\"error\":\"No rows were updated. Verify PatientID and AppointmentID\"}");
    }

    // If the status is "Cancelled", move the first person from the waitlist
    if (strcmp(status, "Cancelled") == 0) {
        // Fetch the first person on the waitlist for the same doctor and date
        snprintf(query, sizeof(query),
                 "SELECT WaitlistID, PatientID, DoctorID, Date "
                 "FROM waitlist "
                 "WHERE DoctorID = (SELECT DoctorID FROM appointments WHERE AppointmentID = %d) "
                 "AND Date = (SELECT DATE(DateTime) FROM appointments WHERE AppointmentID = %d) "
                 "AND Status = 'Waiting' "
                 "ORDER BY ModifiedDateTime ASC LIMIT 1",
                 appointmentID, appointmentID);

        if (mysql_query(conn, query)) {
            return strdup("{\"error\":\"Failed to fetch waitlist\"}");
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (!result) {
            return strdup("{\"error\":\"Failed to store result\"}");
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            int waitlistID = atoi(row[0]);
            int newPatientID = atoi(row[1]);
            int doctorID = atoi(row[2]);
            const char *date = row[3];

            mysql_free_result(result);

            // Move the person from the waitlist to the appointment
            snprintf(query, sizeof(query),
                     "UPDATE appointments SET PatientID = %d, Status = 'Booked', ModifiedDateTime = NOW() "
                     "WHERE AppointmentID = %d",
                     newPatientID, appointmentID);

            if (mysql_query(conn, query)) {
                return strdup("{\"error\":\"Failed to update appointment with waitlist entry\"}");
            }

            // Remove the person from the waitlist
            snprintf(query, sizeof(query),
                     "DELETE FROM waitlist WHERE WaitlistID = %d", waitlistID);

            if (mysql_query(conn, query)) {
                return strdup("{\"error\":\"Failed to remove waitlist entry\"}");
            }

            return strdup("{\"success\":\"Appointment status updated and waitlist entry moved successfully\"}");
        } else {
            mysql_free_result(result);
            return strdup("{\"success\":\"Appointment canceled, but no waitlist entry available\"}");
        }
    }

    return strdup("{\"success\":\"Appointment status updated successfully\"}");
}

