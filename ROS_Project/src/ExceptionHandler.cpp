#include "ExceptionHandler.h"
#include "main.h"

ExceptionHandler::ExceptionHandler()
{
    //ctor
}

void ExceptionHandler::MoveExpHandler(int expNum){
    switch(expNum){

        case(WAYPOINT_NOT_EXIST_EXP):
            ROS_ERROR("Failed to call service GetWaypointByName");
            Speak("There is no this waypoint.");//if
            nState = STATE_ASK;
            break;

        case(FAIL_TO_WAYPOINT_EXP):
            ROS_INFO("Failed to get to %s ...",strGoto.c_str() );
            Speak("Failed to go to the waypoint.");//if
            nState = STATE_ASK;
            break;

        case(MASTER_NOT_EXIST_EXP):
            ROS_ERROR("Failed to call service GetWaypointByName");
            Speak("There is no waypoint named master.");
            nState = STATE_ASK;
            break;


        case(FAIL_TO_MASTER_EXP):
            ROS_INFO("Failed to get to %s ...",strGoto.c_str() );
            Speak("Failed to go to the master.");
            nState = STATE_ASK;
            break;

        default: ;
    }
}
