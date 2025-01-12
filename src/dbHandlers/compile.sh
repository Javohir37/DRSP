#!/bin/bash

# Remove any existing compiled files
rm -f drsp_server *.o

# Compile all .c files with the include path for headers and link necessary libraries
gcc -I../headers -o main \
    main.c \
    docLogin.c \
    docReg.c \
    getCaseHistory.c \
    getDistricts.c \
    getDocNotification.c \
    getDocs.c \
    getHospitals.c \
    getHourInfo.c \
    getNotif.c \
    getRegions.c \
    getSchedule.c \
    getSpecs.c \
    getWaitlist.c \
    joinWaitlist.c \
    jsonParser.c \
    judgeNotification.c \
    localmysql.c \
    ../routers/minRtrGetHospitals.c \
    modifAppoint.c \
    modifWaitlist.c \
    modifyApp.c \
    myApp.c \
    patientLogin.c \
    regPatient.c \
    selectDoc.c \
    selectHospital.c \
    adminLogin.c \
    getIP.c \
    sqlExe.c \
    userJoins.c \
    ../routers/bigBossRouter.c \
    ../routers/minRtrDocLogin.c \
    ../routers/minRtrDocReg.c \
    ../routers/minRtrGetCaseHistory.c \
    ../routers/minRtrGetDistricts.c \
    ../routers/minRtrGetDocNotification.c \
    ../routers/minRtrGetDocs.c \
    ../routers/minRtrGetHourInfo.c \
    ../routers/minRtrGetNotif.c \
    ../routers/minRtrGetRegions.c \
    ../routers/minRtrGetSchedule.c \
    ../routers/minRtrGetSpecs.c \
    ../routers/minRtrGetWaitlist.c \
    ../routers/minRtrJoinWaitlist.c \
    ../routers/minRtrJudgeNotification.c \
    ../routers/minRtrModifAppoint.c \
    ../routers/minRtrModifWaitlist.c \
    ../routers/minRtrModifyApp.c \
    ../routers/minRtrMyApp.c \
    ../routers/minRtrPatientLogin.c \
    ../routers/minRtrRegPatient.c \
    ../routers/minRtrSelectDoc.c \
    ../routers/minRtrSelectHospital.c \
    ../routers/server.c \
    ../routers/minRtrAdminLogin.c \
    ../routers/minRtrGetIP.c \
    ../routers/minRtrSqlExe.c \
    ../routers/minRtrUserJoins.c \
    -lmysqlclient -pthread -ljson-c
