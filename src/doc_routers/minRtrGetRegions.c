#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "getRegions.h"
#include "localmysql.h"

void minRtrGetRegions(const char *json_request, char *response_buffer, size_t buffer_size) {
	MYSQL *conn1 = establish_connection();
    // Call the get_regions logic
    char *result = get_regions(conn1);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to fetch regions\"}");
    }
}

