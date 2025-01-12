/*#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/docReg.h"
#include "../headers/localmysql.h"

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

    struct json_object *function_obj, *args_array;
    if (!json_object_object_get_ex(parsed_json, "function", &function_obj) ||
        !json_object_object_get_ex(parsed_json, "args", &args_array) ||
        !json_object_is_type(args_array, json_type_array)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Expected an array of arguments\"}");
        json_object_put(parsed_json);
        return;
    }

    if (json_object_array_length(args_array) != 7) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Expected exactly 7 arguments\"}");
        json_object_put(parsed_json);
        return;
    }

    // Get array values
    const char *fullName = json_object_get_string(json_object_array_get_idx(args_array, 0));
    const char *spec = json_object_get_string(json_object_array_get_idx(args_array, 1));
    const char *role = json_object_get_string(json_object_array_get_idx(args_array, 2));
    int hospitalID = json_object_get_int(json_object_array_get_idx(args_array, 3));
    const char *phoneNumber = json_object_get_string(json_object_array_get_idx(args_array, 4));
    const char *email = json_object_get_string(json_object_array_get_idx(args_array, 5));
    const char *password = json_object_get_string(json_object_array_get_idx(args_array, 6));

    // Debugging output to verify values
    printf("Parsed JSON values:\n");
    printf("fullName: %s\n", fullName);
    printf("spec: %s\n", spec);
    printf("role: %s\n", role);
    printf("hospitalID: %d\n", hospitalID);
    printf("phoneNumber: %s\n", phoneNumber);
    printf("email: %s\n", email);
    printf("password: %s\n", password);

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
}*/

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/docReg.h"
#include "../headers/minRtrDocReg.h"
#include "../headers/localmysql.h"

void minRtrDocReg(const char *json_request, char *response_buffer, size_t buffer_size) {
    // Validate parameters
    if (!json_request || !response_buffer || buffer_size == 0) {
        fprintf(stderr, "minRtrDocReg error: Invalid function parameters.\n");
        return;
    }

    // Establish a MySQL connection
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the input JSON request
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        mysql_close(conn);
        return;
    }

    struct json_object *function_obj = NULL;
    struct json_object *args_array   = NULL;

    // Check for required JSON members: "function" and "args"
    if (!json_object_object_get_ex(parsed_json, "function", &function_obj) ||
        !json_object_object_get_ex(parsed_json, "args", &args_array) ||
        !json_object_is_type(args_array, json_type_array)) 
    {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Expected an array of arguments\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // For docReg, we expect exactly 7 arguments
    if (json_object_array_length(args_array) != 7) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Expected exactly 7 arguments\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Extract arguments
    const char *fullName    = json_object_get_string(json_object_array_get_idx(args_array, 0));
    const char *spec        = json_object_get_string(json_object_array_get_idx(args_array, 1));
    const char *role        = json_object_get_string(json_object_array_get_idx(args_array, 2));
    int hospitalID          = json_object_get_int(json_object_array_get_idx(args_array, 3));
    const char *phoneNumber = json_object_get_string(json_object_array_get_idx(args_array, 4));
    const char *email       = json_object_get_string(json_object_array_get_idx(args_array, 5));
    const char *password    = json_object_get_string(json_object_array_get_idx(args_array, 6));

    // Call docReg to insert a new doctor into the database
    char *result_json = docReg(conn, fullName, spec, role, hospitalID, phoneNumber, email, password);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to register doctor\"}");
    } else {
        // Write the result to the response buffer
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    // Cleanup
    json_object_put(parsed_json);
    mysql_close(conn);
}
