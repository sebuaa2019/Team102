#include <main.hpp>
#include <start.hpp>
#include <voice.hpp>
#include <ExceptionHandler.hpp>

wpb_home_tutorials::Follow srvFlw;

// 跟随模式开关
void FollowSwitch(bool inActive, float inDist){
	
    if(inActive == true){//命令机器人进入跟随状态 
        srvFlw.request.thredhold = inDist;
        if (!follow_start.call(srvFlw)){//跟随启动返回为假，跟随启动失败 
            ROS_WARN("[CActionManager] - follow start failed...");
        }
    }
    
    else{//命令机器人停止跟随 
        if (!follow_stop.call(srvFlw)){//停止跟随返回为假，停止跟随失败 
            ROS_WARN("[CActionManager] - failed to stop following...");
        }
    }
}

void start(){//启动准备阶段
	//启动后延迟一段时间，让机器人做准备
	nDelay ++;//维护时间计数器，代表了已用的准备时间
	if(nDelay > 100){//准备时间已到，默认准备已经完成
		nDelay = 0;//准备计时结束，计数器清零
		AddNewWaypoint("start");
		nState = STATE_FOLLOW;//准备完成，进入跟随阶段
	}
}

void follow(){//跟随阶段
	//如果计数器时间为0，就说明启动准备刚刚完成
	if(nDelay == 0){
		FollowSwitch(true, 0.7);//命令机器人进入跟随状态
	}
	//如果计数器时间不为0，说明启动早已完成，机器人已经处于跟随状态

	nDelay ++;//维护时间计数器，代表了处于跟随状态的时间

}

class Graph {
	static int MAP_SIZE;
	std::vector<int> graph;
public:
	static void loadMap();
	static bool hasObstacle();
	static void setDestination();
	static void isDestination();
	static void setFree();
};
