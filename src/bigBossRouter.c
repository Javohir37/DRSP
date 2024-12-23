#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "minRtrGetDocs.h"
#include "minRtrGetNotif.h"
#include "minRtrGetSpecs.h"
#include "minRtrJoinWaitlist.h"
#include "minRtrRegPatient.h"
#include "minRtrSelectDoc.h"
#include "minRtrPatientLogin.h"
#include "minRtrGetCaseHistory.h"
#include "minRtrMyApp.h"
#include "minRtrModifAppoint.h" 
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
}else if (strcmp(function_name, "getSpecs") == 0) {
    minRtrGetSpecs(json_request, response_buffer, buffer_size);
}
else if (strcmp(function_name, "joinWaitlist") == 0) {
    minRtrJoinWaitlist(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "regPatient") == 0) {
    minRtrRegPatient(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "selectDoc") == 0) {
    minRtrSelectDoc(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "patientLogin") == 0) {  // Add the patient login handler
        char *response = handlePatientLoginRequest(conn, json_request);
        strncpy(response_buffer, response, buffer_size - 1);
        response_buffer[buffer_size - 1] = '\0';
        free(response);
    }else if (strcmp(function_name, "getCaseHistory") == 0) {  // New case history route
        char *response = handleGetCaseHistoryRequest(conn, json_request);
        strncpy(response_buffer, response, buffer_size - 1);
        response_buffer[buffer_size - 1] = '\0';
        free(response);
    } else if (strcmp(function_name, "myApp") == 0) {
    minRtrMyApp(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "modifAppoint") == 0) {  // Add this route
        char *response = handleModifAppointRequest(conn, json_request);
        strncpy(response_buffer, response, buffer_size - 1);
        response_buffer[buffer_size - 1] = '\0';
        free(response);
    }
  else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Unknown function: %s\"}", function_name);
    }

    json_object_put(parsed_json);
}
