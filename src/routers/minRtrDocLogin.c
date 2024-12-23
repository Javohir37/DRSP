#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "../headers/docLogin.h"
#include "../headers/localmysql.h"

void minRtrDocLogin(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection(); 
    // Parse the JSON request
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments for 'docLogin'\"}");
        json_object_put(parsed_json);
        return;
    }

    int doctorID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *password = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    // Call the docLogin logic
    char *result = docLogin(conn, doctorID, password);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid credentials\"}");
    }

    json_object_put(parsed_json);
}

