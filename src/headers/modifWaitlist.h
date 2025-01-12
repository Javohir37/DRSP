// modifWaitlist.h

#ifndef MODIF_WAITLIST_H
#define MODIF_WAITLIST_H

#include <mysql/mysql.h>

// Function to modify waitlist status
char* modifWaitlist(MYSQL *conn, int patientID, int waitlistID, const char *status);

#endif // MODIF_WAITLIST_H

