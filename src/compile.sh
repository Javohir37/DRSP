#!/bin/bash

# Remove any existing compiled files



gcc -o main \
	main.c \
	server.c \
	bigBossRouter.c \
	getCaseHistory.c \
	getDocs.c \
	getNotif.c \
	getSpecs.c \
	joinWaitlist.c \
	localmysql.c \
	minRtrGetCaseHistory.c\
	minRtrGetDocs.c \
	minRtrGetNotif.c \
	minRtrGetSpecs.c \
	minRtrJoinWaitlist.c\
	minRtrModifAppoint.c\
	minRtrModifWaitlist.c \
	minRtrMyApp.c \
	minRtrPatientLogin.c \
	minRtrRegPatient.c \
        minRtrSelectDoc.c \
        modifAppoint.c \
        modifWaitlist.c myApp.c \
        patientLogin.c regPatient.c \
        selectDoc.c \
        -lmysqlclient -ljson-c

