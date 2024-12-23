// modifAppoint.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "modifAppoint.h"

char* modifAppoint(MYSQL *conn, int patientID, int appointmentID, const char *status) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    char query[512];
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

    return strdup("{\"success\":\"Appointment status updated successfully\"}");
}

