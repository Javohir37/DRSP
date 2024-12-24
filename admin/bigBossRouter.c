#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "minRtrAdminLogin.h"
#include "minRtrGetIP.h"
#include "minRtrUserJoins.h"
#include "minRtrSqlExe.h"
#include "localmysql.h"

void bigBossRouter(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    // Parse the "function" field to decide the route
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    struct json_object *func_obj;
    if (!json_object_object_get_ex(parsed_json, "function", &func_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing 'function' field\"}");
        json_object_put(parsed_json);
        return;
    }

    const char *function_name = json_object_get_string(func_obj);

    // Delegate to the appropriate mini-router
    if (strcmp(function_name, "adminLogin") == 0) {
    printf("DEBUG: Routing to minRtrAdminLogin\n");
    minRtrAdminLogin(json_request, response_buffer, buffer_size);
} else if (strcmp(function_name, "getIP") == 0) {
    minRtrGetIP(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "userJoins") == 0) {
    printf("DEBUG: Routing to minRtrUserJoins\n");
    minRtrUserJoins(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "sqlExe") == 0) {
    printf("DEBUG: Routing to minRtrSqlExe\n");
    minRtrSqlExe(json_request, response_buffer, buffer_size);
}
  else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Unknown function: %s\"}", function_name);
    }

    json_object_put(parsed_json);
}
