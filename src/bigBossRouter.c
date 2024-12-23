#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "minRtrGetDocs.h"
#include "minRtrGetNotif.h"
#include "minRtrGetSpecs.h"
#include "minRtrJoinWaitlist.h"
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
    if (strcmp(function_name, "getDocs") == 0) {
    minRtrGetDocs(json_request, response_buffer, buffer_size);
} else if (strcmp(function_name, "getNotif") == 0) {
    minRtrGetNotif(json_request, response_buffer, buffer_size);
}
else if (strcmp(function_name, "getSpecs") == 0) {
    minRtrGetSpecs(json_request, response_buffer, buffer_size);
}
else if (strcmp(function_name, "joinWaitlist") == 0) {
    minRtrJoinWaitlist(json_request, response_buffer, buffer_size);
}
  else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Unknown function: %s\"}", function_name);
    }

    json_object_put(parsed_json);
}
