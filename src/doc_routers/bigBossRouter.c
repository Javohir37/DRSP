#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "minRtrGetRegions.h"
#include "minRtrGetDistricts.h"
#include "minRtrDocLogin.h"
#include "minRtrGetCaseHistory.h"
#include "minRtrDocReg.h"
#include "minRtrGetDocNotification.h"
#include "minRtrSelectHospital.h"
#include "minRtrGetHourInfo.h"
#include "minRtrGetWaitlist.h"
#include "minRtrJudgeNotification.h"
#include "minRtrGetHospitals.h"
#include "minRtrModifyApp.h"
#include "minRtrGetSchedule.h"
#include "minRtrSelectHospital.h"
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
    if (strcmp(function_name, "docLogin") == 0) {
        minRtrDocLogin(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getDistricts") == 0) {
        minRtrGetDistricts(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getRegions") == 0) {
        minRtrGetRegions(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "docReg") == 0) {
        minRtrDocReg(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getCaseHistory") == 0) {
        printf("DEBUG: Routing to minRtrGetCaseHistory\n");
        minRtrGetCaseHistory(conn, json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getDocNotification") == 0) {
        minRtrGetDocNotification(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "selectHospital") == 0) {
        minRtrSelectHospital(json_request, response_buffer, buffer_size);
    }else if (strcmp(function_name, "getWaitlist") == 0) {
        minRtrGetWaitlist(json_request, response_buffer, buffer_size);
    }else if (strcmp(function_name, "getHourInfo") == 0) {
    printf("DEBUG: Routing to minRtrGetHourInfo\n");
    minRtrGetHourInfo(conn, json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getHospitals") == 0) {
    minRtrGetHospitals(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "judgeNotification") == 0) {
    printf("DEBUG: Routing to minRtrJudgeNotification\n");
    minRtrJudgeNotification(conn, json_request, response_buffer, buffer_size);
    }else if (strcmp(function_name, "selectHospital") == 0) {
        minRtrSelectHospital(json_request, response_buffer, buffer_size);
    }else if (strcmp(function_name, "modifyApp") == 0) {
        minRtrModifyApp(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getSchedule") == 0) {
    minRtrGetSchedule(json_request, response_buffer, buffer_size);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Unknown function: %s\"}", function_name);
    }

    json_object_put(parsed_json);
}
