#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>

// Patient-related headers
#include "../headers/minRtrGetDocs.h"
#include "../headers/minRtrGetNotif.h"
#include "../headers/minRtrGetSpecs.h"
#include "../headers/minRtrJoinWaitlist.h"
#include "../headers/minRtrRegPatient.h"
#include "../headers/minRtrSelectDoc.h"
#include "../headers/minRtrPatientLogin.h"
#include "../headers/minRtrGetCaseHistory.h"
#include "../headers/minRtrMyApp.h"
#include "../headers/minRtrModifAppoint.h"
#include "../headers/minRtrModifWaitlist.h"

// Doctor-related headers
#include "../headers/minRtrGetRegions.h"
#include "../headers/minRtrGetDistricts.h"
#include "../headers/minRtrDocLogin.h"
#include "../headers/minRtrDocReg.h"
#include "../headers/minRtrGetDocNotification.h"
#include "../headers/minRtrSelectHospital.h"
#include "../headers/minRtrGetHourInfo.h"
#include "../headers/minRtrGetWaitlist.h"
#include "../headers/minRtrJudgeNotification.h"
#include "../headers/minRtrGetHospitals.h"
#include "../headers/minRtrModifyApp.h"
#include "../headers/minRtrGetSchedule.h"

//admin related
#include "../headers/minRtrAdminLogin.h"
#include "../headers/minRtrGetIP.h"
#include "../headers/minRtrSqlExe.h"
#include "../headers/minRtrUserJoins.h"
// Common headers
#include "../headers/localmysql.h"

void bigBossRouter(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    
    // Parse the JSON request
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

    // Patient-related routes
   if (strcmp(function_name, "getDocs") == 0) {
    minRtrGetDocs(json_request, response_buffer, buffer_size);
}
 else if (strcmp(function_name, "getNotif") == 0) {
        minRtrGetNotif(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getSpecs") == 0) {
    minRtrGetSpecs(json_request, response_buffer, buffer_size);
}
 else if (strcmp(function_name, "joinWaitlist") == 0) {
        minRtrJoinWaitlist(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "regPatient") == 0) {
    minRtrRegPatient(json_request, response_buffer, buffer_size);
}
else if (strcmp(function_name, "selectDoc") == 0) {
        minRtrSelectDoc(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "patientLogin") == 0) {
    minRtrPatientLogin(json_request, response_buffer, buffer_size);
}
 else if (strcmp(function_name, "myApp") == 0) {
        minRtrMyApp(json_request, response_buffer, buffer_size);
    } 
    else if (strcmp(function_name, "modifAppoint") == 0) {
        minRtrModifAppoint(json_request, response_buffer, buffer_size);
    } 
     else if (strcmp(function_name, "modifWaitlist") == 0) {
        minRtrModifWaitlist(conn, json_request);
    }
    
    // Doctor-related routes
    else if (strcmp(function_name, "docLogin") == 0) {
        minRtrDocLogin(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getDistricts") == 0) {
        minRtrGetDistricts(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getRegions") == 0) {
        minRtrGetRegions(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "docReg") == 0) {
        minRtrDocReg(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getDocNotification") == 0) {
    minRtrGetDocNotification(json_request, response_buffer, buffer_size);
    }else if (strcmp(function_name, "selectHospital") == 0) {
        minRtrSelectHospital(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getWaitlist") == 0) {
        minRtrGetWaitlist(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getHourInfo") == 0) {
        minRtrGetHourInfo(conn, json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getHospitals") == 0) {
        minRtrGetHospitals(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "judgeNotification") == 0) {
    minRtrJudgeNotification(json_request, response_buffer, buffer_size);
}

else if (strcmp(function_name, "modifyApp") == 0) {
        minRtrModifyApp(json_request, response_buffer, buffer_size);
    } else if (strcmp(function_name, "getSchedule") == 0) {
        minRtrGetSchedule(json_request, response_buffer, buffer_size);
    }
    
    // Case history (shared between patient and doctor)
    else if (strcmp(function_name, "getCaseHistory") == 0) {
        minRtrGetCaseHistory(json_request, response_buffer, buffer_size);
    }
    
    //ADMIN
    else if (strcmp(function_name, "adminLogin") == 0) {
    printf("DEBUG: Routing to minRtrAdminLogin\n");
    minRtrAdminLogin(json_request, response_buffer, buffer_size);
} else if (strcmp(function_name, "getIP") == 0) {
    minRtrGetIP(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "userJoins") == 0) {
    printf("DEBUG: Routing to minRtrUserJoins\n");
    minRtrUserJoins(json_request, response_buffer, buffer_size);
}else if (strcmp(function_name, "sqlExe") == 0) {
    minRtrSqlExe(json_request, response_buffer, buffer_size);
}

    
    // Unknown function
    else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Unknown function: %s\"}", function_name);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}
