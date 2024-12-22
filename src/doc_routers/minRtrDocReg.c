#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "docReg.h"
#include "localmysql.h"

void minRtrDocReg(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the input JSON
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    struct json_object *fullName_obj, *spec_obj, *role_obj, *hospitalID_obj, *phoneNumber_obj, *email_obj, *password_obj;

    if (!json_object_object_get_ex(parsed_json, "fullName", &fullName_obj) ||
        !json_object_object_get_ex(parsed_json, "spec", &spec_obj) ||
        !json_object_object_get_ex(parsed_json, "role", &role_obj) ||
        !json_object_object_get_ex(parsed_json, "hospitalID", &hospitalID_obj) ||
        !json_object_object_get_ex(parsed_json, "phoneNumber", &phoneNumber_obj) ||
        !json_object_object_get_ex(parsed_json, "email", &email_obj) ||
        !json_object_object_get_ex(parsed_json, "password", &password_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing required fields\"}");
        json_object_put(parsed_json);
        return;
    }

    // Extract values from JSON
    const char *fullName = json_object_get_string(fullName_obj);
    const char *spec = json_object_get_string(spec_obj);
    const char *role = json_object_get_string(role_obj);
    int hospitalID = json_object_get_int(hospitalID_obj);
    const char *phoneNumber = json_object_get_string(phoneNumber_obj);
    const char *email = json_object_get_string(email_obj);
    const char *password = json_object_get_string(password_obj);

    // Call docReg function
    char *result_json = docReg(conn, fullName, spec, role, hospitalID, phoneNumber, email, password);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to register doctor\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

