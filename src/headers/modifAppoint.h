#ifndef MODIF_APPOINT_H
#define MODIF_APPOINT_H

#include <mysql/mysql.h>

// Function to modify appointment status and handle waitlist logic if status is canceled
char* modifAppoint(MYSQL *conn, int patientID, int appointmentID, const char *status);

#endif // MODIF_APPOINT_H

