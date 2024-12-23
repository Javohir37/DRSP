// modifWaitlist.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "modifWaitlist.h"

char* modifWaitlist(MYSQL *conn, int patientID, int waitlistID, const char *status) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    char query[512];
    snprintf(query, sizeof(query),
             "UPDATE waitlist SET Status = '%s', ModifiedDateTime = NOW() "
             "WHERE WaitlistID = %d AND PatientID = %d",
             status, waitlistID, patientID);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database update failed\"}");
    }

    if (mysql_affected_rows(conn) == 0) {
        return strdup("{\"error\":\"No rows were updated. Verify PatientID and WaitlistID\"}");
    }

    return strdup("{\"success\":\"Waitlist status updated successfully\"}");
}

