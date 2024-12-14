#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "getHospitals.h"

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "Kali_777!"
#define DB_NAME "test"
#define DB_PORT 3306

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

MYSQL* establish_connection() {
    MYSQL *con = mysql_init(NULL);
    
    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    
    if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PASS, 
        DB_NAME, DB_PORT, NULL, 0) == NULL) {
        finish_with_error(con);
    }
    
    return con;
}

int main() {
    MYSQL *con = establish_connection();
    char *output = "we did not get your output";
    
    /* Here you can pass the connection to your function
    Example:
    output = your_function(con, arg2);
    */
    
    printf("output: %s\n", getHospitals(con, "Tashkent", "Ulugbek"));
    
    mysql_close(con);
    return 0;
}
