#include "main.h"
#include "GrabMoniter.h"

// 物品抓取模式开关
static void GrabSwitch(bool inActive)
{
    if(inActive == true)
    {
        behavior_msg.data = "grab start";
        behaviors_pub.publish(behavior_msg);
    }
    else
    {
        behavior_msg.data = "grab stop";
        behaviors_pub.publish(behavior_msg);
    }
}

// 物品递给开关
static void PassSwitch(bool inActive)
{
    if(inActive == true)
    {
        behavior_msg.data = "pass start";
        behaviors_pub.publish(behavior_msg);
    }
    else
    {
        behavior_msg.data = "pass stop";
        behaviors_pub.publish(behavior_msg);
    }
}

// 物品抓取状态
void GrabResultCallback(const std_msgs::String::ConstPtr& res)
{
    int nFindIndex = 0;
    nFindIndex = res->data.find("done");
    if( nFindIndex >= 0 )
    {
        bGrabDone = true;
    }
}

// 物品递给状态
void PassResultCallback(const std_msgs::String::ConstPtr& res)
{
    int nFindIndex = 0;
    nFindIndex = res->data.find("done");
    if( nFindIndex >= 0 )
    {
        bPassDone = true;
    }
}

//触发条件：nState == STATE_GRAB
void GrabObj(){
    if(nDelay == 0){
        bGrabDone = false;
        GrabSwitch(true);
    }
    nDelay ++;
    if(bGrabDone == true){
        GrabSwitch(false);
        Speak("I got it. I am coming back.");
        nState = STATE_COMEBACK;
    }
}

//触发条件：nState == STATE_COMEBACK
void ComeBackWithObj(){
    srvName.request.name = "master";
    if (cliGetWPName.call(srvName)){
        std::string name = srvName.response.name;
        float x = srvName.response.pose.position.x;
        float y = srvName.response.pose.position.y;
        ROS_INFO("[STATE_COMEBACK] Get_wp_name = %s (%.2f,%.2f)", strGoto.c_str(),x,y);

        MoveBaseClient ac("move_base", true);
        if(!ac.waitForServer(ros::Duration(5.0))){
            ROS_INFO("The move_base action server is no running. action abort...");
        }
        else{
            move_base_msgs::MoveBaseGoal goal;
            goal.target_pose.header.frame_id = "map";
            goal.target_pose.header.stamp = ros::Time::now();
            goal.target_pose.pose = srvName.response.pose;
            ac.sendGoal(goal);
            ac.waitForResult();
            if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                ROS_INFO("Arrived at %s!",strGoto.c_str());
                Speak("Hi,master. This is what you wanted.");
                nState = STATE_PASS;
                nDelay = 0;
            }
            else{
                ROS_INFO("Failed to get to %s ...",strGoto.c_str() );
                Speak("Failed to go to the master.");
                nState = STATE_ASK;
            }
        }
    }
    else{
        ROS_ERROR("Failed to call service GetWaypointByName");
        Speak("There is no waypoint named master.");
        nState = STATE_ASK;
    }
}

//触发条件：nState == STATE_PASS
void PassObj(){
    if(nDelay == 0){
        bPassDone = false;
        PassSwitch(true);
    }
    nDelay ++;
    if(bPassDone == true){
        PassSwitch(false);
        Speak("OK. What do you want next?");
        nState = STATE_ASK;
    }
}
