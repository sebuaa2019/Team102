#include <gtest.h>
#include <iostream>
#include "preSet.h"
#include "start.h"

//对FollowSwitch进行测试
TEST(FollowSwitch,case1){
	//启动跟随成功
	warnMsg[0]='1';
	FollowSwitch(true,0.7);
	//启动成功，ROS_WARN不会执行，warnMsg不会被修改
	EXPECT_EQ('1',warnMsg[0]);

	//启动跟随失败
	warnMsg[0]='1';
	FollowSwitch(true,0.0);
	//启动失败，ROS_WARN会执行，warnMsg会被修改
	EXPECT_NE('1',warnMsg[0]);

	//停止跟随成功
	warnMsg[0]='1';
	srvFlw.request.thredhold=0.7;
	FollowSwitch(false,0.7);
	//停止成功，ROS_WARN不会执行，warnMsg不会被修改
	EXPECT_EQ('1',warnMsg[0]);

	//停止跟随失败
	warnMsg[0]='1';
	srvFlw.request.thredhold=0.0;
	FollowSwitch(false,0.7);
	//停止失败，ROS_WARN会执行，warnMsg会被修改
	EXPECT_NE('1',warnMsg[0]);
}

//对start进行测试
TEST(start,case2){
	//启动正在准备中
	addMsg[0]='1';
	nDelay=1;
	nState=0;
	start();

	EXPECT_EQ(2,nDelay);//启动计数器应被更新
	EXPECT_EQ('1',addMsg[0]);//准备尚未结束，AddNewWaypoint不会被执行，addMsg不会被修改
	EXPECT_EQ(0,nState);//准备尚未结束，nState不会被修改

	//启动准备完成
	addMsg[0]='1';
	nDelay=100;
	nState=0;
	start();

	EXPECT_EQ(0,nDelay);//启动计数器应被重置
	EXPECT_NE('1',addMsg[0]);//准备结束，AddNewWaypoint会被执行，addMsg会被修改
	EXPECT_EQ(STATE_FOLLOW,nState);//准备结束，nState会被修改
}

//对follow进行测试
TEST(follow,case3){
	//启动准备刚刚完成
	nDelay=0;
	srvFlw.request.thredhold=0.0;
	follow();

	EXPECT_LT(0.1,srvFlw.request.thredhold);//准备刚刚完成，FollowSwitch被调用，thredhold被更新
	EXPECT_EQ(1,nDelay);//计数器更新
	

	//启动准备早已完成
	nDelay=10;
	srvFlw.request.thredhold=0.11;
	follow();

	EXPECT_LT(srvFlw.request.thredhold,0.12);//准备早已完成，FollowSwitch不会被调用，thredhold不会被更新
	EXPECT_EQ(11,nDelay);//计数器更新
}

int main(int argc,char* argv[]){
	//初始化并启动测试
	testing::InitGoogleTest(&argc, argv); 
	return RUN_ALL_TESTS();
}