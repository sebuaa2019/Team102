#include <main.hpp>
#include <start.hpp>
#include <voice.hpp>
#include <move.hpp>
#include <GrabMonitor.hpp>
#include <ExceptionHandler.hpp>
#include <voice.hpp>
#include <ros/ros.h>
#include <sstream>
#include <SXXCoreTask/CoreTask.hpp>
#include <ctools.hpp>

using namespace std;

string strGoto;
sound_play::SoundRequest spk_msg;
ros::Publisher spk_pub;
ros::Publisher vel_pub;
string strToSpeak = "";
string strKeyWord = "";
ros::ServiceClient clientIAT;
xfyun_waterplus::IATSwitch srvIAT;
ros::ServiceClient cliGetWPName;
waterplus_map_tools::GetWaypointByName srvName;
ros::Publisher add_waypoint_pub;
ros::ServiceClient follow_start;
ros::ServiceClient follow_stop;
ros::ServiceClient follow_resume;

ros::Publisher behaviors_pub;
std_msgs::String behavior_msg;
ros::Subscriber grab_result_sub;
ros::Subscriber pass_result_sub;
bool bGrabDone;
bool bPassDone;
int nState = STATE_READY;
int nDelay = 0;
vector<string> arKeyword;

extern "C" void SCT_main_queue_drain_np();

#define SERVER_IP "10.136.82.54"
#define SERVER_PORT 13246

ExceptionHandler ExceptionHandler::exception(int type) {
    printf("Exception happened: %d\n", type);
    abort();
}

Request::Request(int *src, int *dst, int type) : item(src[0], src[1], src[2], type) {
    this->src[0] = src[0];
    this->src[1] = src[1];
    this->src[2] = src[2];

    this->dst[0] = dst[0];
    this->dst[1] = dst[1];
    this->dst[2] = dst[2];
}

int main(int argc, char** argv)
{
    //ROS初始化
    ros::init(argc, argv, "team102_NULL");

    bool no_daemon = false;

    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--no-daemon") {
            no_daemon = true;
            break;
        }
    }

    

    int read_port = -1, write_port = -1;
    static std::string read_port_prefix = "--read-port=";
    static std::string write_port_prefix = "--write-port=";
    if (!no_daemon) {
        for (int i = 0; i < argc; i++) {
            std::string arg = argv[i];
            if (arg.find(read_port_prefix) == 0) {
                auto port_str = arg.substr(read_port_prefix.size());
                read_port = atoi(port_str.c_str());
            } else if (arg.find(write_port_prefix) == 0) {
                auto port_str = arg.substr(write_port_prefix.size());
                write_port = atoi(port_str.c_str());
            }
        }
    }

    SCT::Queue mainq = SCT::Queue::mainQueue();
    SCT::Source pipe_timer = SCT::Source(SCT::Source::Type::TIMER, 0, 0, mainq);
    pipe_timer.setTimer(SCT::Time::NOW, 100 * NSEC_PER_MSEC, 0);

    if (read_port == -1 || write_port == -1) {
        printf("No port in arg.\n");
    }

    pipe_timer.setEventHandler([write_port] {
        char s[] = "heart";
        sendData(write_port, s, sizeof(s));
    });
    
    //各主题初始化
    ros::NodeHandle n;
    ros::Subscriber sub_sr = n.subscribe("/xfyun/iat", 10, KeywordCB);
    follow_start = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/start");
    follow_stop = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/stop");
    follow_resume = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/resume");
    cliGetWPName = n.serviceClient<waterplus_map_tools::GetWaypointByName>("/waterplus/get_waypoint_name");
    add_waypoint_pub = n.advertise<waterplus_map_tools::Waypoint>( "/waterplus/add_waypoint", 1);
    spk_pub = n.advertise<sound_play::SoundRequest>("/robotsound", 20);
    spk_msg.sound = sound_play::SoundRequest::SAY;
    spk_msg.command = sound_play::SoundRequest::PLAY_ONCE;
    vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    clientIAT = n.serviceClient<xfyun_waterplus::IATSwitch>("xfyun_waterplus/IATSwitch");
    behaviors_pub = n.advertise<std_msgs::String>("/wpb_home/behaviors", 30);
    grab_result_sub = n.subscribe<std_msgs::String>("/wpb_home/grab_result",30,&GrabResultCallback);
    pass_result_sub = n.subscribe<std_msgs::String>("/wpb_home/pass_result",30,&PassResultCallback);

    int sock;
    int ret = connectServer(sock, SERVER_IP, SERVER_PORT);
    //assert(ret >= 0);
    SCT::Source sock_source = SCT::Source(SCT::Source::Type::READ, sock, 0, mainq);

    sock_source.setCancelHandler([] {
        printf("Offline mode\n");
    });

    sock_source.setEventHandler([sock, write_port] {
        void *pdata = NULL;
        int length;
        retrieveData(sock, pdata, length);
        
        //do somthing here via network
        static const char *stopImm = "kill";
        static const char *reFollow = "mapScanning";
        static const char *appoint= "appoint";

        printf("received: %s %d\n", pdata, length);
        #define strcmp(p1, p2)
        
        if (strncmp(static_cast<const char *>(pdata), reFollow, length) == 0) {
            if (nState == STATE_GOTO || nState == STATE_COMEBACK) {
                cancelMove();
            }
            if (nState != STATE_FOLLOW) {
                static const char str[] = "1";
                sendData(sock, str, sizeof(str));
            } else {
                static const char str[] = "0";
                sendData(sock, str, sizeof(str));
            }
            nState = STATE_FOLLOW;
        } else if (strncmp(static_cast<const char *>(pdata), appoint, length) == 0) {
            if (nState == STATE_FOLLOW) {
                stopFollowAndSetMaster();
                static const char str[] = "1";
                sendData(sock, str, sizeof(str));
            } else {
                static const char str[] = "0";
                sendData(sock, str, sizeof(str));
            }
        } 
        else if(strncmp(static_cast<const char *>(pdata), stopImm, length) == 0) {
            geometry_msgs::Twist vel_cmd;

            vel_cmd.linear.x = 0;
            vel_cmd.linear.y = 0;
            vel_cmd.linear.z = 0;

            vel_cmd.angular.x = 0;
            vel_cmd.angular.y = 0;
            vel_cmd.angular.z = 0;
            //for (int i = 0; i < 100; i++) {
            vel_pub.publish(vel_cmd);
            //}
            
            ros::shutdown();
            static const char killall[] = "killall";
            sendData(write_port, killall, sizeof(killall));
        }

        if (pdata != NULL) {
            free(pdata);
        }
    });

    //语音初始化
    InitKeyword();

    ROS_WARN("[main] start looping");
    ros::Rate r(30);
    pipe_timer.resume();
    sock_source.resume();

    //状态机，循环休眠和唤醒检测
    while(ros::ok())
    {
        // 1、刚启动，准备
        if(nState == STATE_READY)start();

        // 2、跟随阶段
        if(nState == STATE_FOLLOW)follow();

        // 3、询问要去哪个航点
        if(nState == STATE_ASK);

        // 4、导航去指定航点
        if(nState == STATE_GOTO)moveto();

        // 5、抓取物品
        if(nState == STATE_GRAB)GrabObj();

        // 6、抓完物品回来
        if(nState == STATE_COMEBACK)moveback();

        // 7、将物品给主人
        if(nState == STATE_PASS)PassObj();

        SCT_main_queue_drain_np();
        
        ros::spinOnce();
        r.sleep();
    }

    return 0;
}
