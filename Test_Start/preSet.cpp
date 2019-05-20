#include "preSet.h"
#include <string.h>
#include <stdio.h>

char warnMsg[256];
char addMsg[256];
int nDelay=0;
SrvFlw srvFlw;

void ROS_WARN(char msg[]){
	strcpy(warnMsg,msg);
}

void AddNewWaypoint(char msg[]){
	strcpy(addMsg,msg);
}

bool Follow::call(SrvFlw srv){
	if(srv.request.thredhold<0.1)return(false);
	else return(true);
}