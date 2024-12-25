#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "../headers/regPatient.h"
#include "../headers/localmysql.h"

void minRtrRegPatient(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the JSON request
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        mysql_close(conn);
        return;
    }

    // Extract arguments
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || 
        json_object_array_length(args_obj) < 8) {
        snprintf(response_buffer, buffer_size, 
                 "{\"error\": \"Missing or invalid arguments. Required: name, dob, address, phoneNumber, email, passportID, citizenship, password\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    const char *name = json_object_get_string(json_object_array_get_idx(args_obj, 0));
    const char *dob = json_object_get_string(json_object_array_get_idx(args_obj, 1));
    const char *address = json_object_get_string(json_object_array_get_idx(args_obj, 2));
    const char *phoneNumber = json_object_get_string(json_object_array_get_idx(args_obj, 3));
    const char *email = json_object_get_string(json_object_array_get_idx(args_obj, 4));
    const char *passportID = json_object_get_string(json_object_array_get_idx(args_obj, 5));
    const char *citizenship = json_object_get_string(json_object_array_get_idx(args_obj, 6));
    const char *password = json_object_get_string(json_object_array_get_idx(args_obj, 7));

    // Call the regPatient function
    const char *result = regPatient(conn, name, dob, address, phoneNumber, email, passportID, citizenship, password);

    if (strcmp(result, "error") == 0) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to register patient\"}");
    } else if (strcmp(result, "exists") == 0) {
        snprintf(response_buffer, buffer_size, "{\"message\": \"Patient already exists\"}");
    } else {
        snprintf(response_buffer, buffer_size, "{\"PatientID\": \"%s\"}", result);
    }

    // Clean up
    json_object_put(parsed_json);
    mysql_close(conn);
}

