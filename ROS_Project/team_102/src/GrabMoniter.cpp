#include <main.hpp>
#include <GrabMoniter.hpp>
#include <voice.hpp>

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
