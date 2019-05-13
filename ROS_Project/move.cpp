#include "main.h"
void moveback(){
            srvName.request.name = (nState==STATE_GOTO)?strGoto:"master"; //if
            if (cliGetWPName.call(srvName))
            {
                std::string name = srvName.response.name;
                float x = srvName.response.pose.position.x;
                float y = srvName.response.pose.position.y;

                if(nState==STATE_GOTO)
                    ROS_INFO("[STATE_GOTO] Get_wp_name = %s (%.2f,%.2f)", strGoto.c_str(),x,y);//if
                else
                    ROS_INFO("[STATE_COMEBACK] Get_wp_name = %s (%.2f,%.2f)", strGoto.c_str(),x,y);

                MoveBaseClient ac("move_base", true);
                if(!ac.waitForServer(ros::Duration(5.0)))
                {
                    ROS_INFO("The move_base action server is no running. action abort...");
                }
                else
                {
                    move_base_msgs::MoveBaseGoal goal;
                    goal.target_pose.header.frame_id = "map";
                    goal.target_pose.header.stamp = ros::Time::now();
                    goal.target_pose.pose = srvName.response.pose;
                    ac.sendGoal(goal);
                    ac.waitForResult();
                    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                    {
                        ROS_INFO("Arrived at %s!",strGoto.c_str());

                        if(nState==STATE_GOTO){
                            Speak("OK. I am taking it.");//if
                            nState = STATE_GRAB;//if
                        }
                        else{
                            Speak("Hi,master. This is what you wanted.");
                            nState = STATE_PASS;
                        }
                        nDelay = 0;
                    }
                    else
                    {
                        ROS_INFO("Failed to get to %s ...",strGoto.c_str() );
                        if(nState==STATE_GOTO) Speak("Failed to go to the waypoint.");//if
                        else Speak("Failed to go to the master.");
                        nState = STATE_ASK;
                    }
                }

            }
            else
            {
                ROS_ERROR("Failed to call service GetWaypointByName");
                if(nState==STATE_GOTO) Speak("There is no this waypoint.");//if
                else  Speak("There is no waypoint named master.");
                nState = STATE_ASK;
            }
}
