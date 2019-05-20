#include "struct.h" 

#define STATE_FOLLOW 1

char warnMsg[];
char addMsg[];
int nDelay;
int nState;

SrvFlw srvFlw;

void ROS_WARN(char msg[]);
void AddNewWaypoint(char msg[]);
Follow follow_start,follow_stop;