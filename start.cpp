#include "main.h" 

void start(){//启动准备阶段

	if(nState == STATE_READY){
		nDelay ++;//维护时间计数器，代表了已用的准备时间
		if(nDelay > 100){//准备时间已到，默认准备已经完成
			nDelay = 0;//准备计时结束，计数器清零
			AddNewWaypoint("start");
			nState = STATE_FOLLOW;//准备完成，进入跟随阶段
		}
	}

}

void follow(){//跟随阶段

	if(nState == STATE_FOLLOW){
		//如果计数器时间为0，就说明启动准备刚刚完成
		if(nDelay == 0){
			FollowSwitch(true, 0.7);//命令机器人进入跟随状态
		}
		//如果计数器时间不为0，说明启动早已完成，机器人已经处于跟随状态

		nDelay ++;//维护时间计数器，代表了处于跟随状态的时间
	}

}
