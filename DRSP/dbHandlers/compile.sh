#!/bin/bash

# Remove any existing compiled files
rm -f drsp_server *.o

# Compile all .c files with the include path for headers and link necessary libraries
gcc -I../headers -o drsp_server \
    main.c \
    docLogin.c \
    docReg.c \
    getCaseHistory.c \
    getDistricts.c \
    getDocNotification.c \
    getHospitals.c \
    getHourInfo.c \
    getRegions.c \
    getSchedule.c \
    getWaitlist.c \
    jsonParser.c \
    judgeNotification.c \
    localmysql.c \
    modifyApp.c \
    selectHospital.c \
    ../routers/bigBossRouter.c \
    ../routers/minRtrDocLogin.c \
    ../routers/minRtrDocReg.c \
    ../routers/minRtrGetCaseHistory.c \
    ../routers/minRtrGetDistricts.c \
    ../routers/minRtrGetDocNotification.c \
    ../routers/minRtrGetHospitals.c \
    ../routers/minRtrGetHourInfo.c \
    ../routers/minRtrGetRegions.c \
    ../routers/minRtrGetSchedule.c \
    ../routers/minRtrGetWaitlist.c \
    ../routers/minRtrJudgeNotification.c \
    ../routers/minRtrModifyApp.c \
    ../routers/minRtrSelectHospital.c \
    -lmysqlclient -ljson-c
