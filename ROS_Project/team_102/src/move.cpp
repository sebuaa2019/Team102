#include <main.hpp>
#include <move.hpp>
#include <ExceptionHandler.hpp>
#include <voice.hpp>
#include <SXXCoreTask/CoreTask.hpp>

static SCT::Queue moveq = SCT::Queue("com.sxx.move-block", SCT::Queue::SERIAL);

static bool cancel_flag = false;
static MoveBaseClient *ac = NULL;


//This function must run in main queue.
void cancelMove() {
    cancel_flag = true;
    ac->cancelAllGoals();
}

void ac_init(void *context) {
    //ac = new MoveBaseClient("move_base", true);
}

bool isQueueRunning = false;

static void genericMove(std::string str, std::function<void(bool isSucceeded)> resultHandler, std::function<void()> errorHandler){
    srvName.request.name = str;

    // SCT_once_t once;
    // SCT_once_f(&once, NULL, ac_init);
    // cancel_flag = false;
    if (!isQueueRunning)
    moveq.async([resultHandler, errorHandler] {
        isQueueRunning = true;
        if (cliGetWPName.call(srvName)) {
            std::string name = srvName.response.name;
            float x = srvName.response.pose.position.x;
            float y = srvName.response.pose.position.y;
            ROS_INFO("[STATE_COMEBACK] Get_wp_name = %s (%.2f,%.2f)", strGoto.c_str(),x,y);

            MoveBaseClient ac("move_base", true);
            if(!ac.waitForServer(ros::Duration(5.0)))
            {
                ROS_INFO("The move_base action server is no running. action abort...");
            } else {
                move_base_msgs::MoveBaseGoal goal;
                goal.target_pose.header.frame_id = "map";
                goal.target_pose.header.stamp = ros::Time::now();
                goal.target_pose.pose = srvName.response.pose;
                ac.sendGoal(goal);
                ac.waitForResult();
                
                SCT::Queue::mainQueue().sync([&ac, &resultHandler] {
                     if (cancel_flag == true) {
                         cancel_flag = false;
                         return;
                     }
                    resultHandler(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED);
                });
            }

        } else {
            SCT::Queue::mainQueue().sync([&errorHandler] {
                 if (cancel_flag == true) {
                     cancel_flag = false;
                     return;
                 }
                errorHandler();
            });
        }
        isQueueRunning = false;
    });
    
}

void moveto(){
    genericMove(strGoto, [](bool isSucceeded) {
        if(isSucceeded) {
            ROS_INFO("Arrived at %s!",strGoto.c_str());
            Speak("OK. I am taking it.");//if
            nState = STATE_GRAB;//if
            nDelay = 0;
        } else {
            ROS_INFO("Failed to get to %s ...",strGoto.c_str() );
            Speak("Failed to go to the waypoint.");//if
            nState = STATE_ASK;
        }
    }, []() {
            ROS_ERROR("Failed to call service GetWaypointByName");
            Speak("There is no this waypoint.");//if
            nState = STATE_ASK;
    });
}

void moveback(){
    static std::string str = "master";
    genericMove(str, [](bool isSucceeded) {
        if (isSucceeded) {
            ROS_INFO("Arrived at %s!",str.c_str());
            Speak("Hi,master. This is what you wanted.");
            nState = STATE_PASS;
            nDelay = 0;
        } else {
            ROS_INFO("Failed to get to %s ...",str.c_str() );
            Speak("Failed to go to the master.");
            nState = STATE_ASK;
        }
    }, []() {
            ROS_ERROR("Failed to call service GetWaypointByName");
            Speak("There is no waypoint named master.");
            nState = STATE_ASK;
    });
}


