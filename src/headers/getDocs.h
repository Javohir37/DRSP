#ifndef GET_DOCS_H
#define GET_DOCS_H

#include <mysql/mysql.h>

// Function to retrieve doctor details based on hospitalID and specialization
char* getDocs(MYSQL *conn, const char *hospitalID, const char *spec);

#endif // GET_DOCS_H

