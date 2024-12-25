// minRtrModifWaitlist.h

#ifndef MIN_RTR_MODIF_WAITLIST_H
#define MIN_RTR_MODIF_WAITLIST_H

#include <mysql/mysql.h>

// Function to handle JSON request for modifying waitlist status
char* minRtrModifWaitlist(MYSQL *conn, const char *requestJSON);

#endif // MIN_RTR_MODIF_WAITLIST_H

