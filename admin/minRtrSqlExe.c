#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "localmysql.h"
#include "sqlExe.h"

/**
 * minRtrSqlExe
 * 
 * This function acts as a mini-router for handling requests
 * to execute a given SQL query.
 * It expects a JSON object containing "args" with the SQL query string.
 *
 * @param json_request    A string containing the JSON request
 * @param response_buffer A buffer to store the JSON response
 * @param buffer_size     Size of the response buffer
 */
void minRtrSqlExe(const char *json_request, char *response_buffer, size_t buffer_size) {
    // Parse the incoming JSON request
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    // Attempt to extract the "args" field from the parsed JSON
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing 'args' field\"}");
        json_object_put(parsed_json);
        return;
    }

    // Ensure we have the required argument in the "args" array
    if (json_object_array_length(args_obj) < 1) {
        snprintf(response_buffer, buffer_size,
                 "{\"error\": \"Missing or invalid argument. Required: SQL query\"}");
        json_object_put(parsed_json);
        return;
    }

    // Extract SQL query argument from the JSON array
    const char *sql_query = json_object_get_string(json_object_array_get_idx(args_obj, 0));

    // Call sqlExe to execute the SQL query
    sqlResult res = sqlExe(sql_query);

    // Check the status of the execution
    if (res.status == 0 && res.result != NULL) {
        // Create a JSON array to hold the result set
        struct json_object *result_array = json_object_new_array();
        MYSQL_ROW row;
        MYSQL_FIELD *field;
        unsigned int num_fields = mysql_num_fields(res.result);

        // Fetch and add each row to the JSON array
        while ((row = mysql_fetch_row(res.result))) {
            struct json_object *row_obj = json_object_new_object();
            unsigned long *lengths = mysql_fetch_lengths(res.result);
            for (unsigned int i = 0; i < num_fields; i++) {
                field = mysql_fetch_field_direct(res.result, i);
                json_object_object_add(row_obj, field->name, json_object_new_string_len(row[i], lengths[i]));
            }
            json_object_array_add(result_array, row_obj);
        }

        // Set the JSON response with the result array
        snprintf(response_buffer, buffer_size, "%s", json_object_to_json_string(result_array));
        json_object_put(result_array);

        // Free the result set
        mysql_free_result(res.result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Query execution failed\"}");
    }

    // Clean up the parsed JSON object
    json_object_put(parsed_json);
}
